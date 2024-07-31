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


// ------------------------------
// Extractor - landmark detection
// ------------------------------

class LandmarkExtractor : public FaceExtractor
{
public:
    LandmarkExtractor() = default;
    LandmarkExtractor(std::vector<std::string>& args);

    bool process(const Frame& frame) override;

    LandmarkDetector::FaceModelParameters params;
    LandmarkDetector::CLNF faceModel;
};