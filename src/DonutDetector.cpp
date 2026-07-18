#include "DonutDetector.hpp"

#include <limits>

namespace
{
    // Geometry Filtering Constants
    constexpr double MIN_DONUT_AREA = 4000.0;
    constexpr double MAX_DONUT_AREA = 200000.0;

    constexpr double MIN_CIRCULARITY = 0.70;
    constexpr double MIN_SOLIDITY = 0.92;
    constexpr double MIN_EXTENT = 0.50;

    constexpr int MIN_BOX_SIZE = 40;
    constexpr int MAX_BOX_SIZE = 250;

    // Hole Filtration Constraints
    constexpr double MIN_COMPONENT_AREA = 800.0;
    constexpr double MIN_HOLE_AREA = 300.0;
    constexpr double MAX_HOLE_AREA = 4000.0;
    constexpr double MIN_HOLE_CIRCULARITY = 0.50;
}

using namespace cv;

DetectionResult DonutDetector::detect(const Mat& image)
{
    DetectionResult result;

    // Phase 1: Binarization
    result.threshold = applyThreshold(image);

    // Phase 2: Morphological Cleanup
    result.morphology = applyMorphology(result.threshold);

    // Phase 3: Raw Contour Extraction
    auto contours = detectContours(result.morphology);

    // Phase 4: Geometric Shape Filtering
    result.donutContours = filterContours(contours, image.size());

    // Phase 5: Generate Solid White Disks for Valid Donuts
    result.donutMask =
        createDonutMask(
            image.size(),
            result.donutContours);

    // Phase 6: Extract Smooth Holes using Matrix Subtraction
    result.holeMask =
        createHoleMask(
            result.morphology, 
            result.donutContours);

    // Phase 7: Punch the holes out of the solid masks to create the final "rings of dough"[cite: 1]
    cv::bitwise_and(
        result.donutMask,
        ~result.holeMask,
        result.donutMask);

    return result;
}

cv::Mat DonutDetector::applyThreshold(const Mat& image) const
{
    Mat singleChannel;

    if (image.channels() == 3)
    {
        cv::cvtColor(image, singleChannel, cv::COLOR_BGR2GRAY);
    }
    else
    {
        singleChannel = image;
    }

    Mat blurred;
    GaussianBlur(singleChannel, blurred, Size(5,5), 0);

    Mat binary;
    threshold(blurred, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);

    return binary;
}

cv::Mat DonutDetector::applyMorphology(const Mat& binary) const
{
    Mat output = binary.clone();

    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(3,3));

    morphologyEx(output, output, MORPH_OPEN, kernel, Point(-1,-1), 2);
    morphologyEx(output, output, MORPH_CLOSE, kernel, Point(-1,-1), 2);
    medianBlur(output, output, 3);

    return output;
}

