#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/ml/ml.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void detectAndDraw(Mat& img,
	CascadeClassifier& cascade,
	double scale);

String cascadeName = "./haarcascade_frontalface_alt.xml";
														 

bool isDetected = false;

int main(int argc, const char** argv)
{
	Mat image;
	CascadeClassifier cascade;
	double scale = 1.3;

	int c=1;
	
	switch (c)
	{
	case 1:
		image = imread("1.png");
		break;
	case 2:
		image = imread("2.jpg");
		break;
	case 3:
		image = imread("3.png");
		break;
	case 4:
		image = imread("4.jpg");
	}


	namedWindow("ĥƤǰ");
	cv::imshow("ĥƤǰ", image);

	if (!cascade.load(cascadeName))
	{
		cerr << "ERROR: Could not load classifier cascade" << endl;
		return 0;
	}

	if (!image.empty())
	{
		detectAndDraw(image, cascade, scale);
		if (isDetected == false)
		{
			Mat dst;

			int value1 = 3, value2 = 1;

			int dx = value1 * 5;    
			double fc = value1*12.5; 
			int p = 50;
			Mat temp1, temp2, temp3, temp4;

			
			bilateralFilter(image, temp1, dx, fc, fc);

			temp2 = (temp1 - image + 128);

			
			GaussianBlur(temp2, temp3, Size(2 * value2 - 1, 2 * value2 - 1), 0, 0);

			temp4 = image + 2 * temp3 - 255;

			dst = (image*(100 - p) + temp4*p) / 100;
			dst.copyTo(image);
		}
		namedWindow("ĥƤ��");
		cv::imshow("ĥƤ��", image);
		waitKey(0);
	}

	return 0;
}

void detectAndDraw(Mat& img,
	CascadeClassifier& cascade,
	double scale)
{
	int i = 0;
	double t = 0;
	vector<Rect> faces;
	const static Scalar colors[] = { CV_RGB(0,0,255),
		CV_RGB(0,128,255),
		CV_RGB(0,255,255),
		CV_RGB(0,255,0),
		CV_RGB(255,128,0),
		CV_RGB(255,255,0),
		CV_RGB(255,0,0),
		CV_RGB(255,0,255) };

	Mat gray, smallImg(cvRound(img.rows / scale), cvRound(img.cols / scale), CV_8UC1);

	cvtColor(img, gray, COLOR_BGR2GRAY);
	resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);
	equalizeHist(smallImg, smallImg);




	cascade.detectMultiScale(smallImg, faces,
		1.1, 2, 0
		
		
		| CASCADE_SCALE_IMAGE
		,
		Size(30, 30));

	for (vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++)
	{
		isDetected = true;
		Mat smallImgROI;
		vector<Rect> nestedObjects;
		Point center, left, right;
		Scalar color = colors[i % 8];
		int radius;
		center.x = cvRound((r->x + r->width*0.5)*scale);
		center.y = cvRound((r->y + r->height*0.5)*scale);
		radius = cvRound((r->width + r->height)*0.25*scale);


		left.x = center.x - radius;
		left.y = cvRound(center.y - radius*1.3);

		if (left.y<0)
		{
			left.y = 0;
		}

		right.x = center.x + radius;
		right.y = cvRound(center.y + radius*1.3);

		if (right.y > img.rows)
		{
			right.y = img.rows;
		}

		rectangle(img, left, right, Scalar(255, 0, 0));



		Mat roi = img(Range(left.y, right.y), Range(left.x, right.x));
		Mat dst;

		int value1 = 3, value2 = 1;

		int dx = value1 * 5;    
		double fc = value1*12.5; 
		int p = 50;
		Mat temp1, temp2, temp3, temp4;

		
		bilateralFilter(roi, temp1, dx, fc, fc);

		temp2 = (temp1 - roi + 128);

		
		GaussianBlur(temp2, temp3, Size(2 * value2 - 1, 2 * value2 - 1), 0, 0);

		temp4 = roi + 2 * temp3 - 255;

		dst = (roi*(100 - p) + temp4*p) / 100;


		dst.copyTo(roi);
	}
}