#include "ContourExtractor.hpp"

std::vector<std::vector<cv::Point>>
ContourExtractor::extractContours(
    const cv::Mat& binaryMask) const
{
    std::vector<std::vector<cv::Point>> contours;

    cv::findContours(
        binaryMask.clone(),
        contours,
        cv::RETR_EXTERNAL,
        cv::CHAIN_APPROX_SIMPLE);

    return contours;
}

cv::Mat ContourExtractor::drawFilledContours(
    const cv::Mat& image,
    const std::vector<std::vector<cv::Point>>& contours) const
{
    cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1);

    if (!contours.empty())
    {
        cv::drawContours(
            mask,
            contours,
            -1,
            cv::Scalar(255),
            cv::FILLED);
    }

    return mask;
}
