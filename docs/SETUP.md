# Setup Guide

## Overview

This document describes the software requirements, project structure, build process, and execution instructions for the Donut Mask Generation project.

The implementation is written in **C++17** using the **OpenCV** library and is designed to be reproducible from a clean repository checkout.

---

# System Requirements

## Operating System

The project has been developed and tested on:

- Windows 10 / Windows 11

The code is portable and should also compile on Linux or macOS with minimal modifications.

---

# Software Requirements

The following software is required.

| Software | Version |
|-----------|---------|
| CMake | 3.16 or later |
| C++ Compiler | C++17 compatible |
| OpenCV | 4.x |
| Git | Latest |

Supported compilers include:

- MSVC (Visual Studio 2022)
- GCC
- Clang

---

# Project Structure

```
project-root/
│
├── src/
├── include/
├── docs/
├── input_samples/
├── output_samples/
│
├── CMakeLists.txt
├── README.md
├── LICENSE
└── .gitignore
```

---

# Clone Repository

```bash
git clone https://github.com/Mohsan-R/donut-masker-cpp.git
cd donut-masker-cpp
```

---

# Build Instructions

Create a build directory.

```bash
mkdir build
cd build
```

Generate build files using CMake.

```bash
cmake ..
```

Compile the project.

```bash
cmake --build .
```

After a successful build, the executable will be generated inside the build directory.

---

# Running the Application

Return to the executable directory and run:

```bash
donut_masker --input ../input_samples --output ../output_samples
```

or

```bash
./donut_masker --input ../input_samples --output ../output_samples
```

depending on the operating system.

---

# Input Directory

The input directory should contain the images to be processed.

Example:

```
input_samples/
├── sample1.jpg
├── sample2.jpg
└── sample3.jpg
```

---

# Output Directory

The output directory will contain the generated results.

Example:

```
output_samples/

├── processed/
├── threshold/
├── morphology/
├── donut_masks/
├── hole_masks/
└── overlays/
```

---

# Generated Outputs

For every processed image, the application generates:

- Preprocessed image
- Threshold image
- Morphological result
- Donut mask
- Hole mask
- Final overlay

These outputs allow visual verification of every processing stage.

---

# Dependencies

The project depends only on:

- OpenCV
- C++ Standard Library

No additional third-party libraries are required.

The implementation does **not** use:

- Deep Learning
- TensorFlow
- PyTorch
- ONNX
- CUDA

The solution relies entirely on classical computer vision techniques.

---

# Build Configuration

The project is configured using CMake.

Key configuration features include:

- C++17 standard
- Automatic OpenCV detection
- Cross-platform build support
- Separate include and source directories

---

# Reproducibility

To ensure reproducibility:

- identical parameters are used for every image
- no manual tuning is required
- no image-specific adjustments are performed
- no learned models are used

A fresh clone of the repository followed by the documented build commands is sufficient to reproduce the reported results.

---

# Troubleshooting

## OpenCV Not Found

If CMake cannot locate OpenCV, specify the OpenCV installation directory manually.

Example:

```bash
cmake -DOpenCV_DIR="C:/opencv/build" ..
```

---

## Build Errors

Ensure that:

- CMake is installed
- OpenCV is correctly installed
- A C++17-compatible compiler is available
- Environment variables are configured correctly

---

## Input Folder Not Found

Verify that the input directory exists and contains supported image files before running the application.

---

# Summary

The project is designed to compile cleanly using a modern C++17 toolchain and OpenCV.

Following the documented setup and build steps allows the complete image processing pipeline to be reproduced without modification.