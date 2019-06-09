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

#define WINDOW_NAME	"Crop Image"
using namespace std;
using namespace cv;

Stitcher::Mode mode = Stitcher::PANORAMA;
vector<Mat> imgs;

Mat img1;
Mat img2;
Mat img3;
Mat img4;


int main(int argc, char* argv[]){

cvui::init(WINDOW_NAME, 20);
Mat frame = cv::Mat(cv::Size(450, 300), CV_8UC3);
bool stitch =false;
bool image1 = false;
bool image2 = false;
bool image3 = false;
bool image4 = false;

img1= imread("Images/eiffel1.jpeg");
if(img1.empty()){
cout<<"Image 1 is empty.";
return -1;
}
namedWindow("Photo1", CV_MINOR_VERSION);
moveWindow("Photo1",70 + frame.cols, 20);
imshow("Photo1", img1);


img2= imread("Images/eiffel2.jpeg");
if(img2.empty()){
cout<<"Image 2 is empty.";
return -1;
}
namedWindow("Photo2", CV_MINOR_VERSION);
moveWindow("Photo2",frame.cols+500, 20);
imshow("Photo2", img2);



img3= imread("Images/eiffel3.jpeg");
if(img3.empty()){
cout<<"Image 3 is empty.";
return -1;
}
namedWindow("Photo3", CV_MINOR_VERSION);
moveWindow("Photo3",frame.cols+70, 100+frame.rows);
imshow("Photo3", img3);


img4= imread("Images/eiffel4.jpeg");
if(img4.empty()){
cout<<"Image 4 is empty.";
return -1;
}

namedWindow("Photo4", CV_MINOR_VERSION);
moveWindow("Photo4",frame.cols+500, 100+frame.rows);
imshow("Photo4", img4);

imgs.push_back(img1); 
imgs.push_back(img2); 
imgs.push_back(img3); 
imgs.push_back(img4);


	while(true){
		frame = cv::Scalar(49, 52, 49);
  		cvui::beginColumn(frame, 20, 20, -1, -1, 6);

		cvui::checkbox(frame, 25, 50, "Stitch", &stitch);

		if(stitch){
			 		

			cvDestroyWindow("Photo1");
			cvDestroyWindow("Photo2");
			cvDestroyWindow("Photo3");
			cvDestroyWindow("Photo4");
			Mat pano;
			
			Ptr<Stitcher> stitcher = Stitcher::create(mode);
			Stitcher::Status status = stitcher->stitch(imgs, pano);

			if (status != Stitcher::OK){
        			cvui::text(frame, 90, 250, "STATUS NOT OK");
   		 	}

			imwrite("Images/stress.jpg", pano);
		 	Mat res = imread("Images/stitch.jpg");

	 	 	namedWindow("Result", CV_MINOR_VERSION);
	         	moveWindow("Result",100 + frame.cols, 80);
	         	imshow("Result", pano);

		}

	cvui::endColumn();
    	cvui::update();
	cv::imshow(WINDOW_NAME, frame);

	}


	return 0;
}










