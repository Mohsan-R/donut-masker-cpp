#include "ImageLoader.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

std::vector<std::string> ImageLoader::getImagePaths(const std::string& folderPath)
{
    std::vector<std::string> imagePaths;

    if (!fs::exists(folderPath))
    {
        std::cerr << "Folder not found: "
                  << folderPath
                  << std::endl;

        return imagePaths;
    }

    for (const auto& entry : fs::directory_iterator(folderPath))
    {
        if (!entry.is_regular_file())
            continue;

        std::string extension = entry.path().extension().string();

        std::transform(
            extension.begin(),
            extension.end(),
            extension.begin(),
            ::tolower);

        if (extension == ".jpg" ||
            extension == ".jpeg" ||
            extension == ".png" ||
            extension == ".bmp")
        {
            imagePaths.push_back(entry.path().string());
        }
    }

    std::sort(imagePaths.begin(), imagePaths.end());

    return imagePaths;
}

cv::Mat ImageLoader::loadImage(const std::string& imagePath)
{
    return cv::imread(imagePath);
}