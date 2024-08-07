#include "src/videoIO.h"
#include "src/extractor.h"
#include "src/visualization.h"
#include "config.h"
#include <iostream>

// Useful hints
// Combining DetectLandmarksInImage() with multi_view=true works much better than DetectLandmarksInVideo()


// ----------
// Image mode
// ----------

void test_image()
{
    ImageIO imageIO(TEST_IMAGE_FILE);

    Frame image = imageIO.loadImage();
    std::cout << "Image " << TEST_IMAGE_FILE << " succesfully loaded\n";

    // OpenFace model parameters
    std::vector<std::string> FaceModelArgs = {
        CONFIG_FILEPATH,     // To extract root directory
        "-multi_view", USE_MULTI_VIEW ? "1" : "0"
    };

    //FaceExtractor extractor(2, 0.75f);
    //LandmarkExtractor extractor(FaceModelArgs, false, 2, 0.75f);
    //PoseExtractor extractor(FaceModelArgs, imageIO.getCalibration(), false, 2, 0.75f);
    GazeExtractor extractor(FaceModelArgs, imageIO.getCalibration(), false, 2, 0.75f);
    //FaceVisualizer visualizer;
    //LandmarkVisualizer visualizer(imageIO.getCalibration());
    //PoseVisualizer visualizer(imageIO.getCalibration());
    GazeVisualizer visualizer(imageIO.getCalibration());

    extractor.process(image);
    Frame result = visualizer.process(image, &extractor);

    cv::imshow("Face detection", result);
    cv::waitKey(0);
}


// ----------
// Video mode
// ----------

void test_video()
{
    VideoIO video(INPUT_FILE, OUTPUT_FILE);

    // OpenFace model parameters
    std::vector<std::string> FaceModelArgs = {
        CONFIG_FILEPATH,     // To extract root directory
        "-multi_view", USE_MULTI_VIEW ? "1" : "0"
    };

    //FaceExtractor extractor(2, 0.75f);
    //LandmarkExtractor extractor(FaceModelArgs, false, 2, 0.75f);    // Apparently image mode with multi_view works much better
    //PoseExtractor extractor(FaceModelArgs, video.getCalibration(), false, 2, 0.75f);
    GazeExtractor extractor(FaceModelArgs, video.getCalibration(), false, 2, 0.75f);
    //FaceVisualizer visualizer;
    //LandmarkVisualizer visualizer(video.getCalibration());
    //PoseVisualizer visualizer(video.getCalibration());
    GazeVisualizer visualizer(video.getCalibration());

    std::cout << "Reading " << INPUT_FILE << " video...\n";
    for (int i = 0; i < MAX_NO_FRAMES; i++) {
        std::optional<Frame> frameOpt = video.loadNextFrame();
        if (!frameOpt.has_value())
            break;

        Frame frame = frameOpt.value();

        if (extractor.process(frame))
        {
            Frame result = visualizer.process(frame, &extractor);
            video.saveNextFrame(result);
        }
        else
            video.saveNextFrame(frame);
    }
}


// -------------
// Main function
// -------------

int main()
{
    // Main mode - video parsing
    if constexpr (RUN_VIDEO)
        test_video();
    // Test mode - image parsing
    else
        test_image();

    return 0;
}