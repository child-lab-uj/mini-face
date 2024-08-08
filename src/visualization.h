#pragma once

#include "extractor.h"
#include <Visualizer.h>


// --------------------
// Visualizer interface
// --------------------

class VisualizerIf
{
public:
    virtual ~VisualizerIf() {}

    virtual Frame process(const Frame& frame, Extractor* extractor) = 0;
};


// --------------------------------------------
// Visualizer - face bounding box visualization
// --------------------------------------------

class FaceVisualizer : public VisualizerIf
{
public:
    FaceVisualizer() = default;

    Frame process(const Frame& frame, Extractor* extractor) override;
};


// -----------------------------------------
// Visualizer - face landmarks visualization
// -----------------------------------------

class LandmarkVisualizer : public VisualizerIf, public Utilities::Visualizer
{
public:
    LandmarkVisualizer(const CameraCalibration& ccal);

    Frame process(const Frame& frame, Extractor* extractor) override;

private:
    CameraCalibration ccal;
};


// ------------------------------------
// Visualizer - head pose visualization
// ------------------------------------

class PoseVisualizer : public VisualizerIf, public Utilities::Visualizer
{
public:
    PoseVisualizer(const CameraCalibration& ccal);

    Frame process(const Frame& frame, Extractor* extractor) override;

private:
    CameraCalibration ccal;
};


// -----------------------------------------
// Visualizer - Gaze direction visualization
// -----------------------------------------

class GazeVisualizer : public VisualizerIf, public Utilities::Visualizer
{
public:
    GazeVisualizer(const CameraCalibration& ccal);

    Frame process(const Frame& frame, Extractor* extractor) override;

private:
    CameraCalibration ccal;
};