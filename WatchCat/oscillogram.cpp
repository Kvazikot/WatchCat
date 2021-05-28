#include <kfr/all.hpp>
#include <stdlib.h>
#include "oscillogram.h"

using namespace kfr;
const float max_amplitude = 32768;

Oscillogram::Oscillogram()
{

    color = cv::Scalar(255,0,0,255);
}

//
void Oscillogram::AllocMemoryForSignal(HWParams& params, int time_sec)
{
    samples_size = params.buffer_frames; //временно!
    if( params.format == SND_PCM_FORMAT_S16_LE)
    {
        samples_size = int(2. * (float)params.rate * time_sec);
        samples = (short*) malloc(samples_size);
        rate = params.rate;
    }


}

void Oscillogram::SetSamples(short* buffer, int buffer_size)
{
    if(buffer!=0)
      memcpy(samples, buffer, buffer_size);
}

// вычисляет где должно находиться окно осцилограмы в изображении img
void Oscillogram::computeWindowRect(cv::Mat& img, WINDOW_CORNER corner=RIGHT_BOTTOM, float percent_of_side=0.5f)
{
    int WINDOW_BORDER = 10;
    switch(corner)
    {
        case RIGHT_BOTTOM:
            window_rect.width = (int)((float)img.cols * percent_of_side);
            window_rect.height = (int)((float)img.rows * percent_of_side);
            window_rect.x = img.cols  - window_rect.width - WINDOW_BORDER;
            window_rect.y = img.rows  - window_rect.height - WINDOW_BORDER;
        break;
        default:
        break;
    }
}

double amp_to_db(double x)
{
    return 20 * log10(std::max(1e-5, (double)x));
}

// Для начала можно отобразить те 128 семплов что есть в буффере SoundRecorder
// Потом можно набирать больше сигнала и делать нормализацию.
// При желании забить кнопки увеличения окна по времени t+ t-
//
void Oscillogram::Render(cv::Mat& img, float max_level)
{
    computeWindowRect(img);
    int n_points = 1000;
    int samples_step = samples_size / n_points;
    float dx =  (float)window_rect.width / (float)n_points;
    max_level+=0.1f;
    float scaler_y = (float)window_rect.height / max_level;
    fprintf(stdout,"samples_size=%d samples_step=%d\n",samples_size,samples_step);
    cv::Point p, p0;
    p = cv::Point(window_rect.x, window_rect.y);
    p0 = cv::Point(window_rect.x + window_rect.width,window_rect.y + window_rect.height);
    cv::rectangle(img, p0, p, cv::Scalar(0,0,255));
    int n=0;
    p0 = p;

    float max_l = -10000;
    for(int s=0; s < samples_size; s+=samples_step)
    {
        //kfr::complex<fbase> amp;
        float sig_level = (float)amp_to_db(samples[s]);
        max_l = std::max(max_l, sig_level);
    }

    for(int s=0; s < samples_size; s+=samples_step)
    {
        p.x = window_rect.x + int((float)n * dx);
        //kfr::complex<fbase> amp;
        float sig_level = (float)amp_to_db(samples[s]);
        //float sig_level = 0;
        if(!(sig_level != sig_level))
        {
            sig_level = ((float)rand()/RAND_MAX - 0.5f) * max_level/sig_level;
            p.y = window_rect.y + window_rect.height / 2 + 100*sig_level;
        }
        else
        {

            fprintf(stdout,"Nan %d\n",s);
        }
        //fprintf(stdout,"sig_level=%f max_l=%f\n",sig_level, max_l);

        cv::line(img, p0, p, color,1);
        p0 = p;
        n++;
    }

}
