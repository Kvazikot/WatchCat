#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include "main.h"

using namespace cv;
using namespace std;

std::string execCommand(const std::string cmd, int& out_exitStatus)
{
    out_exitStatus = 0;
    auto pPipe = ::popen(cmd.c_str(), "r");
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

int MainLoop()
{
    bool bMotionDetected;
    VideoCapture cap;
    AudioFilePlayer player;

    RNG rng(12345);

    player.another_method();

    //printf("aaa");
    //return 0;
    // open the default camera, use something different from 0 otherwise;
    // Check VideoCapture documentation.
    if(!cap.open(0))
        return 0;

    Mat frame, gray, gray1, gray2, gray3, prevFrame, frameDelta, thr;

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
          {
              player.Play();
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

          putText(frame, get_date_time(), Point(10, frame.size[0] - 10), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 1);
          if( bMotionDetected )
             putText(frame, "Motion [DETECTED]", Point(10, 20), cv::FONT_HERSHEY_TRIPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
          else
             putText(frame, "No Motion", Point(10, 20), cv::FONT_HERSHEY_TRIPLEX, 0.5, cv::Scalar(255, 255, 255), 1);

          //if(gray3.rows!=0) imshow("this is you, smile! :)", frame);

          if( frame.empty() ) break; // end of video stream

          if( waitKey(10) == 27 ) break; // stop capturing by pressing ESC
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
    //AudioPlayerTest();
    //return 0;
    return MainLoop();
}
