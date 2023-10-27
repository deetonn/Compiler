cmake_minimum_required(VERSION 3.27.0)
set(CMAKE_CXX_STANDARD 20)

project(Compiler)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})
