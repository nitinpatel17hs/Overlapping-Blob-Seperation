#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include<iostream>
#include<stdio.h>
#include<math.h>

using namespace std;
using namespace cv;

//        /home/patel/Dewinter/Image Gallery/SG IRON/SG Iron (11).jpg

int thresh = 100, nod_thresh = 70;
int max_thresh = 255; RNG rng(12345);
Mat src = imread("GJS-600-7_3.jpg"), cloneimg, new_image, src_gray, binary, gray, canny_out, src2, dst;


Mat makebinary(Mat imageBinary, int thresh)
{

	Mat result(imageBinary.size(), CV_8UC1);


	for (int i = 0; i < imageBinary.rows; i++)
	{
		for (int j = 0; j < imageBinary.cols; j++)
		{
			if (imageBinary.at<uchar>(i, j) > thresh)

				result.at<uchar>(i, j) = 0;    //Make pixel black

			else

				result.at<uchar>(i, j) = 255;  //Make pixel white

		}

	}
	return result;
}

int sgn1(int a, int b)
{
	//int a,b;
	if (a - b < 0)
		return -1;
	if (a - b > 0)
		return 1;
	else
		return 0;
}



int main()
{


	Mat img;
	img = src.clone();

	if (img.empty())
	{
		cout << "Could not open image..." << endl;
		return -1;
	}
	//namedWindow("Image", WINDOW_NORMAL);
	namedWindow("Binary", WINDOW_NORMAL);
	resizeWindow("Binary", 600, 600);
	/*namedWindow("canny_out", WINDOW_AUTOSIZE);
	namedWindow("Final", WINDOW_NORMAL);

	resizeWindow("Image", 600, 600);

	resizeWindow("Final", 600, 600);*/

	Mat cloneBinary = src.clone();

	createTrackbar("Threshold", "Binary", &thresh, 255);

	while (1)
	{

		cvtColor(cloneBinary, gray, COLOR_BGR2GRAY);
		binary = makebinary(gray, thresh);

		imshow("Binary", binary);
		char a = waitKey(33);

		if (a == 'c' || a == 'C')
			break;

		if (!src.data)
		{
			return -1;
		}
	}

	GaussianBlur(binary, binary, Size(5, 5), 0, 0);



	Mat canny_out;

	Canny(binary, canny_out, 100, thresh * 2, 3);

	//imshow("canny_out", canny_out);

	//imshow("gray", gray);

	//Mat element = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));

	vector<vector<Point> > contours;

	findContours(canny_out, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<Point2f>centers1(contours.size());
	vector<float>radius1(contours.size());
	Mat drawing = Mat::zeros(canny_out.size(), CV_8UC1);
	vector<vector<Point> >hull(contours.size());
	vector<Point> corners;
	for (int i = 0;i < contours.size();i++)
	{
		if ((arcLength(contours[i], 1) > 50))
			drawContours(src, contours, (int)i, Scalar(0, 255, 255), 4);
		convexHull(contours[i], hull[i]);
		minEnclosingCircle(contours[i], centers1[i], radius1[i]);
		double r = (contourArea(contours[i]) / contourArea(hull[i]));
		//cout<<i<<" "<<r<<endl;
		if ((arcLength(contours[i], 1) > 150) && (r < 0.94445) && (r >= 0.1))
		{

			//drawContours( src, hull, (int)i, Scalar(0,255,255) , 4 );

			vector<Point> pts;
			vector<Point> pts1;
			vector<double> distances;
			for (int j = 0;j < contours[i].size();j++)
			{
				double dis, dis_min = 100000000;
				Point P1, P2;
				//Point P1 = contours[i][j];

				for (int k = 0;k < hull[i].size();k++)
				{
					//P2=P1;
					dis = cv::norm(contours[i][j] - hull[i][k]);
					if (dis <= dis_min)
					{
						//dis2 = dis1;
						dis_min = dis;
						P1 = contours[i][j];
						P2 = hull[i][k];
					}
				}
				pts.push_back(P1);
				pts1.push_back(P2);
				distances.push_back(dis_min);


			}

			double dis1 = 0, dis2 = 0;
			Point P_new1 = Point(0, 0), P_new2, P_1_hull, P_2_hull;
			int T;
			for (int j = 0;j < distances.size();j++)
			{
				//cout<<distances[j]<<endl;
				if (distances[j] > dis1)
				{
					dis1 = distances[j];
					P_new1 = pts[j];
					P_1_hull = pts1[j];
					T = j;
					//cout<<P_new1<<"YO"<<endl;
				}


			}

			/*for(int j=0;j<distances.size();j++)
			{
				if(((j>=T+distances.size()/2)||(j>=T-distances.size()/2))&&(distances[j]>dis2)&&(j!=T))
				{
					dis2 = distances[j];
					P_new2 = pts[j];
				}
			}*/

			int R;
			for (int j = distances.size() - 1;j >= 0;j--)
			{
				//avoiding the points near the previous point
				if ((j != T) && (j != T - 1) && (j != T + 1) && (j != T - 2) && (j != T + 2) && (j != T - 3) && (j != T + 3) && (j != T - 4) && (j != T + 4) && (j != T - 5) && (j != T + 5) && (j != T - 6) && (j != T + 6) && (j != T - 7) && (j != T + 7) && (j != T - 8) && (j != T + 8) && (j != T - 9) && (j != T + 9) && (j != T - 10) && (j != T + 10) && (j != T - 11) && (j != T + 11) && (j != T - 12) && (j != T + 12) && (j != T - 13) && (j != T + 13) && (j != T - 14) && (j != T + 14) && (j != T - 15) && (j != T + 15) && (j != T - 16) && (j != T + 16) && (distances[j] > dis2))
				{
					dis2 = distances[j];
					P_new2 = pts[j];
					P_2_hull = pts1[j];
					R = j;

				}
			}
			double l1, l2;
			l1 = cv::norm(P_1_hull - P_2_hull);
			l2 = cv::norm(P_new1 - P_new2);

			if ((abs(T - R) >= 50) && ((l2 / l1) <= 0.8))
			{


				//cout<<P_new1<<"  "<<P_new2<<endl;
				corners.push_back(P_new1);
				corners.push_back(P_new2);
				//cout<<distances.size()<<"  "<<contours[i].size()<<endl;
				//if ()
				
					circle(src, P_new1, 4, Scalar(0, 0, 255), -1, 8, 0);
					circle(src, P_new2, 4, Scalar(0, 0, 255), -1, 8, 0);
				
				line(src, P_new1, P_new2, Scalar(0, 0, 255), 2);
				//drawContours(src , contours, i , Scalar(0,255,255),4);
			}
		}


	}
	int ctr = 0;
	ostringstream str1;
	for (size_t idx = 0; idx < corners.size(); idx++)
	{
		ctr++;
		str1 << ctr;
		string str2 = str1.str();
		//cv::circle(src,corners.at(idx),4,Scalar(0,0,255),-1);
		//cout<<corners.at(idx).x<<endl;
		//putText(src, str2, corners.at(idx), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 1, 8, 0);
		str1.str("");
		str1.clear();

	}
	drawContours(drawing, contours, -1, Scalar(255), FILLED);
	namedWindow("DD", WINDOW_NORMAL);
	resizeWindow("DD", 600, 600);
	imshow("DD", drawing);

	namedWindow("Source", WINDOW_NORMAL);
	resizeWindow("Source", 800, 800);
	imshow("Source", src);

	waitKey(0);


	return(0);

}
