cmake_minimum_required (VERSION 2.6)

project (FastFileReader)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(FastFileReader fastfilereader.cpp FastQ.h FastQ.cpp)
