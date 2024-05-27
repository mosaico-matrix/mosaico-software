cmake_minimum_required(VERSION 3.5)
project(RestClient)

# Source files
file(GLOB SOURCES *.cpp)

# Create shared library
add_library(restclient SHARED ${SOURCES})

# Set output directory
set_target_properties(restclient PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/external/restclient)

# Include directory
target_include_directories(restclient PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
