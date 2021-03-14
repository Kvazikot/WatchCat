#ifndef MAIN_H
#define MAIN_H

#include <string>
#include <stdio.h>
#include <vector>
#include <time.h>
#include <unistd.h>
#include "str_utils.h"

using namespace std;

std::string execCommand(const std::string cmd, int& out_exitStatus);

class AudioFilePlayer
{
public:
    string SIREN_FILE;
    string CMD_PLAY_AUDIO;
    time_t t_since_play_start;
    bool   isPlaying;
    long   duration_sec;

    AudioFilePlayer()
    {
        CMD_PLAY_AUDIO = "cvlc %s &";
        SIREN_FILE = "~/WatchCat/SIREN1.wav";
        time(&t_since_play_start);
    }

    void another_method()
    {
        get_duration();
        //t_since_play_start+=duration_sec;
    }

    AudioFilePlayer(string siren_file)
        : AudioFilePlayer()
    {
        SIREN_FILE = siren_file;
        CMD_PLAY_AUDIO = parse_cmd();
    }

    string parse_cmd()
    {
        char cmd[255];
        sprintf(cmd, CMD_PLAY_AUDIO.c_str(), (const char*)SIREN_FILE.c_str());
        return string(cmd);
    }

    string get_duration()
    {
        char cmd[255];
        int result;
        sprintf(cmd, "ffmpeg -i %s  2>&1 | grep Duration | awk '{print $2}' | tr -d ,", SIREN_FILE.c_str());
        printf("\n");
        //printf(cmd);
        string res = execCommand(cmd, result);
        if(res.size()>8)
        {
            //parse string like this "00:00:09.37"
           vector<string> parts =  StrSplitE(res, ":.", false);
           if (parts.size() == 4)
           {
               int h = StrToInt(parts[0]);
               int m = StrToInt(parts[1]);
               int s = StrToInt(parts[2]);
               printf("%d hours %d minutes %d seconds\n",h,m,s);
               duration_sec = h*60*60 + m*60 + s;
           }
        }



    }

    void parse_duration()
    {

    }

    void Play()
    {
        time_t current_time;
        time(&current_time);
        time_t delta = current_time - t_since_play_start;
        printf("delta = %ld duration_sec = %ld\n ", delta, duration_sec);
        //check if previos play is over
        if( delta >= duration_sec)
        {
            string cmd = parse_cmd();

            int result;
            int PID;
            if ((PID=fork())==0){
                string res = execCommand(cmd, result);
            }
            time(&t_since_play_start);
        }

    }
};

#endif // MAIN_H
