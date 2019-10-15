//�������ϴ��뽫����opencv3.0.0�б�д
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgproc.hpp"
#include "iostream"
#include "omp.h"
using namespace cv;
using namespace std;

#define T_ANGLE_THRE 10
#define T_SIZE_THRE 5

void brightAdjust(Mat src, Mat dst, double dContrast, double dBright); //���ȵ��ں���
void getDiffImage(Mat src1, Mat src2, Mat dst, int nThre); //��ֵ��
vector<RotatedRect> armorDetect(vector<RotatedRect> vEllipse); //���װ��
void drawBox(RotatedRect box, Mat img); //���װ��

int main()
{

	VideoCapture cap0("C:\\Users\\ŵ��\\Desktop\\hhh.mp4");
	Mat frame0;

	Size imgSize;
	RotatedRect s;   //������ת����
	vector<RotatedRect> vEllipse; //������ת���ε����������ڴ洢���ֵ�Ŀ������
	vector<RotatedRect> vRlt;
	vector<RotatedRect> vArmor;
	bool bFlag = false;

	vector<vector<Point> > contour;

	cap0 >> frame0;
	imgSize = frame0.size();

	Mat rawImg = Mat(imgSize, CV_8UC3);

	Mat grayImage = Mat(imgSize, CV_8UC1);
	Mat rImage = Mat(imgSize, CV_8UC1);
	Mat gImage = Mat(imgSize, CV_8UC1);
	Mat bImage = Mat(imgSize, CV_8UC1);
	Mat binary = Mat(imgSize, CV_8UC1);
	Mat rlt = Mat(imgSize, CV_8UC1);

	while (1)
	{
		if (cap0.read(frame0))
		{
			brightAdjust(frame0, rawImg, 1, -120);  //ÿ������ÿ��ͨ����ֵ����ȥ120
			//imshow("rawImg", rawImg);
			Mat bgr[3];
			split(rawImg, bgr); //������ͨ��������ֵ����
			bImage = bgr[0];
			gImage = bgr[1];
			rImage = bgr[2];
			//�������Bֵ-Gֵ����25���򷵻صĶ�ֵͼ���ֵΪ255������Ϊ0
			getDiffImage(bImage, gImage, binary, 25);

			dilate(binary, grayImage, Mat(), Point(-1, -1), 3);   //ͼ������
			erode(grayImage, rlt, Mat(), Point(-1, -1), 1);  //ͼ��ʴ���������ڸ�ʴ���ڱ�����

			imshow("grayImage", grayImage);

			findContours(rlt, contour, RETR_CCOMP, CHAIN_APPROX_SIMPLE); //�ڶ�ֵͼ����Ѱ������
			for (int i = 0; i < contour.size(); i++)
			{
				if (contour[i].size() > 10&&contour[i].size()<30)  //�жϵ�ǰ�����Ƿ����10�����ص�
				{
					bFlag = true;   //�������10�������⵽Ŀ������
				  //���Ŀ�������Ϊ��Բ������һ����ת���Σ����ġ��Ƕȡ��ߴ磩
					//RotatedRect fitEllipse(InputArray points)
					s = fitEllipse(contour[i]);
					//for (int nI = 0; nI < 5; nI++)
					//{
					//	for (int nJ = 0; nJ < 5; nJ++)  //��������ת�������ĵ�Ϊ���ĵ�5*5�����ؿ�
					//	{
					//		if (s.center.y - 2 + nJ > 0 && s.center.y - 2 + nJ < 480 && s.center.x - 2 + nI > 0 && s.center.x - 2 + nI < 640)  //�жϸ������Ƿ�����Ч��λ��
					//		{
					//			Vec3b v3b = frame0.at<Vec3b>((int)(s.center.y - 2 + nJ), (int)(s.center.x - 2 + nI)); //��ȡ�����������ֵ
					//		   //�ж����ĵ��Ƿ�ӽ���ɫ
					//			if (v3b[0] < 200 || v3b[1] < 200 || v3b[2] < 200)
					//				bFlag = false;        //������Ĳ��ǰ�ɫ������Ŀ������
					//		}
					//	}
					//}

					if (bFlag)
					{
						vEllipse.push_back(s); //�����ֵ�Ŀ�걣��
					}
				}

			}
			//�����ӳ����������LED������ת���ε�vector���ҳ�װ�׵�λ�ã�����װ����ת���Σ�����vector������
			vRlt = armorDetect(vEllipse);
			for (unsigned int nI = 0; nI < vRlt.size(); nI++) //�ڵ�ǰͼ���б��װ�׵�λ��
				drawBox(vRlt[nI], frame0);
			imshow("src", frame0);
			if (waitKey(3) == 27)
			{
				break;
			}
			vEllipse.clear();
			vRlt.clear();
			vArmor.clear();
		}
		else
		{
			break;
		}
	}
	cap0.release();
	return 0;

}


