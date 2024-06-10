cmake_minimum_required(VERSION 3.5)
project(Mosaico_Software)

# Set matrix dimensions
set(MATRIX_ROWS 64)
set(MATRIX_COLS 64)

# Set frame render rate
set(MATRIX_REFRESH_RATE 30)

# Set various url endpoints
set(API_URL  '"http://host.orb.internal:8000/api/"')

# Set compilation mode
# - NORMAL: Compile for the host machine and output to the RGB LED Matrix
# - SIMULATION: Compile for the host machine and output to x11 window
# - CROSS_COMPILE: Cross compile for the Raspberry Pi and output to the RGB LED Matrix
set(COMPILE_MODE "SIMULATION")
set(DEBUG true)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++17 -static-libstdc++")
# Matrix flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -DMATRIX_ROWS=${MATRIX_ROWS} -DMATRIX_COLS=${MATRIX_COLS} -DMATRIX_REFRESH_RATE=${MATRIX_REFRESH_RATE}")
# Api url
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAPI_URL=${API_URL}")
# Improve chai script performance by disabling threading
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCHAISCRIPT_NO_THREADS")
if (DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")
endif ()

# SIMULATION
if(COMPILE_MODE STREQUAL "SIMULATION")

    message("Compiling the project for the host machine in simulation mode")

    # Compile x11 matrix
    set(MATRIX_DEVICE matrix/matrices/x11-matrix.cpp)

    # X11
    find_package(X11 REQUIRED)
    include_directories(${X11_INCLUDE_DIR})

    # LibCurl
    find_package(CURL REQUIRED)

    # Export debug symbols and define simulation mode
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -DSIMULATION")

    # Set compiler
    set(ENV{CXX} clang++) # Clang is much faster to compile chaiscript
    set(ENV{CC} gcc)

# Cross compile for Raspberry Pi
elseif(COMPILE_MODE STREQUAL "CROSS_COMPILE")

    message("Cross compiling the project for the Raspberry Pi")

    # Compile hardware matrix
    set(MATRIX_DEVICE matrix/matrices/hardware-matrix.cpp)

    # Set compiler
    set(CMAKE_CXX_COMPILER "armv6-rpi-linux-gnueabihf-g++")
    set(CMAKE_C_COMPILER "armv6-rpi-linux-gnueabihf-gcc")
    set(ENV{CXX} ${CMAKE_CXX_COMPILER})
    set(ENV{CC} ${CMAKE_C_COMPILER})

    # Manually built libcurl
    set(CURL_LIBRARY "${CMAKE_SOURCE_DIR}/external/curl/lib/libcurl.a")
    set(CURL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/external/curl/include")
    include_directories(${CURL_INCLUDE_DIR})

# Standard compilation
elseif (COMPILE_MODE STREQUAL "NORMAL")

    # Compile hardware matrix
    set(MATRIX_DEVICE matrix/matrices/hardware-matrix.cpp)

    # LibCurl
    find_package(CURL REQUIRED)

    # Set compiler
    set(ENV{CXX} g++)
    set(ENV{CC} gcc)
endif ()

# Compile RGB LED Matrix library
execute_process(
        COMMAND make
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/rpi-rgb-led-matrix/lib
)
set(RGB_LIBRARY "${PROJECT_SOURCE_DIR}/external/rpi-rgb-led-matrix/lib/librgbmatrix.a")
include_directories(${PROJECT_SOURCE_DIR}/external/rpi-rgb-led-matrix/lib)
include_directories(${PROJECT_SOURCE_DIR}/external/rpi-rgb-led-matrix/include)

# RestClient library
set(RESTCLIENT_SOURCES
        external/restclient/source/connection.cc
        external/restclient/source/restclient.cc
        external/restclient/source/helpers.cc
)

# Add restclient library
add_library(restclient STATIC ${RESTCLIENT_SOURCES})
target_include_directories(restclient PUBLIC ${PROJECT_SOURCE_DIR}/external/restclient/include)


# Source files
set(SOURCES
        main.cpp
        configs.cpp

        # Logger
        logger/console-logger.cpp

        # Matrix
        matrix/matrix-slideshow.cpp
        matrix/canvas-layer.cpp
        matrix/canvas-buffer.cpp

        # Runners
        matrix/runners/matrix-runner.h
        matrix/runners/matrix-runner.cpp
        matrix/runners/loading-runner.cpp

        matrix/runners/dynamic/dynamic-runner.cpp
        matrix/runners/dynamic/dynamic-runner-metadata.cpp
        matrix/runners/dynamic/dynamic-runner-parser.cpp
        matrix/runners/dynamic/modules/rest-module.cpp

        matrix/runners/games/pixel-mover.cpp
        matrix/runners/games/flappy-bird.cpp
        matrix/runners/games/tetris.cpp

        # Drawables
        matrix/drawables/drawable.h
        matrix/drawables/drawable.cpp
        matrix/drawables/colors.cpp
        matrix/drawables/drawable-ppm.cpp
        matrix/drawables/animation-drawer.cpp
        matrix/drawables/drawable-text.cpp

        # Matrices
        matrix/matrices/matrix-device.cpp
        matrix/matrices/matrix-builder.cpp

        # Networking
        networking/server/ble/ble-server.cpp
        #networking/server/database.cpp

        ${MATRIX_DEVICE}
        ${RESTCLIENT_SOURCES}
        playgrounds/database/database.no
)

# Executable
add_executable(main ${SOURCES})

# Include directories
target_include_directories(main PRIVATE ${PROJECT_SOURCE_DIR}/external/rpi-rgb-led-matrix/include)

# Link libraries
if (COMPILE_MODE STREQUAL "SIMULATION")
    target_link_libraries(main PRIVATE ${RGB_LIBRARY} restclient rt m pthread ${CURL_LIBRARIES} ${X11_LIBRARIES} -lX11 -ldl -fuse-ld=gold) # ${LUA_LIBRARIES}
else ()
    target_link_libraries(main PRIVATE ${RGB_LIBRARY} restclient rt m pthread ${CURL_LIBRARY} -ldl)
endif ()