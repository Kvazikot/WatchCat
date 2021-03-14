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


int MainLoop()
{
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
            //imshow("this is you, smile! :)", frameDelta);
            threshold(gray, gray2, 25, 255, cv::THRESH_BINARY);
            InputArray arr(gray3);
            // dilate the thresholded image to fill in holes, then find contours
            // on thresholded image
            int dilation_size = 5;
            Mat element = getStructuringElement( MORPH_ELLIPSE,
                                   Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                   Point( dilation_size, dilation_size ) );
            cv::dilate( gray2, gray, element );
//                      if(gray.rows!=0) imshow("this is you, smile! :)", gray);
            std::cout <<  "!!!!!!!! \n" ;
            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;
            findContours( gray, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE );

            // loop over the contours

            for( size_t i = 0; i< contours.size(); i++ )
                {
                    Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
                    drawContours( gray3, contours, (int)i, color, 2, LINE_8, hierarchy, 0 );
                    std::cout << " Area: " << contourArea(contours[i]) << std::endl;
                    if(contourArea(contours[i]) > 150)
                        cnt++;
                }
          }

          if(cnt > 3)
          {
                player.Play();
          }

            if(gray3.rows!=0) imshow("this is you, smile! :)", gray3);

          std::cout <<  "frame rows " << frame.rows << "\n";
          frame.copyTo(prevFrame);
          frame.copyTo(gray1);
          prevFrame.copyTo(gray);
          std::cout <<  "prevFrame rows " << prevFrame.rows << "\n";


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

    return MainLoop();
}
