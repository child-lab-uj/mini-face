#include "visualizer.h"


// ----------------------
// faceVisualizer methods
// ----------------------

Frame FaceVisualizer::process(const Frame& frame, Extractor* extractor)
{
    FaceExtractor* faceExtractor = dynamic_cast<FaceExtractor*>(extractor);

    Frame result = frame.clone();
    std::vector<BoundingBox> faces = faceExtractor->selectFaces();

    for (const BoundingBox& face : faces) {
        cv::Rect rect(static_cast<int>(face.x), static_cast<int>(face.y),
                      static_cast<int>(face.width), static_cast<int>(face.height));

        // Draw bounding box
        cv::rectangle(result, rect, cv::Scalar(0, 255, 0), 2);   // Green, with thickness 2
    }

    return result;
}