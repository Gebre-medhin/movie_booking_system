//
//  theater.hpp
//  booking_sytem
//
//  Created by Gebremedhin Abreha on 21/09/2023.
//

#ifndef THEATER_HPP
#define THEATER_HPP

#include <stdio.h>
#include <string>
#include <vector>

#include "seat.hpp"


class Theater {

public:
    Theater (const int& id, const std::string& name);
    
    bool bookSeat(const int& id);
    
    std::vector<int> getAvailableSeats() const;
    
    inline std::string getName() const { return mName;}
    
    inline int getId () const { return mId; }

    
private:
    int mId;
    std::string mName;
    const int mSeatCapacity;
    std::vector<Seat> mSeats; // Vector of seats in the theater

    void intializeSeats();
    
};

#endif /* THEATER_HPP */


