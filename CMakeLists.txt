cmake_minimum_required(VERSION 3.10) 
project(Emulator)
project(Assembler)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
 
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared) 
find_package(SDL3_image REQUIRED) 
find_package(SDL3_ttf REQUIRED) 
 
add_executable(Emulator src/emu/main.cpp src/emu/CPU.cpp src/emu/memorymap.cpp src/emu/screen.cpp src/emu/graphics_SDL.cpp src/emu/tests/memory.cpp src/emu/tests/cpu.cpp) 
add_executable(Assembler src/asm/main.cpp src/asm/tokenizer.cpp src/asm/lexer.cpp src/asm/compiler.cpp)
 
target_link_libraries(Emulator PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf) 
target_link_libraries(Assembler PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf) 
