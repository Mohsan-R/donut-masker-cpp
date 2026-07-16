#pragma once

#include <opencv2/opencv.hpp>

#include <string>
#include <vector>

class ImageLoader
{
public:
    std::vector<std::string> getImagePaths(const std::string& folderPath);

    cv::Mat loadImage(const std::string& imagePath);
};