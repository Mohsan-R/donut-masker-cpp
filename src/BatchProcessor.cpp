#include "BatchProcessor.hpp"
#include "ImageLoader.hpp"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

void BatchProcessor::processFolder()
{
    ImageLoader loader;

    // Current working directory
    fs::path current = fs::current_path();

    // If we're inside output_samples, go back to the project root
    if (current.filename() == "output_samples")
    {
        current = current.parent_path();
    }

    fs::path inputFolder = current / "input_samples";

    std::cout << "Working Directory : "
              << current << "\n";

    std::cout << "Input Folder      : "
              << inputFolder << "\n\n";

    auto imagePaths = loader.getImagePaths(inputFolder.string());

    std::cout << "Images Found : "
              << imagePaths.size()
              << "\n\n";

    int count = 1;

    for (const auto& imagePath : imagePaths)
    {
        cv::Mat image = loader.loadImage(imagePath);

        if (image.empty())
        {
            std::cout << "Failed to load: "
                      << imagePath
                      << "\n";

            continue;
        }

        std::cout << "----------------------------------------\n";
        std::cout << "Image " << count++ << "\n";
        std::cout << "File     : "
                  << fs::path(imagePath).filename().string()
                  << "\n";
        std::cout << "Width    : "
                  << image.cols
                  << "\n";
        std::cout << "Height   : "
                  << image.rows
                  << "\n";
        std::cout << "Channels : "
                  << image.channels()
                  << "\n\n";
    }
}