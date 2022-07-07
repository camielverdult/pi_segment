cmake_minimum_required(VERSION 3.18)
project(pi_segment C CXX)

set (CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} wiringPi)
target_link_libraries(${PROJECT_NAME} bcm2835)