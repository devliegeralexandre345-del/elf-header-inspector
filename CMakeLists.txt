cmake_minimum_required(VERSION 3.16)
project(elf_header_inspector LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

include_directories(include)

# Exécutable principal
add_executable(elf_header_inspector src/main.cpp src/elf_parser.cpp)

# Tests unitaires
enable_testing()
add_executable(unit_tests tests/test.cpp src/elf_parser.cpp)
add_test(NAME AllTests COMMAND unit_tests)