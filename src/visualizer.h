#pragma once

#include "extractor.h"


// --------------------
// Visualizer interface
// --------------------

class Visualizer
{
public:
    virtual Frame process(const Frame& frame, Extractor* extractor) = 0;
};


// --------------------------------------------
// Visualizer - face bounding box visualization
// --------------------------------------------

class FaceVisualizer : public Visualizer
{
public:
    FaceVisualizer() = default;

    Frame process(const Frame& frame, Extractor* extractor) override;
};