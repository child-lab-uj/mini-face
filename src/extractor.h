#pragma once

#include "videoIO.h"
#include <vector>

#include <LandmarkCoreIncludes.h>


// -------------------
// Extractor interface
// -------------------

class Extractor
{
public:
    // Returns true if any face has been succesfully detected
    virtual bool process(const Frame& frame) = 0;
};


// -------------------------
// Extractor - face detector
// -------------------------

using BoundingBox = cv::Rect_<float>;

class FaceExtractor : public Extractor
{
public:
    FaceExtractor(int maxNoFaces = 2, float threshold = 0.5);

    bool process(const Frame& frame) override;

    // Processing results
    // Returns no more than (maxNoFaces) results with best confidence, all with confidence above (threshold)
    std::vector<BoundingBox> selectFaces();

private:
    // Detector
    LandmarkDetector::FaceDetectorMTCNN detector;

    // Detection results
    std::vector<BoundingBox> detectedFaces;
    std::vector<float> confidences;

    int maxNoFaces;
    float threshold;
};


// ------------------------
// CLNF Extractor interface
// ------------------------

// An abstract class for all future extractors which use CLNF class
class FaceDetailExtractor : public FaceExtractor
{
public:
    FaceDetailExtractor(bool video, int maxNoFaces, float threshold);
    FaceDetailExtractor(std::vector<std::string>& args, bool video, int maxNoFaces, float threshold);

    virtual bool process(const Frame& frame) override;

    // Abstract method for extracting data from CLNF face model
    virtual void saveResults() = 0;
    // Abstract method for reseting data containers or variables
    virtual void resetResults() = 0;

protected:
    LandmarkDetector::FaceModelParameters params;
    LandmarkDetector::CLNF faceModel;
    bool video;     // Decides whether to use DetectLandmarksInVideo() or DetectLandmarksInImage() function
};


// ------------------------------
// Extractor - landmark detection
// ------------------------------

class LandmarkExtractor : public FaceDetailExtractor
{
public:
    LandmarkExtractor(bool video = true, int maxNoFaces = 2, float threshold = 0.5);
    LandmarkExtractor(std::vector<std::string>& args, bool video = true, int maxNoFaces = 2, float threshold = 0.5);

    void saveResults() override;
    void resetResults() override;

    // Processing results
    std::vector<cv::Mat_<float>> detectedLandmarks2D;
    std::vector<cv::Mat_<int>> detectedLandmarkVisibilities;
};


// -------------------------
// Extractor - pose tracking
// -------------------------

class PoseExtractor : public FaceDetailExtractor
{
public:
    PoseExtractor(CameraCalibration ccal, bool video = true, int maxNoFaces = 2, float threshold = 0.5);
    PoseExtractor(std::vector<std::string>& args, CameraCalibration ccal, bool video = true, int maxNoFaces = 2, float threshold = 0.5);

    virtual void saveResults() override;
    virtual void resetResults() override;

    std::vector<cv::Vec6d> headPoses;

private:
    CameraCalibration ccal;
};


// -------------------------
// Extractor - gaze tracking
// -------------------------

struct GazeData
{
    cv::Point3f direction0 = cv::Point3f(0, 0, -1);
    cv::Point3f direction1 = cv::Point3f(0, 0, -1);
    cv::Vec2d angle = cv::Vec2d(0, 0);
};

// Extract gaze direction & angle
class GazeExtractor : public FaceDetailExtractor
{
public:
    GazeExtractor(CameraCalibration ccal, bool video = true, int maxNoFaces = 2, float threshold = 0.5);
    GazeExtractor(std::vector<std::string>& args, CameraCalibration ccal, bool video = true, int maxNoFaces = 2, float threshold = 0.5);

    void saveResults() override;
    void resetResults() override;

    std::vector<GazeData> gazeData;
    std::vector<std::vector<cv::Point2f>> eyeLandmarks2D;
    std::vector<std::vector<cv::Point3f>> eyeLandmarks3D;

private:
    CameraCalibration ccal;
};