cmake_minimum_required(VERSION 3.10) 
project(SURFADVSRY) 
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 
set(CMAKE_CXX_STANDARD 20)
 
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared) 
find_package(SDL3_image REQUIRED) 
find_package(SDL3_ttf REQUIRED) 
 
add_executable(SURFADVSRY src/emu/main.cpp src/emu/memorymap.cpp src/emu/tests/memory.cpp) 
 
target_link_libraries(SURFADVSRY PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf) 
