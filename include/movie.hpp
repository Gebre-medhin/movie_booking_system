
#ifndef MOVIE_HPP
#define MOVIE_HPP

#include <string>

/**
 * @class Movie
 * @brief Represents a movie with an ID and a name.
 */
class Movie {
    int mId; /**< Unique identifier for the movie. */
    std::string mName; /**< Name of the movie. */
public:
    /*----------------------------------------------------*/
    /**
     * @brief Constructor to initialize the movie with an ID and name.
     *
     * @param id The unique identifier for the movie.
     * @param name The name of the movie.
     */
    Movie (const int& id, const std::string& name):
    mId(id),mName(name){ }
    
    /*----------------------------------------------------*/
    /**
     * @brief Get the name of the movie.
     *
     * @return The name of the movie as a string.
     */
    inline std::string getName() const { return mName;}
    
    /*----------------------------------------------------*/
    /**
     * @brief Get the ID of the movie.
     *
     * @return The ID of the movie as an integer.
     */
    inline int getId () const { return mId; }
    
    /*----------------------------------------------------*/
    /**
     * @brief Equality operator for comparing Movies based on their IDs.
     *
     * @param rhs The Movie to compare with.
     * @return True if the Movies have the same ID, false otherwise.
     */
    inline bool operator == (const Movie &rhs) const {
        if (mId == rhs.mId)
            return true;
        return false;
    }
};

#endif /* MOVIE_HPP */
