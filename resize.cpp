#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ctime>
#include <sstream>

#define CVUI_IMPLEMENTATION
#include "cvui-2.7.0/cvui.h"

#define WINDOW_NAME	"Resize Image"
using namespace cv;
using namespace std;

int timeVar = 50;

void CheckSave(int* isSaved, Mat frame, Mat dst)
{
	if (*isSaved)
	{
		timeVar = timeVar -1;
		cvui::printf(frame,100, 186, 0.5, 0x00ff00,"Image saved");
		if(timeVar == 0) *isSaved = 0;
	}
	if (cvui::button("&Save"))
	{
		imwrite("Images/resize.jpg",dst);
		*isSaved = 1;
		timeVar = 50;
	}
}

int main(int argc, const char *argv[])
{

	Mat frame = cv::Mat(cv::Size(450, 650), CV_8UC3);
  Mat src, dst;
  src = imread("Images/van_gogh.jpg", IMREAD_COLOR);

	double vValue = 1;
	double hValue = 1;
	bool keepProportion = true;
	int vSize, hSize;
	int isSaved = 0;
	std::ostringstream size;
  if(!src.data)
  {
    cout << "Empty data" << endl;
    return -1;
  }

	namedWindow("image", CV_MINOR_VERSION);
	moveWindow("image",60 + frame.cols, 80);
	// Size of trackbars2
	int width = 400;
	cvui::init(WINDOW_NAME, 20);

	while (true) {
		frame = cv::Scalar(49, 52, 49);

		cvui::beginColumn(frame, 20, 20, -1, -1, 6);

		cvui::checkbox(frame, 25, 10, "Keep Proportion", &keepProportion);
		cvui::space(5);

		cvui::text("Vertical");
		cvui::trackbar(width, &vValue, 0.2, 3., 1, "%.3Lf", cvui::TRACKBAR_DISCRETE, 0.001);

		cvui::text("Horizontal");
		cvui::trackbar(width, &hValue, 0.2, 3., 1, "%.3Lf", cvui::TRACKBAR_DISCRETE, 0.001);

		if(keepProportion){
			resize(src, dst, Size(), vValue, vValue, INTER_CUBIC);
			hSize = dst.cols;
			vSize = dst.rows;
			// cout <<  "H: " << hSize << " V: "<< vSize << endl	;
			size << "H: " << hSize << " V: "<< vSize;

		}
		else{
			resize(src, dst, Size(), hValue, vValue, INTER_CUBIC);
			hSize = dst.cols;
			vSize = dst.rows;
			// cout << "H: " << hSize << " V: "<< vSize << endl;
			size << "H: " << hSize << " V: "<< vSize;

		}

    imshow("image",dst);

		cvui::text(size.str());
		size.clear();
		size.str("");

		CheckSave(&isSaved, frame, dst);

		if (cvui::button("&Quit ")) {
			break;
		}
    cvui::endColumn();
    cvui::update();

		cv::imshow(WINDOW_NAME, frame);
	}

	return 0;
}
