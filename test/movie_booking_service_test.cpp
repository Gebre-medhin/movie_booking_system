/**
 * @file movie_booking_service_test.cpp
 * @brief Test for MovieBookingService (API) class
 * @author Gebremedhin Abreha
 */
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "movie_booking_service.hpp"
#include "movie.hpp"
#include "theater.hpp"
#include "seat.hpp"

#include <memory>

//Define mock class for Theater class
class TheaterMock : public Theater {
public:
    TheaterMock(const int& id, const std::string& name, const std::vector<Seat>& seats)
        : Theater(id, name, seats) {}

    MOCK_METHOD(bool, bookSeat, (const int& id), (override));
    MOCK_METHOD(std::vector<int>, getAvailableSeats, (), (const, override));
    MOCK_METHOD(std::string, getName, (), (const, override));
    MOCK_METHOD(int, getId, (), (const, override));
    MOCK_METHOD(void, allocateMovie, (), (override));
    MOCK_METHOD(bool, isAllocated, (), (const, override));
    
    // Define a copy constructor
     TheaterMock(const TheaterMock& other): Theater(other)
    {
         // Copy member variables from 'other' to this object
         mId = other.mId;
         mName = other.mName;
     }

};

// Define a fixture class for MovieBookingService
class MovieBookingServiceFixture : public ::testing::Test
{
protected:

    // Set up the common environment for the test cases
    void SetUp() override
    {
        //Populate sample movies
        mMovies.push_back(std::make_shared<Movie>(0, "Movie00"));
        mMovies.push_back(std::make_shared<Movie>(1, "Movie01"));

        for (int i = 0; i < mSeatCapacity; ++i)
        {
            Seat seat;
            seat.id = i;
            seat.seatNumber = "Seat " + std::to_string(i + 1);
            seat.isBooked = false;
            mSeats.push_back(seat);
        }

        // Populate sample theaters
        mTheaterMocks.push_back( std::make_shared<TheaterMock>(0, "Theater00", mSeats));
        mTheaterMocks.push_back( std::make_shared<TheaterMock>(1, "Theater01", mSeats));

        // Initialize the MovieBookingService with sample data
        mServicePtr = std::make_unique<MovieBookingService>();
        
        for (const auto& movie : mMovies)
        {
            mServicePtr->addMovie(movie);
        }
        
        for (const auto& theaterMock : mTheaterMocks)
        {
            mServicePtr->addTheater(theaterMock);
        }

    }

    std::unique_ptr<MovieBookingService> mServicePtr;
    std::vector<std::shared_ptr<Movie>> mMovies;
    std::vector<Seat> mSeats;
    std::vector<std::shared_ptr<TheaterMock>> mTheaterMocks;
    const int mSeatCapacity = 5;

};

using ::testing::Return;
using ::testing::_;

/*------------------------------------------------------*/
// Test case for getAllMovies
TEST_F(MovieBookingServiceFixture, GetAllMovies) {

    // Call the method and check the result
    auto result = mServicePtr->getAllMovies();
    EXPECT_EQ(result, mMovies);
}

/*------------------------------------------------------*/
// Test case for AddMovie
TEST_F(MovieBookingServiceFixture, AddMovie) {

    // Create a sample movie
    std::shared_ptr<Movie> movie = std::make_shared<Movie>(3, "Movie00");
    
    // Add the movie to the MovieBookingService
    mServicePtr->addMovie(movie);

    // Get the list of all movies from the MovieBookingService
    auto movies = mServicePtr->getAllMovies();

    // Check if the added movie exists in the list of all movies
    bool movieFound = false;
    for (const auto& m : movies) {
        if (m == movie) {
            movieFound = true;
            break;
        }
    }

    // Perform assertions to verify the correctness of the test
    EXPECT_TRUE(movieFound);
}

/*------------------------------------------------------*/
// Test case for getAvailableSeats
TEST_F(MovieBookingServiceFixture, GetAvailableSeats) {
    // Define theater ID and movie ID for which available seats will be retrieved
    int theaterIdToTest = 0;
    int movieIdToTest = 0;
    
    std::vector<int> seatIds= {0,1,2,3,4};
    
    // Set up expectations for the mock
    EXPECT_CALL(*mTheaterMocks[theaterIdToTest], getAvailableSeats())
        .WillOnce(::testing::Return(seatIds));

    // Call the method and check the result
    auto result = mServicePtr->getAvailableSeats(theaterIdToTest, movieIdToTest);
    
    EXPECT_EQ(result, seatIds);

}

