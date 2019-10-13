#include <iostream>
#include "opencv2/opencv.hpp"
#include<opencv2/highgui/highgui.hpp>

//仅识别红色大神符有较大的作用
using namespace std;
using namespace cv;

Mat src,binary;
vector<Mat> imgChannels;	//撕裂通道之后存入这三个通道
Mat RedMinBlue;		//红色通道减去蓝色通道，深化红色 好二值化


Mat dilatelement;
Mat Dilate;
int dilate_value = 7;


Mat erodelement;
Mat Erode;
int erode_value = 7;


Mat openelement;
Mat Open;
int open_value=11;

Mat closeelement;
Mat Close;
int close_value=12;



vector<vector<Point>> contours;	//外形 轮廓		//contours被定义成二维浮点型向量，这里面将来会存储找到的边界的（x,y）坐标
vector<Vec4i> hierarchy;	//等级制度	//vector<Vec4i>hierarchy是定义的层级。这个在找边界findcontours的时候会自动生成

//漫水填充
void floodfilled(int, void*);
int flfiP1 = 10, flfiP2 = 10, flfiS = 0;
//膨胀
void dilatetrack(int, void*);
//腐蚀
void erodetrack(int, void*);
//开
void opentrack(int, void*);
//闭
void closetrack(int, void*);

void contourstrack(int, void*);

//把矩形画上面
Mat drawing;
Scalar color = Scalar(255, 0, 0);


//trackbar的窗口，， 滑动条都放在这两个窗口
char Trackbarwin[] = "trackbarwin";
char Trackbarwin2[] = "trackbarwin2";


vector<Rect>boundRect(contours.size()), rect(1);


int main() {

	VideoCapture cap("/home/hlt/Desktop/task/1opencv/4.avi");
	if (cap.isOpened()) {
		cout << "isopened" << endl;
	}
	else {
		cout << "failed  s" << endl;
		return -1;
	}

	//namedWindow(Trackbarwin, WINDOW_NORMAL);
	//createTrackbar("flfiP1", Trackbarwin, &flfiP1, 255, floodfilled);
	//createTrackbar("flfiP2", Trackbarwin, &flfiP2, 255, floodfilled);
	//createTrackbar("flfiS", Trackbarwin, &flfiS, 255, floodfilled);

	namedWindow(Trackbarwin2, WINDOW_NORMAL);
	//腐蚀和膨胀没有用到
	//createTrackbar("dialte_value", Trackbarwin2, &dilate_value, 30, dilatetrack);
	//createTrackbar("erode_value", Trackbarwin2, &erode_value, 30, erodetrack);
	createTrackbar("open_value", Trackbarwin2, &open_value, 30, opentrack);
	createTrackbar("close_value", Trackbarwin2, &close_value, 30, closetrack);



	while (1) {
		cap.read(src);


		split(src, imgChannels);	//撕裂通道
		RedMinBlue = imgChannels.at(2) - imgChannels.at(1);	//红蓝通道做差 深化图像
		//imshow("redblue", RedMinBlue);

		threshold(RedMinBlue, binary,90,255,THRESH_BINARY);//二值化
		imshow("binary", binary);

		//此时已经得到二值图了

		//floodfilled(0, 0);
		//imshow("floodFill", binary);

		//下面就是自己组合了 

		//dilatetrack(0, 0);
		//imshow("Dilate",Dilate);

		//erodetrack(0, 0);
		//imshow("Erode",Erode);


		//经过多次实验之后发现只用先闭操作
		//作用是  连通区域    把最外面的空空的叶片连通起来

		//然后开操作   把流动灯条给去掉   因为开操作可以去除小物体

		//先闭后开  能把一个灯条单独出来   其他灯条（风叶）连成一团 
		closetrack(0, 0);
		imshow("Close", Close);

		opentrack(0, 0);
		imshow("Open", Open);	//此时下一个要击中的面积较小，其他连成一块
		
		//寻找轮廓并画图
		contourstrack(0,0);

		waitKey(100);
	}
		
	waitKey(0);
	return 0;
}


//漫水 没用到
void floodfilled(int, void*) {
	floodFill(binary, Point(flfiP1,flfiP2), Scalar(flfiS));
	return;
}

//下面是四种形态学操作，，可以自己选择 先谁后谁 
void dilatetrack(int, void*){

	dilatelement = getStructuringElement(MORPH_RECT, Size(dilate_value,dilate_value));
	morphologyEx(binary,Dilate, MORPH_DILATE, dilatelement);
	return;
}

void erodetrack(int, void*){

	erodelement = getStructuringElement(MORPH_RECT, Size(erode_value ,erode_value));
	morphologyEx(Dilate, Erode,MORPH_ERODE, erodelement);
	return;
}

void opentrack(int, void*){

	openelement = getStructuringElement(MORPH_RECT, Size(2 * open_value + 1, 2 * open_value + 1));
	morphologyEx(Close, Open, MORPH_OPEN, openelement);	//先闭后开
	//imshow(first, open);
	return;
}

void closetrack(int, void*){

	closeelement = getStructuringElement(MORPH_RECT, Size(2 * close_value + 1, 2 * close_value + 1));
	morphologyEx(binary, Close, MORPH_CLOSE, closeelement);	//把二值图闭操作
	//imshow(first, close);
	return;
}

//寻找轮廓并画图
void contourstrack(int, void*) {

	findContours(Open, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(-1,-1));

	vector<vector<Point>>contours_poly(contours.size());
	vector<Rect>rect_ploy(contours.size());
	vector<Point2f>ccs(contours.size());
	vector<float>radius(contours.size());

	for (int i = 0; i < contours.size(); i++) {
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		rect_ploy[i] = boundingRect(contours_poly[i]);
		minEnclosingCircle(contours_poly[i], ccs[i], radius[i]);
	}

	//在原图上话 所以复制原图到drawing
	src.copyTo(drawing);
	for (int i = 0; i < contours.size(); i++) {

		//除了要击打的那片风叶 其他风叶连成一块
		//所以如果面积很大，，就不画
		if (contourArea(contours[i]) > 2000) {
			continue;
		}
		Scalar color(255, 0, 0);
		rectangle(drawing, rect_ploy[i],color, 2, 8);
		circle(drawing, ccs[i], radius[i], color, 2, 8);
	}

	imshow("drawing", drawing);
	return;
}