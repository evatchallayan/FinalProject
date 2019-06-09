
// CPP program to Stitch
// input images (panorama) using OpenCV
#include <iostream>
#include <fstream>

// Include header files from OpenCV directory
// required to stitch images.
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching.hpp"

#define CVUI_IMPLEMENTATION
#include "cvui-2.7.0/cvui.h"

#define WINDOW_NAME	"Panorama Stitching"

using namespace std;
using namespace cv;

Stitcher::Mode mode = Stitcher::PANORAMA;

// Array for pictures
vector<Mat> imgs;

int checkBool(bool eiffel1,bool eiffel2,bool eiffel3,bool eiffel4){
  int count = 0;

  if(eiffel1){
     count++;
   }
   if(eiffel2){
      count++;
    }
   if(eiffel3){
      count++;
   }
   if(eiffel4){
      count++;
    }

  cout << "eiffelcount = "<< count << endl;
  return count;
}

int main(void)
{
    cv::Mat result;
    cv::Mat frame = cv::Mat(cv::Size(450, 650), CV_8UC3);
    Mat image1, image2, image3, image4;
    int low_threshold = 50, high_threshold = 150;
    bool use_canny = false;
    bool eiffel1 = false;
    bool eiffel2 = false;
    bool eiffel3 = false;
    bool eiffel4 = false;
    int eiffel_count=0;
    namedWindow("Image", CV_MINOR_VERSION);
    moveWindow("Image",60 + frame.cols, 80);

    image1=imread("Images/eiffel1.jpeg");
    if(!image1.data){
       cout<<"Could not read image 1";
       return -1;
     }
     image2=imread("Images/eiffel2.jpeg");
     if(!image2.data){
        cout<<"Could not read image 2";
   return -1;
    }
     image3=imread("Images/eiffel3.jpeg");
     if(!image3.data){
        cout<<"Could not read image 3";
    return -1;
    }
    image4=imread("Images/eiffel4.jpeg");
    if(!image4.data){
       cout<<"Could not read image 4";
   return -1;
   }

    // Init a OpenCV window and tell cvui to use it.
    cv::namedWindow(WINDOW_NAME);
    cvui::init(WINDOW_NAME);





	 Mat dst;

    	// Create a Stitcher class object with mode panoroma
    	Ptr<Stitcher> stitcher = Stitcher::create(mode);

while (true) {
        imgs.clear();
        // Should we apply Canny edge?
        //if (use_canny) {
            // Yes, we should apply it.
          //  cv::cvtColor(lena, frame, CV_BGR2GRAY);
           // cv::Canny(frame, frame, low_threshold, high_threshold, 3);
         // else {
            // No, so just copy the original image to the displaying frame.
	    // Mat black = imread("rose.jpg");
	    // imgs.push_back(black);
	    // imgs.push_back(black);
	    //black.copyTo(frame);
            //lena.copyTo(frame);
        // }
        cvui::checkbox(frame, 15, 105, "Eiffel 1", &eiffel1);
        cvui::checkbox(frame, 15, 125, "Eiffel 2", &eiffel2);
        cvui::checkbox(frame, 15, 145, "Eiffel 3", &eiffel3);
        cvui::checkbox(frame, 15, 165, "Eiffel 4", &eiffel4);

        if(eiffel1){
      		imgs.push_back(image1);
          eiffel_count ++;
      	}

      	if(eiffel2){
      		imgs.push_back(image2);
      	}

      	if(eiffel3){
      		imgs.push_back(image3);
      	}

      	if(eiffel4){
      		imgs.push_back(image4);
      	}


      eiffel_count = checkBool(eiffel1,eiffel2,eiffel3,eiffel4);

    	// Command to stitch all the images present in the image array
      if(eiffel_count >1){

            Stitcher::Status status = stitcher->stitch(imgs, dst);
            // cout << "eiffel_count = "<< eiffel_count << endl;
            cout <<"3333333333333333333" << endl;

            if (status != Stitcher::OK) {
           	// Check if images could not be stiched
           	// status is OK if images are stiched successfully
           	cout << "Can't stitch images\n";
           	
          }

      }




        // Checkbox to enable/disable the use of Canny edge
        //cvui::checkbox(frame, 15, 85, "Use Canny Edge", &use_canny);
        if(eiffel_count > 1){

          cv::imshow("Image", dst);
        }
        // Update cvui internal stuff
        cvui::update();

        // Show everything on the screen

        cv::imshow(WINDOW_NAME, frame);

         //Check if ESC was pressed
        if (cv::waitKey(30) == 27) {
            break;
        }
    }
    return 0;
}
