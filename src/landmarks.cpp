#include "landmarks.h"
#include <LandmarkDetectorFunc.h>


LandmarkExtractor::LandmarkExtractor(bool videoMode, 
                                     std::optional<bool> wild, std::optional<bool> multi_view, std::optional<bool> limit_pose,
                                     std::optional<int> n_iter, std::optional<float> reg_factor, std::optional<float> weight_factor)
    : useVideoMode(videoMode)
{
    // Add root directory arg
    argList.push_back(MAIN_FILEPATH);

    // Add wild parameter
    if (wild.has_value())
        argList.push_back("-wild");

    // Add multi view parameter
    if (multi_view.has_value()) {
        argList.push_back("-multi_view");
        argList.push_back(multi_view.value() ? "1" : "0");
    }

    // Add n_iter parameter
    if (n_iter.has_value()) {
        argList.push_back("-n_iter");
        argList.push_back(std::to_string(n_iter.value()));
    }

    // Add reg_factor parameter
    if (reg_factor.has_value()) {
        argList.push_back("-reg");
        argList.push_back(std::to_string(reg_factor.value()));
    }

    // Add weight_factor parameter
    if (weight_factor.has_value()) {
        argList.push_back("-w_reg");
        argList.push_back(std::to_string(weight_factor.value()));
    }

    // Initialize dynamic memory
    params = std::make_unique<LandmarkDetector::FaceModelParameters>(argList);
    model = std::make_unique<LandmarkDetector::CLNF>(params->model_location);

    // Set limit_pose parameter
    if (limit_pose.has_value())
        params->limit_pose = limit_pose.value();
}

bool LandmarkExtractor::detectFaceLandmarks(const Frame& frame, double timestamp, const BoundingBox& roi)
{
    Frame empty;

    return useVideoMode ? LandmarkDetector::DetectLandmarksInVideo(frame, roi, *model, *params, empty) :
                          LandmarkDetector::DetectLandmarksInImage(frame, roi, *model, *params, empty);
}