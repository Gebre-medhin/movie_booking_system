cmake_minimum_required(VERSION 3.10)  # CMake version check
project(simple_example)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard

# Add main.cpp file of the project root directory as a source file
set(SOURCE_FILES main.cpp)
set(LIBRARY_FILES movie_booking_service.cpp theater.cpp movie.cpp)

add_library (lib ${LIBRARY_FILES})

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(main ${SOURCE_FILES})

target_link_libraries(main PRIVATE lib)




