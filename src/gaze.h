#pragma once

#include <opencv2/opencv.hpp>
#include <LandmarkCoreIncludes.h>
#include <optional>


// --------------
// Helper defines
// --------------

using Frame = cv::Mat;
using BoundingBox = cv::Rect_<float>;

enum Eye {LEFT_EYE = 0, RIGHT_EYE};

struct Gaze
{
    // I decided to change numbering from 0 and 1 to 1 and 2
    cv::Point3f eye1;
    cv::Point3f direction1;
    cv::Point3f eye2;
    cv::Point3f direction2;
    cv::Vec2d angle;
};


// -------------------
// GazeExtractor class
// -------------------

// This is the main API class
class GazeExtractor
{
public:
    GazeExtractor();
    
    // Setting the parameters
    void setCameraCalibration(float fx, float fy, float cx, float cy);
    void estimateCameraCalibration(const Frame& frame);

    // Gaze detection
    std::optional<Gaze> detectGaze(const Frame& frame, double timestamp, const BoundingBox& face);

private:
    // Helper functions
    cv::Point3f calculateEyeCenter(const std::vector<cv::Point3f> eyeLandmarks, Eye eye) const;

    // Camera parameters
    float fx = -1, fy = -1, cx = -1, cy = -1;

    // Face landmark detection model
    LandmarkDetector::FaceModelParameters params;
    LandmarkDetector::CLNF model;
};