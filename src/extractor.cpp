#include "extractor.h"
#include <GazeEstimation.h>
#include <algorithm>


// ---------------------
// FaceExtractor methods
// ---------------------

FaceExtractor::FaceExtractor(int maxNoFaces)
    : detector(MTCNN_MODEL_FILE), maxNoFaces(maxNoFaces), threshold(FACE_DETECTION_THRESHOLD)
{
}

bool FaceExtractor::process(const Frame& frame, double timestamp)
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
    merged.reserve(detectedFaces.size());
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
    std::transform(merged.begin(), endPtr, std::back_inserter(results), [](const auto& p){return p.first;});

    return results;

}


// -------------------------------------
// FaceDetailExtractor interface methods
// -------------------------------------

FaceDetailExtractor::FaceDetailExtractor(bool video, int maxNoFaces)
    : FaceExtractor(maxNoFaces),
      faceModel(params.model_location),
      video(video)
{
}

FaceDetailExtractor::FaceDetailExtractor(std::vector<std::string>& args, bool video, int maxNoFaces)
    : FaceExtractor(maxNoFaces),
      params(LandmarkDetector::FaceModelParameters(args)),
      faceModel(params.model_location),
      video(video)
{
}

bool FaceDetailExtractor::process(const Frame& frame, double timestamp)
{
    noDetections = 0;

    // Extract faces
    if (!FaceExtractor::process(frame))
        return false;

    auto faces = FaceExtractor::selectFaces();

    // Perform landmark detection for each of detected faces
    resetResults();
    bool anySuccess = false;
    for (const BoundingBox& bbox : faces) {
        Frame emptyFrame;
        bool result = video ? LandmarkDetector::DetectLandmarksInVideo(frame, bbox, faceModel, params, emptyFrame) :
                              LandmarkDetector::DetectLandmarksInImage(frame, bbox, faceModel, params, emptyFrame);
        anySuccess = anySuccess || result;

        processFrame(frame, timestamp, result);

        // Store results
        if (result) {
            noDetections++;
            saveResults();
        }
    }
    
    return anySuccess;
}


// -------------------------
// LandmarkExtractor methods
// -------------------------

LandmarkExtractor::LandmarkExtractor(bool video, int maxNoFaces)
    : FaceDetailExtractor(video, maxNoFaces)
{
}

LandmarkExtractor::LandmarkExtractor(std::vector<std::string>& args, bool video, int maxNoFaces)
    : FaceDetailExtractor(args, video, maxNoFaces)
{
}

void LandmarkExtractor::saveResults()
{
    // Vert important - it's necessary to clone the data
    // OpenCV performs some weird optimization by performing a shallow copy by default
    detectedLandmarks2D.push_back(faceModel.detected_landmarks.clone());
    // Additional info, might be useful for better visualization
    detectedLandmarkVisibilities.push_back(faceModel.GetVisibilities());
}

void LandmarkExtractor::resetResults()
{
    detectedLandmarks2D.clear();
    detectedLandmarkVisibilities.clear();
}


// ---------------------
// PoseExtractor methods
// ---------------------

PoseExtractor::PoseExtractor(CameraCalibration ccal, bool video, int maxNoFaces)
    : FaceDetailExtractor(video, maxNoFaces),
      ccal(ccal)
{
}

PoseExtractor::PoseExtractor(std::vector<std::string>& args, CameraCalibration ccal, bool video, int maxNoFaces)
    : FaceDetailExtractor(args, video, maxNoFaces),
      ccal(ccal)
{
}

void PoseExtractor::saveResults()
{
    headPoses.push_back(LandmarkDetector::GetPose(faceModel, ccal.fx, ccal.fy, ccal.cx, ccal.cy));
}

void PoseExtractor::resetResults()
{
    headPoses.clear();
}


// ---------------------
// GazeExtractor methods
// ---------------------

GazeExtractor::GazeExtractor(CameraCalibration ccal, bool video, int maxNoFaces)
    : FaceDetailExtractor(video, maxNoFaces),
      ccal(ccal)
{
}

