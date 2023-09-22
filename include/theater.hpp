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
     * @brief Constructor to initialize the theater with an ID and name.
     *
     * @param id The unique identifier for the theater.
     * @param name The name of the theater.
     */
    Theater(const int& id, const std::string& name);
    
    /**
     * @brief Book a seat in the theater by its ID.
     *
     * @param id The ID of the seat to be booked.
     * @return True if the seat was booked successfully, false otherwise.
     */
    bool bookSeat(const int& id);
    
    /**
     * @brief Get a vector of available seat IDs in the theater.
     *
     * @return A vector of integers representing the available seat IDs.
     */
    std::vector<int> getAvailableSeats() const;
    
    /**
     * @brief Get the name of the theater.
     *
     * @return The name of the theater as a string.
     */
    inline std::string getName() const { return mName;}
    
    /**
     * @brief Get the ID of the theater.
     *
     * @return The ID of the theater as an integer.
     */
    inline int getId () const { return mId; }
    
    /**
     * @brief Equality operator for comparing Theaters based on their IDs.
     *
     * @param rhs The theater to compare with.
     * @return True if the theater have the same ID, false otherwise.
     */
    bool operator == (const Theater &rhs) const;

    
private:
    int mId;                    /**< Unique identifier for the theater. */
    std::string mName;          /**< Name of the theater. */
    const int mSeatCapacity;    /**< Capacity of the theater in terms of seats. */
    std::vector<Seat> mSeats;   /**< Vector of seats in the theater. */

    /**
     * @brief Initialize the seats in the theater.
     *
     * This function initializes the seats in the theater based on the specified seat capacity.
     */
    void initializeSeats();
};

#endif /* THEATER_HPP */
