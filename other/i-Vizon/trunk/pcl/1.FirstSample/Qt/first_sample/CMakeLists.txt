cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(openni_grabber)

find_package(PCL 1.2 REQUIRED)
SET(BOOST_MIN_VERSION "1.48.0")

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

add_executable (openni_grabber main.cpp)
target_link_libraries (openni_grabber ${PCL_LIBRARIES})