GazeExtractor::GazeExtractor(std::vector<std::string>& args, CameraCalibration ccal, bool video, int maxNoFaces)
    : FaceDetailExtractor(args, video, maxNoFaces),
      ccal(ccal)
{
}

void GazeExtractor::saveResults()
{
    // Gaze estimation
    GazeData data;

    GazeAnalysis::EstimateGaze(faceModel, data.direction0, ccal.fx, ccal.fy, ccal.cx, ccal.cy, true);
    GazeAnalysis::EstimateGaze(faceModel, data.direction1, ccal.fx, ccal.fy, ccal.cx, ccal.cy, false);
    data.angle = GazeAnalysis::GetGazeAngle(data.direction0, data.direction1);

    gazeData.push_back(data);

    // Eye landmarks extraction
    eyeLandmarks2D.push_back(LandmarkDetector::CalculateAllEyeLandmarks(faceModel));
    eyeLandmarks3D.push_back(LandmarkDetector::Calculate3DEyeLandmarks(faceModel, ccal.fx, ccal.fy, ccal.cx, ccal.cy));
}

void GazeExtractor::resetResults()
{
    gazeData.clear();
    eyeLandmarks2D.clear();
    eyeLandmarks3D.clear();
}

cv::Point3f GazeExtractor::eyeCenter(int person, Eye eye) const
{
    cv::Point3f middle(0, 0, 0);

    if (person >= noDetections)
        return middle;
    const auto& landmarks3D = eyeLandmarks3D[person];
    
    // Calculate eye center based on detected landmarks
    for (int i = 0; i < 8; i++) 
        middle = middle + landmarks3D[int(eye) * landmarks3D.size() / 2 + i];
    middle = middle / 8;

    return middle;
}

void GazeExtractor::printResults(std::ostream& os) const
{
    for (int person = 0; person < noDetections; person++) {
        os << " [" 
           << eyeCenter(person, GazeExtractor::LEFT_EYE) << " " << gazeData[person].direction0 << " "
           << eyeCenter(person, GazeExtractor::RIGHT_EYE) << " " << gazeData[person].direction1 << " "
           << gazeData[person].angle
           << "]";
    }
}


// -------------------
// AUExtractor methods
// -------------------

AUExtractorParameters::AUExtractorParameters(bool videoF)
{
    optimize(videoF);
}

AUExtractorParameters::AUExtractorParameters(std::vector<std::string>& args, bool videoF)
    : FaceAnalysis::FaceAnalyserParameters(args)
{
   optimize(videoF);
}

void AUExtractorParameters::optimize(bool videoF)
{
    if (videoF)
        OptimizeForVideos();
    else
        OptimizeForImages();
}

AUExtractor::AUExtractor(bool videoF, bool videoL, int maxNoFaces)
    : FaceDetailExtractor(videoL, maxNoFaces), faceAnalasisParams(videoF), analyser(faceAnalasisParams), videoF(videoF)
{
}

AUExtractor::AUExtractor(std::vector<std::string>& args, bool videoF, bool videoL, int maxNoFaces)
    : FaceDetailExtractor(args, videoL, maxNoFaces), faceAnalasisParams(args, videoF), analyser(faceAnalasisParams), videoF(videoF)
{
}

void AUExtractor::saveResults()
{
    detectedAU.push_back(analyser.GetCurrentAUsClass());
}

void AUExtractor::resetResults()
{
    detectedAU.clear();
}

void AUExtractor::processFrame(const Frame& frame, double timestamp, bool success)
{
    if (videoF)
        analyser.AddNextFrame(frame, faceModel.detected_landmarks, success, timestamp);
    else
        analyser.PredictStaticAUsAndComputeFeatures(frame, faceModel.detected_landmarks);
}

void AUExtractor::printResults(std::ostream& os) const
{
    for (auto& person : detectedAU) {
        os << " [";
        for (auto& au : person)
            os << au.first << " " << au.second << ", ";
        os << "]";
    }
}