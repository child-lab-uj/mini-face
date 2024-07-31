#pragma once

#include <string>


// Main parameters
constexpr bool RUN_VIDEO = false;
constexpr int MAX_NO_FRAMES = 1000;

// Main filepaths
const std::string ROOT_DIRECTORY = "C:/Programowanie/AGH-engeenerka/gaze-tracking-cpp";

const std::string INPUT_FILE = ROOT_DIRECTORY + "/dev/data/point_5_1_kam_4.avi";
const std::string OUTPUT_FILE = ROOT_DIRECTORY + "/dev/output/test_video.avi";
const std::string TEST_IMAGE_FILE = ROOT_DIRECTORY + "/dev/data/test_image.jpg";

// OpenFace model filepaths
const std::string MTCNN_MODEL_FILE = ROOT_DIRECTORY + "/model/mtcnn_detector/MTCNN_detector.txt";