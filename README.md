Based on the provided scripts and files, here's an updated and comprehensive README that includes details on requirements, how to compile and run the software, and how to use different modes of operation.

---

# Mosaico Software

![Mosaico Logo](./icon.png)

Welcome to the Mosaico Software repository! 
This software controls the LED matrix for the Mosaico ecosystem, running on a Raspberry Pi or 
your local machine as a simulator.

---

## Table of Contents

1. [Introduction](#introduction)
2. [Requirements](#requirements)
    - [General Requirements](#general-requirements)
    - [Mode-Specific Requirements](#mode-specific-requirements)
3. [Installation](#installation)
    - [Compiling on Raspberry Pi](#compiling-on-raspberry-pi)
    - [Simulating on a Computer](#simulating-on-a-computer)
    - [Cross-Compiling](#cross-compiling)
    - [Running as a Web Service](#running-as-a-web-service)
4. [External Libraries](#external-libraries)
5. [Creating Your First Widget](#creating-your-first-widget)
6. [Contributing](#contributing)
7. [License](#license)

---

## Introduction
Mosaico allows users and developers to create, share, and display custom widgets on an LED matrix. 
More infos about the project can be found on the [Mosaico Website](https://mosaico.murkrowdev.org).

### Example Widgets
- Display the current time and date.
- Show the weather forecast for your location.
- Create a shopping list widget.
- Upload and display custom pixel art.
- Write custom text messages or quotes.
- Create animations or visual effects.
- And much more!

## Requirements

### General Requirements
- **Python 3**: Required for running Python scripts.
- **CMake**: Required for building the C++ components.
- **Pip3**: Python package installer for managing dependencies.
- **GCC**: Required for compiling C++ code.
- **Git**: Required for cloning the repository and widgets.
- **Pybind11**: Required for Python bindings for C++ code.

### Mode-Specific Requirements

#### Simulation Mode (Local Machine)
- **libx11-dev**: Required for launching the simulation window.

---

## Installation

### Compiling on Raspberry Pi

1. **Clone the Repository:**
   ```bash
   git clone --recursive https://github.com/mosaico-matrix/mosaico-software
   cd mosaico-software
   ```

2. **Install Required Packages:**
   ```bash
   sudo apt-get update
   sudo apt-get install -y pybind11
   ```

3. **Run the Installation Script:**
   ```bash
   ./start.sh default
   ```

   This script sets up the Python virtual environment, installs dependencies, and compiles the application.

4. **Run the Application:**
   ```bash
   ./main
   ```

### Simulating on a Computer

1. **Install X11 Dependencies:**
   ```bash
   sudo apt-get update
   sudo apt-get install -y libx11-dev python3-pybind11
   ```

2. **Run the Installation Script:**
   ```bash
   ./start.sh simulation
   ```

   This sets up the environment and compiles the software to run in an X11 window.

3. **Run the Application:**
   ```bash
   ./main
   ```

### Cross-Compiling

1. **Set Up Cross-Compilation Tools**: Install necessary tools for cross-compiling for Raspberry Pi.
More info in CROSS-COMPILE.md

2. **Run the Installation Script:**
   ```bash
   ./start.sh cross-compile
   ```

   This compiles the application for Raspberry Pi on a different architecture.

---

## External Libraries

The following external libraries are included as submodules:

- **[hzeller/rpi-rgb-led-matrix](https://github.com/hzeller/rpi-rgb-led-matrix)**: Controls LED matrices connected to the Raspberry Pi.
- **[nlohmann/json](https://github.com/nlohmann/json)**: A modern C++ library for JSON operations.
---

## Contributing

We welcome contributions! Feel free to open an issue or submit a pull request.

---

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

Feel free to reach out if you have any questions or need further assistance!