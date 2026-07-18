# Methodology

## Overview

This project implements a classical computer vision pipeline in C++17 using OpenCV for automatic donut and hole mask generation from industrial conveyor belt images.

The objective is to accurately segment donuts under varying illumination conditions while producing clean binary masks suitable for downstream inspection tasks. The implementation avoids deep learning entirely and instead relies on traditional image processing techniques, geometric reasoning, and contour analysis.

The complete processing pipeline consists of the following stages:

```
Input Image
      │
      ▼
Preprocessing
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
Final Masks & Overlay
```

---

# 1. Image Loading

Images are loaded using OpenCV's `imread()` function.

The batch processor recursively processes all supported image formats inside the input directory.

Supported formats include:

- JPG
- JPEG
- PNG
- BMP

Each image is processed independently while preserving the original resolution.

---

# 2. Image Preprocessing

Industrial conveyor images often contain several visual challenges including:

- Uneven illumination
- Corner vignetting
- Conveyor reflections
- Sensor noise
- Surface texture

The preprocessing stage improves image quality before segmentation.

## 2.1 Color Space Conversion

The input image is converted from BGR to grayscale.

Working in grayscale simplifies subsequent operations while preserving sufficient intensity information for segmentation.

```
BGR
 ↓
Grayscale
```

---

## 2.2 Illumination Normalization

A large Gaussian blur is applied to estimate the slowly varying illumination background.

The estimated background is subtracted from the original grayscale image, reducing lighting gradients and improving foreground contrast.

This operation suppresses:

- lighting hotspots
- corner darkening
- conveyor shading

---

## 2.3 Contrast Enhancement

Histogram equalization is applied to improve global contrast.

Benefits include:

- clearer donut boundaries
- improved threshold separation
- stronger edge visibility

---

## 2.4 Noise Reduction

Gaussian smoothing is applied before thresholding.

Purpose:

- suppress sensor noise
- reduce isolated bright pixels
- stabilize Otsu threshold estimation

---

# 3. Thresholding

Binary segmentation is performed using Otsu's automatic thresholding.

Pipeline:

```
Gaussian Blur
        │
        ▼
Median Blur
        │
        ▼
Otsu Threshold
```

Advantages:

- automatic threshold selection
- adapts to varying images
- no manual tuning required

The resulting binary image separates foreground donuts from the conveyor background.

---

# 4. Morphological Processing

Morphological operations improve segmentation quality.

## Opening

Opening removes:

- isolated foreground noise
- small artifacts
- salt noise

```
Erosion
↓

Dilation
```

---

## Closing

Closing repairs:

- small gaps
- broken donut edges
- fragmented boundaries

```
Dilation
↓

Erosion
```

---

## Median Filtering

A median filter removes isolated binary noise while preserving edges.

Compared with Gaussian smoothing, the median filter better preserves object boundaries.

---

# 5. Contour Detection

Contours are extracted using OpenCV's contour detection.

Each contour is treated as a potential donut candidate.

For every contour, several geometric measurements are computed.

These include:

- Area
- Perimeter
- Bounding rectangle
- Circularity
- Solidity
- Extent

---

# 6. Geometric Filtering

Not every connected component corresponds to a donut.

Candidate contours are filtered using multiple geometric constraints.

## Area

Very small contours are rejected as noise.

Very large contours are rejected as merged regions or background.

---

## Circularity

Circularity is computed as:

```
4πA / P²
```

Circular objects produce values close to 1.

Irregular objects produce lower values.

This removes elongated conveyor artifacts.

---

## Solidity

Solidity measures:

```
Area / Convex Hull Area
```

Donuts are expected to be highly convex.

Objects with low solidity are rejected.

---

## Extent

Extent measures how efficiently the contour occupies its bounding box.

Very sparse or elongated objects are discarded.

---

## Bounding Box Size

Contours whose width or height fall outside expected donut dimensions are rejected.

This further improves robustness against noise.

---

# 7. Donut Mask Generation

Validated contours are filled to generate the donut mask.

Each accepted contour is rendered as a solid binary object.

The resulting mask represents the complete donut region.

---

# 8. Hole Extraction

The central donut hole is extracted independently.

Instead of detecting holes directly from the original image, hole candidates are obtained by subtracting the donut mask from the morphologically processed binary image.

```
Morphology
      │
      ▼
Subtract Donut Mask
      │
      ▼
Hole Candidates
```

Candidate holes are filtered using:

- minimum area
- maximum area
- circularity

Only valid hole contours are retained.

Finally, the hole mask is subtracted from the donut mask to produce the final donut segmentation.

---

# 9. Boundary Refinement

Although contour extraction produces accurate masks, small irregularities may remain.

Boundary refinement improves contour smoothness.

The refinement pipeline includes:

```
Opening
↓

Closing
↓

Median Blur
↓

Contour Approximation
```

Contour approximation simplifies boundaries while preserving the overall donut shape.

This produces smoother masks suitable for industrial inspection.

---

# 10. Overlay Generation

For visualization, refined contours are drawn on the original image.

Color coding:

- Green: donut boundary
- Red: donut hole

The overlay enables visual verification of segmentation quality.

---

# Design Decisions

Several classical computer vision techniques were selected because they provide:

- deterministic behaviour
- fast execution
- high interpretability
- no training requirements
- reproducible results

The pipeline avoids machine learning and deep learning models, making it lightweight and suitable for industrial deployment.

---

# Limitations

The current implementation performs well on the provided dataset but may become less reliable under extreme conditions such as:

- severe motion blur
- extremely low contrast
- heavily overlapping donuts
- complete object occlusion

These limitations can be addressed in future work using adaptive illumination correction, more advanced contour fitting, or model-based geometric validation.

---

# Summary

The proposed pipeline combines preprocessing, adaptive thresholding, morphological operations, contour analysis, geometric filtering, hole extraction, and boundary refinement to generate accurate donut masks using only classical computer vision techniques.

The implementation is fully modular, reproducible, and written in modern C++17 using OpenCV, making it suitable for industrial inspection applications.