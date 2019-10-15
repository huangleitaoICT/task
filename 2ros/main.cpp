#include<iostream>
#include "opencv2/opencv.hpp"
#include<opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;

Mat src;
Mat img1;
vector<Mat> imgChannels;	//撕裂通道之后存入这三个通道
Mat img2;
Mat binary;
Mat close;

vector<vector<Point>> contours;	//外形 轮廓		//contours被定义成二维浮点型向量，这里面将来会存储找到的边界的（x,y）坐标
vector<Vec4i> hierarchy;	//等级制度	//vector<Vec4i>hierarchy是定义的层级。这个在找边界findcontours的时候会自动生成


int main() {
	VideoCapture cap("C:\\Users\\诺言\\Desktop\\water.avi");
	if (cap.isOpened()) {
		cout << "isopened" << endl;
	}
	else {
		cout << "failed" << endl;
		return -1;
	}




	while (1) {
		cap.read(src);

	

		split(src, imgChannels);	//撕裂通道
		img1 = imgChannels.at(2) - imgChannels.at(1);	//红蓝通道做差 深化图像
		//imshow("redblue", RedMinBlue);

		threshold(img1, binary, 60, 255, THRESH_BINARY);//二值化
		imshow("binary", binary);


		dilate(binary, close, Mat(), Point(-1, -1), 5);   //图像膨胀
		erode(close, close, Mat(), Point(-1, -1), 3);  //图像腐蚀，先膨胀在腐蚀属于闭运算

		dilate(close, close, Mat(), Point(-1, -1), 5);   //图像膨胀
		erode(close, close, Mat(), Point(-1, -1), 3);  //图像腐蚀，先膨胀在腐蚀属于闭运算

		imshow("close", close);

		findContours(close, contours, RETR_CCOMP, CHAIN_APPROX_SIMPLE); //在二值图像中寻找轮廓
		vector<vector<Point>>contours_poly(contours.size());
		vector<Rect>rect_ploy(contours.size());

		for (int i = 0; i < contours.size(); i++) {
			approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
			rect_ploy[i] = boundingRect(contours_poly[i]);
		}

		for (int i = 0; i < contours.size(); i++) {

			if (contourArea(contours[i]) < 200) {
				continue;
			}
			Scalar color(0, 0, 255);
			rectangle(src, rect_ploy[i], color, 2, 8);
		}



		imshow("stc", src);
		waitKey(5);
	}

	waitKey(0);
	return 0;
}