#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

class ContourExtractor
{
public:
    ContourExtractor() = default;
    ~ContourExtractor() = default;

    std::vector<std::vector<cv::Point>> extractContours(
        const cv::Mat& binaryMask) const;

    cv::Mat drawFilledContours(
        const cv::Mat& image,
        const std::vector<std::vector<cv::Point>>& contours) const;
};
