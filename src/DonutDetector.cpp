#include "DonutDetector.hpp"

#include <opencv2/imgproc.hpp>

cv::Mat DonutDetector::otsuThreshold(const cv::Mat& image)
{
    cv::Mat binary;

    cv::threshold(
        image,
        binary,
        0,
        255,
        cv::THRESH_BINARY_INV | cv::THRESH_OTSU);

    return binary;
}

cv::Mat DonutDetector::adaptiveThresholdImage(const cv::Mat& image)
{
    cv::Mat binary;

    cv::adaptiveThreshold(
        image,
        binary,
        255,
        cv::ADAPTIVE_THRESH_GAUSSIAN_C,
        cv::THRESH_BINARY_INV,
        31,
        5);

    return binary;
}

cv::Mat DonutDetector::removeNoise(const cv::Mat& image)
{
    cv::Mat result;

    cv::Mat kernel =
        cv::getStructuringElement(
            cv::MORPH_ELLIPSE,
            cv::Size(5,5));

    cv::morphologyEx(
        image,
        result,
        cv::MORPH_OPEN,
        kernel);

    return result;
}