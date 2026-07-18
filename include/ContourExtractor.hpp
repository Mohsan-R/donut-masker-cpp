#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

// Extracts and renders contours from binary image masks.
class ContourExtractor
{
public:
    ContourExtractor() = default;
    ~ContourExtractor() = default;

    // Extracts contours from a binary mask.
    std::vector<std::vector<cv::Point>> extractContours(
        const cv::Mat& binaryMask) const;

    // Draws filled contours onto the provided image.
    cv::Mat drawFilledContours(
        const cv::Mat& image,
        const std::vector<std::vector<cv::Point>>& contours) const;
};
