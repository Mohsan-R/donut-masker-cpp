#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

struct DonutCandidate
{
    cv::Point2f center;
    float radius;
};

class CandidateDetector
{
public:

    std::vector<DonutCandidate>
    detectCandidates(const cv::Mat& image);

private:

    std::vector<cv::Vec3f>
    detectCircles(const cv::Mat& image);
};