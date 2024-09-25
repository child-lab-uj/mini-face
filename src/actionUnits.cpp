#include "actionUnits.h"


// -------------------
// AUExtractor methods
// -------------------

AUExtractor::AUExtractor(bool landmarkVideoMode, bool auVideoMode,
                         std::optional<bool> wild, std::optional<bool> multi_view, std::optional<bool> limit_pose,
                         std::optional<int> n_iter, std::optional<float> reg_factor, std::optional<float> weight_factor)
    : LandmarkExtractor(landmarkVideoMode, wild, multi_view, limit_pose, n_iter, reg_factor, weight_factor),
      faceAnalyserVideoMode(auVideoMode)
{
    // Add root directory arg
    faceAnalyserArgList.push_back(MAIN_FILEPATH);

    // Initialize dynamic memory
    faceAnalyserParams = std::make_unique<FaceAnalysis::FaceAnalyserParameters>(faceAnalyserArgList);
    faceAnalyserModel = std::make_unique<FaceAnalysis::FaceAnalyser>(*faceAnalyserParams);
}

std::vector<std::pair<std::string, double>> AUExtractor::detectActionUnits(const Frame& frame, double timestamp, const BoundingBox& face)
{
    Frame empty;

    bool result = detectFaceLandmarks(frame, timestamp, face);
    
    // Different behavior for images and videos
    if (faceAnalyserVideoMode)
        faceAnalyserModel->AddNextFrame(frame, model->detected_landmarks, result, timestamp);
    else
       faceAnalyserModel->PredictStaticAUsAndComputeFeatures(frame, model->detected_landmarks);
    
    // Returns classes, can also return regression confidence levels
    return faceAnalyserModel->GetCurrentAUsClass();
}