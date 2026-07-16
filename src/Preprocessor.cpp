#include "Preprocessor.hpp"

cv::Mat Preprocessor::convertToGray(const cv::Mat& image)
{
    cv::Mat gray;

    cv::cvtColor(
        image,
        gray,
        cv::COLOR_BGR2GRAY);

    return gray;
}

cv::Mat Preprocessor::applyGaussianBlur(const cv::Mat& image)
{
    cv::Mat blurred;

    cv::GaussianBlur(
        image,
        blurred,
        cv::Size(5,5),
        0);

    return blurred;
}

cv::Mat Preprocessor::equalizeHistogram(const cv::Mat& image)
{
    cv::Mat equalized;

    cv::equalizeHist(
        image,
        equalized);

    return equalized;
}

cv::Mat Preprocessor::preprocess(const cv::Mat& image)
{
    cv::Mat gray =
        convertToGray(image);

    cv::Mat blur =
        applyGaussianBlur(gray);

    cv::Mat result =
        equalizeHistogram(blur);

    return result;
}