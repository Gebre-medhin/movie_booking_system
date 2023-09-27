/**
 * @file main.cpp
 * @brief Main program for testing movie booking service functionalities via CLI
 * @author Gebremedhin Abreha
 */

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>

#include "movie_booking_service.hpp"
#include "theater.hpp"
#include "movie.hpp"
#include "seat.hpp"

/**
 * @brief Main function to run the movie booking service CLI.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return Exit code.
 */
int main(int argc, const char * argv[]) {
    
    // Initialize the movie booking service with movie data
    std::vector<std::shared_ptr<Movie>> movies = {
        {std::make_shared<Movie>(1, "Movie01")},
        {std::make_shared<Movie>(2, "Movie02")},
        {std::make_shared<Movie>(3, "Movie03")},
        {std::make_shared<Movie>(4, "Movie04")},
        // Add more movies as needed, but not more than the number of theaters
    };
    
    const int seatCapacity = 20; //Number of seats for each theater
    std::vector<Seat> seats;
    
    //Initialize seats
    for (int i = 0; i < seatCapacity; ++i)
    {
            Seat seat;
            seat.id = i;
            seat.seatNumber = "Seat " + std::to_string(i + 1);
            seat.isBooked = false;
            seats.push_back(seat);
    }
    
    std::vector<std::shared_ptr<Theater>> theaters = {
        {std::make_shared<Theater>(1, "Theater01", seats)},
        {std::make_shared<Theater>(2, "Theater02", seats)},
        {std::make_shared<Theater>(3, "Theater03", seats)},
        {std::make_shared<Theater>(4, "Theater04", seats)},
        {std::make_shared<Theater>(5, "Theater05", seats)},
        {std::make_shared<Theater>(6, "Theater06", seats)},
        {std::make_shared<Theater>(7, "Theater07", seats)},
        {std::make_shared<Theater>(8, "Theater08", seats)},
        {std::make_shared<Theater>(9, "Theater09", seats)},
        {std::make_shared<Theater>(10, "Theater10",seats)},
        //Add more movies as needed
    };

    MovieBookingService bookingService;
    
    
    for (const auto& movie : movies)
    {
        bookingService.addMovie(movie);
    }
    
    for (const auto& theater : theaters)
    {
        bookingService.addTheater(theater);
    }

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
        if (!(std::cin >> choice))
        {
           // Clear the failure state of cin and ignore any remaining characters in the input buffer.
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1:
                // View All Movies
                std::cout << "Available Movies:" << std::endl;
                for (const auto& movie : bookingService.getAllMovies()) {
                    std::cout << "Name: " << movie->name<< "," "Id: "<< movie->id<< std::endl;
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
                        std::cout << "Name: "<<theater->getName() <<", "<<"Id: "<< theater->getId() << std::endl;
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
                    for (const auto& seatId : bookingService.getAvailableSeats(selectedTheaterId)) {
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

                    if (bookingService.bookSeats(selectedTheaterId, seatIds)) {
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

