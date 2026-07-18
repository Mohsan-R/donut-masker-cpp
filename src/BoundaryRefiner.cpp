#include "BoundaryRefiner.hpp"

using namespace cv;

namespace
{
    constexpr int OPEN_KERNEL = 3;
    constexpr int CLOSE_KERNEL = 5;

    constexpr double EPSILON_FACTOR = 0.003;
}

RefinementResult BoundaryRefiner::refine(
    const DetectionResult& detection) const
{
    RefinementResult result;

    //-----------------------------------------
    // Refine donut mask
    //-----------------------------------------

    Mat donut =
        refineMask(
            detection.donutMask);

    auto donutContours =
        smoothContours(donut);

    result.refinedDonutMask =
        drawMask(
            donut.size(),
            donutContours);

    //-----------------------------------------
    // Refine hole mask
    //-----------------------------------------

    Mat holes =
        refineMask(
            detection.holeMask);

    auto holeContours =
        smoothContours(holes);

    result.refinedHoleMask =
        drawMask(
            holes.size(),
            holeContours);

    //-----------------------------------------
    // Remove holes from donut mask
    //-----------------------------------------

    bitwise_and(
        result.refinedDonutMask,
        ~result.refinedHoleMask,
        result.refinedDonutMask);

    result.refinedContours =
        donutContours;

    return result;
}

cv::Mat BoundaryRefiner::refineMask(
    const Mat& mask) const
{
    Mat refined = mask.clone();

    //-----------------------------------------
    // Remove small spikes
    //-----------------------------------------

    Mat openKernel =
        getStructuringElement(
            MORPH_ELLIPSE,
            Size(OPEN_KERNEL,
                 OPEN_KERNEL));

    morphologyEx(
        refined,
        refined,
        MORPH_OPEN,
        openKernel);

    //-----------------------------------------
    // Close small gaps
    //-----------------------------------------

    Mat closeKernel =
        getStructuringElement(
            MORPH_ELLIPSE,
            Size(CLOSE_KERNEL,
                 CLOSE_KERNEL));

    morphologyEx(
        refined,
        refined,
        MORPH_CLOSE,
        closeKernel);

    //-----------------------------------------
    // Smooth edges
    //-----------------------------------------

    medianBlur(
        refined,
        refined,
        3);

    return refined;
}

std::vector<std::vector<cv::Point>>
BoundaryRefiner::smoothContours(
    const Mat& mask) const
{
    std::vector<std::vector<Point>> contours;

    findContours(
        mask,
        contours,
        RETR_EXTERNAL,
        CHAIN_APPROX_NONE);

    std::vector<std::vector<Point>> refined;

    for (const auto& contour : contours)
    {
        double perimeter =
            arcLength(contour, true);

        std::vector<Point> approx;

        approxPolyDP(
            contour,
            approx,
            EPSILON_FACTOR * perimeter,
            true);

        refined.push_back(approx);
    }

    return refined;
}

cv::Mat BoundaryRefiner::drawMask(
    const Size& size,
    const std::vector<std::vector<Point>>& contours) const
{
    Mat mask =
        Mat::zeros(
            size,
            CV_8UC1);

    drawContours(
        mask,
        contours,
        -1,
        Scalar(255),
        FILLED);

    return mask;
}