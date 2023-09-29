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

using ::testing::Return;
using ::testing::_;

//Define mock class for Theater class
class TheaterMock : public Theater {
public:
    TheaterMock(const int& id, const std::string& name, const std::vector<Seat>& seats)
        : Theater(id, name, seats) {}

    MOCK_METHOD(bool, bookSeat, (const int& id), (override));
    MOCK_METHOD(std::vector<int>, getAvailableSeats, (), (const, override));
    MOCK_METHOD(std::string, getName, (), (const, override));
    MOCK_METHOD(int, getId, (), (const, override));
    MOCK_METHOD(void, setAllocated, (bool value), (override));
    MOCK_METHOD(bool, isAllocated, (), (const, override));
    
    // Define a copy constructor
     TheaterMock(const TheaterMock& other): Theater(other)
    {
         // Copy member variables from 'other' to this object
         mId = other.mId;
         mName = other.mName;
        mIsAllocated = other.mIsAllocated;
        mSeats = other.mSeats;
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
        mMovies_.emplace_back(std::make_unique<Movie>(0, "Movie00"));
        mMovies_.emplace_back(std::make_unique<Movie>(1, "Movie01"));

        for (int i = 0; i < mSeatCapacity; ++i)
        {
            Seat seat;
            seat.id = i;
            seat.seatNumber = "Seat " + std::to_string(i + 1);
            seat.isBooked = false;
            mSeats.push_back(seat);
        }

        // Populate sample theaters
        mTheaterMocks_.emplace_back(std::make_unique<TheaterMock>(0, "Theater00", mSeats));
        mTheaterMocks_.emplace_back(std::make_unique<TheaterMock>(1, "Theater01", mSeats));

        // Initialize the MovieBookingService with sample data
        mServicePtr = std::make_unique<MovieBookingService>();
        
        for (auto& movie : mMovies_)
        {
            mMovies.push_back(movie.get());
            mServicePtr->addMovie(std::move(movie));
        }

        for (auto& theaterMock : mTheaterMocks_)
        {

            EXPECT_CALL(*theaterMock, setAllocated(true))
                .Times(::testing::AtLeast(0));
            EXPECT_CALL(*theaterMock, isAllocated())
                .WillRepeatedly(Return(false));

            EXPECT_CALL(*theaterMock, getId())
                .WillOnce(Return(0))
                .WillRepeatedly(Return(1));
            
            mTheaterMocks.push_back(theaterMock.get());

            mServicePtr->addTheater(std::move(theaterMock));
        }
    }

    std::unique_ptr<MovieBookingService> mServicePtr;
    std::vector<std::unique_ptr<Movie>> mMovies_;
    std::vector<Seat> mSeats;
    std::vector<std::unique_ptr<TheaterMock>> mTheaterMocks_;
    const int mSeatCapacity = 5;
    std::vector<TheaterMock*> mTheaterMocks;
    std::vector<Movie*> mMovies;


};

/*------------------------------------------------------*/
//// Test case for getAllMovies
TEST_F(MovieBookingServiceFixture, GetAllMovies) {
    
    std::vector<int> movieIds = {mMovies[0]->id,mMovies[1]->id};

    // Call the method and check the result
    auto result = mServicePtr->getAllMovies();

    EXPECT_EQ(result, movieIds);
}

/*------------------------------------------------------*/
// Test case for AddMovie
TEST_F(MovieBookingServiceFixture, AddMovie) {

    // Create a sample movie
    auto movie = std::make_unique<Movie>(3, "Movie00");
    
    int movieId = movie->id;

    // Add the movie to the MovieBookingService
    mServicePtr->addMovie(std::move(movie));

    // Get the list of all movies from the MovieBookingService
    auto movieIds = mServicePtr->getAllMovies();

    // Check if the added movie exists in the list of all movies
    bool movieFound = false;
    for (const auto& m : movieIds) {
        if (m == movieId) {
            movieFound = true;
            break;
        }
    }

    EXPECT_TRUE(movieFound);
}

/*------------------------------------------------------*/
// Test case for getAvailableSeats
TEST_F(MovieBookingServiceFixture, GetAvailableSeats) {
    // Define theater ID for which available seats will be retrieved
    int theaterIdToTest = 0;

    std::vector<int> seatIds= {0,1,2,3,4};

    // Set up expectations for the mock
    EXPECT_CALL(*mTheaterMocks[theaterIdToTest], getAvailableSeats())
        .WillOnce(::testing::Return(seatIds));

    // Call the method and check the result
    auto result = mServicePtr->getAvailableSeats(theaterIdToTest);

    EXPECT_EQ(result, seatIds);

}

/*------------------------------------------------------*/
// Test case for bookSeats
TEST_F(MovieBookingServiceFixture, BookSeats) {
    // Create sample seat IDs
    std::vector<int> seatIds = {0, 1, 2};
    int theaterIdToTest = 0;

    ::testing::InSequence s;
    // Expectations for the mock theater
    EXPECT_CALL(*mTheaterMocks[theaterIdToTest], bookSeat(0)).WillOnce(::testing::Return(true));
    EXPECT_CALL(*mTheaterMocks[theaterIdToTest], bookSeat(1)).WillOnce(::testing::Return(true));
    EXPECT_CALL(*mTheaterMocks[theaterIdToTest], bookSeat(2)).WillOnce(::testing::Return(true));

    bool result = mServicePtr->bookSeats(0, seatIds); // Assuming theaterId 0 and movieId 0

    EXPECT_TRUE(result); // Expecting successful booking of seats

    ::testing::InSequence s2;
    // Expectations for the mock theater
    EXPECT_CALL(*mTheaterMocks[theaterIdToTest], bookSeat(0)).WillOnce(::testing::Return(true));
    EXPECT_CALL(*mTheaterMocks[theaterIdToTest], bookSeat(1)).WillOnce(::testing::Return(true));
    EXPECT_CALL(*mTheaterMocks[theaterIdToTest], bookSeat(2)).WillOnce(::testing::Return(false)); // One seat booking fails

    // Book seats again (this time one seat fails)
    result = mServicePtr->bookSeats(0, seatIds);

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
TEST_F(MovieBookingServiceFixture, GetTheaterName_ValidId) {
    // Define the theater ID for which the name will be retrieved
    int theaterIdToTest = 0; // Assuming theater ID 0 exists

    // Set up expectations for the mock theaters
    EXPECT_CALL(*mTheaterMocks[theaterIdToTest], getName())
        .WillOnce(::testing::Return("Theater00"));

    // Call the method and check the result
    std::string result = mServicePtr->getTheaterName(theaterIdToTest);

    // Verify that the result matches the expected movie name
    EXPECT_EQ(result, "Theater00");
}

/*------------------------------------------------------*/
// Test case for getTheaterName, Bad Weather
TEST_F(MovieBookingServiceFixture, GetTheaterName_InValidId) {

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

/*------------------------------------------------------*/
// Test case for isValidMovie, Bad weather
TEST_F(MovieBookingServiceFixture, IsValidMovie_InvalidMovieId) {
    // Define a movie ID that does not exist in the service
    int nonExistingMovieId = -1;;

    // Check if an invalid movie ID returns false
    EXPECT_FALSE(mServicePtr->isValidMovie(nonExistingMovieId));
}

