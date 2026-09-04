cmake_minimum_required(VERSION 4.0)
project(gecs)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_library(gecs
        include/component.h
        include/entity.h
        include/gecs.h
        include/types.h
        include/system.h)
set_target_properties(gecs PROPERTIES LINKER_LANGUAGE CXX)

add_executable(gecs_increment_example
        include/component.h
        include/entity.h
        include/gecs.h
        include/types.h
        include/system.h
        examples/increment.cpp)