# Deepfake-Detection-System


# Server Setup

This section explains the CMake configuration file (`CMakeLists.txt`) for the server side of the DeepFake detection project. This file is used to build the server application and manage its dependencies.

## Requirements

- **CMake**: Version 3.10 or higher is required.
- **Boost Library**: Version 1.82.0 is used. The project utilizes Boost's `system`, `thread`, `json`, and `filesystem` modules.
- **ZeroMQ**: The `libzmq` library is required. ZeroMQ facilitates communication between the server and clients.

## Installation

1. **Boost Library**: The Boost library should be placed in the `libs/boost_1_82_0` directory of the project. If it is located elsewhere, update the `BOOST_ROOT` variable accordingly.

2. **ZeroMQ**: ZeroMQ must be installed on your system. Its location is automatically detected using `pkg-config`.

3. **CMake Configuration**: Run the following commands in the root directory of the project to build it:

   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

   These commands will perform the build process in the `build` directory and generate an executable file named `server`.

## Explanation of CMakeLists.txt

- **C++ Standard**: The project uses the C++17 standard.
- **Boost Settings**: The path to the Boost library and the required modules are specified.
- **ZeroMQ Settings**: The ZeroMQ library is located using `pkg-config` and linked to the project.
- **Compilation and Linking**: The `server.cpp` file is compiled, and Boost and ZeroMQ libraries are linked.

