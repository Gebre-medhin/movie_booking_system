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

/*----------------------------------------------------*/
void MovieBookingService::addMovie(const std::shared_ptr<Movie>& movie) {
    // Add the movie to the list of movies
    {
        std::lock_guard<std::mutex> lock(mBookingMutex);
        mMovies.push_back(movie);
    }
    allocateMovieToTheaters(movie);
}

/*----------------------------------------------------*/
void MovieBookingService::addTheater(const std::shared_ptr<Theater>& theater) {
    
    if (!theater) {
        // Handle null pointer
        return;
    }
    
    {
        std::lock_guard<std::mutex> lock(mBookingMutex);
        // Add the theater to the list of theaters
        mTheaters.push_back(theater);
    }
    
    bool isMovieAllocated = false;

    // Check if there are unallocated movies
    for (auto& movie : mMovies)
    {
        if (!movie->isAllocated)
        {
            if (allocateMovieToTheaters(movie))
            {
                isMovieAllocated = true;
            }
        }
    }
    // If all movies are already allocated, randomly pick one movie
    if (!isMovieAllocated && !mMovies.empty())
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, mMovies.size() - 1);
        int randomIndex = dist(gen);
        
        // Allocate the randomly picked movie to the new theater
        isMovieAllocated = allocateMovieToTheaters(mMovies[randomIndex]);
    }
}


/*----------------------------------------------------*/
const std::vector<std::shared_ptr<Movie>>& MovieBookingService::getAllMovies() const
{
    return mMovies;
}

/*----------------------------------------------------*/
std::vector<std::shared_ptr<Theater>> MovieBookingService::getTheatersForMovie(int movieId) const
{
    std::vector<std::shared_ptr<Theater>> theatersForMovie;
    // Check if the movie ID is valid
    if (isValidMovie(movieId))
    {
        // Get the list of theater IDs allocated to the movie
        const std::vector<int>& theaterIds = mMovieTheaterAllocations.at(movieId);

        // Iterate through theaters and find matching theater IDs
        for (const auto& theater : mTheaters)
        {
            for (int allocatedTheaterId : theaterIds)
            {
                if (theater->getId() == allocatedTheaterId)
                {
                    theatersForMovie.push_back(theater);
                    break; // Found a matching theater, no need to continue searching
                }
            }
        }
    }

    return theatersForMovie;
}

/*----------------------------------------------------*/
std::vector<int> MovieBookingService::getAvailableSeats(int theaterId, int movieId) const
{
    std::vector<int> availableSeats;
    // Check if the theater and movie IDs are valid
    if (!isValidTheater(theaterId) || !isValidMovie(movieId))
    {
        return availableSeats; // Return an empty vector for invalid theater or movie
    }
    
    if ( !isMovieShownInTheater(theaterId, movieId))
    {
        return availableSeats; // Return an empty vector for invalid theater or movie
    }
    
   // Find the theater associated with the provided theaterId
    std::shared_ptr<Theater> theater = nullptr;
    for (const auto& t : mTheaters)
    {
        if (t->getId() == theaterId)
        {
            theater = t;
            break;
        }
    }
    
    // Check if the theater was found
    if (!theater)
    {
        return availableSeats; // Return an empty vector if theater not found
    }
    availableSeats = theater->getAvailableSeats();

    return availableSeats;
}

/*----------------------------------------------------*/
bool MovieBookingService::bookSeats(int theaterId, int movieId, const std::vector<int>& seatIds)
{
    // Check if the theater and movie IDs are valid
    if (!isValidTheater(theaterId) || !isValidMovie(movieId) || seatIds.empty())
    {
        return false; // Invalid theater or movie
    }
    
    std::lock_guard<std::mutex> lock(mBookingMutex);
    
    // Get the theater associated with the provided theaterId
    std::shared_ptr<Theater> theater = nullptr;
    
    for (auto& t : mTheaters)
    {
        if (t->getId() == theaterId)
        {
            theater = t;
            break;
        }
    }
        
    // Check if the theater was found
    if (!theater)
    {
        return false; // Theater not found
    }
    
    // If all seat IDs are valid and available, booked the seats one by one
    for (int seatId : seatIds) {
        if (!theater->bookSeat(seatId))
        {
            return false;
        }
    }

    return true; // Seats booked successfully
}

/*----------------------------------------------------*/
bool MovieBookingService::isValidMovie(int movieId) const
{
    for (auto& movie : mMovies)
    {
        if (movie->id == movieId)
        {
            return true; // Found a movie with the specified ID
        }
    }
    return false; // Movie with the specified ID not found
}

/*----------------------------------------------------*/
bool MovieBookingService::isValidTheater(int theaterId) const
{
    for (auto& theater : mTheaters)
    {
        if (theater->getId() == theaterId)
        {
            return true; // Found a theater with the specified ID
        }
    }
    return false; // Theater with the specified ID not found
}

/*----------------------------------------------------*/
std::string MovieBookingService::getMovieName(int movieId) const
{
    for(auto& movie : mMovies)
    {
        if (movie->id == movieId)
        {
            return movie->name;
        }
    }
    // If the theater ID is not found, throw an exception
    throw std::invalid_argument("Movie with the specified ID not found");
}

/*----------------------------------------------------*/
std::string MovieBookingService::getTheaterName(int theaterId) const
{
    for(auto& theater : mTheaters)
    {
        if (theater->getId() == theaterId)
        {
            return theater->getName();
        }
    }
    // If the theater ID is not found, throw an exception
    throw std::invalid_argument("Theater with the specified ID not found");
}

/*----------------------------------------------------*/
bool MovieBookingService::allocateMovieToTheaters(const std::shared_ptr<Movie>& movie) {
    
    std::lock_guard<std::mutex> lock(mBookingMutex);
    
    int movieId = movie->id;
    
    if (!isValidMovie(movieId))
    {
        // Handle invalid movie ID
        return false;
    }

    // Try to allocate the movie to an unallocated theater
    for (auto& theater : mTheaters)
    {
        if (!theater->isAllocated())
        {
            theater->allocateMovie();
            mMovieTheaterAllocations[movieId].push_back(theater->getId());
            movie->isAllocated = true;
            return true;
        }
    }
    return false;
}


/*----------------------------------------------------*/
bool MovieBookingService::isMovieShownInTheater(int theaterId, int movieId) const
{
    // Check if the theater and movie IDs are valid
    if (!isValidTheater(theaterId) || !isValidMovie(movieId))
    {
        return false; // Invalid theater or movie
    }

    // Lock the mutex to protect the critical section
    std::lock_guard<std::mutex> lock(mBookingMutex);

    // Check if the specified theater is allocated for the given movie
    const std::vector<int>& allocatedTheaters = mMovieTheaterAllocations.at(movieId);
    bool isAllocated = std::find(allocatedTheaters.begin(), allocatedTheaters.end(), theaterId) != allocatedTheaters.end();

    return isAllocated;
}

/*-------------------END---------------------------------*/

