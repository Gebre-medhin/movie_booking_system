/**
 * @file movie_booking_service,cpp
 * @brief  Implementation for MovieBookingService (API) class
 * @author Gebremedhin Abreha
 */

#include "movie_booking_service.hpp"

#include <iostream>
#include <vector>
#include <mutex>
#include <stdexcept>
#include <algorithm>

/*----------------------------------------------------*/
MovieBookingService::MovieBookingService(const std::vector<Movie>& movies, const std::vector<Theater>& theaters)
    : mMovies(movies), mTheaters(theaters)
{
    allocateMovieToTheaters();
}

/*----------------------------------------------------*/
std::vector<Movie> MovieBookingService::getAllMovies() const
{
    return mMovies;
}

/*----------------------------------------------------*/
std::vector<Theater> MovieBookingService::getTheatersForMovie(int movieId) const
{
    std::vector<Theater> theatersForMovie;
    // Check if the movie ID is valid
    if (isValidMovie(movieId))
    {
        // Get the list of theater IDs allocated to the movie
        const std::vector<int>& theaterIds = mMovieTheaterAllocations.at(movieId);

        // Iterate through theaters and find matching theater IDs
        for (const Theater& theater : mTheaters)
        {
            for (int allocatedTheaterId : theaterIds)
            {
                if (theater.getId() == allocatedTheaterId)
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
    // Lock the mutex to protect the critical section
    std::lock_guard<std::mutex> lock(mBookingMutex);
    
   // Find the theater associated with the provided theaterId
    const Theater* theater = nullptr;
    for (const Theater& t : mTheaters)
    {
        if (t.getId() == theaterId)
        {
            theater = &t;
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
   std::lock_guard<std::mutex> lock(mBookingMutex);
    
    // Check if the theater and movie IDs are valid
    if (!isValidTheater(theaterId) || !isValidMovie(movieId) || seatIds.empty()) {
        return false; // Invalid theater or movie
    }
    // Get the theater associated with the provided theaterId
    Theater* theater = nullptr;
    for (auto& val : mTheaters)
    {
        if (val.getId() == theaterId)
        {
            theater = &val;
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
        if (movie.getId() == movieId)
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
        if (theater.getId() == theaterId)
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
        if (movie.getId() == movieId)
        {
            return movie.getName();
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
        if (theater.getId() == theaterId)
        {
            return theater.getName();
        }
    }
    // If the theater ID is not found, throw an exception
    throw std::invalid_argument("Theater with the specified ID not found");
}

/*----------------------------------------------------*/
void MovieBookingService::allocateMovieToTheaters()
{

    // Clear existing movie allocations
    mMovieTheaterAllocations.clear();

    int numMovies = static_cast<int>(mMovies.size());
    int numTheaters = static_cast<int>(mTheaters.size());
    
    //If number of movies are more than the number of theaters, throw exceptions
    if (numMovies > numTheaters)
    {
        throw std::invalid_argument("Number of Theaters are too few. All the movies cannot be allocated to a theater.");
    }
    // Calculate the number of movies each theater should be allocated
    int theatersPerMovie = numTheaters/numMovies;
    int remainingTheaters = numTheaters %numMovies;

    int theaterIndex = 0;

    for (int  movieIndex= 0; movieIndex < numMovies; ++movieIndex)
    {
        int theatersToAllocate = theatersPerMovie;

        // Distribute remaining movies one by one to theaters
        if (remainingTheaters > 0)
        {
            ++theatersToAllocate;
            --remainingTheaters;
        }

        for (int i = 0; i < theatersToAllocate; ++i)
        {
            int movieId = mMovies[movieIndex].getId();
            int theaterId = mTheaters[theaterIndex].getId();

            // Store the allocated theater for the movie
            mMovieTheaterAllocations[movieId].push_back(theaterId);

            // Move to the next movie
            ++theaterIndex;
        }
    }
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

