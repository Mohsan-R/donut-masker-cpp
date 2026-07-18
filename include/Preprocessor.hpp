#pragma once

#include <opencv2/opencv.hpp>

// Preprocesses images before detection.
class Preprocessor
{
public:
    Preprocessor() = default;
    ~Preprocessor() = default;

    // Runs the full preprocessing pipeline.
    cv::Mat process(const cv::Mat& input) const;

private:
    // Extracts the blue-yellow channel used by the pipeline.
    cv::Mat extractBlueYellowChannel(const cv::Mat& input) const;

    // Corrects illumination variations in the image.
    cv::Mat correctIllumination(const cv::Mat& input) const;

    // Normalizes contrast across the image.
    cv::Mat normalizeContrast(const cv::Mat& input) const;

    // Reduces noise in the processed image.
    cv::Mat reduceNoise(const cv::Mat& input) const;
};