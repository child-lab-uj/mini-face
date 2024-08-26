#pragma once

#include "videoIO.h"
#include <iostream>
#include <memory>
#include <vector>

#include <LandmarkCoreIncludes.h>
#include <FaceAnalyser.h>


// -------------------
// Extractor interface
// -------------------

class Extractor
{
public:
    virtual ~Extractor() {}

    // Returns true if any face has been succesfully detected
    virtual bool process(const Frame& frame, double timestamp) = 0;

    virtual void printResults(std::ostream& os) const {}
};


// -------------------------
// Extractor - face detector
// -------------------------

using BoundingBox = cv::Rect_<float>;

class FaceExtractor : public Extractor
{
public:
    FaceExtractor(int maxNoFaces = 2);

    bool process(const Frame& frame, double timestamp = -1) override;

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
    FaceDetailExtractor(bool video, int maxNoFaces);
    FaceDetailExtractor(std::vector<std::string>& args, bool video, int maxNoFaces);

    virtual bool process(const Frame& frame, double timestamp = -1) override;

    // Abstract method for extracting data from CLNF face model
    virtual void saveResults() = 0;
    // Abstract method for reseting data containers or variables
    virtual void resetResults() = 0;
    // Abstract method for video processing (like in AUExtractor)
    virtual void processFrame(const Frame& frame, double timestamp, bool success) {}

    int noDetections = 0;

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
    LandmarkExtractor(bool video = true, int maxNoFaces = 2);
    LandmarkExtractor(std::vector<std::string>& args, bool video = true, int maxNoFaces = 2);

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
    PoseExtractor(CameraCalibration ccal, bool video = true, int maxNoFaces = 2);
    PoseExtractor(std::vector<std::string>& args, CameraCalibration ccal, bool video = true, int maxNoFaces = 2);

    void saveResults() override;
    void resetResults() override;

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
    GazeExtractor(CameraCalibration ccal, bool video = true, int maxNoFaces = 2);
    GazeExtractor(std::vector<std::string>& args, CameraCalibration ccal, bool video = true, int maxNoFaces = 2);

    void saveResults() override;
    void resetResults() override;

    void printResults(std::ostream& os) const override;

    // Extracting eye center
    enum Eye {LEFT_EYE = 0, RIGHT_EYE};
    cv::Point3f eyeCenter(int person, Eye eye) const;

    std::vector<GazeData> gazeData;
    std::vector<std::vector<cv::Point2f>> eyeLandmarks2D;
    std::vector<std::vector<cv::Point3f>> eyeLandmarks3D;

private:
    CameraCalibration ccal;
};


// ------------------------
// Extractor - action units
// ------------------------

// Helper defines
// --------------
struct AUExtractorParameters : public FaceAnalysis::FaceAnalyserParameters
{
    AUExtractorParameters(bool videoF = true);
    AUExtractorParameters(std::vector<std::string>& args, bool videoF = true);

    void optimize(bool videoF);
};

using AUList = std::vector<std::vector<std::pair<std::string, double>>>;

// Main class
// ----------
class AUExtractor : public FaceDetailExtractor
{
public:
    // videoL stands for using LandmarkDetector in video mode (the one from FaceDetailExtractor)
    // videoF stands for using FaceAnalyser in video mode (the new one)
    AUExtractor(bool videoF = true, bool videoL = true, int maxNoFaces = 2);
    AUExtractor(std::vector<std::string>& args, bool videoF = true, bool videoL = true, int maxNoFaces = 2);

    void saveResults() override;
    void resetResults() override;
    void processFrame(const Frame& frame, double timestamp, bool success) override;

    void printResults(std::ostream& os) const override;

    AUList detectedAU;

private:
    AUExtractorParameters faceAnalasisParams;
    FaceAnalysis::FaceAnalyser analyser;
    bool videoF;
};