#include "fileOutput.h"
#include "../config.h"
#include <iostream>
#include <fstream>


void process_gaze(std::string videoPath, std::string outputFile, 
                                         int maxNoFrames,
                                         CameraCalibration* ccal)
{
    // Initialize video reader
    VideoIO video(videoPath, "");

    // Extractor parameters
    std::vector<std::string> FaceModelArgs = {
        CONFIG_FILEPATH,     // To extract root directory
        "-multi_view", USE_MULTI_VIEW ? "1" : "0"
    };

    // Initialize extractor
    CameraCalibration activeCcal = ccal == nullptr ? video.getCalibration() : *ccal;
    GazeExtractor extractor(FaceModelArgs, activeCcal, false, 2);

    // Open output file
    std::ofstream file(outputFile, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Unable to open file: " << outputFile << std::endl;
        file.close();
        return;
    }

    int frameID = 0;
    std::optional<Frame> frameOpt = video.loadNextFrame();
    while (frameOpt.has_value() && frameID++ < maxNoFrames) {
        Frame frame = frameOpt.value();

        if (extractor.process(frame)) {
            // Create and save text representation of data
            file << frameID << ":";
            extractor.printResults(file);
            file << "\n";
        }

        frameOpt = video.loadNextFrame();
    }

    file.close();
}