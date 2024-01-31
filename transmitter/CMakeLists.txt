cmake_minimum_required(VERSION 3.28)
project(transmitter)

set(CMAKE_CXX_STANDARD 17)

add_executable(transmitter main.cpp Classes/Sources/transmitter.cpp Classes/Headers/transmitter.h)
