#pragma once

#include "ImageLoader.hpp"
#include "Preprocessor.hpp"
#include "DonutDetector.hpp"
#include "BoundaryRefiner.hpp"

class BatchProcessor
{
public:
    void processFolder();

private:
    ImageLoader loader;
    Preprocessor preprocessor;
    DonutDetector detector;
    BoundaryRefiner refiner;
};