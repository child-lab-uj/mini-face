#include "gaze.h"
#include "config.h"
#include <LandmarkDetectorFunc.h>
#include <GazeEstimation.h>
#include <vector>


namespace {

    std::vector<std::string> FaceModelArgs = {
        CONFIG_FILEPATH, // To extract root directory
        "-multi_view", USE_MULTI_VIEW ? "1" : "0"
    };

}


// -----------------------------
// GazeExtractor methods - setup
// -----------------------------

GazeExtractor::GazeExtractor()
    : params(FaceModelArgs), model(params.model_location)
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
}


// --------------------------------------
// GazeExtractor methods - gaze detection
// --------------------------------------

std::optional<Gaze> GazeExtractor::detectGaze(const Frame& frame, double timestamp, const BoundingBox& face)
{
    Frame empty;
    Gaze gaze;

    // Detect landmarks
    bool result = USE_IMAGE_MODE ? LandmarkDetector::DetectLandmarksInImage(frame, face, model, params, empty) :
                                   LandmarkDetector::DetectLandmarksInVideo(frame, face, model, params, empty);
    if (!result)
        return std::nullopt;
    
    // Calculate eye landmarks in 3D space
    std::vector<cv::Point3f> eyeLandmarks3D = LandmarkDetector::Calculate3DEyeLandmarks(model, fx, fy, cx, cy);

    // Calculate eye center based on the eye landmarks
    gaze.eye1 = calculateEyeCenter(eyeLandmarks3D, LEFT_EYE);
    gaze.eye2 = calculateEyeCenter(eyeLandmarks3D, RIGHT_EYE);

    // Calculate gaze direction
    GazeAnalysis::EstimateGaze(model, gaze.direction1, fx, fy, cx, cy, true);
    GazeAnalysis::EstimateGaze(model, gaze.direction2, fx, fy, cx, cy, false);
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