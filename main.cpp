#include "src/gaze.h"
#include <opencv2/opencv.hpp>

// Note: this is just a test code. To use it, you need to tell CMake to generate .exe file in CMakeLists.txt
// The project itself does not generate any executable if not set in CMakeLists, it's just a .pyd or .so python library

int main()
{
    Frame image = cv::imread(ROOT_DIRECTORY + "/test_image.jpg");
    BoundingBox roi(214.467, 96.9926, 110.877, 117.08);

    GazeExtractor extractor;
    extractor.estimateCameraCalibration(image);
    auto result = extractor.detectGaze(image, 0, roi);

    if (result.has_value())
        std::cout << result.value().eye1 << std::endl;

    return 0;
}