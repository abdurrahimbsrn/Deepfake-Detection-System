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


---

# Docker Setup

This section provides instructions for setting up the Docker environment for the DeepFake detection project. The Dockerfile is configured to use CUDA for GPU acceleration and includes all necessary dependencies for running the detection model.

## Requirements

- **NVIDIA Docker**: Ensure that your system has NVIDIA Docker installed to enable GPU support.
- **NPR Model**: The NPR model is required for detection. You can download it from the [NPR-DeepfakeDetection repository](https://github.com/chuangchuangtan/NPR-DeepfakeDetection/tree/main).

## Dockerfile Explanation

- **Base Image**: The Dockerfile uses the `nvidia/cuda:12.1.1-runtime-ubuntu22.04` base image to support CUDA and GPU acceleration.
- **Working Directory**: The `/app` directory is created as the working directory.
- **GPU Access**: The `CUDA_VISIBLE_DEVICES` environment variable is set to `0` to specify the GPU device.
- **System Packages**: Required system packages, such as Python 3, pip, and OpenCV dependencies, are installed.
- **PyTorch Installation**: PyTorch and related libraries are installed with CUDA 12.1 support.
- **Additional Libraries**: Libraries like NumPy, OpenCV, scikit-learn, and PyZMQ are installed.
- **OpenCV Verification**: The OpenCV installation is verified by printing its version.
- **Model and Code**: The NPR model and detection code are copied into the `/app/NPR` directory.
- **Run Command**: The container runs the `detection.py` script with the specified model path and batch size.

## Building and Running the Docker Image

1. **Download the NPR Model**: Clone or download the NPR model from the [NPR-DeepfakeDetection repository](https://github.com/chuangchuangtan/NPR-DeepfakeDetection/tree/main) and place it in the appropriate directory.

2. **Build the Docker Image**: Run the following command to build the Docker image:

   ```bash
   docker build -t deepfake_detector .
   ```

3. **Run the Docker Container**: Start the container with GPU support and map the necessary ports:

   ```bash
   docker run --gpus all --rm -it -p 5560:5560 deepfake_detector
   ```

   This command will start the container and run the DeepFake detection script.


---
# DeepFake Detection Project - Docker Setup for Face Detection

This section provides instructions for setting up the Docker environment for the face detection component of the DeepFake detection project. The Dockerfile is configured to use Python 3.9 and includes all necessary dependencies for running the face detection script.

## Requirements

- **Docker**: Ensure that Docker is installed on your system.
- **Python Libraries**: The required Python libraries, such as OpenCV, NumPy, PyZMQ, RetinaFace, and tf-keras, are installed automatically.

## Dockerfile Explanation

- **Base Image**: The Dockerfile uses the `python:3.9` base image to ensure compatibility with Python 3.9.
- **System Libraries**: Necessary system libraries, such as OpenGL and development tools, are installed.
- **Working Directory**: The `/app` directory is set as the working directory.
- **Python Libraries**: Required Python libraries, including OpenCV, NumPy, PyZMQ, RetinaFace, and tf-keras, are installed using pip.
- **Copy Script**: The `face_detection.py` script is copied into the `/app` directory.
- **Run Command**: The container runs the `face_detection.py` script when started.

## Building and Running the Docker Image

1. **Build the Docker Image**: Run the following command to build the Docker image:

   ```bash
   docker build -t deepfake_detector .
   ```

2. **Run the Docker Container**: Start the container and map the necessary ports:

   ```bash
   docker run -it --rm -p 5555:5555 deepfake_detector
   ```

   This command will start the container and run the face detection script.

---

# DeepFake Detection UI

This is a React-based user interface for a DeepFake detection system. It allows users to upload an image, which is then sent to a backend server for analysis. The result of the analysis is displayed on the screen.

## Features
- Upload an image from your device
- Preview the selected image before uploading
- Send the image to a backend server for DeepFake detection
- Display the analysis result in a user-friendly format

## Installation

To include this UI in your own project, follow these steps:

1. Clone this repository or copy `App.js` into your React project.
2. Ensure you have Node.js and npm installed.
3. Install the required dependencies by running:
   ```sh
   npm install
   ```

## Usage

To run the application in development mode:

```sh
npm start
```

For production deployment, build the project:

```sh
npm run build
```

The built files will be in the `build/` directory. You can serve them using a static file server or integrate them into your backend.

## Backend Integration

This UI expects a backend API running at `http://localhost:8080/upload`, where it sends the uploaded image as a Base64-encoded string. Ensure your backend is properly set up to handle the request and return an appropriate response.

## Customization

You can modify the UI by changing the styles, colors, and layout in `App.js`. The design uses Material UI for styling, which you can further customize by adjusting the `sx` properties.


