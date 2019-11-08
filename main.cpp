#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


Mat grayscaling(Mat input, int height, int width) {
	Mat img_grayscale(height, width, CV_8UC1);

	//grayscaling
	for (int y = 0; y < height; y++) {

		uchar* pointer_input = input.ptr<uchar>(y);
		uchar* pointer_ouput = img_grayscale.ptr<uchar>(y);
		for (int x = 0; x < width; x++) {

			// 컬러영상의 경우 픽셀값 읽어오기
			uchar b = pointer_input[x * 3 + 0];
			uchar g = pointer_input[x * 3 + 1];
			uchar r = pointer_input[x * 3 + 2];

			//그레이스케일의 경우 픽셀값 저장하기
			pointer_ouput[x] = (r + g + b) / 3.0;
		}
	}
	return img_grayscale;
}

Mat binarycaling(Mat input, int height, int width) {
	Mat img_binaryscale(height, width, CV_8UC1);
	int threshold = 128;
	int intensity;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			intensity = input.at<uchar>(y, x);

			if (intensity > threshold) {
				img_binaryscale.at<uchar>(y, x) = uchar(255);
			}
			else {
				img_binaryscale.at<uchar>(y, x) = uchar(0);
			}
		}
	}

	return img_binaryscale;
}



int main()
{
	Mat img_color = imread("C:\\Users\\RyuJaesang\\Desktop\\IPTeamProject01\\triangle3.png", IMREAD_COLOR);
	int height = img_color.rows;
	int width = img_color.cols;


	Mat img_grayscale = grayscaling(img_color, height, width);
	Mat img_binaryscale = binarycaling(img_grayscale, height, width);
	

	/*
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			intensity = img_grayscale.at<uchar>(y, x);



		}
	}
	*/



	imshow("color", img_color);
	imshow("grayscale", img_grayscale);
	imshow("binaryscale", img_binaryscale);

	waitKey(0);


	return 0;
}






