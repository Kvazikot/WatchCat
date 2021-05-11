#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <iostream>
#include <opencv2/core/ocl.hpp>
#include <unistd.h>
#include <sys/stat.h>
#include "face_detection.h"
#include "str_utils.h"
#include "main.h"

using namespace cv;
using namespace std;

std::string execCommand(const std::string cmd, int& out_exitStatus, bool readPipe)
{
    out_exitStatus = 0;
    string bash_cmd = cmd;

    auto pPipe = ::popen(bash_cmd.c_str(), "r");
    if(pPipe == nullptr)
    {
        throw std::runtime_error("Cannot open pipe");
    }

    std::array<char, 256> buffer;

    std::string result;

    if( readPipe )
    while(not std::feof(pPipe))
    {
        auto bytes = std::fread(buffer.data(), 1, buffer.size(), pPipe);
        result.append(buffer.data(), bytes);
    }

    auto rc = ::pclose(pPipe);

    if(WIFEXITED(rc))
    {
        out_exitStatus = WEXITSTATUS(rc);
    }

    return result;
}


string get_date_time()
{
    char tmp[1024];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(tmp, "now: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    return tmp;
}

static const string keys = "{ help h   |   | print help message }"
                           "{ no_video nv | 0 | don't show video }"
                           "{ no_sound ns | 0 | don't play sound }"
                           "{ stdout   s | 0 | std output in jmpeg }"
                           "{ sound_file  f | 0 | sound file }"
                           "{ show_date d  |   | print date }"
                           "{ input i  | 0 | take input video or stream }"
                           "{ email e  | 0 | email me }"
                           "{ output o  |  | save output video }";


int MainLoop(WatchCatOptions& opts)
{

    return 0;
}


void AudioPlayerTest()
{
    AudioFilePlayer player;
    player.another_method();

    player.Play();
    sleep(10);
    player.Play();
    player.Play();
    player.Play();
    sleep(10);
    player.Play();

}

int main(int argc, char** argv)
{
    WatchCatOptions opts;
    time_t now;
    TickMeter timer;
    double sec_since_last_siren=0;
    double time_since_exec=0;

    now = time(NULL);
    struct tm *tm_struct = localtime(&now);
    printf ( "Current local time and date: %s", asctime (tm_struct) );

    timer.start();

    string time_s = IntToStr(tm_struct->tm_hour) + ":" + IntToStr(tm_struct->tm_min) + ":" + IntToStr(tm_struct->tm_sec);
    string date_s = IntToStr(tm_struct->tm_mday) + "." + IntToStr(tm_struct->tm_mon) + "." + IntToStr(tm_struct->tm_year + 1900 - 2000);

    //setting default options
    opts.OUTPUT_VIDEO = "watchcat_output_" + date_s + "_" + time_s + ".avi";



    CommandLineParser parser(argc, argv, keys);
    parser.about("This program work as motion detector with alarm and email notification.");
    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }
    if (!parser.check())
    {
        parser.printErrors();
        return 1;
    }

    opts.ShowVideo = !parser.get<bool>("no_video");
    if (parser.get<string>("output")!="true")
        opts.OUTPUT_VIDEO = parser.get<string>("output");
    if (parser.get<string>("output")=="false")
    {
        opts.OUTPUT_VIDEO = "";
        opts.WriteOutputVideo = false;
    }
    else {
        opts.WriteOutputVideo = true;
    }
    cout << "opts.OUTPUT_VIDEO "  << opts.OUTPUT_VIDEO << endl;
    opts.RedirectVideoToStdout = parser.get<bool>("stdout");
    if (parser.get<bool>("sound_file"))
        opts.SIREN_FILE = parser.get<string>("sound_file");
    opts.ShowDate =  parser.get<bool>("show_date");
    opts.PlaySound = !parser.get<bool>("no_sound");
    opts.INPUT_VIDEO = parser.get<string>("input");
    cout << "opts.OUTPUT_VIDEO is " << opts.OUTPUT_VIDEO << endl;

    if (parser.get<string>("email")=="true")
    {
        cout << "EMAIL " << parser.get<string>("email") << "\n";
        opts.SendEmail = true;
        opts.EMAIL = parser.get<string>("email");
        printf("parser.has(email)");
    }
    else
        opts.SendEmail = false;


    bool bMotionDetected;
    VideoCapture cap;
    AudioFilePlayer sound_player(opts.SIREN_FILE);
    VideoWriter outputVideo; // For writing the video

    RNG rng(12345);

    sound_player.another_method();

    std::cout << opts.INPUT_VIDEO << "\n";

    int dev_id = StrToInt(opts.INPUT_VIDEO);
    if( dev_id != INT_MAX)
        cap.open(dev_id );
    else
        if(!cap.open(opts.INPUT_VIDEO))
            return 0;

    //TODO: parse this output "ffmpeg -f v4l2 -list_formats all -i /dev/video0"
    cap.set(CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CAP_PROP_FRAME_HEIGHT, 480);

    double fps = 25;
    int frame_width = 640;
    int frame_height = 480;

    printf(cv::getBuildInformation().c_str());
    cout << "Frames per second using video.get(CAP_PROP_FPS) : " << fps << endl;
    cout << "frame_width : " << frame_width << endl;

    // Open up the video for writing
    string filename = opts.OUTPUT_VIDEO; // Declare name of file here

    // Declare FourCC code - OpenCV 3.x and beyond
    int fourcc = VideoWriter::fourcc('H','2','6','4');
    //int fourcc = VideoWriter::fourcc('F', 'M', 'P', '4');

    if( opts.WriteOutputVideo)
    {
        printf("outputVideo ");
        outputVideo.open(filename, fourcc, fps, Size(frame_width, frame_height));
    }


    Mat frame, gray, gray1, gray2, gray3, prevFrame, frameDelta, thr, frame_overlay;

    for(;;)
    {
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        vector<Rect> boundRect;

        cap >> frame;

        int cnt = 0;
        if( prevFrame.rows!=0 )
        {
            // compute the absolute difference between the current frame and
            //	 first frame
            cvtColor(prevFrame, gray, cv::COLOR_BGR2GRAY);
            cvtColor(frame, gray1, cv::COLOR_BGR2GRAY);
            GaussianBlur(gray, gray2, Size(21, 21), 0);
            GaussianBlur(gray1, gray3, Size(21, 21), 0);
            //GaussianBlur(gray1, prevFrame, Size(21, 21), 0);
            //frame.copyTo(frameDelta);
            absdiff(gray2, gray3, gray);
            threshold(gray, gray2, 25, 255, cv::THRESH_BINARY);
            InputArray arr(gray3);
            // dilate the thresholded image to fill in holes, then find contours
            // on thresholded image
            int dilation_size = 5;
            Mat element = getStructuringElement( MORPH_ELLIPSE,
                                                 Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                                 Point( dilation_size, dilation_size ) );
            cv::dilate( gray2, gray, element );
            findContours( gray, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE );
            boundRect.resize(contours.size());
            // loop over the contours

            for( size_t i = 0; i< contours.size(); i++ )
            {
                Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
                boundRect[i] = boundingRect( contours[i] );
                //drawContours( gray3, contours, (int)i, color, 2, LINE_8, hierarchy, 0 );
                //std::cout << " Area: " << contourArea(contours[i]) << std::endl;
                if(contourArea(contours[i]) > 150)
                    cnt++;
            }


        }

        if(cnt > 3)
            bMotionDetected = true;
        else
            bMotionDetected = false;

        timer.stop();
        //cout << "timer.getAvgTimeSec()" << timer.getAvgTimeSec() << "\n";
        sec_since_last_siren+=timer.getTimeSec();
        time_since_exec+=timer.getTimeSec();
        timer.reset();
        timer.start();


        // alarm
        if(time_since_exec > opts.LAUNCHING_TIME_DELAY_SEC)
        if(bMotionDetected && opts.SendEmail)
        {
            int ret;
            cout << "sec_since_last_siren " << sec_since_last_siren << "\n";

            if(sec_since_last_siren > opts.email_interval_sec)
            {
                if(opts.PlaySound) sound_player.Play();
                cout << "MOTION DETECTED SENDING EMAIL  " <<
                        "sec_since_last_siren " << timer.getAvgTimeSec() << "\n ";
                imwrite("/home/vova/frame.jpg", frame);
                execCommand("python3 " + opts.EMAIL_SCRIPT, ret );
                printf("-----------------EMAIL SENDED---------------------!\n");
                sec_since_last_siren = 0;

            }

        }


        // draw the text and timestamp on the frame

        frame.copyTo(prevFrame);
        frame.copyTo(gray1);
        prevFrame.copyTo(gray);

        // overlay drawings
        for( size_t i = 0; i< contours.size(); i++ )
        {
            Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
            //rectangle( frame, boundRect[i].tl(), boundRect[i].br(), color, 2 );
        }

        if( opts.ShowDate )
            putText(frame, get_date_time(), Point(10, frame.size[0] - 10), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 1);

        if( bMotionDetected )
            putText(frame, "Motion [DETECTED]", Point(10, 20), cv::FONT_HERSHEY_TRIPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
        else
            putText(frame, "No Motion", Point(10, 20), cv::FONT_HERSHEY_TRIPLEX, 0.5, cv::Scalar(255, 255, 255), 1);


        //resize camera frame
        //cv::resize(frame, frame, cv::Size(), 2, 2);
        //cv::resize(frame_overlay, gray2, cv::Size(), 0.2, 0.2);

        // resize overlay video
        /*
               //cv::resize(frame_overlay, frame_overlay, cv::Size(frame_overlay.cols * 0.2,frame_overlay.rows * 0.2), 0, 0, CV_INTER_LINEAR);
              cv::resize(frame_overlay, frame_overlay, cv::Size(320,240), 0, 0, CV_INTER_LINEAR);

              //embed overlay video
              cv::Rect rect( cv::Point(frame.size[1]/2, frame.size[0]/2), cv::Size( 320, 240 ));
              //cout << roi;
              Mat dst_roi = frame(rect);
              //make a border
              Scalar s(255, 0, 0, 255);
              //cv::copyMakeBorder(frame_overlay, frame_overlay, 10, 10, 10, 10, cv::BORDER_CONSTANT, s);
              cv::rectangle(frame_overlay,Point(0,0),Point(rect.width,rect.height),Scalar(0,0,255),10);

              if( sound_player.isPlaying() )
                 frame_overlay.copyTo(dst_roi);
    */


        if(frame.rows!=0 && opts.ShowVideo) imshow("this is you, smile! :)", frame);

        if( opts.OUTPUT_VIDEO!="" && bMotionDetected ) outputVideo.write(prevFrame);

        if( frame.empty() ) break; // end of video stream

        if( waitKey(10) == 27 ) return 0; // stop capturing by pressing ESC

        if( opts.RedirectVideoToStdout )
        {
            std::vector<uchar> buffer;
            cv::imencode(".jpg", frame, buffer);
            std::fwrite(buffer.data(), 1, buffer.size(), stdout);
            std::fflush(stdout);
        }
    }

    cap.release();
    outputVideo.release();
    cv::destroyAllWindows();
    sleep(1);
    cout << "closed!";
    //AudioPlayerTest();
    //return 0;
    return 0;
    //return FaceDetectionLoop();
}
