#include "ImageLoader.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

// Scan a folder and collect all supported image file paths.
std::vector<std::string> ImageLoader::getImagePaths(const std::string& folderPath)
{
    std::vector<std::string> imagePaths;

    // Return an empty list if the folder does not exist.
    if (!fs::exists(folderPath))
    {
        std::cerr << "Folder not found: "
                  << folderPath
                  << std::endl;

        return imagePaths;
    }

    // Inspect each entry in the directory and keep supported image files.
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

        // Accept common image formats used by the pipeline.
        if (extension == ".jpg" ||
            extension == ".jpeg" ||
            extension == ".png" ||
            extension == ".bmp")
        {
            imagePaths.push_back(entry.path().string());
        }
    }

    // Keep the output order deterministic.
    std::sort(imagePaths.begin(), imagePaths.end());

    return imagePaths;
}

// Load an image from disk into an OpenCV matrix.
cv::Mat ImageLoader::loadImage(const std::string& imagePath)
{
    return cv::imread(imagePath);
}