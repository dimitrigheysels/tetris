cmake_minimum_required(VERSION 3.25)
project(MyTetris LANGUAGES CXX)

#set(SFML_STATIC_LIBRARIES ON)
set(SFML_DIR "$ENV{HOME}/libs/SFML-3.0.0/lib/cmake/SFML")
find_package(SFML 3 COMPONENTS Graphics Window System Audio REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_compile_options(-g -rdynamic)

add_executable(MyTetris src/main.cpp src/ui.cpp src/game.cpp src/player_profile.cpp src/field.cpp src/tile.cpp src/block.cpp src/level.cpp)

target_link_libraries(MyTetris PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio)
