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

bool FaceExtractor::process(const Frame& frame)
{
    // std::cout << "[FaceExtractor]: processing frame...\n";
    detectedFaces.clear();
    confidences.clear();
    
    return LandmarkDetector::DetectFacesMTCNN(detectedFaces, frame, detector, confidences);
}

std::vector<BoundingBox> FaceExtractor::selectFaces()
{
    // Create merged vector to consider confidences
    std::vector<std::pair<BoundingBox, float>> merged;
    std::transform(detectedFaces.begin(), detectedFaces.end(), confidences.begin(), std::back_inserter(merged), 
                   [](BoundingBox bb, float conf){return std::make_pair(bb, conf);});

    // Filter part
    // It is guaranteed that both vectors are matched
    merged.erase(std::remove_if(merged.begin(), merged.end(), [this](const auto& p){return p.second < this->threshold;}),
                 merged.end());

    // Sorting part
    std::sort(merged.begin(), merged.end(), [](const auto& a, const auto& b){return a.second > b.second;});     // Descending order, by confidence

    // Final selection of max(maxNoElements, detectedFaces.size()) elements
    auto endPtr = merged.size() <= maxNoFaces ? merged.end() : merged.begin() + maxNoFaces;

    // Transform back to regular BoundingBox vector
    std::vector<BoundingBox> results;
    std::transform(merged.begin(), merged.end(), std::back_inserter(results), [](const auto& p){return p.first;});

    return results;

}


// -------------------------
// LandmarkExtractor methods
// -------------------------

