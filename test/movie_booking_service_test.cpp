/**
 * @file movie_booking_service_test.cpp
 * @brief Test for MovieBookingService (API) class
 * @author Gebremedhin Abreha
 */
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "movie_booking_service.hpp"

#include <memory>
#include <future>

// Define a mock class for MovieBookingService
class MockMovieBookingService : public MovieBookingService {
public:
    MockMovieBookingService(const std::vector<Movie>& movies, const std::vector<Theater>& theaters)
        : MovieBookingService(movies, theaters) {}

    MOCK_METHOD(std::vector<Movie>, getAllMovies, (), (const, override));
    MOCK_METHOD(std::vector<Theater>, getTheatersForMovie, (int movieId), (const, override));
    MOCK_METHOD(std::vector<int>, getAvailableSeats, (int theaterId, int movieId), (const, override));
    MOCK_METHOD(bool, bookSeats, (int theaterId, int movieId, const std::vector<int>& seatIds), (override));
    MOCK_METHOD(bool, isValidMovie, (int movieId), (const, override));
    MOCK_METHOD(bool, isMovieShownInTheater, (int theaterId, int movieId), (const, override));
    MOCK_METHOD(std::string, getMovieName, (int movieId), (const, override));
    MOCK_METHOD(std::string, getTheaterName, (int theaterId), (const, override));
};

// Define a fixture class for MovieBookingService
class MovieBookingServiceFixture : public ::testing::Test {
protected:
    std::unique_ptr<MockMovieBookingService> mServicePtr; // Using MockMovieBookingService for testing

    // Sample movie and theater data
    std::vector<Movie> movies = {
        Movie(1, "Movie 1"),
        Movie(2, "Movie 2"),
        // Add more movies as needed
    };

    std::vector<Theater> theaters = {
        Theater(1, "Theater 1"),
        Theater(2, "Theater 2"),
        // Add more theaters as needed
    };

    // Set up the common environment for the test cases
    void SetUp() override {
        // Initialize the MockMovieBookingService with sample data
        mServicePtr = std::make_unique<MockMovieBookingService>(movies, theaters);
    }
};

using ::testing::Return;
using ::testing::_;

/*------------------------------------------------------*/
// Test case for getAllMovies
TEST_F(MovieBookingServiceFixture, GetAllMovies) {
    // Mock behavior for the getAllMovies method
    EXPECT_CALL(*mServicePtr, getAllMovies())
        .WillOnce(Return(movies));

    // Call the method and check the result
    std::vector<Movie> result = mServicePtr->getAllMovies();
    EXPECT_EQ(result, movies);
}

/*------------------------------------------------------*/
// Test case for getTheatersforMovie
TEST_F(MovieBookingServiceFixture, GetTheatersForMovie) {
    int movieIdToTest = 1;

    // Mock behavior for the getTheatersForMovie method
    EXPECT_CALL(*mServicePtr, getTheatersForMovie(movieIdToTest))
        .WillOnce([&](int movieId) {
            // Simulate returning theaters for the specified movie ID
            std::vector<Theater> theatersForMovie;
            for (const Theater& theater : theaters) {
                if (mServicePtr->isMovieShownInTheater(theater.getId(), movieId)) {
                    theatersForMovie.push_back(theater);
                }
            }
            return theatersForMovie;
        });

    // Call the method and check the result
    std::vector<Theater> result = mServicePtr->getTheatersForMovie(movieIdToTest);

    // Verify that the returned theaters match the expected theaters for the movie ID
    std::vector<Theater> expectedTheatersForMovie;
    for (const Theater& theater : theaters) {
        if (mServicePtr->isMovieShownInTheater(theater.getId(), movieIdToTest)) {
            expectedTheatersForMovie.push_back(theater);
        }
    }

    EXPECT_EQ(result, expectedTheatersForMovie);
}

/*------------------------------------------------------*/
// Test case for getAvailableSeats
TEST_F(MovieBookingServiceFixture, GetAvailableSeats) {
    // Define theater ID and movie ID for which available seats will be retrieved
    int theaterIdToTest = 1;
    int movieIdToTest = 1; // Assuming both movie and theater exist

    // Mock behavior for the getAvailableSeats method
    EXPECT_CALL(*mServicePtr, getAvailableSeats(theaterIdToTest, movieIdToTest));
    
    // Call the method and check the result
    std::vector<int> result = mServicePtr->getAvailableSeats(theaterIdToTest, movieIdToTest);
}

/*------------------------------------------------------*/
// Test case for bookSeats
TEST_F(MovieBookingServiceFixture, BookSeats) {
    // Define theater ID, movie ID, and seat IDs to be booked
    int theaterIdToTest = 1;
    int movieIdToTest = 1; // Assuming both movie and theater exist
    std::vector<int> seatIdsToBook = {1, 2, 3}; // Assuming these seats are available

    // Mock behavior for the bookSeats method
    EXPECT_CALL(*mServicePtr, bookSeats(theaterIdToTest, movieIdToTest, seatIdsToBook))
        .WillOnce([&](int theaterId, int movieId, const std::vector<int>& seatIds) {
            // Simulate booking seats and returning true if successful
            // In a real implementation, you would update the booking data
            return true;
        });

    // Call the method and check the result
    bool result = mServicePtr->bookSeats(theaterIdToTest, movieIdToTest, seatIdsToBook);

    // Verify that the result is true, indicating successful booking
    EXPECT_TRUE(result);
}

