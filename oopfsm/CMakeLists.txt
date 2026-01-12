cmake_minimum_required(VERSION 4.0)
project(oopfsm C)

set(CMAKE_C_STANDARD 11)

add_library(myfsm STATIC myfsm.c)

add_executable(app)

target_sources(app PUBLIC main.c)
target_link_libraries(app myfsm)