#pragma once

#include "landmarks.h"
#include <vector>


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
class GazeExtractor : public LandmarkExtractor
{
public:
    /* 
        Brief description of parameters:
        - videoMode: decides to use DetectLandmarksInVideo() or DetectLandmarksInImage() function.
                     I noticed from a practice that videoMode = false gives better results (combined with multi_view).
        - wild: a specyfic set of parameters for tough conditions (various lighting, incomplete view of the face)
        - multi_view: decides whether to consider multiple views during model reinit.
                      It significantly improves the results when combined with DetectLandmarksInImage()
        - limit_pose: should pose be limited to 180 degrees frontal.
        - n_iter: number of optimization iterations.
        - reg_factor: regularization parameter.
        - weight_factor: refers to how much weight is applied to certain constraints during the optimization process.
    */
    GazeExtractor(std::string model_loc, bool videoMode, 
                  std::optional<bool> wild, std::optional<bool> multi_view, std::optional<bool> limit_pose,
                  std::optional<int> n_iter, std::optional<float> reg_factor, std::optional<float> weight_factor);
    
    // Setting the parameters
    // I decided to keep this method to allow one extractor read frames from multiple cameras
    void setCameraCalibration(float fx, float fy, float cx, float cy);

    // Gaze detection
    std::optional<Gaze> detectGaze(const Frame& frame, double timestamp, const BoundingBox& roi);

private:
    // Helper functions
    cv::Point3f calculateEyeCenter(const std::vector<cv::Point3f> eyeLandmarks, Eye eye) const;

    // Camera parameters
    float fx = -1, fy = -1, cx = -1, cy = -1;
};