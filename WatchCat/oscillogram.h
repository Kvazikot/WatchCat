#ifndef OSCILLOGRAM_H
#define OSCILLOGRAM_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "soundrecorder.h"

enum WINDOW_CORNER
{
    LEFT_TOP,
    RIGHT_TOP,
    LEFT_BOTTOM,
    RIGHT_BOTTOM
};

class Oscillogram
{
    cv::Rect window_rect;
    short*  samples;
    int     samples_size;
    int     rate;
    float   max_amplitude;
    cv::Scalar color;

public:
    Oscillogram();
    void AllocMemoryForSignal(HWParams& params, int time);
    void computeWindowRect(cv::Mat& img, WINDOW_CORNER corner, float percent_of_side);
    void Render(cv::Mat& img);
    void SetSamples(short* buffer, int buffer_size);
};

#endif // OSCILLOGRAM_H
