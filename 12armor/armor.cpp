#include<iostream>
#include "opencv2/opencv.hpp"
#include<opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;



Mat src;
Mat img1;
vector<Mat> imgChannels;	//˺��ͨ��֮�����������ͨ��
Mat img2;
Mat binary;
Mat close;


Mat drawing;

vector<vector<Point>> contours;	//���� ����		//contours������ɶ�ά�����������������潫����洢�ҵ��ı߽�ģ�x,y������
vector<Vec4i> hierarchy;	//�ȼ��ƶ�	//vector<Vec4i>hierarchy�Ƕ���Ĳ㼶��������ұ߽�findcontours��ʱ����Զ�����


vector<Rect> Rects;

vector<Rect> findRight(vector<Rect>  rects);

void ReduceColor(Mat src, Mat dst,double bright)
{
	int val;
	for (int i = 0; i < src.rows; i++)
	{
		Vec3b* p1 = src.ptr<Vec3b>(i);//�е�ָ��
		Vec3b* p2 = dst.ptr<Vec3b>(i);
		for (int j = 0; j < src.cols; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				//ÿ�����ص�ÿ��ͨ����ֵ���������Ա任
				val = (int)(p1[j][k] + bright);
				if (val < 0)
					val = 0;
				if (val > 255)
					val = 255;
				p2[j][k] = val;//��i�еĵ�j�е�kͨ��
			}
		}
	}
}


int main() {
	VideoCapture cap("/home/hlt/Desktop/armor/armor.mp4");
	if (cap.isOpened()) {
		cout << "isopened" << endl;
	}
	else {
		cout << "failed" << endl;
		return -1;
	}




	while (1) {
		cap.read(src);

		img1=src.clone();	//û���ȿ�¡ֱ�ӱ���ÿ��Ԫ�� �˷��˴��ʱ��
		//imshow("src", src);
		
		ReduceColor(src, img1, -100);
		//imshow("img1", img1);

		split(img1, imgChannels);	//˺��ͨ��
		img2 = imgChannels.at(1);
		//imshow("img2", img2);

		threshold(img2, binary, 25, 255, THRESH_BINARY);//��ֵ��
		//imshow("binary", binary);

		dilate(binary, close, Mat(), Point(-1, -1), 3);   //ͼ������
		erode(close, close, Mat(), Point(-1, -1), 1);  //ͼ��ʴ���������ڸ�ʴ���ڱ�����
		//imshow("close", close);


		findContours(close, contours, RETR_CCOMP, CHAIN_APPROX_SIMPLE); //�ڶ�ֵͼ����Ѱ������
		vector<vector<Point>>contours_poly(contours.size());
		vector<Rect>rect_ploy(contours.size());

		for (int i = 0; i < contours.size(); i++) {
			approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
			rect_ploy[i] = boundingRect(contours_poly[i]);
		}

		src.copyTo(drawing);//��¡
		for (int i = 0; i < contours.size(); i++) {

			if (contourArea(contours[i]) > 300) {
				continue;
			}
			Scalar color(0, 0, 255);
			rectangle(drawing, rect_ploy[i], color, 2, 8);
		}

		//Rects = findRight(rect_ploy);
		//Scalar color(255, 0, 0);
		//for (int i = 0; i < Rects.size(); i++) {
		//	rectangle(drawing, Rects[i], color, 2, 8);
		//}


		imshow("drawing", drawing);
		waitKey(5);
	}

	waitKey(0);
	return 0;
}

vector<Rect> findRight(vector<Rect>  rects) {
	vector<Rect> rightRect;
	Rect temp;
	rightRect.clear();
	int h, w;
	if (rects.size() < 2) {
		return rightRect;
	}

	if (rects.size() < 6) {
		return rightRect;
	}

	for (int i = 0; i < rects.size() - 1; i++) {
		for (int j = i + 1; j < rects.size(); j++) {
			if ((rects[i].width - rects[j].width) < 4 &&
				(rects[i].height - rects[j].height) < 4) {

				h = (rects[i].height + rects[j].height) / 2;
				w = (rects[i].width + rects[j].width) / 2;

				temp.height = h;
				temp.width = w;

				rightRect.push_back(temp);
			}


		}

	}

	return rightRect;
}

