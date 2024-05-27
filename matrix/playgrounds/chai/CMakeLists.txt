cmake_minimum_required(VERSION 3.5)
project(Tests)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++17 -static-libstdc++")

# Improve chai script performance by disabling threading
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCHAISCRIPT_NO_THREADS")

# Source files
set(SOURCES
        main.cpp
)

# Executable
add_executable(main ${SOURCES})

target_link_libraries(main -ldl)
