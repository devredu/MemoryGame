cmake_minimum_required(VERSION 3.15)
project(MemoryGame)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

add_subdirectory(external/raylib)

add_executable(MemoryGame
        src/main.cpp
        include/user_interface.h
        src/user_interface.cpp
        include/game_logic.h
        src/game_logic.cpp
        include/common.h
)

target_link_libraries(MemoryGame PRIVATE
        raylib
)

if(EXISTS "${CMAKE_SOURCE_DIR}/assets")
    add_custom_command(TARGET MemoryGame POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets"
            "$<TARGET_FILE_DIR:MemoryGame>/assets")
endif()