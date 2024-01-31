cmake_minimum_required(VERSION 3.28)
project(data_exchange)

set(CMAKE_CXX_STANDARD 17)

add_executable(data_exchange main.cpp)
add_subdirectory(transmitter reciever)