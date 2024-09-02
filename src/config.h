#pragma once

#include <string>


// ---------
// Filepaths
// ---------

const std::string ROOT_DIRECTORY = "C:/Programowanie/AGH-engeenerka/gaze-tracking";
const std::string CONFIG_FILEPATH = ROOT_DIRECTORY + "/config.h";


// ---------------------------
// OpenFace runtime parameters
// ---------------------------

constexpr bool USE_IMAGE_MODE = true;   // For LandmarkDetector, false = video mode (results are significantly worse)
constexpr bool USE_MULTI_VIEW = true;   // Improves landmark detector abilities, but only with image mode