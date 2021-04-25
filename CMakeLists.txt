cmake_minimum_required(VERSION 3.20)
project("exqudens-cpp-test" VERSION 1.0.0)

message(STATUS "project: ${PROJECT_NAME}-${PROJECT_VERSION}")

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/install")

add_subdirectory("library-${CMAKE_PROJECT_NAME}")
add_subdirectory("test-${CMAKE_PROJECT_NAME}")
