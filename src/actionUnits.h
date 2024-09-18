#pragma once

#include "helpers.h"
#include <LandmarkDetectorModel.h>
#include <FaceAnalyser.h>
#include <vector>


// --------------
// Helper defines
// --------------

struct FaceAnalyserParameters : public FaceAnalysis::FaceAnalyserParameters
{
    FaceAnalyserParameters(std::vector<std::string>& args, bool videoF = true)
        : FaceAnalysis::FaceAnalyserParameters(args) { if (videoF) OptimizeForVideos(); else OptimizeForImages(); }
};


// -----------------
// AUExtractor class
// -----------------

class AUExtractor
{
public:
    AUExtractor(bool video);

    // Main API method
    std::vector<std::pair<std::string, double>> detectActionUnits(const Frame& frame, double timestamp, const BoundingBox& roi);

private:
    // Face landmark detection model
    LandmarkDetector::FaceModelParameters paramsLD;
    LandmarkDetector::CLNF modelLD;

    // Face analyser model
    FaceAnalyserParameters paramsFA;
    FaceAnalysis::FaceAnalyser modelFA;
    bool video;
};