#pragma once

#include "helpers.h"
#include <LandmarkDetectorModel.h>
#include <memory>
#include <optional>


// ---------------------------
// LandmarkExtractor interface
// ---------------------------

class LandmarkExtractor
{
public:
    LandmarkExtractor(bool videoMode, 
                      std::optional<bool> wild, std::optional<bool> multi_view, std::optional<bool> limit_pose,
                      std::optional<int> n_iter, std::optional<float> reg_factor, std::optional<float> weight_factor);
    virtual ~LandmarkExtractor() = default;

    bool detectFaceLandmarks(const Frame& frame, double timestamp, const BoundingBox& roi);

protected:
    // Face landmark detection parameters
    bool useVideoMode;
    std::vector<std::string> argList;
    std::unique_ptr<LandmarkDetector::FaceModelParameters> params;

    // Face landmark detection model
    std::unique_ptr<LandmarkDetector::CLNF> model;
};