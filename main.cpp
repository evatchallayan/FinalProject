#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ctime>
#include <sstream>
#include <stdlib.h>
#define CVUI_IMPLEMENTATION
#include "cvui-2.7.0/cvui.h"

#define WINDOW_NAME	"Operations"
#define X 600
#define Y 1000


using namespace cv;
using namespace std;

Mat src, dst;
bool use_draw = false;
bool use_canny = false;
bool use_dilation = false;
bool use_erosion = false;
int isSaved = 0;
bool leftButton = false;
String colour = "Black";
int r = 0,g = 0,b = 0;
int s = 1;
int timeVar = 50;

void CheckSave(int* isSaved, Mat frame, Mat dst)
{
	if (*isSaved)
	{
		timeVar = timeVar -1;
		cvui::printf(frame,X-110, 70, 0.5, 0x00ff00,"Image saved");
		if(timeVar == 0) *isSaved = 0;
	}
	if (cvui::button(frame, X-80, 40,"Save"))
	{
		imwrite("Images/newImage.jpg",dst);
		*isSaved = 1;
		timeVar = 50;
	}
}

void setRGB(int red, int green, int blue){
  // cout << "setRGB" << endl;

  r = red;
  g = green;
  b = blue;
}

void setSize(int size){
  s = size;
}
void showColor(int c)
{
  switch (c) {
    case 1:
      setRGB(0,0,0);
      colour = "Black";
      break;
    case 2:
      setRGB(255,0,0);
       colour = "Red";
       break;
    case 3:
      setRGB(0,255,0);
       colour = "Green";
       break;
    case 4:
      setRGB(0,0,255);
       colour = "Blue";
       break;
    case 5:
      setRGB(255,255,255);
       colour = "White";
       break;
    default:
      setRGB(0,0,0);
      colour = "Black";
      break;

  }

}

void turnOffUseVar()
{
  use_canny = false;
  use_dilation = false;
  use_erosion = false;

}
void CallBackFunc(int event, int x, int y, int flags, void* param)
{
  if(use_draw){
    turnOffUseVar();
    Mat& m = *(cv::Mat*) param;

    if(event == 1){
      leftButton = true;
    }
    else if(event == 4 ){
      leftButton = false;
    }

    if  (event == 0 && leftButton)
    {
      // cout << "callback-0" << endl;
      for(int i = -s; i < s + 2; i++){
        for(int j = -s; j < s + 2; j++){
          for(int c = 0; c < m.channels(); c++){

            m.at<Vec3b>(y+j,x+i)[0] = b; //blue
            m.at<Vec3b>(y+j,x+i)[1] = g; //green
            m.at<Vec3b>(y+j,x+i)[2] = r; //red
          }
        }
      }
      cv::imshow("Image", m);

    }
    // else if ( event == 1 )
    // {
    //
    //   // for(int c = 0; c < m.channels(); c++){
    //     // cout << "Chanenel c=" << c << " -->" << (int)m.at<Vec3b>(y,x)[c] << endl;
    //
    //   }
    // }
  }
}


