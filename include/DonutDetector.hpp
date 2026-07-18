#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

// Stores the result of donut detection.
struct DetectionResult
{
    cv::Mat threshold;
    cv::Mat morphology;

    cv::Mat donutMask;
    cv::Mat holeMask;

    std::vector<std::vector<cv::Point>> donutContours;
};

// Detects donut-shaped regions in an image.
class DonutDetector
{
public:
    DonutDetector() = default;
    ~DonutDetector() = default;

    // Runs the full detection pipeline on the provided image.
    DetectionResult detect(const cv::Mat& image);

private:
    // Applies thresholding to isolate candidate regions.
    cv::Mat applyThreshold(
        const cv::Mat& image) const;

    // Applies morphological operations to clean the mask.
    cv::Mat applyMorphology(
        const cv::Mat& binary) const;

    // Extracts contours from the processed binary image.
    std::vector<std::vector<cv::Point>>
    detectContours(
        const cv::Mat& binary) const;

    // Filters out invalid or weak contours.
    std::vector<std::vector<cv::Point>>
    filterContours(
        const std::vector<std::vector<cv::Point>>& contours,
        const cv::Size& imageSize) const;

    // Creates the donut mask from the selected contours.
    cv::Mat createDonutMask(
        const cv::Size& size,
        const std::vector<std::vector<cv::Point>>& contours) const;

    // Creates the hole mask from the detected donut contours.
    cv::Mat createHoleMask(
        const cv::Mat& threshold,
        const std::vector<std::vector<cv::Point>>& donuts) const;

    // Checks whether a contour is valid for detection.
    bool isValidContour(
        const std::vector<cv::Point>& contour,
        const cv::Size& imageSize) const;

    // Computes the circularity of a contour.
    double circularity(
        const std::vector<cv::Point>& contour) const;

    // Computes the solidity of a contour.
    double solidity(
        const std::vector<cv::Point>& contour) const;
};
