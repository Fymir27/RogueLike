cmake_minimum_required(VERSION 3.5.0)
project(RogueLike)
set(EXECUTABLE_NAME ${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

include_directories(include)
include_directories(/usr/bin/include/SFML)
file(GLOB SOURCES "src/*.cpp")

add_executable(${EXECUTABLE_NAME} ${SOURCES})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML REQUIRED system window graphics)
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})



