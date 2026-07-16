#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

#include "CandidateDetector.hpp"

class DonutMaskGenerator
{
public:

    cv::Mat generateMask(
        const cv::Size& imageSize,
        const std::vector<DonutCandidate>& candidates);

};