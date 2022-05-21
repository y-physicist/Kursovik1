cmake_minimum_required(VERSION 3.21)
project(Kursovik1)

set(CMAKE_CXX_STANDARD 14)

add_executable(Kursovik1 main.cpp general.h Button.h Textbox.h)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML-2.5.1/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(c:/SFML-2.5.1/include)
target_link_libraries(Kursovik1 sfml-system sfml-window sfml-graphics sfml-audio)