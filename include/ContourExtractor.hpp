#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

class ContourExtractor
{
public:

    std::vector<std::vector<cv::Point>>
    extractContours(const cv::Mat& binaryImage);

    std::vector<std::vector<cv::Point>>
    filterContoursByArea(
        const std::vector<std::vector<cv::Point>>& contours,
        double minArea);

    cv::Mat drawFilledContours(
        const cv::Mat& referenceImage,
        const std::vector<std::vector<cv::Point>>& contours);
};