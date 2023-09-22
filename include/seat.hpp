/**
 * @file seat.hpp
 * @author Gebremedhin Abreha
 */
#ifndef SEAT_HPP
#define SEAT_HPP

#include <string>

/**
 * @struct Seat
 * @brief Represents a seat with an ID, seat number, and booking status.
 */
struct Seat {
    int id;             /**< Unique identifier for the seat. */
    std::string seatNumber; /**< Seat number or identifier. */
    bool isBooked;      /**< True if the seat is booked, false if it's available. */
    
    /*----------------------------------------------------*/
    /**
     * @brief Equality operator for comparing seats based on their  unique IDs.
     *
     * @param rhs The seat to compare with.
     * @return True if the seats have the same ID, false otherwise.
     */
    inline bool operator==(const Seat& rhs) const {
        if (id == rhs.id)
            return true;
        return false;
    }
};

#endif /* SEAT_HPP */
