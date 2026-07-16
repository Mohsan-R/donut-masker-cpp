#include "BatchProcessor.hpp"
#include "ImageLoader.hpp"
#include "Preprocessor.hpp"
#include "DonutDetector.hpp"
#include "CandidateDetector.hpp"
#include "DonutMaskGenerator.hpp"
#include "HoleDetector.hpp"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

void BatchProcessor::processFolder()
{
    ImageLoader loader;
    Preprocessor preprocessor;
    DonutDetector detector;
    CandidateDetector candidateDetector;
    DonutMaskGenerator maskGenerator;
    HoleDetector holeDetector;

    //----------------------------------------------------
    // Working Directories
    //----------------------------------------------------

    fs::path current = fs::current_path();

    if (current.filename() == "output_samples")
    {
        current = current.parent_path();
    }

    fs::path inputFolder = current / "input_samples";
    fs::path outputFolder = current / "output_samples";

    fs::create_directories(outputFolder);

    //----------------------------------------------------
    // Console Header
    //----------------------------------------------------

    std::cout << "=========================================\n";
    std::cout << "         DONUT MASKER CPP\n";
    std::cout << "=========================================\n\n";

    std::cout << "Working Directory : "
              << current
              << "\n";

    std::cout << "Input Folder      : "
              << inputFolder
              << "\n";

    std::cout << "Output Folder     : "
              << outputFolder
              << "\n\n";

    //----------------------------------------------------
    // Load Images
    //----------------------------------------------------

    auto imagePaths =
        loader.getImagePaths(inputFolder.string());

    std::cout << "Images Found : "
              << imagePaths.size()
              << "\n\n";

    int count = 1;

    //----------------------------------------------------
    // Process Images
    //----------------------------------------------------

    for (const auto& imagePath : imagePaths)
    {
        cv::Mat image =
            loader.loadImage(imagePath);

        if (image.empty())
        {
            std::cout
                << "Failed to load : "
                << imagePath
                << "\n\n";

            continue;
        }

        //----------------------------------------------------
        // Step 1 - Preprocessing
        //----------------------------------------------------

        cv::Mat processed =
            preprocessor.preprocess(image);

        //----------------------------------------------------
        // Step 1 - Otsu Threshold
        //----------------------------------------------------

        cv::Mat otsu =
            detector.removeNoise(
                detector.otsuThreshold(processed));

        //----------------------------------------------------
        // Step 1 - Adaptive Threshold
        //----------------------------------------------------

        cv::Mat adaptive =
            detector.removeNoise(
                detector.adaptiveThresholdImage(processed));

        //----------------------------------------------------
        // Step 1 - Candidate Detection
        //----------------------------------------------------

        auto candidates =
            candidateDetector.detectCandidates(processed);

        //----------------------------------------------------
        // Step 1 - Initial Donut Mask
        //----------------------------------------------------

        cv::Mat donutMask =
            maskGenerator.generateMask(
                image.size(),
                candidates);

        //----------------------------------------------------
        // Step 1 - Initial Hole Mask
        //----------------------------------------------------

        cv::Mat holeMask =
            holeDetector.generateHoleMask(
                processed,
                candidates);

        //----------------------------------------------------
        // Draw Candidate Detection
        //----------------------------------------------------

        cv::Mat candidateImage =
            image.clone();

        for (const auto& candidate : candidates)
        {
            cv::circle(
                candidateImage,
                candidate.center,
                static_cast<int>(candidate.radius),
                cv::Scalar(0,255,0),
                3);

            cv::circle(
                candidateImage,
                candidate.center,
                3,
                cv::Scalar(0,0,255),
                -1);
        }

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
        // Save Otsu
        //----------------------------------------------------

        fs::path otsuFile =
            outputFolder /
            ("otsu_" +
             fs::path(imagePath).filename().string());

        cv::imwrite(
            otsuFile.string(),
            otsu);

        //----------------------------------------------------
        // Save Adaptive
        //----------------------------------------------------

        fs::path adaptiveFile =
            outputFolder /
            ("adaptive_" +
             fs::path(imagePath).filename().string());

        cv::imwrite(
            adaptiveFile.string(),
            adaptive);

        //----------------------------------------------------
        // Save Candidate Detection
        //----------------------------------------------------

        fs::path candidateFile =
            outputFolder /
            ("candidate_" +
             fs::path(imagePath).filename().string());

        cv::imwrite(
            candidateFile.string(),
            candidateImage);

        //----------------------------------------------------
        // Save Donut Mask
        //----------------------------------------------------

        fs::path donutMaskFile =
            outputFolder /
            ("donut_mask_" +
             fs::path(imagePath).filename().string());

        cv::imwrite(
            donutMaskFile.string(),
            donutMask);

        //----------------------------------------------------
        // Save Hole Mask
        //----------------------------------------------------

        fs::path holeMaskFile =
            outputFolder /
            ("hole_mask_" +
             fs::path(imagePath).filename().string());

        cv::imwrite(
            holeMaskFile.string(),
            holeMask);

        //----------------------------------------------------
        // Console Output
        //----------------------------------------------------

        std::cout << "----------------------------------------\n";

        std::cout << "Image        : "
                  << count++
                  << "\n";

        std::cout << "File         : "
                  << fs::path(imagePath).filename().string()
                  << "\n";

        std::cout << "Width        : "
                  << image.cols
                  << "\n";

        std::cout << "Height       : "
                  << image.rows
                  << "\n";

        std::cout << "Channels     : "
                  << image.channels()
                  << "\n";

        std::cout << "Candidates   : "
                  << candidates.size()
                  << "\n";

        std::cout << "Saved        : "
                  << processedFile.filename().string()
                  << "\n";

        std::cout << "Saved        : "
                  << otsuFile.filename().string()
                  << "\n";

        std::cout << "Saved        : "
                  << adaptiveFile.filename().string()
                  << "\n";

        std::cout << "Saved        : "
                  << candidateFile.filename().string()
                  << "\n";

        std::cout << "Saved        : "
                  << donutMaskFile.filename().string()
                  << "\n";

        std::cout << "Saved        : "
                  << holeMaskFile.filename().string()
                  << "\n\n";
    }

    //----------------------------------------------------
    // Finished
    //----------------------------------------------------

    std::cout << "=========================================\n";
    std::cout << "Batch Processing Complete\n";
    std::cout << "=========================================\n";
}