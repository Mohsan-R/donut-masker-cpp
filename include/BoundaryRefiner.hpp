#pragma once

#include <opencv2/opencv.hpp>

#include "DonutDetector.hpp"

// Stores the refined masks and contours returned by boundary refinement.
struct RefinementResult
{
    cv::Mat refinedDonutMask;
    cv::Mat refinedHoleMask;

    std::vector<std::vector<cv::Point>> refinedContours;
};

// Refines detected donut boundaries into cleaner masks and contours.
class BoundaryRefiner
{
public:
    // Refines the result of donut detection.
    RefinementResult refine(
        const DetectionResult& detection) const;

private:
    // Refines a binary mask before contour extraction.
    cv::Mat refineMask(
        const cv::Mat& mask) const;

    // Smooths contours extracted from the refined mask.
    std::vector<std::vector<cv::Point>>
    smoothContours(
        const cv::Mat& mask) const;

    // Draws a mask from a set of contours.
    cv::Mat drawMask(
        const cv::Size& size,
        const std::vector<std::vector<cv::Point>>& contours) const;
};