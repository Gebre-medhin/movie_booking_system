/**
 * @file movie.hpp
 * @brief Represents a movie with an ID, and name. 
 * @author Gebremedhin Abreha
 */
#ifndef MOVIE_HPP
#define MOVIE_HPP

#include <string>

/**
 * @struct Movie
 * @brief Represents a movie with an ID, and movie name.
 */
struct Movie {
    int id;             /**< Unique identifier for the movie. */
    std::string name;  /**< Name of the movie. */
    bool isAllocated;   /**< Flag indicating if the movie is allocated to a theater. */
    
    /**
     * @brief Constructor to initialize the movie with an ID and name.
     *
     * @param id_ The unique identifier for the movie.
     * @param name_ The name of the movie.
     */
    Movie(int id_, const std::string& name_) : id(id_), name(name_) { }
    
    Movie(Movie&& other) = default;

    Movie& operator=(Movie&& other) = default;

    
    /**
     * @brief Equality operator for comparing movies based on their  unique IDs.
     *
     * @param rhs The movie to compare with.
     * @return True if the movie have the same ID, false otherwise.
     */
    inline bool operator==(const Movie& rhs) const
    {
        if (id == rhs.id)
            return true;
        return false;
    }
};

#endif /* MOVIE_HPP */
