#pragma once

#include <opencv2/opencv.hpp>

class Preprocessor
{
public:
    Preprocessor() = default;
    ~Preprocessor() = default;

    // Complete preprocessing pipeline
    cv::Mat process(const cv::Mat& input) const;

private:
    cv::Mat extractBlueYellowChannel(const cv::Mat& input) const;
    cv::Mat correctIllumination(const cv::Mat& input) const;
    cv::Mat normalizeContrast(const cv::Mat& input) const;
    cv::Mat reduceNoise(const cv::Mat& input) const;
};