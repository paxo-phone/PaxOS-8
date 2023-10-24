cmake_minimum_required( VERSION 3.18 )
project( Neper )

set(CMAKE_CXX_STANDARD 11)

file( GLOB_RECURSE src src/*.cpp )
set(CMAKE_CXX_FLAGS "-D DEBUG")
add_executable( main ${src} main.cpp )