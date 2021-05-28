#include <kfr/dft.hpp>
#include <kfr/base.hpp>
#include <kfr/dft.hpp>
#include <kfr/dsp.hpp>
#include <kfr/io.hpp>

#include "wave_in.h"
#include "soundrecorder.h"
#include <stdio.h>
#include <stdlib.h>


using namespace kfr;
HWParams SoundRecorder::data;

int TestKFR()
{
    println(library_version());

    // fft size
    const size_t size = 128;

    // initialize input & output buffers
    univector<complex<fbase>, size> in  = sin(linspace(0.0, c_pi<fbase, 2> * 4.0, size));
    univector<complex<fbase>, size> out = scalar(qnan);

    // initialize fft
    const dft_plan<fbase> dft(size);

    dft.dump();

    // allocate work buffer for fft (if needed)
    univector<u8> temp(dft.temp_size);

    // perform forward fft
    dft.execute(out, in, temp);

    // scale output
    out = out / size;

    // get magnitude and convert to decibels
    univector<fbase, size> dB = amp_to_dB(cabs(out));

    println("max  = ", maxof(dB));
    println("min  = ", minof(dB));
    println("mean = ", mean(dB));
    println("rms  = ", rms(dB));

    println(in);
    println();
    println(dB);
    return 0;
}

SoundRecorder::SoundRecorder()
{
    set_device(AUDIO_DEVICE);
}

// data->device_str must contain correct device name
int open_device(HWParams* data)
{
  int i;
  int err;  

  if ((err = snd_pcm_open (&(data->capture_handle), data->device_str.c_str(), SND_PCM_STREAM_CAPTURE, 0)) < 0) {
    fprintf (stderr, "cannot open audio device %s (%s)\n",
             data->device_str.c_str(),
             snd_strerror (err));
    exit (1);
  }

  fprintf(stdout, "audio interface opened\n");

  if ((err = snd_pcm_hw_params_malloc (&data->hw_params)) < 0) {
    fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
             snd_strerror (err));
    exit (1);
  }

  fprintf(stdout, "hw_params allocated\n");

  if ((err = snd_pcm_hw_params_any ((data->capture_handle), data->hw_params)) < 0) {
    fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
             snd_strerror (err));
    exit (1);
  }

  fprintf(stdout, "hw_params initialized\n");

  if ((err = snd_pcm_hw_params_set_access (data->capture_handle, data->hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
    fprintf (stderr, "cannot set access type (%s)\n",
             snd_strerror (err));
    exit (1);
  }

  fprintf(stdout, "hw_params access setted\n");

  if ((err = snd_pcm_hw_params_set_format(data->capture_handle, data->hw_params, data->format)) < 0) {
    fprintf (stderr, "cannot set sample format (%s)\n",
             snd_strerror (err));
    exit (1);
  }

  fprintf(stdout, "hw_params format setted\n");

  if ((err = snd_pcm_hw_params_set_rate_near (data->capture_handle, data->hw_params, &data->rate, 0)) < 0) {
    fprintf (stderr, "cannot set sample rate (%s)\n",
             snd_strerror (err));
    exit (1);
  }

  fprintf(stdout, "hw_params rate setted\n");

/*  if ((err = snd_pcm_hw_params_set_channels (capture_handle, hw_params, 1)) < 0) {
    fprintf (stderr, "cannot set channel count (%s)\n",
             snd_strerror (err));
    exit (1);
  }


  fprintf(stdout, "hw_params channels setted\n");
  */

  if ((err = snd_pcm_hw_params (data->capture_handle, data->hw_params)) < 0) {
    fprintf (stderr, "cannot set parameters (%s)\n",
             snd_strerror (err));
    exit (1);
  }

  fprintf(stdout, "hw_params setted\n");

  snd_pcm_hw_params_free (data->hw_params);

  fprintf(stdout, "hw_params freed\n");

  if ((err = snd_pcm_prepare (data->capture_handle)) < 0) {
    fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
             snd_strerror (err));
    exit (1);
  }

  fprintf(stdout, "audio interface prepared\n");

  data->buffer = (short*)malloc(128 * snd_pcm_format_width(data->format) / 8 * 2);

  fprintf(stdout, "buffer allocated\n");
  return 1;


}

// virtual
void SoundRecorder::processing()
{
    fprintf(stdout, "SoundRecorder::processing()\n");

}

void close_device(HWParams* data)
{
    free(data->buffer);
    fprintf(stdout, "buffer freed\n");
    snd_pcm_close (data->capture_handle);
    fprintf(stdout, "audio interface closed\n");

}

