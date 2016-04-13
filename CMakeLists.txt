cmake_minimum_required(VERSION 3.3)
project(LibHeaderGen)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(HEADER_FILES
        src/lineentry.h
        src/outputfile.h
        src/misc.h)

set(SOURCE_FILES
        src/main.cpp
        src/lineentry.cpp
        src/outputfile.cpp
        src/misc.cpp)

add_executable(LibHeaderGen ${HEADER_FILES} ${SOURCE_FILES})