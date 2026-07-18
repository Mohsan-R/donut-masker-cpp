#pragma once

#include <opencv2/opencv.hpp>

#include <string>
#include <vector>

class ImageLoader
{
public:

    // Returns all supported image paths in the specified directory.
    std::vector<std::string> getImagePaths(
        const std::string& folderPath);

    // Loads a BGR image from disk.
    cv::Mat loadImage(
        const std::string& imagePath);
};