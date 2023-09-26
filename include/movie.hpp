/**
 * @file movie.hpp
 * @brief Represents a movie with an ID, and name. 
 * @author Gebremedhin Abreha
 */
/**
 * @file theater.hpp

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
    
    /**
     * @brief Equality operator for comparing movies based on their  unique IDs.
     *
     * @param rhs The movie to compare with.
     * @return True if the movie have the same ID, false otherwise.
     */
    inline bool operator==(const Movie& rhs) const {
        if (id == rhs.id)
            return true;
        return false;
    }
};


///**
// * @class Movie
// * @brief Represents a movie with an ID and a name.
// */
//class Movie {
//
//public:
//    /**
//     * @brief Constructor to initialize the movie with an ID and name.
//     *
//     * @param id The unique identifier for the movie.
//     * @param name The name of the movie.
//     */
//    Movie (const int& id, const std::string& name);
//
//    /**
//     * @brief Destructor for the Movie class.
//     */
//   virtual  ~Movie() = default;
//    /**
//     * @brief Get the name of the movie.
//     *
//     * @return The name of the movie as a string.
//     */
//    virtual std::string getName() const;
//
//    /**
//     * @brief Get the ID of the movie.
//     *
//     * @return The ID of the movie as an integer.
//     */
//    virtual int getId () const;
//
//    /**
//     * @brief Equality operator for comparing Movies based on their IDs.
//     *
//     * @param rhs The Movie to compare with.
//     * @return True if the Movies have the same ID, false otherwise.
//     */
//    virtual bool operator == (const Movie &rhs) const;
//
//private:
//    int mId; /**< Unique identifier for the movie. */
//    std::string mName; /**< Name of the movie. */
//
//};
//
#endif /* MOVIE_HPP */
