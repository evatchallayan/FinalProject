#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <stdio.h>

#define CVUI_IMPLEMENTATION
#include "cvui.h"

#define WINDOW_NAME "Face detection Settings"

using namespace std;
using namespace cv;

String face_cascade_name = "haarcascade_frontalface_alt.xml";//haarcascade_frontalface_default
String smile_cascade_name = "haarcascade_smile.xml";
String eye_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";//haarcascade_eye_tree_eyeglasses
String setting = "Face detection";

CascadeClassifier cascade;
CascadeClassifier smile_cascade;
CascadeClassifier eyes_cascade;

Mat imge = imread("friends.jpg", IMREAD_COLOR);

bool detect_eyes = true, detect_smile = true;

void detectAndDraw(Mat& img) {
	vector<Rect> faces;
	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	equalizeHist(gray, gray);

	//-- Detect faces
	cascade.detectMultiScale(gray, faces, 1.2, 5, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
	for (size_t i = 0; i < faces.size(); i++)
	{
		Rect r = faces[i];
		Scalar color = Scalar(255, 0, 0);
		rectangle(img, Point(r.x, r.y), Point(r.x + r.width-1, r.y + r.height-1), color, 3, 8, 0);

		Mat faceROI = gray(faces[i]);

		if (detect_eyes) {
			//-- In each face, detect eyes
			std::vector<Rect> eyes;
			eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 1, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
			//eyes_cascade.detectMultiScale(faceROI, eyes);
			for (size_t j = 0; j < eyes.size(); j++)
			{
				Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
				int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
				circle(img, center, radius, Scalar(0, 0, 255), 2, 8, 0);
			}
		}
		if (detect_smile) {
			//-- Detect Smile
			std::vector<Rect> smile;
			smile_cascade.detectMultiScale(faceROI, smile, 1.7, 20);
			for (int j = 0; j < smile.size(); j++)
			{
				Rect re = smile[j];
				rectangle(img, Point(re.x + r.x, re.y + r.y), Point(re.x + r.x + re.width - 2, re.y + r.y + re.height - 2), Scalar(0, 255, 0), 2, 8, 0);
			}
		}
	}

	cvui::imshow(setting, img);
}

int main(int argc, const char** argv)
{
	//Frame in which we'll put settings
	Mat frame = cv::Mat(cv::Size(400, 300), CV_8UC3);

	Mat img = imread("friends.jpg", IMREAD_COLOR);
	//namedWindow(WINDOW_NAME, WINDOW_AUTOSIZE);
	namedWindow(setting);
	namedWindow(WINDOW_NAME, CV_MINOR_VERSION);
	//moveWindow("image", 160 + img.rows, 80);
	cvui::init(WINDOW_NAME,20);
	if (img.empty()) {
		cout << "Error" << endl;
		return -1;
	}
	if (!cascade.load(face_cascade_name))
	{ 
		printf("--(!)Error loading\n");
		return -1; 
	};
	if (!smile_cascade.load(smile_cascade_name))
	{
		printf("--(!)Error loading\n");
		return -1;
	};
	if (!eyes_cascade.load(eye_cascade_name))
	{
		printf("--(!)Error loading\n");
		return -1;
	};

	detectAndDraw(img);
	
	while (true) {
		frame = cv::Scalar(49, 52, 49);

		cvui::beginColumn(frame, 20, 20, 100, 100, 10);
		cvui::text("Click for detect Smile or/and Eyes");

		cvui::checkbox(frame, 25, 35, "Eyes", &detect_eyes);
		cvui::checkbox(frame, 25, 55, "Smile", &detect_smile);

		cvui::endColumn();
		cvui::imshow(WINDOW_NAME, frame);
		//imshow(setting, imge);
		if (waitKey(20) == 27) {
			break;
		}
	}

	return 0;
}