#include <opencv2/opencv.hpp>
#include <iostream>


using namespace cv;
using namespace std;

typedef struct {
	int row;
	int col;
} Dot;

typedef struct {
	Dot upside;
	Dot leftside;
} SideDetect;

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

SideDetect findsidepoints(Mat img_binaryscale, int height, int width) {

	SideDetect tempSidePoints;
		//upsideXY
	bool upsideDetect = false;
	int upsideX = 0;
	int upsideY = 0;
	int i, j;
	int intensity;

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			intensity = img_binaryscale.at<uchar>(i, j);
			if (intensity == 0) {
				upsideY = i;
				upsideX = j;
				upsideDetect = true;
			//	printf("intensity(%d, %d) is %d\n", upsideX, upsideY, intensity);
			}
			if (upsideDetect) {
				break;
			}
		}
		if (upsideDetect) {
			break;
		}

	}

	//leftsideXY
	bool leftsideDetect = false;
	int leftsideX = 0;
	int leftsideY = 0;
	int k, l;

	for (k = 0; k < width; k++) {
		for (l = 0; l < height; l++) {
			intensity = img_binaryscale.at<uchar>(l, k);
			if (intensity == 0) {
				leftsideY = l;
				leftsideX = k;
				leftsideDetect = true;
			//	printf("intensity(%d, %d) is %d\n", leftsideX, leftsideY, intensity);
			}
			if (leftsideDetect) {
				break;
			}
		}
		if (leftsideDetect) {
			break;
		}

	}
	tempSidePoints.upside.row = upsideY;
	tempSidePoints.upside.col = upsideX;
	tempSidePoints.leftside.row = leftsideY;
	tempSidePoints.leftside.col = leftsideX;

	return tempSidePoints;
}

void distinguish (Mat img_binaryscale, int height, SideDetect sidepoints) {
	SideDetect mid;
	Dot real_mid;

	mid.upside.col = (sidepoints.upside.col + sidepoints.leftside.col) / 2;
	//printf("up : %d \n", sidepoints.upside.col);	
	//printf("left : %d \n", sidepoints.leftside.col);
	//printf("mid : %d \n", mid.upside.col);
	mid.upside.row = (sidepoints.upside.row + sidepoints.leftside.row) / 2;

	int testY = 0;
	bool shapeDetect = false;
	for (testY; testY < height; testY++) {
		int intensity = img_binaryscale.at<uchar>(testY, mid.upside.col);
		if (intensity == 0) {
			shapeDetect = true;
		//	printf("testY is %d\n", testY);
		//	printf("%d\n", mid.upside.row);
		}
		if (shapeDetect) break;
	}

	if (abs(testY - mid.upside.row) < 4) {
		printf("Triangle \n");
	}
	else {
		printf("Circle \n");
	}
}

int main()
{

	String srcImageName_a = "C:\\input\\a.jpg";
	String srcImageName_b = "C:\\input\\b.jpg";
	String srcImageName_c = "C:\\input\\c.jpg";
	String srcImageName_d = "C:\\input\\d.jpg";

	Mat img_color_a = imread(srcImageName_a, IMREAD_COLOR);
	int height_a = img_color_a.rows;
	int width_a = img_color_a.cols; 
	Mat img_color_b = imread(srcImageName_b, IMREAD_COLOR);
	int height_b = img_color_b.rows;
	int width_b = img_color_b.cols;
	Mat img_color_c = imread(srcImageName_c, IMREAD_COLOR);
	int height_c = img_color_c.rows;
	int width_c = img_color_c.cols;
	Mat img_color_d = imread(srcImageName_d, IMREAD_COLOR);
	int height_d = img_color_d.rows;
	int width_d = img_color_d.cols;

	printf("Enter image :\n");
	cout << srcImageName_a << endl;
	cout << srcImageName_b << endl;
	cout << srcImageName_c << endl;
	cout << srcImageName_d << endl;
	printf("\n");

	Mat img_grayscale_a = grayscaling(img_color_a, height_a, width_a);
	Mat img_grayscale_b = grayscaling(img_color_b, height_b, width_b);
	Mat img_grayscale_c = grayscaling(img_color_c, height_c, width_c);
	Mat img_grayscale_d = grayscaling(img_color_d, height_d, width_d);

	Mat img_binaryscale_a = binarycaling(img_grayscale_a, height_a, width_a);
	Mat img_binaryscale_b = binarycaling(img_grayscale_b, height_b, width_b);
	Mat img_binaryscale_c = binarycaling(img_grayscale_c, height_c, width_c);
	Mat img_binaryscale_d = binarycaling(img_grayscale_d, height_d, width_d);

	//printf("IMP is done\n");
	//printf("height is %d \n", height_a);
	//printf("width is %d \n", width_a);

	SideDetect sideDetect_a = findsidepoints(img_binaryscale_a,height_a,width_a);
	SideDetect sideDetect_b = findsidepoints(img_binaryscale_b, height_b, width_b);
	SideDetect sideDetect_c = findsidepoints(img_binaryscale_c, height_c, width_c);
	SideDetect sideDetect_d = findsidepoints(img_binaryscale_d, height_d, width_d);


	printf("Result :\n");
	distinguish(img_binaryscale_a, height_a, sideDetect_a);
	distinguish(img_binaryscale_b, height_b, sideDetect_b);
	distinguish(img_binaryscale_c, height_c, sideDetect_c);
	distinguish(img_binaryscale_d, height_d, sideDetect_d);
	printf("\n");


	//imshow("color", img_color_a);
	//imshow("grayscale", img_grayscale_a);
	//imshow("binaryscale", img_binaryscale_a);

	waitKey(0);


	return 0;
}


