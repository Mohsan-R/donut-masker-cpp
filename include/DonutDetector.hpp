#pragma once

#include <opencv2/opencv.hpp>

class DonutDetector
{
public:
    cv::Mat otsuThreshold(const cv::Mat& image);

    cv::Mat adaptiveThresholdImage(const cv::Mat& image);

    cv::Mat removeNoise(const cv::Mat& image);
};