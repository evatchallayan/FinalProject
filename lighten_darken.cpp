#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#define CVUI_IMPLEMENTATION
#include "cvui-2.7.0/cvui.h"

#define WINDOW_NAME	"Trackbar and columns"
using namespace cv;



int main(int argc, const char *argv[])
{

	double doubleValue1 = 0;
	Mat frame = cv::Mat(cv::Size(450, 650), CV_8UC3);
  Mat src, dst;
  src = imread("Images/van_gogh.jpg", IMREAD_COLOR);

	namedWindow("image", CV_MINOR_VERSION);
	// Size of trackbars
	int width = 400;
	int isSaved = 0;

	// Init cvui and tell it to use a value of 20 for cv::waitKey()
	// because we want to enable keyboard shortcut for
	// all components, e.g. button with label "&Quit".
	// If cvui has a value for waitKey, it will call
	// waitKey() automatically for us within cvui::update().
	cvui::init(WINDOW_NAME, 20);

	while (true) {
		frame = cv::Scalar(49, 52, 49);

		cvui::beginColumn(frame, 20, 20, -1, -1, 6);


			cvui::text("double trackbar, label %.1Lf, TRACKBAR_DISCRETE");
			cvui::trackbar(width, &doubleValue1, -255., 255., 1, "%.1Lf", cvui::TRACKBAR_DISCRETE, 1.);
			cvui::space(5);


      src.convertTo(dst,-1,1,doubleValue1);
      imshow("image",dst);

      if(cvui::button("&Save")){
        imwrite("images/ligh_dark_img.jpg",dst);
        isSaved = 1;
      }
			if(isSaved){
				cvui::printf(frame,100, 105, 0.5, 0x00ff00,"Image saved");
			}
			if (cvui::button("&Quit")) {
				break;
			}
      cvui::endColumn();


		// Since cvui::init() received a param regarding waitKey,
		// there is no need to call cv::waitKey() anymore. cvui::update()
		// will do it automatically.
		cvui::update();

		cv::imshow(WINDOW_NAME, frame);
	}

	return 0;
}
