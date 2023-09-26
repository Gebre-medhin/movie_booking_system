/**
 * @file theater.cpp
 * @brief Implementation for Theater class
 * @author Gebremedhin Abreha
 */

#include "theater.hpp"

/*----------------------------------------------------*/
Theater::Theater (const int& id, const std::string& name, const std::vector<Seat>& seats):
mId(id), mName(name), mSeats(seats), mIsAllocated(false)
{
}

/*----------------------------------------------------*/
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

/*----------------------------------------------------*/
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

/*----------------------------------------------------*/
std::string Theater::getName() const
{
    return mName;
    
}

/*----------------------------------------------------*/
int Theater::getId() const
{
    return mId;
    
}

/*----------------------------------------------------*/
void Theater::allocateMovie ()
{
    mIsAllocated = true;
}

/*----------------------------------------------------*/
bool Theater::isAllocated () const
{
    return mIsAllocated;
}

/*----------------------------------------------------*/
bool Theater::operator == (const Theater &rhs) const
{
    if (mId == rhs.mId)
        return true;
    return false;
}
/*-------------------END-------------------------------*/
