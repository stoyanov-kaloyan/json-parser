cmake_minimum_required(VERSION 3.10)
project(JsonParser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(JsonParser
    src/main.cpp
    src/JsonParser.cpp
    src/Tokenizer.cpp
    src/JsonNode.cpp
    src/JsonCLI.cpp
    src/UnitTester.cpp
)

target_include_directories(JsonParser PRIVATE src)

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_compile_options(/utf-8)
endif()