/*------------------------------------------------------*/
// Test case for bookSeats
TEST_F(MovieBookingServiceFixture, BookSeats) {
    // Create sample seat IDs
    std::vector<int> seatIds = {0, 1, 2};
    int theaterIdToTest = 0;

    // Expectations for the mock theater
    EXPECT_CALL(*mTheaterMocks[theaterIdToTest], bookSeat(0)).WillOnce(::testing::Return(true));
    EXPECT_CALL(*mTheaterMocks[theaterIdToTest], bookSeat(1)).WillOnce(::testing::Return(true));
    EXPECT_CALL(*mTheaterMocks[theaterIdToTest], bookSeat(2)).WillOnce(::testing::Return(true));

    // Book seats in the MovieBookingService
    bool result = mServicePtr->bookSeats(0, 0, seatIds); // Assuming theaterId 0 and movieId 0

    // Perform assertions to verify the correctness of the test
    EXPECT_TRUE(result); // Expecting successful booking of seats
    
    // Expectations for the mock theater
    EXPECT_CALL(*mTheaterMocks[theaterIdToTest], bookSeat(0)).WillOnce(::testing::Return(true));
    EXPECT_CALL(*mTheaterMocks[theaterIdToTest], bookSeat(1)).WillOnce(::testing::Return(true));
    EXPECT_CALL(*mTheaterMocks[theaterIdToTest], bookSeat(2)).WillOnce(::testing::Return(false)); // One seat booking fails

    // Book seats again (this time one seat fails)
    result = mServicePtr->bookSeats(0, 0, seatIds);

    // Perform assertions to verify the correctness of the test
    EXPECT_FALSE(result); // Expecting one seat booking to fail
}

/*------------------------------------------------------*/
// Test case for getMovieName
TEST_F(MovieBookingServiceFixture, GetMovieName) {
    // Define a movie ID for which the name will be retrieved
    int existingMovieId = 1;

    // Call the method to get the movie name
    std::string result = mServicePtr->getMovieName(existingMovieId);

    // Verify that the result matches the expected movie name
    EXPECT_EQ(result, mMovies[existingMovieId]->name);

    // Define a movie ID that does not exist in the service
    int nonExistingMovieId = 999;

    EXPECT_THROW(mServicePtr->getMovieName(nonExistingMovieId), std::invalid_argument);

}

/*------------------------------------------------------*/
// Test case for getTheaterName
TEST_F(MovieBookingServiceFixture, GetTheaterName) {
    // Define the theater ID for which the name will be retrieved
    int theaterIdToTest = 0; // Assuming theater ID 0 exists
    
    // Set up expectations for the mock theaters
    EXPECT_CALL(*mTheaterMocks[theaterIdToTest], getName())
        .WillOnce(::testing::Return("Theater00"));

    // Call the method and check the result
    std::string result = mServicePtr->getTheaterName(theaterIdToTest);

    // Verify that the result matches the expected movie name
    EXPECT_EQ(result, "Theater00");

    // Define a movie ID that does not exist in the service
    int nonExistingTheaterId = 999;

    EXPECT_THROW(mServicePtr->getTheaterName(nonExistingTheaterId), std::invalid_argument);

}

/*------------------------------------------------------*/
// Test case for isValidMovie, Good weather
TEST_F(MovieBookingServiceFixture, IsValidMovie_ValidMovieId) {
    // Define a movie ID for which the name will be retrieved
    int existingMovieId = 1;

    // Check if a valid movie ID returns true
    EXPECT_TRUE(mServicePtr->isValidMovie(1));
}

// Test case for isValidMovie, Bad weather
/*------------------------------------------------------*/
TEST_F(MovieBookingServiceFixture, IsValidMovie_InvalidMovieId) {
    // Define a movie ID that does not exist in the service
    int nonExistingMovieId = -1;;

    // Check if an invalid movie ID returns false
    EXPECT_FALSE(mServicePtr->isValidMovie(nonExistingMovieId));
}

