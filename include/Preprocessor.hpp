#pragma once

#include <opencv2/opencv.hpp>

class Preprocessor
{
public:

    cv::Mat convertToGray(const cv::Mat& image);

    cv::Mat applyGaussianBlur(const cv::Mat& image);

    cv::Mat equalizeHistogram(const cv::Mat& image);

    cv::Mat preprocess(const cv::Mat& image);
};