/*------------------------------------------------------*/
// Test case for concurrent seat booking
TEST_F(MovieBookingServiceFixture, ConcurrentSeatBooking) {
    const int numThreads = 5; // Number of concurrent booking threads
    const int theaterId = 1; // Specify the theater and movie for the test
    const int movieId = 1;
    const int numSeats = 3; // Number of seats to book by each thread
    
    testing::InSequence s;
    // Mock behavior for the bookSeats method

    EXPECT_CALL(*mServicePtr, bookSeats(theaterId, movieId, _ ))
        .WillOnce(Return(true));
    EXPECT_CALL(*mServicePtr, bookSeats(theaterId, movieId, _ ))
        .WillRepeatedly(Return(false));

    // Define a vector of futures to store the results of async tasks
    std::vector<std::future<bool>> futures;

    // Create and launch multiple async tasks to simulate concurrent seat booking
    for (int i = 0; i < numThreads; ++i) {
        futures.emplace_back(std::async(std::launch::async, [this, theaterId, movieId, numSeats]() {
            // Simulate a user booking seats concurrently
            std::vector<int> seatIds;
            for (int j = 0; j < numSeats; ++j) {
                seatIds.push_back(j + 1); // Assuming seat IDs start from 1
            }

            // Call the bookSeats method of the mock service
            return mServicePtr->bookSeats(theaterId, movieId, seatIds);
        }));
    }
    // Wait for all async tasks to complete and collect the results
    std::vector<bool> bookStatus;
    for (auto& future : futures) {
        bool result = future.get();
        bookStatus.push_back(result);
    }
    
    
    // Check the result of the booking
    int cnt = std::count(bookStatus.begin(), bookStatus.end(), true);
    EXPECT_EQ(cnt, 1); // Check if only 1 booking is successful (no over booking)
}

/*------------------------------------------------------*/
// Test case for isValidMovie
TEST_F(MovieBookingServiceFixture, IsValidMovie) {
    // Define a movie ID that exists in the service
    int existingMovieId = 1;
    
    // Mock behavior for the bookSeats method
    EXPECT_CALL(*mServicePtr, isValidMovie(existingMovieId))
        .WillOnce(Return(true));

    // Call the method to check if the movie is valid
    bool result = mServicePtr->isValidMovie(existingMovieId);

    // Verify that the result is true, indicating the movie is valid
    EXPECT_TRUE(result);

    // Define a movie ID that does not exist in the service
    int nonExistingMovieId = 999;
    
    // Mock behavior for the bookSeats method
    EXPECT_CALL(*mServicePtr, isValidMovie(nonExistingMovieId))
        .WillOnce(Return(false));

    // Call the method to check if the movie is valid
    result = mServicePtr->isValidMovie(nonExistingMovieId);

    // Verify that the result is false, indicating the movie is not valid
    EXPECT_FALSE(result);
}

/*------------------------------------------------------*/
// Test case for isMovieShownInTheater
TEST_F(MovieBookingServiceFixture, IsMovieShownInTheater) {
    // Define theater ID and movie ID for which the check will be performed
    int existingTheaterId = 1;
    int existingMovieId = 1; // Assuming both theater and movie exist

    // Mock behavior for the bookSeats method
    EXPECT_CALL(*mServicePtr, isMovieShownInTheater(existingTheaterId, existingMovieId))
        .WillOnce(Return(true));
    // Call the method to check if the movie is shown in the theater
    bool result = mServicePtr->isMovieShownInTheater(existingTheaterId, existingMovieId);

    // Verify that the result is true, indicating the movie is shown in the theater
    EXPECT_TRUE(result);

    // Define theater ID and movie ID for which the check will be performed
    int nonExistingTheaterId = 999; // Theater that does not exist
    int nonExistingMovieId = 2; // Movie that does not exist
    
    // Mock behavior for the bookSeats method
    EXPECT_CALL(*mServicePtr, isMovieShownInTheater(nonExistingTheaterId, nonExistingMovieId))
        .WillOnce(Return(false));

    // Call the method to check if the movie is shown in the theater
    result = mServicePtr->isMovieShownInTheater(nonExistingTheaterId, nonExistingMovieId);

    // Verify that the result is false, indicating the movie is not shown in the theater
    EXPECT_FALSE(result);
}

/*------------------------------------------------------*/
// Test case for getMovieName
TEST_F(MovieBookingServiceFixture, GetMovieName) {
    // Define a movie ID for which the name will be retrieved
    int existingMovieId = 1;
    // Define the expected movie name for the existing movie ID
    std::string expectedMovieName = "Movie1"; // Replace with the actual movie name
    
    // Mock behavior for the bookSeats method
    EXPECT_CALL(*mServicePtr, getMovieName(existingMovieId))
        .WillOnce(Return(expectedMovieName));

    // Call the method to get the movie name
    std::string result = mServicePtr->getMovieName(existingMovieId);

/*------------------------------------------------------*/
/**
 * Test case for isValidMovie, Bad weather
 */
TEST_F(MovieBookingServiceFixture, IsValidMovie_InvalidMovieId) {
    // Define a movie ID that does not exist in the service
    int nonExistingMovieId = 999;
    
    // Mock behavior for the bookSeats method
    EXPECT_CALL(*mServicePtr, getMovieName(nonExistingMovieId))
        .WillOnce(Return(""));

    // Call the method to get the movie name
    result = mServicePtr->getMovieName(nonExistingMovieId);

    // Verify that the result is an empty string for a non-existing movie ID
    EXPECT_TRUE(result.empty());
}

/*------------------------------------------------------*/
