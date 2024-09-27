#include "gaze.h"
#include <LandmarkDetectorFunc.h>
#include <GazeEstimation.h>
#include <sstream>
#include <vector>


// ----------------------
// Gaze structure methods
// ----------------------

std::string Gaze::toString() const
{
    std::ostringstream stream;
    stream << "(" << eye1 << ", " << direction1 << ", " << eye2 << ", " << direction2 << ", " << angle << ")";

    return stream.str();
}


// -----------------------------
// GazeExtractor methods - setup
// -----------------------------

GazeExtractor::GazeExtractor(bool videoMode, 
                             std::optional<bool> wild, std::optional<bool> multi_view, std::optional<bool> limit_pose,
                             std::optional<int> n_iter, std::optional<float> reg_factor, std::optional<float> weight_factor)
    : LandmarkExtractor(videoMode, wild, multi_view, limit_pose, n_iter, reg_factor, weight_factor)
{
}

void GazeExtractor::setCameraCalibration(float fx, float fy, float cx, float cy)
{
    this->fx = fx;
    this->fy = fy;
    this->cx = cx;
    this->cy = cy;
}

void GazeExtractor::estimateCameraCalibration(const Frame& frame)
{
    // The same heuristic as in OpenFace
    fx = 500.0f * (frame.size().width / 640.0f);
    fy = 500.0f * (frame.size().height / 480.0f);
    fx = (fx + fy) / 2.0f;
    fy = fx;

    cx = frame.size().width / 2.0f;
    cy = frame.size().height / 2.0f;

    std::cout << "Camera calibration parameters estimated as: " << fx << ", " << fy << ", " << cx << ", " << cy << "\n";
}


// --------------------------------------
// GazeExtractor methods - gaze detection
// --------------------------------------

std::optional<Gaze> GazeExtractor::detectGaze(const Frame& frame, double timestamp, const BoundingBox& face)
{
    Gaze gaze;

    // Detect landmarks
    bool result = detectFaceLandmarks(frame, timestamp, face);
    if (!result)
        return std::nullopt;
    
    if (fx < 0)
        estimateCameraCalibration(frame);
    
    // Calculate eye landmarks in 3D space
    std::vector<cv::Point3f> eyeLandmarks3D = LandmarkDetector::Calculate3DEyeLandmarks(*model, fx, fy, cx, cy);

    // Calculate eye center based on the eye landmarks
    gaze.eye1 = calculateEyeCenter(eyeLandmarks3D, LEFT_EYE);
    gaze.eye2 = calculateEyeCenter(eyeLandmarks3D, RIGHT_EYE);

    // Calculate gaze direction
    GazeAnalysis::EstimateGaze(*model, gaze.direction1, fx, fy, cx, cy, true);
    GazeAnalysis::EstimateGaze(*model, gaze.direction2, fx, fy, cx, cy, false);
    gaze.angle = GazeAnalysis::GetGazeAngle(gaze.direction1, gaze.direction2);

    return std::make_optional(gaze);
}


// ----------------------------------------
// GazeExtractor methods - helper functions
// ----------------------------------------

cv::Point3f GazeExtractor::calculateEyeCenter(const std::vector<cv::Point3f> eyeLandmarks, Eye eye) const
{
    constexpr int NO_LANDMARKS = 8;
    cv::Point3f middle(0, 0, 0);

    for (int i = 0; i < NO_LANDMARKS; i++)
        middle = middle + eyeLandmarks[int(eye) * NO_LANDMARKS + i];
    middle = middle / 8;
    
    return middle;
}