cmake_minimum_required(VERSION 3.25)
project(MyTetris LANGUAGES CXX)

#set(SFML_STATIC_LIBRARIES ON)
# set(SFML_DIR "$ENV{HOME}/libs/SFML-3.0.0/lib/cmake/SFML")
# find_package(SFML 3 COMPONENTS Graphics Window System Audio REQUIRED)

include(FetchContent)

#set(SFML_STATIC_LIBRARIES TRUE)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON) 
set(CMAKE_CXX_STANDARD 23)
set(BUILD_SHARED_LIBS OFF)

FetchContent_Declare(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.15.3
)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    )
    
FetchContent_MakeAvailable(spdlog SFML)


#add_compile_options(-g)
#add_compile_options(-std=c++20)

add_executable(MyTetris src/main.cpp src/game.cpp src/player_profile.cpp src/field.cpp src/tile.cpp src/block.cpp src/level.cpp src/sound_manager.cpp)


target_include_directories(MyTetris PUBLIC "${CMAKE_CURRENT_LIST_DIR}/build/_deps/spdlog-src/include")

target_link_libraries(MyTetris PRIVATE spdlog::spdlog SFML::Graphics SFML::Window SFML::System SFML::Audio)
