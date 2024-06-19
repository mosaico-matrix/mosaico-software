#!/bin/bash

# Define colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to check command existence
check_command() {
    if ! command -v "$1" &> /dev/null; then
        echo -e "${RED}$2 could not be found, please install $2${NC}"
        exit 1
    fi
}

# Function to run commands with optional verbosity
run_command() {
    if [ "$VERBOSE" == "true" ]; then
        "$@"
    else
        "$@" > /dev/null 2>&1
    fi
}

# Print usage template
print_usage() {
    echo -e "${YELLOW}Usage:${NC} ./start.sh [-v] <mode>"
    echo -e "${GREEN}Options:${NC}"
    echo -e "  ${GREEN}-v${NC}  Enable verbose mode"
    echo -e "${GREEN}Modes of operation:${NC}"
    echo -e "  ${GREEN}default${NC}: This mode is intended to run directly on the Raspberry Pi and uses the hardware matrix"
    echo -e "  ${GREEN}simulation${NC}: This mode is intended to run on a local machine for testing purposes and starts the matrix as a x11 window"
    echo -e "  ${GREEN}cross-compile${NC}: This mode is intended to cross-compile the application for the Raspberry Pi and uses the hardware matrix"
    echo -e "  ${GREEN}web${NC}: This mode is intended to run the application as a web service and run the matrix as a web window"
    exit 1  # Exiting with 1 because incorrect usage
}

# Parse command-line options
VERBOSE="false"

# Check if no arguments are provided
if [ $# -eq 0 ]; then
    print_usage
fi

# Handle options and mode
while [[ $# -gt 0 ]]; do
    case $1 in
        -v)
            VERBOSE="true"
            shift
            ;;
        default|simulation|cross-compile|web)
            MODE=$1
            shift
            ;;
        *)
            echo -e "${RED}Invalid argument: $1${NC}"
            print_usage
            ;;
    esac
done

# Check if mode is set
if [ -z "$MODE" ]; then
    echo -e "${RED}Mode argument is required.${NC}"
    print_usage
fi

# Check for required commands and libraries
check_command "python3" "Python3"
check_command "cmake" "CMake"
check_command "pip3" "Pip3"
if [ "$MODE" != "cross-compile" ]; then
    echo -e "${YELLOW}Make sure you have libcurl installed${NC}"
fi
if [ "$MODE" == "simulation" ]; then
    echo -e "${YELLOW}Make sure you have libx11-dev installed${NC}"
fi

# Create python virtual environment
cd networking
run_command python3 -m venv venv
source venv/bin/activate

# Install the required python packages
echo -e "${YELLOW}Installing python packages${NC}"
run_command pip3 install -r requirements.txt
echo -e "${GREEN}Python packages installed${NC}"

# Exit virtual environment
deactivate
cd ..

# Compile the application
echo -e "${YELLOW}Compiling the application${NC}"
cd matrix
run_command cmake -DCOMPILE_MODE=${MODE} .
run_command make

# Exit if compilation failed
if [ $? -ne 0 ]; then
    echo -e "${RED}Compilation failed${NC}"
    exit 1
fi

echo -e "${GREEN}Application compiled${NC}"



# Start the application (example command, adjust as needed)
./main
