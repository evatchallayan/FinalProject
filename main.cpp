#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ctime>
#include <sstream>
#include <stdlib.h>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/stitching.hpp"


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


bool stitch =false;
bool image1 = false;
bool image2 = false;
bool image3 = false;
bool image4 = false;

Stitcher::Mode mode = Stitcher::PANORAMA;
vector<Mat> imgs;





String colour = "Black";
int red, green, blue;
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

/**---------DIVISION COUNTER & MATRIXES------**/
int counterImages = 2;
bool divide = false;
Mat crop1;
Mat crop2;
Mat crop3;
Mat crop4;

  /**--------PANORAMA IMAGES----------**/


Mat img1= imread("Images/mark1.jpg");
if(img1.empty()){
cout<<"Image 1 is empty.";
return -1;
}

Mat img2= imread("Images/mark2.jpg");
if(img2.empty()){
cout<<"Image 2 is empty.";
return -1;
}

Mat img3= imread("Images/mark3.jpg");
if(img3.empty()){
cout<<"Image 3 is empty.";
return -1;
}

Mat img4= imread("Images/mark4.jpg");
if(img4.empty()){
cout<<"Image 4 is empty.";
return -1;
}

Mat img5= imread("Images/mark5.jpg");
if(img4.empty()){
cout<<"Image 5 is empty.";
return -1;
}


imgs.push_back(img1); 
imgs.push_back(img2); 
//imgs.push_back(img3); 
//imgs.push_back(img4);
imgs.push_back(img5); 

/**---------------------**/
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
    int y_resize= y_canny+220;

    cvui::window(frame, x_resize, y_resize, 200,200, "Resize");

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
    cvui::window(frame, x_light, y_light, 200,200, "Lighten & Darken");
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

    /*  PANORAMA BAR */

    int x_panorama= x_draw;
    int y_panorama= y_draw+220;
    cvui::window(frame, x_panorama, y_panorama, 190,100, "Panorama Stitching");
    
	

    cvui::checkbox(frame, x_panorama+10, y_panorama+50, "Stitch", &stitch);
	
	if(stitch){
		
	Mat pano;	
	Ptr<Stitcher> stitcher = Stitcher::create(mode);
	Stitcher::Status status = stitcher->stitch(imgs, pano);

	if (status != Stitcher::OK){
        	cvui::text(frame, 90, 250, "STATUS NOT OK");
   	}

	imwrite("Images/stitch.jpg", pano);
	Mat res = imread("Images/stitch.jpg");

	
	namedWindow("Result", CV_MINOR_VERSION);
	moveWindow("Result",frame.cols+500, 100+frame.rows);
	imshow("Result", pano);
	
			
	}else{
		cvDestroyWindow("Result");
	}
	
   /*----------Finish---------*/
  /* DIVISION BAR */
	
	int x_division = x_panorama;
    	int y_division = y_panorama+120;
    	cvui::window(frame, x_division, y_division, 190,200, "Division");
	cvui::checkbox(frame, x_division+10, y_division+50, "Division", &divide);
	cvui::counter(frame, x_division+10,y_division+90, &counterImages);


	if(divide){
	
		if(counterImages==2){
		cvDestroyWindow("image3");
		cvDestroyWindow("image4");
		crop1 = src (Range(0,src.rows), Range(0,(src.cols/2)+100));
		crop2 = src (Range(0,src.rows), Range(src.cols/2,src.cols));
		imwrite("Images/image1crop.jpg",crop1);
	 	imwrite("Images/image2crop.jpg",crop2);
		}

		if(counterImages==3){
		crop1 = src (Range(0,src.rows), Range(0,(src.cols/3)+100));
		crop2 = src (Range(0,src.rows), Range(src.cols/3,(2*(src.cols)/3))+100);
		crop3 = src (Range(0,src.rows), Range(2*(src.cols)/3,src.cols));
		imwrite("Images/image1crop.jpg",crop1);
	 	imwrite("Images/image2crop.jpg",crop2);
		imwrite("Images/image3crop.jpg",crop3);
		cvDestroyWindow("image4");
		}

		if(counterImages==4){
		crop1 = src (Range(0,src.rows), Range(0,(src.cols/4)+100));
		crop2 = src (Range(0,src.rows), Range(src.cols/4,(2*(src.cols)/4))+100);
		crop3 = src (Range(0,src.rows), Range(2*(src.cols)/4,(3*(src.cols)/4)+100));
		crop4 = src (Range(0,src.rows), Range(3*(src.cols)/4,src.cols));
		imwrite("Images/image1crop.jpg",crop1);
	 	imwrite("Images/image2crop.jpg",crop2);
		imwrite("Images/image3crop.jpg",crop3);
		imwrite("Images/image4crop.jpg",crop3);
		}

		if(counterImages<2 || counterImages>4){
		   cvui::text(frame, 90, 250, "You can only get from 2 to 4 crops of the image.");
		}

	 	if(crop1.data){
		namedWindow("image1", CV_MINOR_VERSION);
		moveWindow("image1",frame.cols+200, 20);
		imshow("image1", crop1 );
		}

		if(crop2.data){
		namedWindow("image2", CV_MINOR_VERSION);
		moveWindow("image2",frame.cols+500, 20);
		imshow("image2", crop2);
		}

		if(crop3.data){
		namedWindow("image3", CV_MINOR_VERSION);
		moveWindow("image3",frame.cols+200, 100+frame.rows);
		imshow("image3", crop3);
		}

		if(crop4.data){
		namedWindow("image4", CV_MINOR_VERSION);
		moveWindow("image4",frame.cols+500, 100+frame.rows);
		imshow("image4", crop4);
		}


	}else{
          cvDestroyWindow("image1");
	  cvDestroyWindow("image2");
          cvDestroyWindow("image3");
          cvDestroyWindow("image4");
		
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
