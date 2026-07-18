#pragma once

#include <opencv2/opencv.hpp>

#include "DonutDetector.hpp"

struct RefinementResult
{
    cv::Mat refinedDonutMask;
    cv::Mat refinedHoleMask;

    std::vector<std::vector<cv::Point>> refinedContours;
};

class BoundaryRefiner
{
public:

    RefinementResult refine(
        const DetectionResult& detection) const;

private:

    cv::Mat refineMask(
        const cv::Mat& mask) const;

    std::vector<std::vector<cv::Point>>
    smoothContours(
        const cv::Mat& mask) const;

    cv::Mat drawMask(
        const cv::Size& size,
        const std::vector<std::vector<cv::Point>>& contours) const;
};