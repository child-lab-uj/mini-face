#include "config.h"
#include "src/fileOutput.h"
#include "test/test.h"
#include <iostream>

// Useful hints
// ------------
// 1. Combining DetectLandmarksInImage() with multi_view=true works much better than DetectLandmarksInVideo()


// -------------
// Main function
// -------------

int main(int argc, char* argv[])
{
    // Note
    // ----
    // You can comment / uncomment the sections below to choose the one you want to run

    // --------
    // Test API
    // --------

    // // Test mode - video parsing
    // if constexpr (RUN_VIDEO)
    //     test_video();
    // // Test mode - image parsing
    // else
    //     test_image();


    // --------
    // Main API
    // --------

    std::string video = "";
    std::string output = "results.txt";
    CameraCalibration ccal;
    int maxNoFrames = MAX_NO_FRAMES;

    // Arg parsing
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        std::size_t pos = arg.find('=');

        if (pos == std::string::npos) {
            std::cerr << "[ERROR] Argument without assigned value\n";
            return 1;
        }

        if (arg.compare(0, 5, "video") == 0)
            video = arg.substr(pos + 1);
        else if (arg.compare(0, 6, "output"))
            output = arg.substr(pos + 1);
        else if (arg.compare(0, 6, "frames"))
            maxNoFrames = std::stoi(arg.substr(pos + 1));
        // TODO: add camera calibration matrix parsing
    }

    if (video == "") {
        std::cerr << "[ERROR] Video file not specified\n";
        return 1;
    }

    // Main processing function
    process_gaze(video, output, maxNoFrames, nullptr);

    return 0;
}