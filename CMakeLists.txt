cmake_minimum_required(VERSION 3.25)
project(MyTetris LANGUAGES CXX)

include(FetchContent)

set(SFML_STATIC_LIBRARIES TRUE)

FetchContent_Declare(SFML
 GIT_REPOSITORY https://github.com/SFML/SFML.git
 GIT_TAG 2.5.1)
FetchContent_MakeAvailable(SFML)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_compile_options(-g -rdynamic)

add_executable(MyTetris src/main.cpp src/ui.cpp src/game.cpp src/player.cpp src/field.cpp src/tile.cpp src/block.cpp)

target_link_libraries(MyTetris PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)
