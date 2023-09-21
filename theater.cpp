//
//  theater.cpp
//
//  Created by Gebremedhin Abreha on 21/09/2023.
//

#include "theater.hpp"

Theater::Theater (const int& id, const std::string& name):
mId(id), mName(name), mSeatCapacity(20)
{
    intializeSeats();
}

bool Theater::bookSeat(const int& seatId)
{
    for (auto& seat: mSeats)
    {
        if (seat.id == seatId)
        {
            if (seat.isBooked)
                return false; //Already booked
            else
            {
                seat.isBooked = true;
                return true;
            }
        }
    }
    
    return false;
}

std::vector<int> Theater::getAvailableSeats() const
{
    std::vector<int> availableSeats;
    
    for (const auto& seat: mSeats)
    {
        if(!seat.isBooked)
        {
            availableSeats.push_back(seat.id);
        }
    }
        
    return availableSeats;
}

void Theater::intializeSeats ()
{
    for (int i = 0; i < mSeatCapacity; ++i) {
        Seat seat;
        seat.id = i;
        seat.seatNumber = "Seat " + std::to_string(i + 1);
        seat.isBooked = false;
        mSeats.push_back(seat);
    }
}

