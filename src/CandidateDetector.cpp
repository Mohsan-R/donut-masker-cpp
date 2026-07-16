#include "CandidateDetector.hpp"

std::vector<cv::Vec3f>
CandidateDetector::detectCircles(const cv::Mat& image)
{
    std::vector<cv::Vec3f> circles;

    cv::HoughCircles(
        image,
        circles,
        cv::HOUGH_GRADIENT,
        1.2,
        80,
        120,
        30,
        35,
        90);

    return circles;
}

std::vector<DonutCandidate>
CandidateDetector::detectCandidates(const cv::Mat& image)
{
    std::vector<DonutCandidate> candidates;

    auto circles =
        detectCircles(image);

    for (const auto& circle : circles)
    {
        DonutCandidate candidate;

        candidate.center =
            cv::Point2f(circle[0], circle[1]);

        candidate.radius =
            circle[2];

        candidates.push_back(candidate);
    }

    return candidates;
}