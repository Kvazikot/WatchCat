#include <stdlib.h>
#include "oscillogram.h"

Oscillogram::Oscillogram()
{
    max_amplitude = 32768;
    color = cv::Scalar(255,0,0,255);
}

//
void Oscillogram::AllocMemoryForSignal(HWParams& params, int time_sec)
{
    if( params.format == SND_PCM_FORMAT_S16_LE)
    {
        samples_size = 2 * params.rate * time_sec;
        samples = (short*) malloc(samples_size);
        rate = params.rate;
    }

    samples_size = params.buffer_frames; //временно!

}

void Oscillogram::SetSamples(short* buffer, int buffer_size)
{
    //memcpy(samples, buffer, buffer_size);
}

// вычисляет где должно находиться окно осцилограмы в изображении img
void Oscillogram::computeWindowRect(cv::Mat& img, WINDOW_CORNER corner=RIGHT_BOTTOM, float percent_of_side=0.3f)
{
    switch(corner)
    {
        case RIGHT_BOTTOM:
            window_rect.width = (int)((float)img.cols * percent_of_side);
            window_rect.height = (int)((float)img.rows * percent_of_side);
            window_rect.x = img.cols  - window_rect.width;
            window_rect.y = img.rows  - window_rect.height;
        break;
        default:
        break;
    }
}

// Для начала можно отобразить те 128 семплов что есть в буффере SoundRecorder
// Потом можно набирать больше сигнала и делать нормализацию.
// При желании забить кнопки увеличения окна по времени t+ t-
//
void Oscillogram::Render(cv::Mat& img)
{
    computeWindowRect(img);
    int dx =  window_rect.width / samples_size;
    float scaler_y = window_rect.height / max_amplitude;
    cv::Point p, p0;
    for(int s=0; s < samples_size; s++)
    {
        p.x = window_rect.x + s * dx;
        p.y = window_rect.y + scaler_y * samples[s];
        cv::line(img, p0, p, color);
        p0 = p;
    }

}
