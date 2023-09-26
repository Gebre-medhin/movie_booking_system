# Add the 'include' directory to the include path
include_directories(../include)

include_directories("${PROJECT_SOURCE_DIR}")

add_executable(main_test main_test.cpp)
target_link_libraries(main_test gtest)

add_executable(movie_booking_service movie_booking_service_test.cpp ../src/movie_booking_service.cpp ../src/theater.cpp )
target_link_libraries(movie_booking_service gtest gmock_main)
add_test(NAME movie_booking_service_tests COMMAND movie_booking_service)
