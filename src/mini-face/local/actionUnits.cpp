#include "actionUnits.h"


// -------------------
// AUExtractor methods
// -------------------

AUExtractor::AUExtractor(std::string model_loc, bool landmarkVideoMode, bool auVideoMode,
                         std::optional<bool> wild, std::optional<bool> multi_view, std::optional<bool> limit_pose,
                         std::optional<int> n_iter, std::optional<float> reg_factor, std::optional<float> weight_factor)
    : LandmarkExtractor(model_loc, landmarkVideoMode, wild, multi_view, limit_pose, n_iter, reg_factor, weight_factor),
      faceAnalyserVideoMode(auVideoMode)
{
    // OpenFace will extract parent directory of model_loc and mark it as root
    faceAnalyserArgList.push_back(model_loc);

    // Initialize dynamic memory
    faceAnalyserParams = std::make_unique<FaceAnalysis::FaceAnalyserParameters>(faceAnalyserArgList);
    faceAnalyserModel = std::make_unique<FaceAnalysis::FaceAnalyser>(*faceAnalyserParams);
}

std::vector<std::pair<std::string, double>> AUExtractor::detectActionUnitPresence(const Frame& frame, double timestamp, const BoundingBox& face)
{
    loadFrame(frame, timestamp, face);
    
    // Returns classes
    return faceAnalyserModel->GetCurrentAUsClass();
}

std::vector<std::pair<std::string, double>> AUExtractor::detectActionUnitIntensity(const Frame& frame, double timestamp, const BoundingBox& face)
{
    loadFrame(frame, timestamp, face);
    
    // Returns classes
    return faceAnalyserModel->GetCurrentAUsReg();
}

void AUExtractor::loadFrame(const Frame& frame, double timestamp, const BoundingBox& face)
{
    Frame empty;

    bool result = detectFaceLandmarks(frame, timestamp, face);
    
    // Different behavior for images and videos
    if (faceAnalyserVideoMode)
        faceAnalyserModel->AddNextFrame(frame, model->detected_landmarks, result, timestamp);
    else
       faceAnalyserModel->PredictStaticAUsAndComputeFeatures(frame, model->detected_landmarks);
}