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
     * @brief Constructor that initializes the service with movie and theater data.
     *
     * @param movies Movies to initialize the service with.
     * @param theaters Theaters to initialize the service with.
     */
    MovieBookingService(const std::vector<Movie>& movies, const std::vector<Theater>& theaters);
    
    /**
     * @brief Destructor for the MovieBookingService class.
     */
    virtual ~MovieBookingService() = default;

    /**
     * @brief Get a list of all playing movies.
     *
     * @return A vector of Movie objects representing available movies.
     */
    virtual std::vector<Movie> getAllMovies() const;
    
    /**
     * @brief Get theaters showing a specific movie.
     *
     * @param movieId The ID of the movie.
     * @return A vector of Theater objects showing the specified movie.
     */
    virtual std::vector<Theater> getTheatersForMovie(int movieId) const;
    
    /**
     * @brief Get available (free/unbooked) seats for a specific theater and movie.
     *
     * @param theaterId The ID of the theater.
     * @param movieId The ID of the movie.
     * @return A vector of seat IDs representing available seats.
     */
    virtual std::vector<int> getAvailableSeats(int theaterId, int movieId) const;

    /**
     * @brief Book seats for a specific theater and movie.
     *
     * @param theaterId The ID of the theater.
     * @param movieId The ID of the movie.
     * @param seatIds A vector of seat IDs to be booked.
     * @return True if seats were booked successfully, false otherwise.
     */
    virtual bool bookSeats(int theaterId, int movieId, const std::vector<int>& seatIds);
    
    /**
     * @brief Check if a movie with a given ID exists.
     *
     * @param movieId The ID of the movie to check.
     * @return True if the movie exists, false otherwise.
     */
    virtual bool isValidMovie(int movieId) const;
    
    /**
     * @brief Validate and check if a theater shows a given movie.
     *
     * @param theaterId The ID of the theater to check.
     * @param movieId The ID of the movie to check.
     * @return True if the movie is shown in the theater, false otherwise.
     */
    virtual bool isMovieShownInTheater(int theaterId, int movieId) const;
    
    /**
     * @brief Get the name of a movie by its ID.
     *
     * @param movieId The ID of the movie.
     * @return The name of the movie.
     * @note Can throw invalid_argument exception
     */
    virtual std::string getMovieName(int movieId) const;
    
    /**
     * @brief Get the name of a theater by its ID.
     *
     * @param theaterId The ID of the theater.
     * @return The name of the theater.
     * @note Can throw invalid_argument exception
     */
    virtual std::string getTheaterName(int theaterId) const;

private:
    mutable std::mutex mBookingMutex; // Mutex for synchronization

    std::vector<Movie> mMovies; // Stores movie data
    std::vector<Theater> mTheaters; // Stores theater data

    std::unordered_map<int, std::vector<int>> mMovieTheaterAllocations; // Movie allocations to theaters
    
    /**
     * @brief Allocate each movie to theaters. It allocates each movie to one or more theaters.
     * @note If the number of movies are greater than the number of theaters, throws invalid_argument exception
     */
    void allocateMovieToTheaters();
    
    /**
     * @brief Check if a theater with a given ID exists.
     *
     * @param theaterId The ID of the theater to check.
     * @return True if the theater exists, false otherwise.ß
     */
    bool isValidTheater(int theaterId) const;

};

#endif // MOVIE_BOOKING_SERVICE_HPP

