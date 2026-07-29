cmake_minimum_required(VERSION 3.16)
project(RetroDcsDemo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

add_executable(retro_dcs_demo
    src/main.cpp
)

target_link_libraries(retro_dcs_demo PRIVATE Qt5::Widgets)

