cmake_minimum_required(VERSION 3.5.1)
project(basler_test)
set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl, -E")
find_package(OpenCV REQUIRED)

find_package(Pylon QUIET)

if (NOT ${Pylon_FOUND})
    include("${CMAKE_CURRENT_SOURCE_DIR}/CMakeFiles/FindPylon.cmake")
endif()

include_directories(
        #add other includes directories 
        ${Pylon_INCLUDE_DIRS}
)
add_executable(basler_test openCVGrab.cpp)
#target_include_directories(basler_test /opt/pylon5/include)
target_link_libraries(basler_test ${OpenCV_LIBS} ${Pylon_LIBRARIES})
