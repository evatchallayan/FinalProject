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

Mat source;

Mat src, dst;
Stitcher::Mode mode = Stitcher::PANORAMA;


int main(int argc, char* argv[]){

Mat frame = cv::Mat(cv::Size(450, 300), CV_8UC3);

bool divide = false;
bool stitch = false;

Mat crop1;
Mat crop2;
Mat crop3;
Mat crop4;

	
cvui::init(WINDOW_NAME, 20);

int count = 2;


while(true){

	
	frame = cv::Scalar(49, 52, 49);
  	
	cvui::beginColumn(frame, 20, 20, -1, -1, 6);

	cvui::checkbox(frame, 25, 50, "Divide", &divide);

 	source = imread(argv[1]);

	cvui::counter(frame, 90, 50, &count);

	

	if(!source.data){
		cout<<"No image could be loaded";
		return -1;
	}




	namedWindow("Original", CV_MINOR_VERSION);
	moveWindow("Original",70 + frame.cols, 20);
	imshow("Original", source);

	


	if(divide){




	if(count==2){
	cvDestroyWindow("image3");
        cvDestroyWindow("image4");
	crop1 = source (Range(0,source.rows), Range(0,(source.cols/2)+100));
	crop2 = source (Range(0,source.rows), Range(source.cols/2,source.cols));
	imwrite("image1crop.jpg",crop1);
 	imwrite("image2crop.jpg",crop2);
	}

	if(count==3){
	crop1 = source (Range(0,source.rows), Range(0,(source.cols/3)+100));
	crop2 = source (Range(0,source.rows), Range(source.cols/3,(2*(source.cols)/3))+100);
	crop3 = source (Range(0,source.rows), Range(2*(source.cols)/3,source.cols));
	imwrite("image1crop.jpg",crop1);
 	imwrite("image2crop.jpg",crop2);
	imwrite("image3crop.jpg",crop3);
	cvDestroyWindow("image4");
	}

	if(count==4){
	crop1 = source (Range(0,source.rows), Range(0,(source.cols/4)+100));
	crop2 = source (Range(0,source.rows), Range(source.cols/4,(2*(source.cols)/4))+100);
	crop3 = source (Range(0,source.rows), Range(2*(source.cols)/4,(3*(source.cols)/4)+100));
	crop4 = source (Range(0,source.rows), Range(3*(source.cols)/4,source.cols));
	imwrite("image1crop.jpg",crop1);
 	imwrite("image2crop.jpg",crop2);
	imwrite("image3crop.jpg",crop3);
	imwrite("image4crop.jpg",crop3);
	}

	if(count<2 || count>4){
	   cvui::text(frame, 90, 250, "You can only get from 2 to 4 crops of the image.");
	}

 	if(crop1.data){
	namedWindow("Original", CV_MINOR_VERSION);
	imshow("Original", crop1 );
	}

	if(crop2.data){
	namedWindow("image2", CV_MINOR_VERSION);
	moveWindow("image2",frame.cols+500, 20);
	imshow("image2", crop2);
	}

	if(crop3.data){
	namedWindow("image3", CV_MINOR_VERSION);
	moveWindow("image3",frame.cols+70, 100+frame.rows);
	imshow("image3", crop3);
	}

	if(crop4.data){
	namedWindow("image4", CV_MINOR_VERSION);
	moveWindow("image4",frame.cols+500, 100+frame.rows);
	imshow("image4", crop4);
	}


	}else{
	  cvDestroyWindow("image2");
          cvDestroyWindow("image3");
          cvDestroyWindow("image4");
	}

	

	cvui::endColumn();
    	cvui::update();
	cv::imshow(WINDOW_NAME, frame);

	}
 

return 0;
}










