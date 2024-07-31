#pragma once

#include "../config.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <optional>


// --------------
// Helper defines
// --------------

using Frame = cv::Mat;

enum class VideoMode {
    XVID = 1,
    MJPG,
    MPEG_4,
    H_264,

    NONE = 0
};


// -------------
// VideoIO class
// -------------

class VideoIO
{
public:
    VideoIO(std::string inputFilepath, std::string outputFilepath, VideoMode mode = VideoMode::XVID);
    ~VideoIO();

    // Frame read & write
    std::optional<Frame> loadNextFrame();
    void saveNextFrame(const Frame& frame);

    // Getters
    int getFrameWidth() const;
    int getFrameHeight() const;
    int getFps() const;

private:
    // Video read
    cv::VideoCapture input;

    // Video write
    cv::VideoWriter output;
};