#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

struct DetectionResult
{
    cv::Mat threshold;
    cv::Mat morphology;

    cv::Mat donutMask;
    cv::Mat holeMask;

    std::vector<std::vector<cv::Point>> donuts;
};