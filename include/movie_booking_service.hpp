/**
 * @file movie_booking_service.hpp (API header)
 * @brief Movie booking service API.
 * @author Gebremedhin Abreha
 */

#ifndef MOVIE_BOOKING_SERVICE_HPP
#define MOVIE_BOOKING_SERVICE_HPP

#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <map>

#include "movie.hpp"
#include "theater.hpp"

/**
 * @class MovieBookingService
 * @brief A class representing a movie booking service.
 *
 * The MovieBookingService class provides methods for managing movie bookings,
 * theaters, and seat allocations.
 */
class MovieBookingService {
public:

    /**
     * @brief Constructor
     */
     MovieBookingService() = default;

    /**
     * @brief Destructor for the MovieBookingService class.
     */
     ~MovieBookingService() = default;
    
    /**
      * @brief Add a movie to the list of available movies.
      *
      * @param movie A shared pointer to the movie to be added.
      * @return True if movie is added successfully , false otherwise
      */
     bool addMovie(std::shared_ptr<Movie> movie);
     
     /**
      * @brief Add a theater to the list of available theaters.
      *
      * @param theater A shared pointer to the theater to be added.
      * @return True if theater is added successfully , false otherwise
      */
    bool addTheater(std::shared_ptr<Theater> theater);

    /**
     * @brief Get a list of all playing movies.
     *
     * @return A vector of Movie objects representing available movies.
     */
    std::vector<std::shared_ptr<Movie> > getAllMovies() const;
    
    /**
     * @brief Get theaters showing a specific movie.
     *
     * @param movieId The ID of the movie.
     * @return A vector of Theater objects showing the specified movie.
     */
    std::vector<std::shared_ptr<Theater>> getTheatersForMovie(int movieId) const;
    
    /**
     * @brief Get available (free/unbooked) seats for a specific theater and movie.
     *
     * @param theaterId The ID of the theater.
     * @return A vector of seat IDs representing available seats.
     */
    std::vector<int> getAvailableSeats(int theaterId ) const;

    /**
     * @brief Book seats for a specific theater and movie.
     *
     * @param theaterId The ID of the theater.
     * @param seatIds A vector of seat IDs to be booked.
     * @return True if seats were booked successfully, false otherwise.
     */
    bool bookSeats(int theaterId, const std::vector<int>& seatIds);
    
    /**
     * @brief Check if a movie with a given ID exists.
     *
     * @param movieId The ID of the movie to check.
     * @return True if the movie exists, false otherwise.
     */
    bool isValidMovie(int movieId) const;
    
    /**
     * @brief Validate and check if a theater shows a given movie.
     *
     * @param theaterId The ID of the theater to check.
     * @param movieId The ID of the movie to check.
     * @return True if the movie is shown in the theater, false otherwise.
     */
    bool isMovieShownInTheater(int theaterId, int movieId) const;
    
    /**
     * @brief Get the name of a movie by its ID.
     *
     * @param movieId The ID of the movie.
     * @return The name of the movie.
     * @note Can throw invalid_argument exception
     */
    std::string getMovieName(int movieId) const;
    
    /**
     * @brief Get the name of a theater by its ID.
     *
     * @param theaterId The ID of the theater.
     * @return The name of the theater.
     * @note Can throw invalid_argument exception
     */
    std::string getTheaterName(int theaterId) const;
    
private:

    mutable std::mutex mBookingMutex;  /**< Mutex for synchronization of booking operations. */

    std::map<int, std::shared_ptr<Movie>> mMovies; /**< Stores movie data*/

    std::map<int, std::shared_ptr<Theater>> mTheaters; /**< Stores theater  data*/
    /**
     * @brief A map to track movie allocations to theaters.
     *
     * This map associates movie IDs with vectors of theater IDs to represent
     * which theaters are allocated for each movie.
     */
    std::map<int, std::vector<int>> mMovieTheaterAllocations;
    
    /**
     * @brief Allocate a movie to one or more theaters.
     *
     * This method allocates the specified movie to one or more theaters.
     *
     * @param movie A shared pointer to the movie to allocate to theaters.
     * @return True if the allocated, false otherwise.
     */
    bool allocateMovieToTheaters(std::shared_ptr<Movie> movie);
    
    /**
     * @brief Check if a theater with a given ID exists.
     *
     * @param theaterId The ID of the theater to check.
     * @return True if the theater exists, false otherwise.
     */
    bool isValidTheater(int theaterId) const;

};

#endif // MOVIE_BOOKING_SERVICE_HPP

