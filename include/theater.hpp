/**
 * @file theater.hpp
 * @brief Represents a theater with an ID, name, and seats.
 * @author Gebremedhin Abreha
 */
#ifndef THEATER_HPP
#define THEATER_HPP

#include <string>
#include <vector>
#include "seat.hpp"

/**
 * @class Theater
 * @brief Represents a theater with an ID, name, and seats.
 *
 * The Theater class provides methods for managing seats in a theater.
 */
class Theater {
public:
    /**
     * @brief Constructor to initialize the theater with an ID, name and seats
     *
     * @param id The unique identifier for the theater.
     * @param name The name of the theater.
     * @param seats A vector of Seat objects representing seats in the theater.
     */
    Theater(const int& id, const std::string& name, const std::vector<Seat>& seats);
    
    /**
     * @brief Destructor for the Movie class.
     */
    virtual ~Theater () = default;
    
    /**
     * @brief Book a seat in the theater by its ID.
     *
     * @param id The ID of the seat to be booked.
     * @return True if the seat was booked successfully, false otherwise.
     */
    virtual bool bookSeat(const int& id);
    
    /**
     * @brief Get a vector of available seat IDs in the theater.
     *
     * @return A vector of integers representing the available seat IDs.
     */
    virtual std::vector<int> getAvailableSeats() const;
    
    /**
     * @brief Get the name of the theater.
     *
     * @return The name of the theater as a string.
     */
    virtual std::string getName() const;
    
    /**
     * @brief Get the ID of the theater.
     *
     * @return The ID of the theater as an integer.
     */
    virtual int getId () const;
    
    /**
      * @brief Allocate a movie to this theater.
      */
     virtual void allocateMovie ();
     
     /**
      * @brief Check if a movie is allocated to this theater.
      *
      * @return True if a movie is allocated, false otherwise.
      */
    virtual bool isAllocated () const;
    
    /**
     * @brief Equality operator for comparing Theaters based on their IDs.
     *
     * @param rhs The theater to compare with.
     * @return True if the theater have the same ID, false otherwise.
     */
    virtual bool operator == (const Theater &rhs) const;

protected:
    int mId;                    /**< Unique identifier for the theater. */
    std::string mName;          /**< Name of the theater. */
    std::vector<Seat> mSeats;   /**< Vector of seats in the theater. */
    bool mIsAllocated;          /**< Flag indicating if a movie is allocated to the theater. */

};

#endif /* THEATER_HPP */
