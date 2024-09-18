#pragma once

#include "helpers.h"
#include <LandmarkDetectorModel.h>
#include <optional>


// --------------
// Helper defines
// --------------

enum Eye {LEFT_EYE = 0, RIGHT_EYE};

struct Gaze
{
    // I decided to change numbering from 0 and 1 to 1 and 2
    cv::Point3f eye1;
    cv::Point3f direction1;
    cv::Point3f eye2;
    cv::Point3f direction2;
    cv::Vec2d angle;

    Gaze() = default;
    Gaze(cv::Point3f e1, cv::Point3f d1, cv::Point3f e2, cv::Point3f d2, cv::Vec2d a)
        : eye1(e1), direction1(d1), eye2(e2), direction2(d2), angle(a) {}

    std::string toString() const;
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
    std::optional<Gaze> detectGaze(const Frame& frame, double timestamp, const BoundingBox& roi);

private:
    // Helper functions
    cv::Point3f calculateEyeCenter(const std::vector<cv::Point3f> eyeLandmarks, Eye eye) const;

    // Camera parameters
    float fx = -1, fy = -1, cx = -1, cy = -1;

    // Face landmark detection model
    LandmarkDetector::FaceModelParameters params;
    LandmarkDetector::CLNF model;
};