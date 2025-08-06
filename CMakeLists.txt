cmake_minimum_required(VERSION 3.22)
project(MyTetris LANGUAGES CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON) 
set(CMAKE_CXX_STANDARD 23)
set(BUILD_SHARED_LIBS ON)

FetchContent_Declare(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.15.3
    GIT_PROGRESS TRUE
)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_PROGRESS TRUE
    )

FetchContent_Declare(json 
    GIT_REPOSITORY https://github.com/nlohmann/json/
    GIT_TAG v3.12.0
    GIT_PROGRESS TRUE )

    
FetchContent_MakeAvailable(spdlog)
FetchContent_MakeAvailable(SFML)
FetchContent_MakeAvailable(json)


#add_compile_options(-g)
#add_compile_options(-std=c++20)

add_executable(MyTetris src/main.cpp src/game.cpp src/player_profile.cpp src/field.cpp src/tile.cpp src/block.cpp src/level.cpp src/sound_manager.cpp src/menu.cpp src/state.cpp src/state_gameover.cpp src/state_menu.cpp src/state_paused.cpp src/state_playing.cpp src/resource_manager.cpp)

target_link_libraries(MyTetris PRIVATE nlohmann_json::nlohmann_json spdlog::spdlog SFML::Graphics SFML::Window SFML::System SFML::Audio)
