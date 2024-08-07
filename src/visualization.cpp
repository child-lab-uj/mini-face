#include "visualization.h"


// ----------------------
// FaceVisualizer methods
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


// ------------------------------
// FaceLandmarkVisualizer methods
// ------------------------------

// We need to specify what do we want to visualize (by default, we use first parameter - vis_align)
LandmarkVisualizer::LandmarkVisualizer(const CameraCalibration& ccal)
    : Utilities::Visualizer(true, false, false, false), ccal(ccal)
{
}

Frame LandmarkVisualizer::process(const Frame& frame, Extractor* extractor)
{
    LandmarkExtractor* landmarkExtractor = dynamic_cast<LandmarkExtractor*>(extractor);

    SetImage(frame, ccal.fx, ccal.fy, ccal.cx, ccal.cy);
    for (int i = 0; i < landmarkExtractor->detectedLandmarks2D.size(); i++) {
        // TODO: how to extract real confidence?
        SetObservationLandmarks(landmarkExtractor->detectedLandmarks2D[i], 1.f, landmarkExtractor->detectedLandmarkVisibilities[i]);
    }

    return GetVisImage();
}


// ----------------------
// PoseVisualizer methods
// ----------------------

PoseVisualizer::PoseVisualizer(const CameraCalibration& ccal)
    : Utilities::Visualizer(true, false, false, false), ccal(ccal)
{
}

Frame PoseVisualizer::process(const Frame& frame, Extractor* extractor)
{
    PoseExtractor* poseExtractor = dynamic_cast<PoseExtractor*>(extractor);

    SetImage(frame, ccal.fx, ccal.fy, ccal.cx, ccal.cy);
    for (const cv::Vec6d& pose : poseExtractor->headPoses) {
        SetObservationPose(pose, 1.f);
    }

    return GetVisImage();
}


// ----------------------
// GazeVisualizer methods
// ----------------------

GazeVisualizer::GazeVisualizer(const CameraCalibration& ccal)
    : Utilities::Visualizer(true, false, false, false), ccal(ccal)
{
}

Frame GazeVisualizer::process(const Frame& frame, Extractor* extractor)
{
    GazeExtractor* gazeExtractor = dynamic_cast<GazeExtractor*>(extractor);

    SetImage(frame, ccal.fx, ccal.fy, ccal.cx, ccal.cy);
    for (int i = 0; i < gazeExtractor->gazeData.size(); i++) {
        const GazeData& gaze = gazeExtractor->gazeData[i];
        SetObservationGaze(gaze.direction0, gaze.direction1, 
                           gazeExtractor->eyeLandmarks2D[i], gazeExtractor->eyeLandmarks3D[i], 1.f);
    }

    return GetVisImage();
}