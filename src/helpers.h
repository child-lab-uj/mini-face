#pragma once

#include <opencv2/opencv.hpp>


// --------------
// Helper defines
// --------------

const std::string ROOT_DIRECTORY = PROJECT_ROOT;
const std::string MAIN_FILEPATH = ROOT_DIRECTORY + "/main.cpp";

using Frame = cv::Mat;
using BoundingBox = cv::Rect_<float>;


// ----------------
// Helper functions
// ----------------