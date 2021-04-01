#ifndef MAIN_H
#define MAIN_H

#include <string>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <time.h>
#include <iostream>
#include <unistd.h>
#include "str_utils.h"

using namespace std;

std::string execCommand(const std::string cmd, int& out_exitStatus, bool readPipe=true);

struct WatchCatOptions
{
    bool ShowVideo;
    bool RedirectVideoToStdout;
    bool ShowDate;
    bool ShowRectangles;
    bool PlaySound;
    bool WriteOutputVideo;
    bool  SendEmail;
    double   email_interval_sec;
    string  SIREN_FILE;
    string  EMAIL_SCRIPT;
    string  OUTPUT_VIDEO;
    string  INPUT_VIDEO;
    string  EMAIL;

    WatchCatOptions()
    {
        ShowVideo = true;
        PlaySound = true;
        RedirectVideoToStdout = false;
        ShowDate = false;
        ShowRectangles = false;
        SIREN_FILE = "~/WatchCat/asdl1_tone2.wav";
        EMAIL_SCRIPT = "~/src/send_mail/send_mail.py";
        INPUT_VIDEO = "";
        email_interval_sec = 15;
        SendEmail = false;
    }
};

pthread_t tid[2];
static char strbuf[1024];

void* playSound(void *arg)
{
    int result;
    string res = execCommand(strbuf, result, false);
    printf(strbuf);
    return NULL;
}

class AudioFilePlayer
{
public:
    string SIREN_FILE;
    string CMD_PLAY_AUDIO;
    time_t t_since_play_start;
    long   duration_sec;

    AudioFilePlayer()
    {
        CMD_PLAY_AUDIO = "ffplay -nodisp -autoexit";
        SIREN_FILE = "/home/vova/WatchCat/SIREN1.wav";
        time(&t_since_play_start);
    }

    void another_method()
    {
        get_duration();
        FILE* f =  fopen(SIREN_FILE.c_str(),"r");
        if(f == NULL)
        {
            cout << "\n";
            cout << "cannot open file " << SIREN_FILE << endl;
        }
        else
          fclose(f);

        //t_since_play_start+=duration_sec;
    }

    AudioFilePlayer(string siren_file)
        : AudioFilePlayer()
    {
        SIREN_FILE = siren_file;
        CMD_PLAY_AUDIO = create_cmd();

    }

    string create_cmd()
    {
        string cmd;
        cmd  =  CMD_PLAY_AUDIO + " " + SIREN_FILE + " &";
        return string(cmd);
    }

    string get_duration()
    {
        string cmd;
        int result;
        cmd = "ffmpeg -i " + SIREN_FILE + " 2>&1 | grep Duration | awk '{print $2}' | tr -d ,";
        printf("\n");
        printf(cmd.c_str());
        printf("\n");
        string res = execCommand(cmd, result);
        printf(res.c_str());

        if(res.size()>8)
        {
            //parse string like this "00:00:09.37"
           vector<string> parts =  StrSplitE(res, ":.", false);
           printf("CountSeps %d", CountSeps(res, ":."));
           //return "";
           if (parts.size() == 4)
           {
               int h = StrToInt(parts[0]);
               int m = StrToInt(parts[1]);
               int s = StrToInt(parts[2]);
               std::cout << parts[0] << " " << parts[1] << " " << parts[2] << "\n";
               printf("%d hours %d minutes %d seconds\n",h,m,s);
               duration_sec = h*60*60 + m*60 + s;
           }
        }

        return "";

    }

    void parse_duration()
    {

    }

    bool isPlaying()
    {
        time_t current_time;
        time(&current_time);
        time_t delta = current_time - t_since_play_start;
        if( delta >= duration_sec)
          return false;
        else
            return true;
    }

    void Play()
    {
        time_t current_time;
        time(&current_time);
        time_t delta = current_time - t_since_play_start;
        //printf("delta = %ld duration_sec = %ld\n ", delta, duration_sec);
        //check if previos play is over
        if( delta >= duration_sec)
        {
            string cmd = create_cmd();
            strcpy((char*)strbuf, (char*)cmd.c_str());
            int err = pthread_create(&(tid[0]), NULL, &playSound, (void*)strbuf);

            if (err != 0)
                printf("\ncan't create thread :[%d]", err);
            else
                printf("\n Thread created successfully\n");
            time(&t_since_play_start);
        }

    }
};

#endif // MAIN_H