std::vector<std::vector<cv::Point>> DonutDetector::detectContours(const Mat& binary) const
{
    cv::Mat cleaned = binary.clone();
    std::vector<std::vector<cv::Point>> components;

    cv::findContours(cleaned, components, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    cleaned.setTo(0);

    for (const auto& contour : components)
    {
        double area = cv::contourArea(contour);
        if (area < MIN_COMPONENT_AREA) continue;

        cv::drawContours(
            cleaned,
            std::vector<std::vector<cv::Point>>{contour},
            -1,
            cv::Scalar(255),
            cv::FILLED);
    }

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(cleaned, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    return contours;
}

std::vector<std::vector<cv::Point>> DonutDetector::filterContours(
    const std::vector<std::vector<Point>>& contours,
    const Size& imageSize) const
{
    std::vector<std::vector<Point>> output;
    for (const auto& contour : contours)
    {
        if (isValidContour(contour, imageSize))
            output.push_back(contour);
    }
    return output;
}

cv::Mat DonutDetector::createDonutMask(
    const Size& size,
    const std::vector<std::vector<Point>>& contours) const
{
    cv::Mat mask = cv::Mat::zeros(size, CV_8UC1);
    cv::drawContours(mask, contours, -1, cv::Scalar(255), cv::FILLED);
    return mask;
}

bool DonutDetector::isValidContour(const std::vector<Point>& contour, const Size& imageSize) const
{
    // -------------------------------------------------------------------------
    // 1. Area Check
    // -------------------------------------------------------------------------
    double area = contourArea(contour);
    if (area < MIN_DONUT_AREA || area > MAX_DONUT_AREA) return false;

    // -------------------------------------------------------------------------
    // 2. Bounding Box Structural Limits Check
    // -------------------------------------------------------------------------
    Rect box = boundingRect(contour);
    if (box.width < MIN_BOX_SIZE || box.height < MIN_BOX_SIZE) return false;
    if (box.width > MAX_BOX_SIZE || box.height > MAX_BOX_SIZE) return false;

    // -------------------------------------------------------------------------
    // 3. Aspect Ratio Dimensional Normalization Check
    // -------------------------------------------------------------------------
    double aspect = static_cast<double>(box.width) / box.height;
    if (aspect < 0.75 || aspect > 1.25) return false;

    // -------------------------------------------------------------------------
    // 4. Circularity Verification
    // -------------------------------------------------------------------------
    double circ = circularity(contour);
    if (circ < MIN_CIRCULARITY) return false;

    // -------------------------------------------------------------------------
    // 5. Solidity Verification (Density Profiling)
    // -------------------------------------------------------------------------
    double solid = solidity(contour);
    if (solid < MIN_SOLIDITY) return false;

    // -------------------------------------------------------------------------
    // 6. Extent Spatial Fill Check
    // -------------------------------------------------------------------------
    double extent = area / (box.width * box.height);
    if (extent < MIN_EXTENT) return false;

    double perimeter = arcLength(contour, true);
    if (perimeter < 200.0) return false;

    // FIX: Removed the rigid BORDER_MARGIN condition.
    // Allowing objects to pass when touching edges ensures compliance with the mandatory 
    // "Handle partial donuts at frame borders" functional requirement[cite: 1].
    return true;
}

double DonutDetector::circularity(const std::vector<Point>& contour) const
{
    double area = contourArea(contour);
    double peri = arcLength(contour, true);
    if (peri <= 0.0) return 0.0;
    return (4.0 * CV_PI * area) / (peri * peri);
}

double DonutDetector::solidity(const std::vector<Point>& contour) const
{
    std::vector<Point> hull;
    convexHull(contour, hull);
    double hullArea = contourArea(hull);
    if (hullArea <= 0.0) return 0.0;
    return contourArea(contour) / hullArea;
}

cv::Mat DonutDetector::createHoleMask(
    const cv::Mat& morphologyMat,
    const std::vector<std::vector<cv::Point>>& donuts) const
{
    // 1. Generate a solid white disk mask using ONLY the validated donuts
    cv::Mat solidDonuts = cv::Mat::zeros(morphologyMat.size(), CV_8UC1);
    cv::drawContours(solidDonuts, donuts, -1, cv::Scalar(255), cv::FILLED);

    // 2. Subtract the hollow rings from the solid disks
    // This perfectly isolates the inner hole regions while canceling out the background![cite: 1]
    cv::Mat holesRegion;
    cv::subtract(solidDonuts, morphologyMat, holesRegion);

    // 3. Apply a quick morphological opening to remove any fringe noise or edge artifacts
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    cv::morphologyEx(holesRegion, holesRegion, cv::MORPH_OPEN, kernel);

    // 4. Trace the clean boundaries of these isolated hole regions
    std::vector<std::vector<cv::Point>> holeContours;
    cv::findContours(holesRegion, holeContours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat finalHoleMask = cv::Mat::zeros(morphologyMat.size(), CV_8UC1);

    // 5. Filter the hole candidates and render them perfectly smooth and solid
    for (const auto& contour : holeContours)
    {
        double area = cv::contourArea(contour);
        if (area >= MIN_HOLE_AREA && area <= MAX_HOLE_AREA)
        {
            double circ = circularity(contour);
            if (circ >= MIN_HOLE_CIRCULARITY)
            {
                // Drawing the clean outer contour filled creates a perfect smooth circle[cite: 1]
                cv::drawContours(
                    finalHoleMask, 
                    std::vector<std::vector<cv::Point>>{contour}, 
                    -1, 
                    cv::Scalar(255), 
                    cv::FILLED);
            }
        }
    }

    return finalHoleMask;
}