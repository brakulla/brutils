cmake_minimum_required(VERSION 3.6)
project(brutils
        VERSION 1.0.0
        DESCRIPTION "Burak Ongay (brakulla) utility library")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_BUILD_TYPE MATCHES Test)
    include_directories(include/brutils)
    add_executable(brutilsTest main.cpp src/brutils/json_parser.cpp src/brutils/json_generator.cpp)
    target_link_libraries(brutilsTest
            #            brutils
            pthread
            )
else ()
    add_library(brutils STATIC
            main.cpp
            include/brutils/Thread/ThreadPool.h
            src/brutils/Thread/ThreadPool.cpp
            include/brutils/Thread/Thread.h
            src/brutils/Thread/Thread.cpp
            include/brutils/HttpServer/HttpResponse.h
            src/brutils/HttpServer/HttpResponse.cpp
            include/brutils/HttpServer/HttpRequest.h
            src/brutils/HttpServer/HttpRequest.cpp
            include/brutils/HttpServer/HttpConnection.h
            src/brutils/HttpServer/HttpConnection.cpp
            include/brutils/HttpServer/RequestParser.h
            src/brutils/HttpServer/RequestParser.cpp
            include/brutils/TcpSocket/TcpSocket.h
            src/brutils/TcpSocket/TcpSocket.cpp
            include/brutils/TcpServer/TcpServer.h
            src/brutils/TcpServer/TcpServer.cpp
            include/brutils/timers/combined_timer.h
            src/brutils/timers/combined_timer.cpp
            include/brutils/timers/timer.h
            include/brutils/queue_pc.h
            include/brutils/string_utils.h
            include/brutils/time_utils.h
            include/brutils/br_object.h
            include/brutils/variant.h
            include/brutils/json_parser.h
            include/brutils/json_generator.h
            src/brutils/json_parser.cpp
            src/brutils/json_generator.cpp)
    target_include_directories(brutils PUBLIC include)
    set_target_properties(brutils PROPERTIES
            LINKER_LANGUAGE CXX
            VERSION ${PROJECT_VERSION}
            )
endif()