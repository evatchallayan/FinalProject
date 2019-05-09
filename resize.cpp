#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ctime>
#include <iostream>

#define CVUI_IMPLEMENTATION
#include "cvui-2.7.0/cvui.h"

#define WINDOW_NAME	"Trackbar and columns"
using namespace cv;
using namespace std;



int main(int argc, const char *argv[])
{

	Mat frame = cv::Mat(cv::Size(450, 650), CV_8UC3);
  Mat src, dst;
  src = imread("Images/van_gogh.jpg", IMREAD_COLOR);

	double rValue = 1;
	int isSaved = 0;

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


		cvui::text("Resize rate");
		cvui::trackbar(width, &rValue, 0.2, 3., 1, "%.3Lf", cvui::TRACKBAR_DISCRETE, 0.001);


    resize(src, dst, Size(), rValue, rValue, INTER_CUBIC);
    imshow("image",dst);


		if (isSaved) cvui::printf(frame,100, 95, 0.5, 0x00ff00,"Image saved");
		if(cvui::button("&Save"))
		{
		 	imwrite("Images/resize.jpg",dst);
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
