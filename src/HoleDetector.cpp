#include "HoleDetector.hpp"

#include <opencv2/imgproc.hpp>

cv::Rect HoleDetector::createROI(
    const DonutCandidate& candidate,
    const cv::Size& imageSize)
{
    //----------------------------------------------------
    // ROI Size
    //----------------------------------------------------

    int radius =
        static_cast<int>(candidate.radius);

    int x =
        std::max(
            0,
            static_cast<int>(candidate.center.x - radius));

    int y =
        std::max(
            0,
            static_cast<int>(candidate.center.y - radius));

    int width =
        std::min(
            radius * 2,
            imageSize.width - x);

    int height =
        std::min(
            radius * 2,
            imageSize.height - y);

    return cv::Rect(
        x,
        y,
        width,
        height);
}

cv::Mat HoleDetector::thresholdROI(
    const cv::Mat& roi)
{
    cv::Mat binary;

    cv::threshold(
        roi,
        binary,
        0,
        255,
        cv::THRESH_BINARY_INV |
        cv::THRESH_OTSU);

    cv::Mat kernel =
        cv::getStructuringElement(
            cv::MORPH_ELLIPSE,
            cv::Size(3,3));

    cv::morphologyEx(
        binary,
        binary,
        cv::MORPH_OPEN,
        kernel);

    return binary;
}

std::vector<std::vector<cv::Point>>
HoleDetector::extractContours(
    const cv::Mat& binary)
{
    std::vector<std::vector<cv::Point>> contours;

    cv::findContours(
        binary,
        contours,
        cv::RETR_EXTERNAL,
        cv::CHAIN_APPROX_SIMPLE);

    return contours;
}

double HoleDetector::circularity(
    const std::vector<cv::Point>& contour)
{
    double area =
        cv::contourArea(contour);

    double perimeter =
        cv::arcLength(
            contour,
            true);

    if (perimeter == 0)
        return 0.0;

    return
        4.0 *
        CV_PI *
        area /
        (perimeter * perimeter);
}

int HoleDetector::selectBestContour(
    const std::vector<std::vector<cv::Point>>& contours,
    const cv::Point2f& roiCenter)
{
    int bestIndex = -1;

    double bestDistance =
        1e9;

    for (int i = 0; i < contours.size(); i++)
    {
        double area =
            cv::contourArea(contours[i]);

        if (area < 100)
            continue;

        double circ =
            circularity(contours[i]);

        if (circ < 0.60)
            continue;

        cv::Moments m =
            cv::moments(contours[i]);

        if (m.m00 == 0)
            continue;

        cv::Point2f center(
            static_cast<float>(m.m10 / m.m00),
            static_cast<float>(m.m01 / m.m00));

        double distance =
            cv::norm(center - roiCenter);

        if (distance < bestDistance)
        {
            bestDistance = distance;
            bestIndex = i;
        }
    }

    return bestIndex;
}

cv::Mat HoleDetector::generateHoleMask(
    const cv::Mat& grayImage,
    const std::vector<DonutCandidate>& candidates)
{
    cv::Mat holeMask =
        cv::Mat::zeros(
            grayImage.size(),
            CV_8UC1);

    for (const auto& candidate : candidates)
    {
        //----------------------------------------------
        // Crop ROI
        //----------------------------------------------

        cv::Rect roiRect =
            createROI(
                candidate,
                grayImage.size());

        cv::Mat roi =
            grayImage(roiRect);

        //----------------------------------------------
        // Threshold ROI
        //----------------------------------------------

        cv::Mat binary =
            thresholdROI(roi);

        //----------------------------------------------
        // Find Contours
        //----------------------------------------------

        auto contours =
            extractContours(binary);

        //----------------------------------------------
        // ROI Center
        //----------------------------------------------

        cv::Point2f roiCenter(
            roi.cols / 2.0f,
            roi.rows / 2.0f);

        //----------------------------------------------
        // Select Best Contour
        //----------------------------------------------

        int best =
            selectBestContour(
                contours,
                roiCenter);

        if (best == -1)
            continue;

        //----------------------------------------------
        // Draw Filled Hole
        //----------------------------------------------

        cv::Mat roiMask =
            cv::Mat::zeros(
                roi.size(),
                CV_8UC1);

        cv::drawContours(
            roiMask,
            contours,
            best,
            cv::Scalar(255),
            cv::FILLED);

        roiMask.copyTo(
            holeMask(roiRect));
    }

    return holeMask;
}