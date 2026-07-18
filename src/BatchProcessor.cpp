#include "BatchProcessor.hpp"
#include <opencv2/opencv.hpp>

#include <chrono>
#include <filesystem>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

namespace
{
    bool saveImage(const fs::path& path,
                   const cv::Mat& image)
    {
        if (!cv::imwrite(path.string(), image))
        {
            std::cerr
                << "Failed to save: "
                << path
                << '\n';

            return false;
        }

        return true;
    }
}

void BatchProcessor::processFolder()
{
    const fs::path current =
        fs::current_path();

    const fs::path inputFolder =
        current / "input_samples";

    const fs::path outputFolder =
        current / "output_samples";

    if (!fs::exists(inputFolder))
    {
        std::cerr
            << "Input folder not found: "
            << inputFolder
            << '\n';

        return;
    }

    const std::vector<std::string> folders =
    {
        "",
        "processed",
        "threshold",
        "morphology",
        "donut_masks",
        "hole_masks",
        "overlays"
    };

    for (const auto& folder : folders)
    {
        fs::create_directories(
            outputFolder / folder);
    }

    auto imagePaths =
        loader.getImagePaths(
            inputFolder.string());

    if (imagePaths.empty())
    {
        std::cout
            << "No images found in "
            << inputFolder
            << '\n';

        return;
    }

    std::cout
        << "\n=====================================\n";
    std::cout
        << "Donut Inspection Pipeline\n";
    std::cout
        << "Images : "
        << imagePaths.size()
        << '\n';
    std::cout
        << "=====================================\n\n";

    size_t processed = 0;
    size_t failed = 0;
    double totalTime = 0.0;

    int imageIndex = 1;

    //-----------------------------------------
    // Create the boundary refiner once and reuse it
    //-----------------------------------------

    BoundaryRefiner refiner;

    //-----------------------------------------
    // Process each input image in the batch
    //-----------------------------------------

    for (const auto& imagePath : imagePaths)
    {
        auto start =
            std::chrono::steady_clock::now();

        cv::Mat image =
            loader.loadImage(imagePath);

        if (image.empty())
        {
            ++failed;

            std::cout
                << "Cannot load "
                << imagePath
                << '\n';

            continue;
        }

        //-----------------------------------------
        // Preprocess the image before detection
        //-----------------------------------------

        cv::Mat processedImage =
            preprocessor.process(image);

        //-----------------------------------------
        // Detect the donut regions in the image
        //-----------------------------------------

        DetectionResult detection =
            detector.detect(processedImage);

        //-----------------------------------------
        // Refine the detected donut and hole masks
        //-----------------------------------------

        RefinementResult refined =
            refiner.refine(detection);

        //-----------------------------------------
        // Create a visualization overlay
        //-----------------------------------------

        cv::Mat overlay =
            image.clone();

        cv::drawContours(
            overlay,
            refined.refinedContours,
            -1,
            cv::Scalar(0,255,0),
            2);

        std::vector<std::vector<cv::Point>>
            holeContours;

        cv::findContours(
            refined.refinedHoleMask,
            holeContours,
            cv::RETR_EXTERNAL,
            cv::CHAIN_APPROX_SIMPLE);

        cv::drawContours(
            overlay,
            holeContours,
            -1,
            cv::Scalar(0,0,255),
            2);

        //-----------------------------------------
        // Save the intermediate and final results
        //-----------------------------------------

        const fs::path filename =
            fs::path(imagePath).filename();

        bool success = true;

        success &= saveImage(
            outputFolder / "processed" / filename,
            processedImage);

        success &= saveImage(
            outputFolder / "threshold" / filename,
            detection.threshold);

        success &= saveImage(
            outputFolder / "morphology" / filename,
            detection.morphology);

        success &= saveImage(
            outputFolder / "donut_masks" / filename,
            refined.refinedDonutMask);

        success &= saveImage(
            outputFolder / "hole_masks" / filename,
            refined.refinedHoleMask);

        success &= saveImage(
            outputFolder / "overlays" / filename,
            overlay);

        if (!success)
        {
            std::cerr
                << "One or more outputs could not be saved for "
                << filename.string()
                << '\n';
        }

        //-----------------------------------------
        // Report timing and processing statistics
        //-----------------------------------------

        auto end =
            std::chrono::steady_clock::now();

        auto duration =
            std::chrono::duration_cast<
                std::chrono::milliseconds>(
                    end - start);

        totalTime += duration.count();

        ++processed;

        std::cout
            << "["
            << imageIndex++
            << "/"
            << imagePaths.size()
            << "] "
            << filename.string()
            << " | Donuts: "
            << refined.refinedContours.size()
            << " | "
            << duration.count()
            << " ms\n";
    }

    //-----------------------------------------
    // Print the final batch summary
    //-----------------------------------------

    std::cout
        << "\n=====================================\n";
    std::cout
        << "Batch Processing Complete\n";
    std::cout
        << "=====================================\n";
    std::cout
        << "Processed : "
        << processed
        << '\n';
    std::cout
        << "Failed    : "
        << failed
        << '\n';

    if (processed > 0)
    {
        std::cout
            << "Average   : "
            << totalTime / processed
            << " ms/image\n";
    }

    std::cout << '\n';
}