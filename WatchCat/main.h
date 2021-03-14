#ifndef MAIN_H
#define MAIN_H

#include <string>
#include <stdio.h>
#include <vector>
#include <time.h>
#include <iostream>
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
        CMD_PLAY_AUDIO = "cvlc";
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
        return res;


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
            string cmd = create_cmd();


            int PID;
            if ((PID=fork())==0){
                int result;
                string res = execCommand(cmd, result);
            }
            time(&t_since_play_start);
        }

    }
};

#endif // MAIN_H
