#include "BatchProcessor.hpp"
#include "ImageLoader.hpp"
#include "Preprocessor.hpp"
#include "DonutDetector.hpp"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

void BatchProcessor::processFolder()
{
    ImageLoader loader;
    Preprocessor preprocessor;
    DonutDetector detector;

    // Current working directory
    fs::path current = fs::current_path();

    // If running from output_samples, go back to project root
    if (current.filename() == "output_samples")
    {
        current = current.parent_path();
    }

    fs::path inputFolder = current / "input_samples";
    fs::path outputFolder = current / "output_samples";

    fs::create_directories(outputFolder);

    std::cout << "Working Directory : " << current << "\n";
    std::cout << "Input Folder      : " << inputFolder << "\n";
    std::cout << "Output Folder     : " << outputFolder << "\n\n";

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
                      << "\n\n";
            continue;
        }

        //----------------------------------------------------
        // Preprocessing
        //----------------------------------------------------

        cv::Mat processed =
            preprocessor.preprocess(image);

        //----------------------------------------------------
        // Otsu Threshold
        //----------------------------------------------------

        cv::Mat otsu =
            detector.removeNoise(
                detector.otsuThreshold(processed));

        //----------------------------------------------------
        // Adaptive Threshold
        //----------------------------------------------------

        cv::Mat adaptive =
            detector.removeNoise(
                detector.adaptiveThresholdImage(processed));

        //----------------------------------------------------
        // Save Processed Image
        //----------------------------------------------------

        fs::path processedFile =
            outputFolder /
            ("processed_" +
             fs::path(imagePath).filename().string());

        cv::imwrite(
            processedFile.string(),
            processed);

        //----------------------------------------------------
        // Save Otsu Image
        //----------------------------------------------------

        fs::path otsuFile =
            outputFolder /
            ("otsu_" +
             fs::path(imagePath).filename().string());

        cv::imwrite(
            otsuFile.string(),
            otsu);

        //----------------------------------------------------
        // Save Adaptive Image
        //----------------------------------------------------

        fs::path adaptiveFile =
            outputFolder /
            ("adaptive_" +
             fs::path(imagePath).filename().string());

        cv::imwrite(
            adaptiveFile.string(),
            adaptive);

        //----------------------------------------------------
        // Console Output
        //----------------------------------------------------

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
                  << "\n";

        std::cout << "Saved    : "
                  << processedFile.filename().string()
                  << "\n";

        std::cout << "Saved    : "
                  << otsuFile.filename().string()
                  << "\n";

        std::cout << "Saved    : "
                  << adaptiveFile.filename().string()
                  << "\n\n";
    }

    std::cout << "=========================================\n";
    std::cout << "Batch Processing Complete\n";
    std::cout << "=========================================\n";
}