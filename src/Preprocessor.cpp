#include "Preprocessor.hpp"

using namespace cv;

cv::Mat Preprocessor::process(const cv::Mat& input) const
{
    CV_Assert(!input.empty());

    // Step 1
    Mat labChannel = extractBlueYellowChannel(input);

    // Step 2
    Mat illuminationCorrected = correctIllumination(labChannel);

    // Step 3
    Mat normalized = normalizeContrast(illuminationCorrected);

    // Step 4
    Mat denoised = reduceNoise(normalized);

    return denoised;
}

cv::Mat Preprocessor::extractBlueYellowChannel(const cv::Mat& input) const
{
    Mat lab;
    cvtColor(input, lab, COLOR_BGR2Lab);

    std::vector<Mat> channels;
    split(lab, channels);

    // b-channel emphasizes blue conveyor against donuts
    return channels[2];
}

cv::Mat Preprocessor::correctIllumination(const cv::Mat& input) const
{
    Mat background;

    // Estimate slow illumination changes
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