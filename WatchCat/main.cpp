#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include "face_detection.h"
#include "main.h"

using namespace cv;
using namespace std;

std::string execCommand(const std::string cmd, int& out_exitStatus)
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

int MainLoop(WatchCatOptions& opts)
{
    bool bMotionDetected;
    VideoCapture cap;
    AudioFilePlayer sound_player(opts.SIREN_FILE);

    VideoCapture cap2(opts.COPS_VIDEO);

    RNG rng(12345);

    sound_player.another_method();

    //printf("aaa");
    //return 0;
    // open the default camera, use something different from 0 otherwise;
    // Check VideoCapture documentation.
    if(!cap.open(0))        
        return 0;

    if(!cap2.open(opts.COPS_VIDEO))
    {
        fprintf(stderr, "cannot open videos");
        return 0;
    }

    Mat frame, gray, gray1, gray2, gray3, prevFrame, frameDelta, thr, frame_overlay;

    for(;;)
    {
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        vector<Rect> boundRect;

          cap >> frame;
          cap2 >> frame_overlay;
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
          {
              if(opts.PlaySound) sound_player.Play();
              bMotionDetected = true;
          }
          else
              bMotionDetected = false;

    // draw the text and timestamp on the frame

          frame.copyTo(prevFrame);
          frame.copyTo(gray1);
          prevFrame.copyTo(gray);

          // overlay drawings
          for( size_t i = 0; i< contours.size(); i++ )
          {
              Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
              rectangle( frame, boundRect[i].tl(), boundRect[i].br(), color, 2 );
          }

          if( opts.ShowDate )
            putText(frame, get_date_time(), Point(10, frame.size[0] - 10), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 1);

          if( bMotionDetected )
              putText(frame, "Motion [DETECTED]", Point(10, 20), cv::FONT_HERSHEY_TRIPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
          else
              putText(frame, "No Motion", Point(10, 20), cv::FONT_HERSHEY_TRIPLEX, 0.5, cv::Scalar(255, 255, 255), 1);


          //resize camera frame
          cv::resize(frame, frame, cv::Size(), 2, 2);
          //cv::resize(frame_overlay, gray2, cv::Size(), 0.2, 0.2);

          // resize overlay video
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

          //cv::Mat roi1( frame, cv::Rect(cv::Point(frame.size[1]/2, frame.size[0]/2), cv::Size( 320, 240 )) );
          //cv::Mat roi2( frame_overlay, cv::Rect(0,0,frame_overlay.cols,frame_overlay.rows) );
          //cv::addWeighted( frame_overlay, 0.5, frame, 0.5, 0.0, frame_overlay );
          cout << "size gray = " << gray.rows << " " << gray.cols << "\n";
          cout << "size frame_overlay = " << frame_overlay.rows << " " << frame_overlay.cols << "\n";


          if(frame.rows!=0 && opts.ShowVideo) imshow("this is you, smile! :)", frame);

          if( frame.empty() ) break; // end of video stream

          if( waitKey(10) == 27 ) break; // stop capturing by pressing ESC

          if( opts.RedirectVideoToStdout )
          {
              std::vector<uchar> buffer;
              cv::imencode(".jpg", frame, buffer);
              std::fwrite(buffer.data(), 1, buffer.size(), stdout);
              std::fflush(stdout);
          }
    }
    // the camera will be closed automatically upon exit
    // cap.close();

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
    for(int i=0; i < argc; i++)
    {
        if( string(argv[i]) == "--no_video" )
            opts.ShowVideo = false;
        if( string(argv[i]) == "--no_sound" )
            opts.PlaySound = false;
        if( string(argv[i]) == "--show_date" )
            opts.ShowDate = true;
        if( string(argv[i]) == "--file" )
        {
            if((i+1) < argc)
                opts.SIREN_FILE =  string(argv[i+1]);
        }
        if( string(argv[i]) == "--stdout" )
            opts.RedirectVideoToStdout = true;

    }
    //AudioPlayerTest();
    //return 0;
    return MainLoop(opts);
    //return FaceDetectionLoop();
}
