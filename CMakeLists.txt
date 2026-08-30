cmake_minimum_required(VERSION 4.0)
project(gecs)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_library(gecs
        include/component.h
        include/entity.h
        src/entity.cpp
        include/gecs.h
        src/component.cpp
        include/types.h
        include/system.h
        src/system.cpp)
