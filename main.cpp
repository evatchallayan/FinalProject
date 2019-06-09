#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ctime>
#include <sstream>
#include <stdlib.h>

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>

#define CVUI_IMPLEMENTATION
#include "cvui-2.7.0/cvui.h"

#define WINDOW_NAME	"Operations"
#define X 600
#define Y 1000


using namespace cv;
using namespace std;

Mat src=imread("Images/Mark-Zuckerberg.jpg", IMREAD_COLOR), dst;
bool use_draw = false;
bool use_canny = false;
bool use_dilation = false;
bool use_erosion = false;
int isSaved = 0;
bool leftButton = false;
String colour = "Black";
int red, green, blue;
int s = 1;
int timeVar = 50;

String face_cascade_name = "XML/haarcascade_frontalface_alt.xml";
String smile_cascade_name = "XML/haarcascade_smile.xml";
String eye_cascade_name = "XML/haarcascade_eye_tree_eyeglasses.xml";

CascadeClassifier cascade;
CascadeClassifier smile_cascade;
CascadeClassifier eyes_cascade;

bool detect_eyes = false, detect_face=false, detect_smile=false;
bool isFacedetected=false, isEyedetected=false,isSmiledetected=false;

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

void setRGB(int x, int y, int z){
  // cout << "setRGB" << endl;

  red = x;
  green = y;
  blue = z;
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
      if(x!= m.cols && x!= 0 && y!=m.rows && y!=0 ){
        for(int i = -s; i < s +1; i++){
          for(int j = -s; j < s + 1; j++){
            for(int c = 0; c < m.channels(); c++){

              m.at<Vec3b>(y+j,x+i)[0] = blue; //blue
              m.at<Vec3b>(y+j,x+i)[1] = green; //green
              m.at<Vec3b>(y+j,x+i)[2] = red; //red
            }
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
void face(Mat frame)
{
	if (cvui::button(frame, X-80, 80,"Face"))
	{
		system("./face_recognition");
	}
}

void detectAndDraw(Mat& img) {

	vector<Rect> faces;
	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	equalizeHist(gray, gray);

	//-- Detect faces
	if(detect_face){
		cascade.detectMultiScale(gray, faces, 1.2, 5, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
		for (size_t i = 0; i < faces.size(); i++)
		{
			Rect r = faces[i];
		//if the face is already detected(the rectangle already drawn) don't draw again
			if(!isFacedetected){
				Scalar color = Scalar(255, 0, 0);
			rectangle(img, Point(r.x, r.y), Point(r.x + r.width-1, r.y + r.height-1), color, 3, 8, 0);
			}
		Mat faceROI = gray(faces[i]);
		Mat SmileROI = gray(faces[i]);

		if (detect_eyes) {
			//-- In each face, detect eyes
			std::vector<Rect> eyes;
			eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 1, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
			//eyes_cascade.detectMultiScale(faceROI, eyes);
			for (size_t j = 0; j < eyes.size(); j++)
			{
				//if the eyes are already detected(the circle already drawn) don't draw again
				if(!isEyedetected){
					Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
					int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
					circle(img, center, radius, Scalar(0, 0, 255), 2, 8, 0);
				}
			}
			isEyedetected=true;
		}
		if (detect_smile){
			//-- Detect Smile
			std::vector<Rect> smile;
			smile_cascade.detectMultiScale(SmileROI, smile, 1.7, 20);
			for (int j = 0; j < smile.size(); j++)
			{
				//if the smile is already detected(the rectangle already drawn) don't draw again
				if(!isSmiledetected){
					Rect re = smile[j];
					rectangle(img, Point(re.x + r.x, re.y + r.y), Point(re.x + r.x + re.width - 2, re.y + r.y + re.height - 2), Scalar(0, 255, 0), 2, 8, 0);
				}
			}
			isSmiledetected=true;
		}
		}
		isFacedetected=true;
	}
	//cvui::imshow(setting, img);
}

int main(int argc, const char *argv[])
{
  Mat frame = cv::Mat(cv::Size(X, Y), CV_8UC3);
  Mat kernel; //for erosion
  int count = 1;
  int si = 1;
  int r = 0,g = 0,b = 0;

  // VideoCapture cap ("Images/chaplin.mp4");
  // if(!cap.isOpened()){
  //   cout << "Error opening video stream or file" << endl;
  //   return -1;
  // }

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
		 face(frame);

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
    int y_resize= y_canny+205;

    cvui::window(frame, x_resize, y_resize, 190,200, "Resize");

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

    int x_light= 260;
    int y_light= y_resize;
    cvui::window(frame, x_light, y_light, 190,200, "Lighten & Darken");
    cvui::text(frame, x_light+10, y_light+25, "Brightness trackbar");
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
    int y_erosion= y_light+205;
    cvui::window(frame, x_erosion, y_erosion, 230,200, "Dilation & Erosion");
    cvui::checkbox(frame, x_erosion+10, y_erosion+25 , "Dilation" , &use_dilation);
    cvui::checkbox(frame, x_erosion+90, y_erosion+25 , "Erosion", &use_erosion);
    //creating the trackbar for choosing the shape of kernel
    cvui::text(frame, x_erosion, y_erosion+50, "Shape: 0-Box  1-Cross  2-Ellipse");
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

    int x_draw = 260;
    int y_draw = 10;
    cvui::window(frame, x_draw, y_draw, 190,200, "Draw");
    cvui::checkbox(frame, x_draw+10, y_draw+25 , "Pencil", &use_draw);

    cvui::text(frame, x_draw+10, y_draw+55 , "R:");
    cvui::trackbar(frame, x_draw+20, y_draw+35, 150, &r, 0,255);
    cvui::text(frame, x_draw+10, y_draw+100 , "G:");
    cvui::trackbar(frame, x_draw+20, y_draw+78, 150, &g, 0,255);
    cvui::text(frame, x_draw+10, y_draw+145 , "B:");
    cvui::trackbar(frame, x_draw+20, y_draw+125, 150, &b, 0,255);
    setRGB((int)r,(int)g,(int)b);
    // cvui::text(frame, x_draw+10, y_draw+55 , "Colour:");
    // cvui::counter(frame, x_draw+10,  y_draw+70, &count);
    // showColor(count);
    // cvui::text(frame, x_draw+110, y_draw+75 , colour);

    cvui::text(frame, x_draw+10, y_draw+175 , "Size:");
    cvui::counter(frame, x_draw+60,  y_draw+170, &si);
    setSize(si);

    if (use_draw){
      cv::setMouseCallback("Image", CallBackFunc, &dst);
      // cv::imshow("Image", dst);
      // cv::waitKey(25);
    }

    /*----------Finish---------*/



    /*Face detection FUNCTION*/


    int x_face = 260;
    int y_face = y_erosion;
    cvui::window(frame, x_face, y_face, 190,200, "Face Detection");
    cvui::checkbox(frame, x_face+10, y_face+25, "Detect Face", &detect_face);
		cvui::checkbox(frame, x_face+10, y_face+65, "Detect Eyes", &detect_eyes);
		cvui::checkbox(frame, x_face+10, y_face+45, "Detect Smile", &detect_smile);
    cvui::text(frame, x_face+10, y_face+115 , "You have to detect face");
    cvui::text(frame, x_face+10, y_face+130 ,"to detect smile & eyes");

    //Make a clone image, will be useful to clear the face drawing
    Mat dummy=dst.clone();

    if (!cascade.load(face_cascade_name)|| !smile_cascade.load(smile_cascade_name) || !eyes_cascade.load(eye_cascade_name))
	  { 
		  printf("--(!)Error loading\n");
		  exit(0);
  	};

        dummy.copyTo(dst);
				isFacedetected=false;
				isSmiledetected=false;
				isEyedetected=false;

        if(detect_face){
          detectAndDraw(dst);
        }

    /*----------Finish---------*/


    cvui::update();
    cv::imshow("Image", dst);
    cv::imshow(WINDOW_NAME, frame);

    // Check if ESC key was pressed
    if (cv::waitKey(20) == 27) {
      break;
    }

  }

  return 0;
}
