#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ctime>

#define CVUI_IMPLEMENTATION
#include "cvui-2.7.0/cvui.h"

#define WINDOW_NAME	"Dilation Erosion"
using namespace cv;
using namespace std;

int main(int argc, const char *argv[]) {
	Mat source = imread("Images/lena.jpg", IMREAD_COLOR);
	Mat dest = source.clone();

	//Size of the kernel for Erode and Clone
	int siz = 0;
	// Size of trackbars
	int width = 300;
	//shape of kernel
	int	shape_type=0;
	//for saving the image
	int isSaved = 0;

	//for eroding or dilating
	bool use_dilation = true;

	//Frame in which we'll put settings
	Mat frame = cv::Mat(cv::Size(400, 300), CV_8UC3);
	//the kernel
	Mat kernel;

	//check if the image is open successfully
	if (!source.data)
	{
		cout << "Empty data" << endl;
		return -1;
	}

	//define windows
	String nom = "Image";
	namedWindow(WINDOW_NAME, WINDOW_AUTOSIZE);
	namedWindow(nom);
	//moveWindow(nom, 80 + frame.cols, 0);
	moveWindow(nom, 80 + frame.rows,0);

	//init cvui
	cvui::init(WINDOW_NAME,30);

	while (true) {
		frame = cv::Scalar(49, 52, 49);
		//computing the kernel using the size
		if (shape_type == 0) {
			kernel = getStructuringElement(MORPH_RECT, Size(2 * siz + 1, 2 * siz + 1));
		}
		else if(shape_type==1) {
			kernel = getStructuringElement(MORPH_CROSS, Size(2 * siz + 1, 2 * siz + 1));
		}
		else if(shape_type==2){
			kernel = getStructuringElement(MORPH_ELLIPSE, Size(2 * siz + 1, 2 * siz + 1));
		}
		// cv::Mat, x, y, width, height, padding
		cvui::beginColumn(frame, 20, 20, 100, 200, 10);

		if (use_dilation) {
			dilate(source, dest, kernel);
		}
		else {
			erode(source, dest, kernel);
		}

		cvui::space(5);
		cvui::checkbox(frame, 25, 10, "Dilation (if unchecked we apply erosion)", &use_dilation);

		//creating the trackbar for choosing the shape of kernel
		cvui::text("Shape: 0=Rectangular box, 1=Cross & 2=Ellipse");
		cvui::trackbar(width, &shape_type, 0, 2);
		//creating the trackbar for reading the size
		cvui::text("Size of kernel(2n+1)");
		cvui::trackbar(width, &siz, 0, 21);

		imshow(nom, dest);
		if (isSaved) cvui::printf(frame, 100, 195, 0.5, 0x00ff00, "Image saved");
		if (cvui::button("&Save"))
		{
			if (use_dilation) {
				imwrite("Images/dilate.jpg", dest);
			}
			else {
				imwrite("Images/erode.jpg", dest);
			}
			isSaved = 1;
		}
		if (cvui::button("&Quit ")) {
			break;
		}

		cvui::endColumn();

		cvui::update();
		imshow(WINDOW_NAME, frame);

		if (waitKey(30) == 27) {
			break;
		}
	}

	return 0;
}
