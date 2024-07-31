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
// VideoIO methods
// ---------------

VideoIO::VideoIO(std::string inputFilepath, std::string outputFilepath, VideoMode mode)
    : input(inputFilepath), output(outputFilepath, codec_type(mode), getFps(), cv::Size(getFrameWidth(), getFrameHeight()))
{
    if (!input.isOpened())
        throw std::runtime_error("Unable to open video: " + inputFilepath);
    if (!output.isOpened())
        throw std::runtime_error("Unable to save video to: " + outputFilepath);
}

VideoIO::~VideoIO()
{
    input.release();
    output.release();
}

std::optional<Frame> VideoIO::loadNextFrame()
{
    cv::Mat frame;

    input >> frame;
    if (frame.empty())
        return std::nullopt;
    
    return std::make_optional(frame);
}

void VideoIO::saveNextFrame(const Frame& frame)
{
    output.write(frame);
}

int VideoIO::getFrameWidth() const
{
    return static_cast<int>(input.get(cv::CAP_PROP_FRAME_WIDTH));
}

int VideoIO::getFrameHeight() const
{
    return static_cast<int>(input.get(cv::CAP_PROP_FRAME_HEIGHT));
}

int VideoIO::getFps() const
{
    return static_cast<int>(input.get(cv::CAP_PROP_FPS));
}