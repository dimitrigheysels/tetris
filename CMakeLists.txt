cmake_minimum_required(VERSION 3.25)
project(MyTetris LANGUAGES CXX)

#set(SFML_STATIC_LIBRARIES ON)
# set(SFML_DIR "$ENV{HOME}/libs/SFML-3.0.0/lib/cmake/SFML")
# find_package(SFML 3 COMPONENTS Graphics Window System Audio REQUIRED)

include(FetchContent)

set(SFML_STATIC_LIBRARIES TRUE)

set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_VERBOSE_MAKEFILE ON) 
set(CMAKE_CXX_STANDARD 20)
set(BUILD_SHARED_LIBS OFF)


FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    )
    
FetchContent_MakeAvailable(SFML)


#add_compile_options(-g )

add_executable(MyTetris src/main.cpp src/game.cpp src/player_profile.cpp src/field.cpp src/tile.cpp src/block.cpp src/level.cpp src/sound_manager.cpp)



target_link_libraries(MyTetris PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio)
