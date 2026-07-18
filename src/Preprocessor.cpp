#include "Preprocessor.hpp"

using namespace cv;

// Run the full preprocessing pipeline in a fixed sequence.
cv::Mat Preprocessor::process(const cv::Mat& input) const
{
    CV_Assert(!input.empty());

    // Step 1: extract the most informative color channel.
    Mat labChannel = extractBlueYellowChannel(input);

    // Step 2: compensate for slow illumination variations.
    Mat illuminationCorrected = correctIllumination(labChannel);

    // Step 3: normalize contrast to stabilize later thresholding.
    Mat normalized = normalizeContrast(illuminationCorrected);

    // Step 4: reduce noise while preserving important edges.
    Mat denoised = reduceNoise(normalized);

    return denoised;
}

// Convert the image to Lab color space and return the blue-yellow channel.
cv::Mat Preprocessor::extractBlueYellowChannel(const cv::Mat& input) const
{
    Mat lab;
    cvtColor(input, lab, COLOR_BGR2Lab);

    std::vector<Mat> channels;
    split(lab, channels);

    // The b-channel helps separate the blue conveyor from the donuts.
    return channels[2];
}

// Estimate broad illumination changes and subtract them from the image.
cv::Mat Preprocessor::correctIllumination(const cv::Mat& input) const
{
    Mat background;

    // Estimate slow illumination changes with a large Gaussian blur.
    GaussianBlur(
        input,
        background,
        Size(151, 151),
        0);

    Mat corrected;

    subtract(
        input,
        background,
        corrected);

    normalize(
        corrected,
        corrected,
        0,
        255,
        NORM_MINMAX);

    corrected.convertTo(corrected, CV_8U);

    return corrected;
}

// Stretch the pixel values so the image has a more stable dynamic range.
cv::Mat Preprocessor::normalizeContrast(const cv::Mat& input) const
{
    Mat normalized;

    normalize(
        input,
        normalized,
        0,
        255,
        NORM_MINMAX);

    return normalized;
}

// Apply bilateral filtering to reduce noise while preserving important edges.
cv::Mat Preprocessor::reduceNoise(const cv::Mat& input) const
{
    Mat output;

    bilateralFilter(
        input,
        output,
        9,
        75,
        75);

    return output;
}