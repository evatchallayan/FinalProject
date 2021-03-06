#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ctime>

#define CVUI_IMPLEMENTATION
#include "cvui-2.7.0/cvui.h"

#define WINDOW_NAME	"Trackbar and columns"
using namespace cv;
using namespace std;



int main(int argc, const char *argv[])
{

	Mat frame = cv::Mat(cv::Size(450, 650), CV_8UC3);
	double bValue = 0, cValue = 1;
	int isSaved = 0;
  Mat src, dst;
  src = imread("Images/van_gogh.jpg", IMREAD_COLOR);

	if(!src.data)
	{
		cout << "Empty data" << endl;
		return -1;
	}

	namedWindow("image", CV_MINOR_VERSION);
	moveWindow("image",60 + frame.cols, 80);
	// Size of trackbars
	int width = 400;

	cvui::init(WINDOW_NAME, 20);

	while (true) {
		frame = cv::Scalar(49, 52, 49);

		cvui::beginColumn(frame, 20, 20, -1, -1, 6);


			cvui::text("brightness trackbar");
			cvui::trackbar(width, &bValue, -255., 255., 1, "%.1Lf", cvui::TRACKBAR_DISCRETE, 1.);
			cvui::space(5);
			cvui::text("COntrast trackbar");
			cvui::trackbar(width, &cValue, 0., 6., 1, "%.2Lf", cvui::TRACKBAR_DISCRETE, 0.01);
			cvui::space(5);


      src.convertTo(dst,-1,cValue,bValue);
      imshow("image",dst);


			if (isSaved) cvui::printf(frame,100, 182, 0.5, 0x00ff00,"Image saved");
			if(cvui::button("&Save"))
			{
			 	imwrite("Images/ligh_dark_img.jpg",dst);
			 	isSaved = 1;
		 	}
			if (cvui::button("&Quit ")) {
				break;
			}
      cvui::endColumn();
		cvui::update();

		cv::imshow(WINDOW_NAME, frame);
	}

	return 0;
}
