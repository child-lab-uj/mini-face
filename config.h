#pragma once

#include <string>
#include <vector>


// Main parameters
constexpr bool RUN_VIDEO = true;
constexpr int MAX_NO_FRAMES = 1100;

// Main filepaths
const std::string ROOT_DIRECTORY = "C:/Programowanie/AGH-engeenerka/gaze-tracking";
const std::string CONFIG_FILEPATH = ROOT_DIRECTORY + "/config.h";

//const std::string INPUT_FILE = ROOT_DIRECTORY + "/dev/data/point_5_1_kam_4.avi";
const std::string INPUT_FILE = ROOT_DIRECTORY + "/dev/data/point_5_3_kam_3a.avi";
const std::string OUTPUT_FILE = ROOT_DIRECTORY + "/dev/output/test_video.avi";
//const std::string TEST_IMAGE_FILE = ROOT_DIRECTORY + "/dev/data/test_image.jpg";
//const std::string TEST_IMAGE_FILE = ROOT_DIRECTORY + "/dev/data/test_image_two_persons.jpg";
const std::string TEST_IMAGE_FILE = ROOT_DIRECTORY + "/dev/data/test_image_from_video.png";
//const std::string TEST_IMAGE_FILE = ROOT_DIRECTORY + "/dev/data/test_image_from_video_hard.png";

// OpenFace model filepaths
const std::string MTCNN_MODEL_FILE = ROOT_DIRECTORY + "/model/mtcnn_detector/MTCNN_detector.txt";

// OpenFace runtime parameters
constexpr bool USE_MULTI_VIEW = true;
constexpr float FACE_DETECTION_THRESHOLD = 0.75f;