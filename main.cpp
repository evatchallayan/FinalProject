#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ctime>

#define CVUI_IMPLEMENTATION
#include "cvui.h"

#define WINDOW_NAME	"Operations"
#define X 600
#define Y 1000

using namespace cv;
using namespace std;


int main(int argc, const char *argv[])
{
    Mat frame = cv::Mat(cv::Size(X, Y), CV_8UC3);
    Mat src, dst;
    Mat kernel; //for erosion
    src = imread("mark.jpg", IMREAD_COLOR);

    if(!src.data)
    {
        cout << "Empty data" << endl;
        exit(0) ;
    }


    int low_threshold = 50, high_threshold = 150;
    int width = 400;

    bool use_canny = false;
    bool use_dilation = false;
    bool use_erosion = false;
    bool isSaved = false;
    //shape of kernel for erosion
    int	shape_type=0;
    //Size of the kernel for Erode and Clone
    int siz = 0;


    cvui::init(WINDOW_NAME);

    while (true) {
        frame = cv::Scalar(49, 52, 49);

        //canny_edge(src,dst, frame);

        if (cvui::button(frame, X-80, 10, "Quit")) {
            break;
        }
        if (cvui::button(frame, X-80, 40, "Save")) {
            imwrite("canny_edge.jpg",dst);
            isSaved = true;
        }
       // if (isSaved) cvui::printf(frame,X-100, 50, 0.4, 0x00ff00,"Image saved");



       /*  CANNY EDGE BAR */

        int x_canny=10;
        int y_canny=10;
        cvui::window(frame, x_canny, y_canny, 400,200, "Canny");

        cvui::checkbox(frame, x_canny+10, y_canny+25 , "Use Canny Edge", &use_canny);

        cvui::trackbar(frame, x_canny, y_canny+50, 150, &low_threshold, 5, 150);
        cvui::trackbar(frame, x_canny, y_canny+100, 150, &high_threshold, 80, 300);

        if (use_canny) {
            cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);
            cv::Canny(dst, dst, low_threshold, high_threshold, 3);
        }
        else {
            src.copyTo(dst);
        }

        /*----------Finish---------*/



        /*  RISE BAR */


        int x_resize= x_canny;
        int y_resize= y_canny+220;
        cvui::window(frame, x_resize, y_resize, 400,200, "Resize");

        /*----------Finish---------*/


        /*  LIGHTEN BAR */

        int x_light= x_canny;
        int y_light= y_resize+220;
        cvui::window(frame, x_light, y_light, 400,200, "Lighten & Darken");
        cvui::text(frame, x_light+10, y_light+25, "brightness trackbar");
        cvui::trackbar(frame, x_light, y_light+50, width, &bValue, -225.,255.);
       // cvui::trackbar(width, &bValue, -255., 255., 1, "%.1Lf", cvui::TRACKBAR_DISCRETE, 1.);
       // cvui::space(5);
        cvui::text(frame, x_light+10, y_light+120, "COntrast trackbar");
        cvui::trackbar(frame, x_light, y_light+150, width, &cValue,0.,6.);

        //cvui::trackbar(width, &cValue, 0., 6., 1, "%.2Lf", cvui::TRACKBAR_DISCRETE, 0.01);
       // cvui::space(5);

        dst.convertTo(dst,-1,cValue,bValue);


        /*----------Finish---------*/





        /*  EROSION BAR */

        int x_erosion= x_canny;
        int y_erosion= y_light+220;
        cvui::window(frame, x_erosion, y_erosion, 400,200, "dilation erosion");
        cvui::checkbox(frame, x_erosion+10, y_erosion+25 , "Dilation (if unchecked we apply erosion)", &use_dilation);
        //creating the trackbar for choosing the shape of kernel
        cvui::text(frame, x_erosion, y_erosion+50, "Shape: 0=Rectangular box, 1=Cross & 2=Ellipse");
        cvui::trackbar(frame, x_erosion, y_erosion+60, 150, &shape_type, 0, 2);
        cvui::checkbox(frame, x_erosion+10, y_erosion+110 , "Erosion", &use_erosion);
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

        if (use_dilation) {
            dilate(dst, dst, kernel);
        }
        else if(use_erosion){
            erode(dst, dst, kernel);
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
