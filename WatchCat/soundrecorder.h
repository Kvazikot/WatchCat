#ifndef SOUNDRECORDER_H
#define SOUNDRECORDER_H

#include <string>
#include <alsa/asoundlib.h>

using namespace std;

struct HWParams
{
    string device_str;
    snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_t *capture_handle;
    short* buffer;
    int buffer_frames = 128;
    unsigned int rate = 44100;
    bool bStopThread = false;
};

class SoundRecorder
{ 
public:
    SoundRecorder();
    int open_device(string device);
    void close_device();
    void set_device(string device_s);
    snd_pcm_t* device(){ return data.capture_handle;}
    string device_name(){ return data.device_str;}
    virtual void processing();

    static HWParams data;
};

class SoundThresholding : public SoundRecorder
{
    public:
        SoundThresholding();
        virtual void processing();
        void Test1();
        void Test2();
        void StartProcessingThread();

        pthread_t tid[2];

};

#endif // SOUNDRECORDER_H
