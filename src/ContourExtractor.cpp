#include "ContourExtractor.hpp"

// Extract external contours from a binary image mask.
std::vector<std::vector<cv::Point>>
ContourExtractor::extractContours(
    const cv::Mat& binaryMask) const
{
    std::vector<std::vector<cv::Point>> contours;

    // Find the outermost contours only.
    cv::findContours(
        binaryMask.clone(),
        contours,
        cv::RETR_EXTERNAL,
        cv::CHAIN_APPROX_SIMPLE);

    return contours;
}

// Render all supplied contours as a filled binary mask.
cv::Mat ContourExtractor::drawFilledContours(
    const cv::Mat& image,
    const std::vector<std::vector<cv::Point>>& contours) const
{
    // Create an empty single-channel mask sized like the input image.
    cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1);

    if (!contours.empty())
    {
        // Fill every contour with white pixels.
        cv::drawContours(
            mask,
            contours,
            -1,
            cv::Scalar(255),
            cv::FILLED);
    }

    return mask;
}
