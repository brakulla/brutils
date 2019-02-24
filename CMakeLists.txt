cmake_minimum_required(VERSION 3.10)
project(brutils
        LANGUAGES C CXX
        VERSION 1.0.0
        DESCRIPTION "Burak Ongay (brakulla) utility library")

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

include(GNUInstallDirs)
add_library(brutils STATIC
        include/brutils/Signal.hpp)
set_target_properties(brutils PROPERTIES
        LINKER_LANGUAGE CXX
        VERSION ${PROJECT_VERSION}
        SOVERSION 1
        PUBLIC_HEADER include/brutils/Signal.hpp)
install(TARGETS brutils
        LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib
        ARCHIVE DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib
        )