cmake_minimum_required(VERSION 3.6)
project(brutils
        VERSION 1.0.0
        DESCRIPTION "Burak Ongay (brakulla) utility library")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)

if (CMAKE_BUILD_TYPE MATCHES Test)
    include_directories(include/brutils)
    add_executable(brutilsTest src/main.cpp src/brutils/json_parser.cpp)
    target_link_libraries(brutilsTest
            #            brutils
            pthread
            )
else ()
    include_directories(include/brutils)
    add_library(brutils STATIC
            #        src/main.cpp
            include/brutils/queue_pc.hpp
            include/brutils/string_utils.hpp
            include/brutils/timer.hpp
            include/brutils/br_object.hpp
            include/brutils/variant.h
            include/brutils/json_parser.h
            src/brutils/json_parser.cpp)
    set_target_properties(brutils PROPERTIES
            LINKER_LANGUAGE CXX
            VERSION ${PROJECT_VERSION}
            )
    install(TARGETS brutils
            LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib
            ARCHIVE DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib
            )
endif()