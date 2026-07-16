#include "DonutMaskGenerator.hpp"

cv::Mat DonutMaskGenerator::generateMask(
    const cv::Size& imageSize,
    const std::vector<DonutCandidate>& candidates)
{
    //----------------------------------------------------
    // Create Empty Binary Image
    //----------------------------------------------------

    cv::Mat mask =
        cv::Mat::zeros(
            imageSize,
            CV_8UC1);

    //----------------------------------------------------
    // Draw Every Detected Donut
    //----------------------------------------------------

    for (const auto& candidate : candidates)
    {
        cv::circle(
            mask,
            candidate.center,
            static_cast<int>(candidate.radius),
            cv::Scalar(255),
            -1);
    }

    //----------------------------------------------------
    // Return Binary Mask
    //----------------------------------------------------

    return mask;
}