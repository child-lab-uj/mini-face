#pragma once

#include "landmarks.h"
#include <FaceAnalyser.h>
#include <vector>


// -----------------
// AUExtractor class
// -----------------

class AUExtractor : public LandmarkExtractor
{
public:
    /* 
        Brief description of parameters:
        - landmarkVideoMode: decides to use DetectLandmarksInVideo() or DetectLandmarksInImage() function.
                             I noticed from a practice that videoMode = false gives better results (combined with multi_view).
        - auVideoMode: decides to use AddNextFrame() or PredictStaticAUsAndComputeFeatures() function.
                       AddNextFrame() is more suitable for videos, as it provides some sort of memory.
        - wild: a specyfic set of parameters for tough conditions (various lighting, incomplete view of the face)
        - multi_view: decides whether to consider multiple views during model reinit.
                      It significantly improves the results when combined with DetectLandmarksInImage()
        - limit_pose: should pose be limited to 180 degrees frontal.
        - n_iter: number of optimization iterations.
        - reg_factor: regularization parameter.
        - weight_factor: refers to how much weight is applied to certain constraints during the optimization process.
    */
    AUExtractor(std::string model_loc, bool landmarkVideoMode, bool auVideoMode,
                std::optional<bool> wild, std::optional<bool> multi_view, std::optional<bool> limit_pose,
                std::optional<int> n_iter, std::optional<float> reg_factor, std::optional<float> weight_factor);

    // Main API methods
    std::vector<std::pair<std::string, double>> detectActionUnitPresence(const Frame& frame, double timestamp, const BoundingBox& roi);
    std::vector<std::pair<std::string, double>> detectActionUnitIntensity(const Frame& frame, double timestamp, const BoundingBox& roi);

private:
    void loadFrame(const Frame& frame, double timestamp, const BoundingBox& face);

    // Face analyser parameters
    bool faceAnalyserVideoMode;
    std::vector<std::string> faceAnalyserArgList;
    std::unique_ptr<FaceAnalysis::FaceAnalyserParameters> faceAnalyserParams;

    // Face analyser model
    std::unique_ptr<FaceAnalysis::FaceAnalyser> faceAnalyserModel;
};