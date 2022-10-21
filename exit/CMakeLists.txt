cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
set (CMAKE_CXX_FLAGS "-O3 -g3 -std=c++17")
project(exit)

include_directories(/usr/include ${PROJECT_SOURCE_DIR})
if(APPLE)
  include_directories(/usr/local/include /opt/homebrew/include)
endif(APPLE)

link_directories(${PROJECT_SOURCE_DIR})
if(APPLE)
  link_directories(/usr/local/lib /opt/homebrew/lib)
endif(APPLE)


add_executable (exit exit.cpp)
target_link_libraries(exit zmq)