void brightAdjust(Mat src, Mat dst, double dContrast, double dBright)
{
	int nVal;
	for (int nI = 0; nI < src.rows; nI++)
	{
		Vec3b* p1 = src.ptr<Vec3b>(nI);//�е�ָ��
		Vec3b* p2 = dst.ptr<Vec3b>(nI);
		for (int nJ = 0; nJ < src.cols; nJ++)
		{
			for (int nK = 0; nK < 3; nK++)
			{
				//ÿ�����ص�ÿ��ͨ����ֵ���������Ա任
				nVal = (int)(dContrast * p1[nJ][nK] + dBright);
				if (nVal < 0)
					nVal = 0;
				if (nVal > 255)
					nVal = 255;
				p2[nJ][nK] = nVal;//��i�еĵ�j�е�kͨ��
			}
		}
	}
}

void getDiffImage(Mat src1, Mat src2, Mat dst, int nThre)
{
	for (int nI = 0; nI < src1.rows; nI++)
	{
		uchar* pchar1 = src1.ptr<uchar>(nI);
		uchar* pchar2 = src2.ptr<uchar>(nI);
		uchar* pchar3 = dst.ptr<uchar>(nI);
		for (int nJ = 0; nJ < src1.cols; nJ++)
		{
			if (pchar1[nJ] - pchar2[nJ] > nThre) 
			{
				pchar3[nJ] = 255;
			}
			else
			{
				pchar3[nJ] = 0;
			}
		}
	}
}

vector<RotatedRect> armorDetect(vector<RotatedRect> vEllipse)
{
	vector<RotatedRect> vRlt;
	RotatedRect armor; //����װ���������ת����
	int nL, nW;
	double dAngle;
	vRlt.clear();
	if (vEllipse.size() < 2) //�����⵽����ת���θ���С��2����ֱ�ӷ���
		return vRlt;



	//�������ν���ƽ�У�����ת���εĽǶ�֮��ӽ�0�Ȼ�180��
	//������ת���εĿ�͸�Ӧ������
	for (unsigned int nI = 0; nI < vEllipse.size() - 1; nI++) //������������ת���εļн�
	{
		for (unsigned int nJ = nI + 1; nJ < vEllipse.size(); nJ++)
		{
			dAngle = abs(vEllipse[nI].angle - vEllipse[nJ].angle);
			while (dAngle > 180)
				dAngle -= 180;
			//�ж���������ת�����Ƿ���һ��װ�׵�����LED����
			if ((dAngle < T_ANGLE_THRE || 180 - dAngle < T_ANGLE_THRE) &&
				abs(vEllipse[nI].size.height - vEllipse[nJ].size.height) < 
				(vEllipse[nI].size.height + vEllipse[nJ].size.height) / T_SIZE_THRE && abs(vEllipse[nI].size.width - vEllipse[nJ].size.width) 
				< (vEllipse[nI].size.width + vEllipse[nJ].size.width) / T_SIZE_THRE)
			{
				armor.center.x = (vEllipse[nI].center.x + vEllipse[nJ].center.x) / 2; //װ�����ĵ�x���� 
				armor.center.y = (vEllipse[nI].center.y + vEllipse[nJ].center.y) / 2; //װ�����ĵ�y����
				armor.angle = (vEllipse[nI].angle + vEllipse[nJ].angle) / 2;   //װ��������ת���ε���ת�Ƕ�
				if (180 - dAngle < T_ANGLE_THRE)
					armor.angle += 90;
				nL = (vEllipse[nI].size.height + vEllipse[nJ].size.height) / 2; //װ�׵ĸ߶�
				nW = sqrt((vEllipse[nI].center.x - vEllipse[nJ].center.x) * (vEllipse[nI].center.x - vEllipse[nJ].center.x) + 
					(vEllipse[nI].center.y - vEllipse[nJ].center.y) * (vEllipse[nI].center.y - vEllipse[nJ].center.y)); //װ�׵Ŀ�ȵ�������LED������ת������������ľ���
				if (nL < nW)
				{
					armor.size.height = nL;
					armor.size.width = nW;
				}
				else
				{
					armor.size.height = nW;
					armor.size.width = nL;
				}
				vRlt.push_back(armor); //���ҳ���װ�׵���ת���α��浽vector
			}
		}
	}
	return vRlt;
}

void drawBox(RotatedRect box, Mat img)
{
	Point2f pt[4];
	int i;
	for (i = 0; i < 4; i++)
	{
		pt[i].x = 0;
		pt[i].y = 0;
	}
	box.points(pt); //�����ά���Ӷ��� 

	line(img, pt[0], pt[1], CV_RGB(255, 0, 0), 2, 8, 0);
	line(img, pt[1], pt[2], CV_RGB(255, 0, 0), 2, 8, 0);
	line(img, pt[2], pt[3], CV_RGB(255, 0, 0), 2, 8, 0);
	line(img, pt[3], pt[0], CV_RGB(255, 0, 0), 2, 8, 0);
}