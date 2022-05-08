cmake_minimum_required(VERSION 3.6)
project(brutils
    VERSION 2.0.0
    DESCRIPTION "Burak Ongay (brakulla) utility library")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_SHARED_LIBS "Enable shared library build for brutils" OFF)
option(BUILD_TESTS "Enable test builds for brutils" OFF)

if (BUILD_SHARED_LIBS)
  add_library(brutils SHARED)
else()
  add_library(brutils STATIC)
endif()

target_include_directories(brutils
    PUBLIC
        src/
)
target_sources(brutils
    PRIVATE
        src/brutils/time/time_utils.cpp
        src/brutils/json/json_generator.cpp
        src/brutils/json/json_parser.cpp
)
