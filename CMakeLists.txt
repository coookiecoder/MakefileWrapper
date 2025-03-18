cmake_minimum_required(VERSION 3.30)
project(MakefileWrapper)

set(CMAKE_CXX_STANDARD 23)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(MakefileWrapper main.cpp main_menu.cpp display_menu.cpp)

target_link_libraries(MakefileWrapper ${CURSES_LIBRARIES})