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
  Mat src, dst;
  int isSaved = 0;
  bool use_canny = false;

  int low_threshold = 50, high_threshold = 150;
  src = imread("Images/Mark-Zuckerberg.jpg", IMREAD_COLOR);

	if(!src.data)
	{
		cout << "Empty data" << endl;
		return -1;
	}

	namedWindow("image", CV_MINOR_VERSION);
  moveWindow("image",60 + frame.cols, 0);
	// Size of trackbars
	int width = 400;

	cvui::init(WINDOW_NAME, 20);

	while (true) {
		frame = cv::Scalar(49, 52, 49);


		cvui::beginColumn(frame, 20, 20, -1, -1, 6);

    if (use_canny) {
        cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);
        cv::Canny(dst, dst, low_threshold, high_threshold, 3);
    }
    else {
        src.copyTo(dst);
    }

    cvui::space(5);
    cvui::checkbox(frame, 25, 10, "Use Canny Edge", &use_canny);
    cvui::trackbar(width, &low_threshold, 5, 150);
    cvui::trackbar(width, &high_threshold, 80, 300);


      imshow("image",dst);

      if (isSaved) cvui::printf(frame,100, 140, 0.5, 0x00ff00,"Image saved");
			if(cvui::button("&Save"))
			{
			 	imwrite("Images/canny_edge.jpg",dst);
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