int main(int argc, const char *argv[])
{
  Mat frame = cv::Mat(cv::Size(X, Y), CV_8UC3);
  Mat kernel; //for erosion
  int count = 1;
  int si = 1;


  // VideoCapture cap ("Images/chaplin.mp4");
  // if(!cap.isOpened()){
  //   cout << "Error opening video stream or file" << endl;
  //   return -1;
  // }

  src = imread("Images/Mark-Zuckerberg.jpg", IMREAD_COLOR);
  if(!src.data)
  {
    cout << "Empty data" << endl;
    exit(0) ;
  }

  namedWindow("Image", CV_MINOR_VERSION);
  moveWindow("Image",60 + frame.cols, 80);

  int low_threshold = 50, high_threshold = 150;
  int width = 400;


  double vValue = 1;
  double hValue = 1;
  bool keepProportion = true;
  int vSize, hSize;
  std::ostringstream size;


  //shape of kernel for erosion
  int	shape_type=0;
  //Size of the kernel for Erode and Clone
  int siz = 0;
  double bValue = 0, cValue = 1;

  cvui::init(WINDOW_NAME);

  while (true) {
    frame = cv::Scalar(49, 52, 49);

    //canny_edge(src,dst, frame);

    if (cvui::button(frame, X-80, 10, "Quit")) {
      break;
    }

     CheckSave(&isSaved, frame, dst);


    /*  CANNY EDGE BAR */

    int x_canny=10;
    int y_canny=10;
    cvui::window(frame, x_canny, y_canny, 190,200, "Canny");

    cvui::checkbox(frame, x_canny+10, y_canny+25 , "Use Canny Edge", &use_canny);

    cvui::trackbar(frame, x_canny, y_canny+50, 150, &low_threshold, 5, 150);
    cvui::trackbar(frame, x_canny, y_canny+100, 150, &high_threshold, 80, 300);

    if(!use_draw){
      if (use_canny) {
        cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);
        cv::Canny(dst, dst, low_threshold, high_threshold, 3);
      }
      else {
        src.copyTo(dst);
      }
    }

    /*----------Finish---------*/




    /*  RESIZE BAR */

    Mat dst2 = dst;
    int x_resize= x_canny;
    int y_resize= y_canny+220;

    cvui::window(frame, x_resize, y_resize, 400,200, "Resize");

    cvui::checkbox(frame, x_resize+10, y_resize+25, "Keep Proportion", &keepProportion);
    cvui::text(frame, x_resize+60, y_resize+50, "Vertical");
    cvui::trackbar(frame, x_resize+10, y_resize+55, 150, &vValue, 0.2, 3., 1, "%.3Lf",  1, 0.001 );
    cvui::text(frame, x_resize+50, y_resize+110,"Horizontal");
    cvui::trackbar(frame, x_resize+10, y_resize+115, 150, &hValue, 0.2, 3., 1, "%.3Lf", 1, 0.001);
    cvui::text(frame, x_resize+40, y_resize+175,size.str());
    size.clear();
    size.str("");

    if(!use_draw){
      if(keepProportion){
        resize(dst2, dst, Size(), vValue, vValue, INTER_CUBIC);
        hSize = dst.cols;
        vSize = dst.rows;
        // cout <<  "H: " << hSize << " V: "<< vSize << endl	;
        size << "H: " << hSize << " V: "<< vSize;

      }
      else{
        resize(dst2, dst, Size(), hValue, vValue, INTER_CUBIC);
        hSize = dst.cols;
        vSize = dst.rows;
        // cout << "H: " << hSize << " V: "<< vSize << endl;
        size << "H: " << hSize << " V: "<< vSize;

      }

    }
    else{
      size << "H: " << hSize << " V: "<< vSize;
    }
    /*----------Finish---------*/


    /*  LIGHTEN BAR */

    int x_light= x_canny;
    int y_light= y_resize+220;
    cvui::window(frame, x_light, y_light, 400,200, "Lighten & Darken");
    cvui::text(frame, x_light+10, y_light+25, "brightness trackbar");
    cvui::trackbar(frame, x_light, y_light+50, 150, &bValue, -225.,255.);
    // cvui::trackbar(width, &bValue, -255., 255., 1, "%.1Lf", cvui::TRACKBAR_DISCRETE, 1.);
    // cvui::space(5);
    cvui::text(frame, x_light+10, y_light+120, "Contrast trackbar");
    cvui::trackbar(frame, x_light, y_light+150, 150, &cValue,0.,6.);

    //cvui::trackbar(width, &cValue, 0., 6., 1, "%.2Lf", cvui::TRACKBAR_DISCRETE, 0.01);
    // cvui::space(5);
    if(!use_draw){
      dst.convertTo(dst,-1,cValue,bValue);
    }

    /*----------Finish---------*/





    /*  EROSION BAR */

    int x_erosion= x_canny;
    int y_erosion= y_light+220;
    cvui::window(frame, x_erosion, y_erosion, 400,200, "Dilation & Erosion");
    cvui::checkbox(frame, x_erosion+10, y_erosion+25 , "Dilation" , &use_dilation);
    cvui::checkbox(frame, x_erosion+90, y_erosion+25 , "Erosion", &use_erosion);
    //creating the trackbar for choosing the shape of kernel
    cvui::text(frame, x_erosion, y_erosion+50, "Shape:  0-Box  1-Cross  2-Ellipse");
    cvui::trackbar(frame, x_erosion, y_erosion+60, 150, &shape_type, 0, 2);

    //creating the trackbar for reading the size
    cvui::text(frame, x_erosion, y_erosion+130, "Size of kernel(2n+1)");
    cvui::trackbar(frame, x_erosion, y_erosion+150, 150, &siz, 0, 21);

    if (shape_type == 0) {
      kernel = getStructuringElement(MORPH_RECT, Size(2 * siz + 1, 2 * siz + 1));
    }
    else if(shape_type==1) {
      kernel = getStructuringElement(MORPH_CROSS, Size(2 * siz + 1, 2 * siz + 1));
    }
    else if(shape_type==2){
      kernel = getStructuringElement(MORPH_ELLIPSE, Size(2 * siz + 1, 2 * siz + 1));
    }
    if(!use_draw){
      if (use_dilation) {
        dilate(dst, dst, kernel);
      }
      else if(use_erosion){
        erode(dst, dst, kernel);
      }
    }
    /*----------Finish---------*/


    /*PEN FUNCTION*/

    int x_draw = 220;
    int y_draw = 10;
    cvui::window(frame, x_draw, y_draw, 190,200, "Draw");
    cvui::checkbox(frame, x_draw+10, y_draw+25 , "Pen", &use_draw);

    cvui::text(frame, x_draw+10, y_draw+55 , "Colour:");
    cvui::counter(frame, x_draw+10,  y_draw+70, &count);
    showColor(count);
    cvui::text(frame, x_draw+110, y_draw+75 , colour);

    cvui::text(frame, x_draw+10, y_draw+100 , "Size:");
    cvui::counter(frame, x_draw+10,  y_draw+115, &si);
    setSize(si);

    if (use_draw){
      cv::setMouseCallback("Image", CallBackFunc, &dst);
      // cv::imshow("Image", dst);
      // cv::waitKey(25);
    }

    /*----------Finish---------*/



    cv::imshow("Image", dst);


    cvui::update();
    cv::imshow(WINDOW_NAME, frame);

    // Check if ESC key was pressed
    if (cv::waitKey(20) == 27) {
      break;
    }

  }


  return 0;
}
