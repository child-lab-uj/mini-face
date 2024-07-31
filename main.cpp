#include "src/videoIO.h"
#include "src/extractor.h"
#include "src/visualizer.h"
#include "config.h"
#include <iostream>


int main()
{   
    // Main mode - video parsing
    if constexpr (RUN_VIDEO) {
        VideoIO video(INPUT_FILE, OUTPUT_FILE);

        FaceExtractor extractor;

        std::cout << "Reading " << INPUT_FILE << " video...\n";
        for (int i = 0; i < MAX_NO_FRAMES; i++)
        {
            std::optional<Frame> frameOpt = video.loadNextFrame();
            if (!frameOpt.has_value())
                break;

            Frame frame = frameOpt.value();

            extractor.process(frame);
            auto faces = extractor.selectFaces();
            if (!faces.empty())
                std::cout << "Kurwa sukces XD\n";
        }
    }
    // Test mode - image parsing
    else {
        Frame image = cv::imread(TEST_IMAGE_FILE);
        if (image.empty()) {
            std::cerr << "Unable to load image: " << TEST_IMAGE_FILE << "\n";
            return -1;
        }
        else
            std::cout << "Image " << TEST_IMAGE_FILE << " succesfully loaded\n";

        FaceExtractor extractor;
        FaceVisualizer visualizer;

        extractor.process(image);
        std::cout << "Succesfully performed face detection\n";
        auto faces = extractor.selectFaces();
        if (!faces.empty())
            std::cout << "Kurwa sukces XD\n";
        else
            std::cout << "No face detected :(\n";

        Frame result = visualizer.process(image, &extractor);

        cv::imshow("Face detection", result);
        cv::waitKey(0);
    }

    return 0;
}