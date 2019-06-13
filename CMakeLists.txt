cmake_minimum_required(VERSION 3.9)
project(LockedShmemTest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread -std=c++11 -Wno-long-long")

add_executable(LockedShmemTest main.cpp LockedShMemTest.cpp LockedShMemTest.h)

target_link_libraries(LockedShmemTest rt)