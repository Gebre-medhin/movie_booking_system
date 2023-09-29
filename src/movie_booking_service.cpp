/**
 * @file movie_booking_service.cpp
 * @brief  Implementation for MovieBookingService (API) class
 * @author Gebremedhin Abreha
 */

#include "movie_booking_service.hpp"

#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <random>
#include <ctime>

/*----------------------------------------------------*/
bool MovieBookingService::addMovie( std::unique_ptr<Movie> movie) {
    // Add the movie to the list of movies
    bool result = false;
    if (!movie) {
        // Handle null pointer
        return false;
    }
        
    if (const auto& [itr, done] = mMovies.insert({movie->id, std::move(movie)}); done)
    {
        result = done;
        allocateMovieToTheaters(itr->second->id);
    }
    return result;
}

/*----------------------------------------------------*/
bool MovieBookingService::addTheater( std::unique_ptr<Theater> theater) {
    
    bool result = false;
    if (!theater) {
        // Handle null pointer
        return result;
    }
    int theaterId = theater->getId();
    
    if (const auto& [itr, done] = mTheaters.insert({theaterId, std::move(theater)}); done)
    {
        result = done;
        
        bool isMovieAllocated = false;
        
        // Check if there are unallocated movies
        for (const auto& [movieId, movie] : mMovies)
        {
            if (!movie->isAllocated)
            {
                if (allocateMovieToTheaters(movieId))
                {
                    isMovieAllocated = true;
                }
            }
        }
        // If all movies are already allocated, randomly pick one movie
        if (!isMovieAllocated && !mMovies.empty())
        {
            std::random_device rd;
            //std::mt19937 gen(std::time(nullptr));
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dist(1, mMovies.size());
            int randomId = dist(gen);
            
            if (isValidMovie(randomId))
            {
                // Allocate the randomly picked movie to the new theater
                isMovieAllocated = allocateMovieToTheaters(randomId);
            }
            else
            {
                throw std::invalid_argument("Movie with the specified random ID not found");

            }
         }
    }
    return result;
}


/*----------------------------------------------------*/
std::vector<int> MovieBookingService::getAllMovies() const
{
    std::vector<int> movieIds;
    
    for (const auto& [id, movie]: mMovies)
    {
        movieIds.push_back(id);
    }
    return movieIds;
}

/*----------------------------------------------------*/
std::vector<int> MovieBookingService::getTheatersForMovie(int movieId) const
{
    std::vector<int> theatersForMovie;
        
    // Check if the movie ID is valid
    if (isValidMovie(movieId))
    {
        // Get the list of theater IDs allocated to the movie
        const std::vector<int>& theaterIds = mMovieTheaterAllocations.at(movieId);

        // Iterate through theaters and find matching theater IDs
        for (const auto& allocatedTheaterId : theaterIds)
        {
            if (auto itr = mTheaters.find(allocatedTheaterId); itr != mTheaters.end())
            {
                theatersForMovie.push_back(itr->first);
            }
        }
    }

    return theatersForMovie;
}

/*----------------------------------------------------*/
std::vector<int> MovieBookingService::getAvailableSeats(int theaterId) const
{
    std::vector<int> availableSeats;
    // Check if the theater and movie IDs are valid
    if (!isValidTheater(theaterId))
    {
        return availableSeats; // Return an empty vector for invalid theater or movie
    }
    
   // Find the theater associated with the provided theaterId
    
    if (auto itr = mTheaters.find(theaterId); itr != mTheaters.end())
    {
        availableSeats = itr->second->getAvailableSeats();
    }

    return availableSeats;
}

/*----------------------------------------------------------------------*/
bool MovieBookingService::bookSeats(int theaterId, const std::vector<int>& seatIds)
{
    // Check if the theater and movie IDs are valid
    if (!isValidTheater(theaterId) || seatIds.empty()) {
        return false; // Invalid theater or movie
    }
    
    std::lock_guard<std::mutex> lock(mBookingMutex);
    
    // Get the theater associated with the provided theaterId
    
    if (auto itr = mTheaters.find(theaterId); itr != mTheaters.end())
    {
        for (auto seatId : seatIds)
        {
            if (!itr->second->bookSeat(seatId)) return false;
        }
    }
    
    return true; // Seats booked successfully
}

/*----------------------------------------------------*/
bool MovieBookingService::isValidMovie(int movieId) const
{
    
    if (mMovies.find(movieId) != mMovies.end())
    {
        return true; // Found a movie with the specified ID
    }
    return false;
}

/*----------------------------------------------------*/
bool MovieBookingService::isValidTheater(int theaterId) const
{
    if (mTheaters.find(theaterId)!= mTheaters.end())
    {
        return true; // Found a theater with the specified ID
    }
    return false;
}

/*----------------------------------------------------*/
std::string MovieBookingService::getMovieName(int movieId) const
{
    
    if (auto itr = mMovies.find(movieId); itr != mMovies.end())
    {
        return itr->second->name; // Found a movie with the specified ID
    }
    // If the theater ID is not found, throw an exception
    throw std::invalid_argument("Movie with the specified ID not found");
}

/*----------------------------------------------------*/
std::string MovieBookingService::getTheaterName(int theaterId) const
{
    if (auto itr = mTheaters.find(theaterId); itr != mTheaters.end())
    {
        return itr->second->getName(); // Found a theater with the specified ID
    }
    // If the theater ID is not found, throw an exception
    throw std::invalid_argument("Theater with the specified ID not found");
}

/*----------------------------------------------------*/
bool MovieBookingService::allocateMovieToTheaters( int movieId)
{
    
    std::lock_guard<std::mutex> lock(mBookingMutex);

    if (!isValidMovie(movieId)) {
        // Handle invalid movie ID
        return false;
    }

    // Try to allocate the movie to an unallocated theater
    for (auto& [id, theater] : mTheaters)
    {
        if (!theater->isAllocated())
        {
            theater->setAllocated(true);
            mMovieTheaterAllocations[movieId].push_back(theater->getId());
            mMovies.at(movieId)->isAllocated = true;
            return true;
        }
    }
    return false;
}


///*----------------------------------------------------*/
bool MovieBookingService::isMovieShownInTheater(int theaterId, int movieId) const
{
    // Check if the theater and movie IDs are valid
    if (!isValidTheater(theaterId) || !isValidMovie(movieId))
    {
        return false; // Invalid theater or movie
    }

    // Check if the specified theater is allocated for the given movie
    const auto& allocatedTheaters = mMovieTheaterAllocations.at(movieId);
    bool isAllocated = std::find(allocatedTheaters.begin(), allocatedTheaters.end(), theaterId) != allocatedTheaters.end();

    return isAllocated;
}

/*-------------------END---------------------------------*/

