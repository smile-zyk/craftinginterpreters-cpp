cmake_minimum_required(VERSION 3.10)

project(lox-cpp LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(magic_enum CONFIG REQUIRED)

if(MSVC)
    add_compile_options(/utf-8)
endif()

add_executable(${PROJECT_NAME} 
src/main.cc
src/lox.h
src/lox.cc
src/object.h
src/object.cc
src/token.h
src/token.cc
src/scanner.h
src/scanner.cc
src/parser.h
src/parser.cc
src/ast_printer.h
src/ast_printer.cc
src/interpreter.h
src/interpreter.cc
src/ast.h
src/error.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE magic_enum::magic_enum)
