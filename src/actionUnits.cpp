#include "config.h"
#include "actionUnits.h"
#include <LandmarkDetectorFunc.h>


namespace {

    std::vector<std::string> FaceModelArgs = {
        CONFIG_FILEPATH, // To extract root directory
        "-multi_view", USE_MULTI_VIEW ? "1" : "0"
    };

    std::vector<std::string> FaceAnalyserArgs = {
        CONFIG_FILEPATH, // To extract root directory
    };

}


// -------------------
// AUExtractor methods
// -------------------

AUExtractor::AUExtractor(bool video)
    : paramsLD(FaceModelArgs), modelLD(paramsLD.model_location),
      paramsFA(FaceAnalyserArgs, video), modelFA(paramsFA), video(video)
{
}

std::vector<std::pair<std::string, double>> AUExtractor::detectActionUnits(const Frame& frame, double timestamp, const BoundingBox& face)
{
    Frame empty;

    bool result = USE_IMAGE_MODE ? LandmarkDetector::DetectLandmarksInImage(frame, face, modelLD, paramsLD, empty) :
                                   LandmarkDetector::DetectLandmarksInVideo(frame, face, modelLD, paramsLD, empty);
    
    // Different behavior for images and videos
    if (video)
        modelFA.AddNextFrame(frame, modelLD.detected_landmarks, result, timestamp);
    else
       modelFA.PredictStaticAUsAndComputeFeatures(frame, modelLD.detected_landmarks);
    
    // Returns classes, can also return regression confidence levels
    return modelFA.GetCurrentAUsClass();
}