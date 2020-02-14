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
      # brutils
      pthread
      )
else ()
  add_library(brutils STATIC
      include/brutils/Thread/ThreadPool.h
      include/brutils/Thread/Thread.h
      include/brutils/HttpServer/HttpResponse.h
      include/brutils/HttpServer/HttpRequest.h
      include/brutils/HttpServer/HttpConnection.h
      include/brutils/HttpServer/RequestParser.h
      include/brutils/TcpSocket/TcpSocket.h
      include/brutils/TcpServer/TcpServer.h
      include/brutils/timers/combined_timer.h
      include/brutils/timers/timer.h
      include/brutils/queue_pc.h
      include/brutils/string_utils.h
      include/brutils/time_utils.h
      include/brutils/br_object.h
      include/brutils/variant.h
      include/brutils/json_parser.h
      include/brutils/json_generator.h
      src/brutils/Thread/ThreadPool.cpp
      src/brutils/Thread/Thread.cpp
      src/brutils/HttpServer/HttpResponse.cpp
      src/brutils/HttpServer/HttpRequest.cpp
      src/brutils/HttpServer/HttpConnection.cpp
      src/brutils/HttpServer/RequestParser.cpp
      src/brutils/TcpSocket/TcpSocket.cpp
      src/brutils/TcpServer/TcpServer.cpp
      src/brutils/timers/combined_timer.cpp
      src/brutils/json_parser.cpp
      src/brutils/json_generator.cpp)
  target_include_directories(brutils PUBLIC include)
  set_target_properties(brutils PROPERTIES
      LINKER_LANGUAGE CXX
      VERSION ${PROJECT_VERSION}
      )

  install(TARGETS brutils DESTINATION lib)
  install(
      DIRECTORY
      include/
      DESTINATION
      include
  )
endif ()