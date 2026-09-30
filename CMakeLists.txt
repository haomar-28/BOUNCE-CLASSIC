cmake_minimum_required(VERSION 3.15)
project(BounceClassic C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Raylib is built from the source included in dependencies/raylib (no download needed).
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_subdirectory(dependencies/raylib)

# Put the finished game in ./bin (same place on every OS and build type).
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
foreach(cfg DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
  set("CMAKE_RUNTIME_OUTPUT_DIRECTORY_${cfg}" "${CMAKE_SOURCE_DIR}/bin")
endforeach()

add_executable(BounceClassic finalGameShowdown.c)
target_link_libraries(BounceClassic PRIVATE raylib)
if(NOT WIN32)
  target_link_libraries(BounceClassic PRIVATE m)
endif()
if(MSVC)
  target_compile_definitions(BounceClassic PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
# The game loads "assets/..." and "leaderboard.txt" relative to the project folder.
set_target_properties(BounceClassic PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
