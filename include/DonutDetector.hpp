#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

struct DetectionResult
{
    cv::Mat threshold;
    cv::Mat morphology;

    cv::Mat donutMask;
    cv::Mat holeMask;

    std::vector<std::vector<cv::Point>> donutContours;
};

class DonutDetector
{
public:
    DonutDetector() = default;
    ~DonutDetector() = default;

    DetectionResult detect(const cv::Mat& image);

private:
    cv::Mat applyThreshold(
        const cv::Mat& image) const;

    cv::Mat applyMorphology(
        const cv::Mat& binary) const;

    std::vector<std::vector<cv::Point>>
    detectContours(
        const cv::Mat& binary) const;

    std::vector<std::vector<cv::Point>>
    filterContours(
        const std::vector<std::vector<cv::Point>>& contours,
        const cv::Size& imageSize) const;

    cv::Mat createDonutMask(
        const cv::Size& size,
        const std::vector<std::vector<cv::Point>>& contours) const;

    cv::Mat createHoleMask(
        const cv::Mat& threshold,
        const std::vector<std::vector<cv::Point>>& donuts) const;

    bool isValidContour(
        const std::vector<cv::Point>& contour,
        const cv::Size& imageSize) const;

    double circularity(
        const std::vector<cv::Point>& contour) const;

    double solidity(
        const std::vector<cv::Point>& contour) const;
};
