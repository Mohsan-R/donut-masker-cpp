#include "ContourExtractor.hpp"

std::vector<std::vector<cv::Point>>
ContourExtractor::extractContours(const cv::Mat& binaryImage)
{
    std::vector<std::vector<cv::Point>> contours;

    cv::findContours(
        binaryImage,
        contours,
        cv::RETR_EXTERNAL,
        cv::CHAIN_APPROX_SIMPLE);

    return contours;
}

std::vector<std::vector<cv::Point>>
ContourExtractor::filterContoursByArea(
    const std::vector<std::vector<cv::Point>>& contours,
    double minArea)
{
    std::vector<std::vector<cv::Point>> filtered;

    for (const auto& contour : contours)
    {
        if (cv::contourArea(contour) >= minArea)
        {
            filtered.push_back(contour);
        }
    }

    return filtered;
}

cv::Mat ContourExtractor::drawFilledContours(
    const cv::Mat& referenceImage,
    const std::vector<std::vector<cv::Point>>& contours)
{
    cv::Mat mask =
        cv::Mat::zeros(
            referenceImage.size(),
            CV_8UC1);

    cv::drawContours(
        mask,
        contours,
        -1,
        cv::Scalar(255),
        cv::FILLED);

    return mask;
}