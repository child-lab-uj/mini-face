#include "videoIO.h"
#include <exception>


// ----------------
// Helper functions
// ----------------

namespace {

    constexpr int codec_type(VideoMode mode)
    {
        return mode == VideoMode::XVID ? cv::VideoWriter::fourcc('X', 'V', 'I', 'D') :
               mode == VideoMode::MJPG ? cv::VideoWriter::fourcc('M', 'J', 'P', 'G') :
               mode == VideoMode::MPEG_4 ? cv::VideoWriter::fourcc('M', 'P', '4', 'V') :
               mode == VideoMode::H_264 ? cv::VideoWriter::fourcc('A', 'V', 'C', '1') : 0;
    }

}


// ---------------
// ImageIO methods
// ---------------

ImageIO::ImageIO(std::string imageFilepath)
{
    if (!OpenImageFiles({imageFilepath}))
        throw std::runtime_error("Unable to load image from: " + imageFilepath);
    
    image = GetNextImage();
}

Frame ImageIO::loadImage()
{
    return image;
}

CameraCalibration ImageIO::getCalibration() const
{
    return { this->fx, this->fy, this->cx, this->cy };
}


// ---------------
// VideoIO methods
// ---------------

VideoIO::VideoIO(std::string inputFilepath, std::string outputFilepath, VideoMode mode)
    : Utilities::SequenceCapture()
{
    // Read setup
    if (!OpenVideoFile(inputFilepath))
        throw std::runtime_error("Unable to load video from: " + inputFilepath);

    // Write setup
    output = std::make_unique<cv::VideoWriter>(outputFilepath, codec_type(mode), fps, cv::Size(frame_width, frame_height));
    if (!output->isOpened())
        throw std::runtime_error("Unable to save video to: " + outputFilepath);
}

std::optional<Frame> VideoIO::loadNextFrame()
{
    Frame frame = GetNextFrame();
    return frame.empty() ? std::nullopt : std::make_optional(frame);
}

void VideoIO::saveNextFrame(const Frame& frame)
{
    output->write(frame);
}

CameraCalibration VideoIO::getCalibration() const
{
    return { this->fx, this->fy, this->cx, this->cy };
}