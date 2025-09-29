cmake_minimum_required(VERSION 3.5)
project(numerical-methods-cource-work)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/libs)

execute_process(
        COMMAND git clone -b SDL2 "https://github.com/libsdl-org/SDL" sdl2
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
add_subdirectory(${CMAKE_BINARY_DIR}/libs/sdl2 sdl2)


add_executable(main
        main.cpp
)

target_link_libraries(main SDL2)