#pragma once

#include "ImageLoader.hpp"
#include "Preprocessor.hpp"
#include "DonutDetector.hpp"
#include "BoundaryRefiner.hpp"

// Coordinates the processing pipeline for a folder of images.
class BatchProcessor
{
public:
    // Processes all supported images in the target folder.
    void processFolder();

private:
    // Loads images from disk.
    ImageLoader loader;

    // Prepares images before detection.
    Preprocessor preprocessor;

    // Detects donut-shaped regions.
    DonutDetector detector;

    // Refines detected boundaries.
    BoundaryRefiner refiner;
};