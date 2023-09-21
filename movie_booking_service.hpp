//
// movie_booking_service.hpp (API header)
//  booking_sytem
//
//  Created by Gebremedhin Abreha on 21/09/2023.
//

#ifndef MOVIE_BOOKING_SERVICE_HPP
#define MOVIE_BOOKING_SERVICE_HPP

#include <string>
#include <vector>
#include <memory>
#include <mutex>

 #include "movie.hpp"
 #include "theater.hpp"

class MovieBookingService {
public:

    // Initialize the service with movie and theater data
    MovieBookingService(const std::vector<Movie>& movies, const std::vector<Theater>& theaters);

    // Get a list of all playing movies
    std::vector<Movie> getAllMovies() const;

    // Get theaters showing a specific movie
    std::vector<Theater> getTheatersForMovie(int movieId) const;

    // Get available seats for a specific theater and movie
    // Returns true if seats were booked successfully, false otherwise
    std::vector<int> getAvailableSeats(int theaterId, int movieId) const;

    // Book seats for a specific theater and movie
    bool bookSeats(int theaterId, int movieId, const std::vector<int>& seatIds);

    // Check if a movie with a given ID exists
    bool isValidMovie(int movieId) const;
    
    // Validate and check if a theater shows a given movie
    bool isMovieShownInTheater(int theaterId, int movieId) const;

    // Get the name of a movie by its ID
    std::string getMovieName(int movieId) const;

    // Get the name of a theater by its ID
    std::string getTheaterName(int theaterId) const;

private:
    mutable std::mutex mBookingMutex; // Mutex for synchronization

    std::vector<Movie> mMovies;
    std::vector<Theater> mTheaters;

    std::unordered_map<int, std::vector<int>> mMovieTheaterAllocations; // Movie allocations to theaters
    
    //Allocate each movies to the Theaters
    void allocateMovieToTheaters();
    // Check if a theater with a given ID exists
    bool isValidTheater(int theaterId) const;

};

#endif // MOVIE_BOOKING_SERVICE_HPP