void SoundRecorder::set_device(string device_s)
{
    data.device_str = device_s;

}

SoundThresholding::SoundThresholding()
    :SoundRecorder()
{

}

void SoundThresholding::Test2()
{
    printf("SoundThresholding::Test2()\n");

}

void SoundThresholding::Test1()
{
    printf("SoundThresholding::Test1()\n");
    set_device(AUDIO_DEVICE);
    SoundThresholding::processing();
}



/*
void* processingc(void *arg)
{
    void (SoundThresholding::*processing)()  = NULL;
    processing = &SoundRecorder::processing;
    SoundThresholding* obj_ptr = (SoundThresholding*)(arg);
    fprintf(stdout, "addr of this = %d\n", (void*)obj_ptr);
    fprintf(stdout, "processingc\n");
    //(*obj_ptr).open_device(obj_ptr->device_name());
    //(*obj_ptr.*processing)();
    //(*obj_ptr).close_device();

    return NULL;
}
*/

int test()
{

    int src = 2;
    int dst;


    asm ("mov %1, %0\n\t"
        "add $1, %0"
        : "=r" (dst)
        : "r" (src));

    printf("%d\n", dst);
  return 0;
}


void* SoundThresholding_processing(void *arg)
{
    int err;
    fprintf(stdout, "SoundThresholding::processing()\n");
    sleep(5);
    HWParams* data = (HWParams*)(arg);
    //HWParams* data = *data1;

    open_device(data);
    fprintf(stdout,"SoundThresholding_processing %s\n", data->device_str.c_str());

    // fft size
    const size_t size = 128;

    // initialize input & output buffers
    univector<complex<fbase>, size> in ; //sin(linspace(0.0, c_pi<fbase, 2> * 4.0, size));
    univector<complex<fbase>, size> out = scalar(qnan);

    // initialize fft
    const dft_plan<fbase> dft(size);

    //dft.dump();

    // allocate work buffer for fft (if needed)
    univector<u8> temp(dft.temp_size);

    int processing_time_sec = 5;
    int n_samples = processing_time_sec * data->rate;
    int n_buffers = n_samples / data->buffer_frames;
    int data_size = data->rate * processing_time_sec * sizeof(short); //in bytes
    short* samples_output= (short*) malloc(data_size);
    short* data_ptr = samples_output;
    fprintf(stdout, "data_size = %d n_buffers = %d n_samples = %d", data_size, n_buffers, n_samples);


    {
        #ifdef TEST_MODE
          for (int i = 0; i < n_buffers; ++i)
        #else
        while( !data->bStopThread )
        #endif
        {
           if ((err = snd_pcm_readi (data->capture_handle, (short*)data->buffer, data->buffer_frames)) != data->buffer_frames) {
              fprintf (stderr, "read from audio interface failed (%s)\n",
                       err, snd_strerror (err));
           }

            int n_fft_blocks = data->buffer_frames / size;
            for (int n = 0; n < n_fft_blocks; ++n) {

                //fill input complex samples
                for(int j=0; j < size; j++)
                {
                    complex<fbase> sample(data->buffer[n * size + j],0);
                    in[j] = sample;
                    //data->i* n_buffers + n * size + j] = (float) sample.real() / 32768;
                    #ifdef TEST_MODE
                        *data_ptr = data->buffer[n * size + j];//sample.real(); /// 32768;
                        data_ptr++;
                    #endif
                }

                // perform forward fft
                dft.execute(out, in, temp);

                // scale output
                out = out / size;

                // get magnitude and convert to decibels
                univector<fbase, size> dB = amp_to_dB(cabs(out));
                data->result.max_level = mean(dB);
                //println("max  = ", maxof(dB));
                //println("min  = ", minof(dB));
                //println("mean = ", mean(dB));
                //println("rms  = ", rms(dB));
            }

          }

    }
    close_device(data);
#ifdef TEST_MODE
    wavwrite("/home/vova/WatchCat/temp_soundthreshlding.wav", samples_output, data_size, data->rate, 1);
#endif
    return 0;
}

void SoundThresholding::StartProcessingThread()
{
    void* (*fun)(void*)  = NULL;
    HWParams* param = &this->data;
    fun = &SoundThresholding_processing;
    int err = pthread_create(&(tid[0]), NULL, fun, param);
    // wait for the completion of thread 2
    //pthread_join(tid[0], NULL);
}

void SoundThresholding::processing()
{
    HWParams* param = &this->data;
    SoundThresholding_processing(param);
}
