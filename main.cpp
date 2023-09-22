//
//  main.cpp
//  Movie booking service
//
//  Created by Gebremedhin Abreha on 21/09/2023.

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "movie_booking_service.hpp" // Include your MovieBookingService class header
#include "theater.hpp"
#include "movie.hpp"
#include "seat.hpp"

int main(int argc, const char * argv[]) {
    // Initialize the movie booking service with movie data
    std::vector<Movie> movies = {
        {1, "Movie A"},
        {2, "Movie B"},
        {3, "Movie C"},
        {4, "Movie D"},
        // Add more movies as needed, but not more than the number of theaters
    };
    
    const int seatCapacity = 20; //Number of seats for each theater
    std::vector<Seat> seats(seatCapacity);
    
    std::vector<Theater> theaters = {
        {Theater(1, "Theater One")},
        {Theater(2, "Theater Two")},
        {Theater(3, "Theater Three")},
        {Theater(4, "Theater Four")},
        {Theater(5, "Theater Five")},
        {Theater(6, "Theater Six")},
        {Theater(7, "Theater Seven")},
        {Theater(8, "Theater Eight")},
        {Theater(9, "Theater Nine")},
        {Theater(10, "Theater Ten")},
        //Add more movies as needed
    };

    MovieBookingService bookingService(movies, theaters);

    int selectedMovieId = -1;
    int selectedTheaterId = -1;

    while (true) {
        std::cout << std::endl<<"==== Movie Booking System ====" << std::endl;
        std::cout << "1. View All Movies" << std::endl;
        std::cout << "2. Select a Movie" << std::endl;
        std::cout << "3. See Theaters showing the selected movie" << std::endl;
        std::cout << "4. Select a Theater for the move you have selected" << std::endl;
        std::cout << "5. See Available Seats for the slected movie and theater" << std::endl;
        std::cout << "6. Book Seats" << std::endl;
        std::cout << "7. Exit" << std::endl;
        std::cout << "=============================" << std::endl;

        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                // View All Movies
                std::cout << "Available Movies:" << std::endl;
                for (const auto& movie : bookingService.getAllMovies()) {
                    std::cout << "Name: " << movie.getName() << "," "Id: "<< movie.getId() << std::endl;
                }
                break;

            case 2:
                // Select a Movie
                int movieChoice;
                std::cout << "Enter the Movie ID: ";
                std::cin >> movieChoice;

                // Check if the selected movie exists
                if (bookingService.isValidMovie(movieChoice)) {
                    selectedMovieId = movieChoice;
                    std::cout << "Selected movie name: " << bookingService.getMovieName(selectedMovieId) << std::endl;
                } else {
                    std::cout << "Invalid movie ID." << std::endl;
                }
                break;

            case 3:
                // See Theaters
                if (selectedMovieId != -1) {
                    std::cout << "Theaters for the selected movie ( " << bookingService.getMovieName(selectedMovieId) << "):" << std::endl;
                    for (const auto& theater : bookingService.getTheatersForMovie(selectedMovieId)) {
                        std::cout << "Name: "<<theater.getName() <<", "<<"Id: "<< theater.getId() << std::endl;
                    }
                } else {
                    std::cout << "Please select a movie first." << std::endl;
                }
                break;

            case 4:
                // Select a Theater
                int theaterChoice;
                std::cout << "Enter the ID of the theater you want to select: ";
                std::cin >> theaterChoice;

                // Check if the selected theater exists
                if (bookingService.isMovieShownInTheater(theaterChoice, selectedMovieId)) {
                    selectedTheaterId = theaterChoice;
                    std::cout << "Selected Theater: " << bookingService.getTheaterName(selectedTheaterId) << std::endl;
                } else {
                    std::cout << "Invalid Theater ID or not available for the selected movie." << std::endl;
                }
                break;

            case 5:
                // See Available Seats
                if (selectedTheaterId != -1) {
                    std::cout << "Available Seats for " << bookingService.getTheaterName(selectedTheaterId) << ":" << std::endl;
                    for (const auto& seatId : bookingService.getAvailableSeats(selectedTheaterId, selectedMovieId)) {
                        std::cout << "Seat Id: "<< seatId << std::endl;
                    }
                } else {
                    std::cout << "Please select a theater first." << std::endl;
                }
                break;

            case 6:
                // Book Seats
                if (selectedTheaterId != -1) {
                    std::cout << "Enter the seat IDs (comma-separated, e.g., 1,2,3): ";
                    std::string seatIdsInput;
                    std::cin.ignore(); // Clear newline character from previous input
                    std::getline(std::cin, seatIdsInput);

                    std::vector<int> seatIds;
                    std::stringstream ss(seatIdsInput);
                    int seatId;
                    while (ss >> seatId) {
                        seatIds.push_back(seatId);
                        if (ss.peek() == ',') {
                            ss.ignore();
                        }
                    }

                    if (bookingService.bookSeats(selectedTheaterId, selectedMovieId, seatIds)) {
                        std::cout << "Seats booked successfully!" << std::endl;
                    } else {
                        std::cout << "Failed to book seats. Please check seat availability and seat IDs." << std::endl;
                    }
                } else {
                    std::cout << "Please select a theater first." << std::endl;
                }
                break;

            case 7:
                // Exit
                return 0;

            default:
                std::cout << "Invalid choice. Please select a valid option." << std::endl;
                break;
        }
    }

    return 0;
}

