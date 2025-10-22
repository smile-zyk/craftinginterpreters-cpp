cmake_minimum_required(VERSION 3.10)

project(lox-cpp LANGUAGES C CXX)

find_package(magic_enum CONFIG REQUIRED)

add_executable(${PROJECT_NAME} 
src/main.cc
src/lox.h
src/lox.cc
src/token.h
src/token.cc
src/scanner.h
src/scanner.cc
)

target_link_libraries(${PROJECT_NAME} PRIVATE magic_enum::magic_enum)
