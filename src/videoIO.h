#pragma once

#include "../config.h"
#include <opencv2/opencv.hpp>
#include <SequenceCapture.h>
#include <ImageCapture.h>
#include <iostream>
#include <string>
#include <optional>
#include <memory>


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

struct CameraCalibration
{
    float fx;
    float fy;
    float cx;
    float cy;
};


// -------------
// ImageIO class
// -------------

class ImageIO : public Utilities::ImageCapture
{
public:
    ImageIO(std::string imageFilepath);

    // Image read
    Frame loadImage();

    // Getters
    CameraCalibration getCalibration() const;

private:
    Frame image;
};


// -------------
// VideoIO class
// -------------

class VideoIO : public Utilities::SequenceCapture
{
public:
    VideoIO(std::string inputFilepath, std::string outputFilepath, VideoMode mode = VideoMode::XVID);

    // Frame read & write
    std::optional<Frame> loadNextFrame();
    void saveNextFrame(const Frame& frame);

    // Getters
    CameraCalibration getCalibration() const;
    double getCurrentTimestamp() const;

private:
    // Video write
    std::unique_ptr<cv::VideoWriter> output;
};