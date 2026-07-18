# 🍩 Donut Mask Generation using Classical Computer Vision (C++17 + OpenCV)

An industrial computer vision pipeline developed in **C++17** using **OpenCV** to detect donuts on a conveyor belt and generate accurate outer and inner masks for inspection.

This project was developed as part of the **XIS AI & Computer Vision Technical Assessment** and demonstrates robust image preprocessing, segmentation, contour analysis, geometric filtering, and boundary refinement using classical image processing techniques.

---

# Features

- Classical Computer Vision (No Machine Learning)
- Batch image processing
- Robust preprocessing for uneven illumination
- Noise suppression using morphological operations
- Automatic thresholding using Otsu's method
- Donut contour detection
- Geometric contour filtering
- Inner hole extraction
- Boundary refinement and contour smoothing
- Overlay visualization for inspection
- Modular C++17 architecture

---

# Processing Pipeline

```
Input Image
      │
      ▼
Image Preprocessing
      │
      ▼
Thresholding
      │
      ▼
Morphological Processing
      │
      ▼
Contour Detection
      │
      ▼
Geometric Filtering
      │
      ▼
Donut Mask Generation
      │
      ▼
Hole Extraction
      │
      ▼
Boundary Refinement
      │
      ▼
Final Overlay Visualization
```

---

# Project Structure

```
donut-masker-cpp/
│
├── CMakeLists.txt
├── README.md
├── LICENSE
├── .gitignore
│
├── include/
│   ├── BatchProcessor.hpp
│   ├── BoundaryRefiner.hpp
│   ├── DonutDetector.hpp
│   ├── ImageLoader.hpp
│   └── Preprocessor.hpp
│
├── src/
│   ├── main.cpp
│   ├── BatchProcessor.cpp
│   ├── BoundaryRefiner.cpp
│   ├── DonutDetector.cpp
│   ├── ImageLoader.cpp
│   └── Preprocessor.cpp
│
├── docs/
│
├── input_samples/
│
└── output_samples/
    ├── processed/
    ├── threshold/
    ├── morphology/
    ├── donut_masks/
    ├── hole_masks/
    └── overlays/
```

---

# Technologies

- C++17
- OpenCV 4.x
- CMake
- STL
- Visual Studio 2022

---

# Image Processing Pipeline

## 1. Image Preprocessing

The preprocessing stage prepares images for robust segmentation under varying lighting conditions.

### Operations

- Convert image to grayscale
- Contrast enhancement
- Background normalization
- Noise reduction
- Gaussian smoothing

Purpose:

- Reduce illumination variation
- Improve foreground separation
- Suppress sensor noise
- Preserve donut boundaries

---

## 2. Thresholding

Global Otsu thresholding is applied to automatically separate donuts from the conveyor background.

Advantages:

- Automatic threshold selection
- Robust across varying lighting
- No manual tuning required

---

## 3. Morphological Processing

Morphological operations improve binary mask quality.

Operations:

- Opening
- Closing
- Median filtering

Purpose:

- Remove isolated noise
- Repair broken boundaries
- Smooth object edges

---

## 4. Contour Detection

Contours are extracted from the cleaned binary mask.

Small connected components are removed before the final contour extraction to reduce false detections.

---

## 5. Geometric Filtering

Candidate contours are filtered using geometric properties.

Filtering criteria include:

- Minimum area
- Maximum area
- Aspect ratio
- Circularity
- Solidity
- Extent
- Bounding box constraints

These filters reject background artifacts while preserving valid donut candidates.

---

## 6. Donut Mask Generation

Validated contours are rendered as filled binary masks to create the outer donut regions.

---

## 7. Hole Extraction

Inner donut holes are extracted from validated donut regions using binary mask operations and contour filtering.

Hole candidates are validated using:

- Area
- Circularity

Only valid hole regions are retained.

---

## 8. Boundary Refinement

Final masks are refined using:

- Morphological smoothing
- Contour approximation
- Boundary cleanup

This improves contour quality and reduces jagged edges.

---

# Output

For each input image the pipeline generates:

| Folder | Description |
|---------|-------------|
| processed | Preprocessed image |
| threshold | Binary threshold image |
| morphology | Morphological processing result |
| donut_masks | Final donut masks |
| hole_masks | Extracted hole masks |
| overlays | Final inspection overlay |

---

# Build Requirements

- CMake 3.16 or newer
- C++17 compatible compiler
- OpenCV 4.x

---

# Build Instructions

```bash
git clone <repository-url>

cd donut-masker-cpp

mkdir build

cd build

cmake ..

cmake --build . --config Release
```

---

# Run

Place test images inside:

```
input_samples/
```

Run:

```
donut_masker.exe
```

Generated results are saved automatically inside:

```
output_samples/
```

---

# Design Principles

The project follows a modular architecture where each component has a single responsibility.

| Module | Responsibility |
|---------|----------------|
| ImageLoader | Image loading |
| Preprocessor | Image enhancement |
| DonutDetector | Segmentation and contour detection |
| BoundaryRefiner | Boundary refinement |
| BatchProcessor | Batch processing and output generation |

---

# Future Improvements

Possible future enhancements include:

- Adaptive local thresholding
- Watershed-based separation for touching donuts
- Ellipse fitting for improved hole estimation
- Parallel image processing
- GPU acceleration using CUDA/OpenCL
- Automatic parameter optimization
- Quality measurement of detected donuts

---

# Sample Results

The pipeline produces:

- Binary segmentation
- Refined donut masks
- Hole masks
- Contour overlays
- Batch processing statistics

Example output folders:

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

# Author

**Mohsan Raza**

BS Computer Science (FAST-NUCES)

GitHub: https://github.com/Mohsan-R

---

# License

This project is provided for educational and technical assessment purposes.