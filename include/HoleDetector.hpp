#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

#include "CandidateDetector.hpp"

class HoleDetector
{
public:

    cv::Mat generateHoleMask(
        const cv::Mat& grayImage,
        const std::vector<DonutCandidate>& candidates);

private:

    cv::Rect createROI(
        const DonutCandidate& candidate,
        const cv::Size& imageSize);

    cv::Mat thresholdROI(
        const cv::Mat& roi);

    std::vector<std::vector<cv::Point>>
    extractContours(
        const cv::Mat& binary);

    int selectBestContour(
        const std::vector<std::vector<cv::Point>>& contours,
        const cv::Point2f& roiCenter);

    double circularity(
        const std::vector<cv::Point>& contour);
};