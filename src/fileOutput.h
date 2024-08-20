#pragma once

#include "extractor.h"
#include "visualization.h"
#include <numeric>


// Processes given video and saves all detected gaze data in output text file
void process_gaze(std::string videoPath, std::string outputFile = "results.txt",
                                         int maxNoFrames = std::numeric_limits<int>::max(),
                                         CameraCalibration* ccal = nullptr);