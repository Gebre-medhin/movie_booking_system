# Movie booking service

This is an implementation of a backend service in C++ which is used by some other services ( e.g., CLI) for booking online movie tickets. Booking a ticket means just blocking
the required number of particular seats.
The end-user can perform the following actions through CLI:
1. View all playing movies
2. Select a movie
3. See all theaters showing the movie
4. Select a theater
5. See available seats for the selected theater & movie (I assumed that all theaters have 20 seats capacity)
6. Book one or more of the available seats. Service can handle
multiple requests simultaneously (no over-bookings) with the help of mutex syncronization.
   

You can build the code using the following steps:

     1. mkdir bld  //-->if it does not already exist
     2. cd bld
     3. cmake .. 
     4. make
     
After successful build, you can test the code using the following steps:

     1. ./main       //-> To test it using CLI
     4. make test     //-> To run the unit tests
