cmake_minimum_required(VERSION 3.5.1)
project(basler_test)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} pigpio/pigpio-master/)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl, -E")
find_package(OpenCV REQUIRED)

find_package(Pylon QUIET)
find_package(pigpio QUIET)

if (NOT ${Pylon_FOUND})
    include("${CMAKE_CURRENT_SOURCE_DIR}/CMakeFiles/FindPylon.cmake")
endif()

include_directories(
        #add other includes directories 
        ${Pylon_INCLUDE_DIRS}
)

add_executable(basler_test openCVGrab.cpp)
find_library(lib_pigpio pigpio)
#target_include_directories(basler_test /opt/pylon5/include)
target_link_libraries(basler_test ${OpenCV_LIBS} ${Pylon_LIBRARIES} ${lib_pigpio})
