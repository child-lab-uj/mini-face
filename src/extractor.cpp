#include "extractor.h"
#include <algorithm>
#include <iostream>


// ---------------------
// FaceExtractor methods
// ---------------------

// Might be necessary to give model path to MTCNN detector
FaceExtractor::FaceExtractor(int maxNoFaces, float threshold)
    : detector(MTCNN_MODEL_FILE), maxNoFaces(maxNoFaces), threshold(threshold)
{
}

void FaceExtractor::process(const Frame& frame)
{
    std::cout << "[FaceExtractor]: processing frame...\n";
    bool success = LandmarkDetector::DetectFacesMTCNN(detectedFaces, frame, detector, confidences);

    if (!success)
        std::cerr << "[FaceDetector]: Unable to perform face detection\n";
}

std::vector<BoundingBox> FaceExtractor::selectFaces()
{
    // Filter part
    // It is guaranteed that both vectors are matched
    detectedFaces.erase(std::remove_if(detectedFaces.begin(), detectedFaces.end(),
                                       // Predicate
                                       [this](const BoundingBox& box){
                                           std::size_t idx = &box - &detectedFaces[0];
                                           return this->confidences[idx] < this->threshold;
                                       }), 
                        detectedFaces.end()
    );
    confidences.erase(std::remove_if(confidences.begin(), confidences.end(), [this](float conf){return conf < this->threshold;}), confidences.end());

    // Sorting part
    std::sort(detectedFaces.begin(), detectedFaces.end(), [this](const BoundingBox& a, const BoundingBox& b) {
        std::size_t idx_a = &a - &detectedFaces[0];
        std::size_t idx_b = &b - &detectedFaces[0];
        return this->confidences[idx_a] > this->confidences[idx_b];     // Descending order
    });

    // Final selection of max(maxNoElements, detectedFaces.size()) elements
    auto endPtr = detectedFaces.size() <= maxNoFaces ? detectedFaces.end() : detectedFaces.begin() + maxNoFaces;

    return std::vector<BoundingBox>(detectedFaces.begin(), endPtr);

}


// -------------------------
// LandmarkExtractor methods
// -------------------------

