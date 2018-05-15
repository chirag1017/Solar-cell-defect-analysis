// Solar cell defect analysis

#include<opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include "opencv2/imgproc/imgproc.hpp"
#include<iostream>
#include<conio.h>          
using namespace cv;
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
	cv::Mat imgOriginal;        // input image
	cv::Mat imgBlurred;         // intermediate blured image
	cv::Mat imgCanny;           // Canny edge image
	cv::Mat imgHough;           // Hough line transform image
	cv::Mat imgFinal;           // Finall output image

	imgOriginal = cv::imread("image2.bmp");          // open image
	imgFinal = imgOriginal.clone();

	if (imgOriginal.empty()) {                                  // if unable to open image
		std::cout << "error: image not read from file\n\n";     // show error message on command line
		_getch();
		return(0);                                              // and exit program
	}

	cv::GaussianBlur(imgOriginal, imgBlurred, cv::Size(5, 5), 1.5);   // Gaussian filter to blurr image and noise removal
	cv::Canny(imgOriginal, imgCanny, 15, 40);          // Canny edge detection           
	cvtColor(imgCanny, imgHough, CV_GRAY2BGR);

	float angle;
	//float linelength;
	float angles[1000] = {};
	//float linelengths[1000] = {};
	vector<Vec4i> lines;
	HoughLinesP(imgCanny, lines, 1, CV_PI / 1800, 40, 24, 10);  // Factoring the pixel unit to length unit in mm such that 
	for (size_t i = 0; i < lines.size(); i++)                   // 1mm = 12 pixels.Note: This is an approximate criterion
	{															// Refer the readme PDF in the project folder for factoring calculations
		Vec4i l = lines[i];
		angle = atan2(l[3] - l[1], l[2] - l[0]) * 180.0 / CV_PI;       //calculate line angle
		angles[i] = angle;
		//float ylength =  l[3] - l[1];
		//float xlength = l[2] - l[0];
		//linelength = sqrt(xlength*xlength + ylength*ylength);
		// linelengths[i] = linelength;

		if ((angle > -86 && angle < -5) || (angle > 5 && angle < 86))    //eliminating horizontal and vertical lines
		{

			line(imgHough, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 255, 255), 3, CV_AA);
			line(imgFinal, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 255, 255), 3, CV_AA);
		}

	}
	// declare windows
	cv::namedWindow("imgOriginal", CV_WINDOW_NORMAL);     // use CV_WINDOW_NORMAL which allows resizing the window
	cv::namedWindow("imgCanny", CV_WINDOW_NORMAL);
	cv::namedWindow("imgHough", CV_WINDOW_NORMAL);
	cv::namedWindow("imgFinal", CV_WINDOW_NORMAL);

	cv::imshow("imgOriginal", imgOriginal);     // show windows
	//cv::imshow("imgBlurred", imgBlurred);
	cv::imshow("imgCanny", imgCanny);
	cv::imshow("imgHough", imgHough);
	cv::imshow("imgFinal", imgFinal);

	cv::waitKey(0);                 // hold windows open until user presses a key
	return(0);
}



