cmake_minimum_required(VERSION 3.5.0)
project(RogueLike)

set(EXECUTABLE_NAME ${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

#set(SFML_STATIC_LIBRARIES TRUE)

#set(CMAKE_PREFIX_PATH "D:/Libraries/OpenJPEG" ${CMAKE_PREFIX_PATH})

find_package(SFML 2.4 COMPONENTS graphics window system REQUIRED)
#find_package(OpenJPEG REQUIRED)

include_directories(include)
include_directories(${SFML_INCLUDE_DIR})
#include_directories(${OPENJPEG_INCLUDE_DIRS})
file(GLOB SOURCES "src/*.cpp")

add_executable(${EXECUTABLE_NAME} ${SOURCES})

#target_link_libraries(${OPENJPEG_LIBRARIES})

#target_link_libraries(${EXECUTABLE_NAME} -shared-libgcc)
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} )#${SFML_DEPENDENCIES})





