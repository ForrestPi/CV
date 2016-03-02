// Chapter7.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>  
#include<opencv2/core/core.hpp>  
#include<opencv2/highgui/highgui.hpp> 
#include<opencv2/imgproc/imgproc.hpp> 
#include "EdgeDetector.h"
#include "LineFinder.h"

#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	/*
	cv::Mat image = cv::imread("../images/road.jpg",0);
	if (!image.data)
	{
		cout<<"Cannot Load File!"<<endl;
		return -1;
	}
	// Compute Sobel
	EdgeDetector ed;
	ed.computeSobel(image);

	// Display the Sobel orientation
	cv::namedWindow("Sobel (orientation)");
	cv::imshow("Sobel (orientation)",ed.getSobelOrientationImage());
	cv::imwrite("ori.bmp",ed.getSobelOrientationImage());

	// Display the Sobel low threshold
	cv::namedWindow("Sobel (low threshold)");
	cv::imshow("Sobel (low threshold)",ed.getBinaryMap(125));

	// Display the Sobel high threshold
	cv::namedWindow("Sobel (high threshold)");
	cv::imshow("Sobel (high threshold)",ed.getBinaryMap(350));*/

	/*
	cv::Mat image = cv::imread("../images/road.jpg",0);
	if (!image.data)
	{
		cout<<"Cannot Load File!"<<endl;
		return -1;
	}
	// Compute Sobel
	EdgeDetector ed;
	ed.computeSobel(image);

	cv::Mat contours;
	cv::Canny(image,contours,125,350);
	cv::Mat contoursInv;
	cv::threshold(contours,contoursInv,128,255,cv::THRESH_BINARY_INV);
	cv::namedWindow("Canny Image");
	cv::imshow("Canny Image",contoursInv);

	// Hough tranform for line detection
	std::vector<cv::Vec2f> lines;
	cv::HoughLines(contours,lines,1,PI/180,80);

	// Draw the lines
	cv::Mat result(contours.rows,contours.cols,CV_8U,cv::Scalar(255));
	image.copyTo(result);

	std::cout << "Lines detected: " << lines.size() << std::endl;

	std::vector<cv::Vec2f>::const_iterator it= lines.begin();
	while (it!=lines.end()) {

		float rho= (*it)[0];   // first element is distance rho
		float theta= (*it)[1]; // second element is angle theta

		if (theta < PI/4. || theta > 3.*PI/4.) { // ~vertical line

			// point of intersection of the line with first row
			cv::Point pt1(rho/cos(theta),0);        
			// point of intersection of the line with last row
			cv::Point pt2((rho-result.rows*sin(theta))/cos(theta),result.rows);
			// draw a white line
			cv::line( result, pt1, pt2, cv::Scalar(255), 1); 

		} else { // ~horizontal line

			// point of intersection of the line with first column
			cv::Point pt1(0,rho/sin(theta));        
			// point of intersection of the line with last column
			cv::Point pt2(result.cols,(rho-result.cols*cos(theta))/sin(theta));
			// draw a white line
			cv::line( result, pt1, pt2, cv::Scalar(255), 1); 
		}

		std::cout << "line: (" << rho << "," << theta << ")\n"; 

		++it;
	}

	// Display the detected line image
	cv::namedWindow("Detected Lines with Hough");
	cv::imshow("Detected Lines with Hough",result);

	// Create LineFinder instance
	LineFinder ld;

	// Set probabilistic Hough parameters
	ld.setLineLengthAndGap(100,20);
	ld.setMinVote(80);

	// Detect lines
	std::vector<cv::Vec4i> li= ld.findLines(contours);
	ld.drawDetectedLines(image);
	cv::namedWindow("Detected Lines with HoughP");
	cv::imshow("Detected Lines with HoughP",image);

	std::vector<cv::Vec4i>::const_iterator it2= li.begin();
	while (it2!=li.end()) {

		std::cout << "(" << (*it2)[0] << ","<< (*it2)[1]<< ")-(" 
			<< (*it2)[2]<< "," << (*it2)[3] << ")" <<std::endl;

		++it2;
	}

	// Display one line
	image= cv::imread("../images/road.jpg",0);
	int n=0;
	cv::line(image, cv::Point(li[n][0],li[n][1]),cv::Point(li[n][2],li[n][3]),cv::Scalar(255),5);
	cv::namedWindow("One line of the Image");
	cv::imshow("One line of the Image",image);

	// Extract the contour pixels of the first detected line
	cv::Mat oneline(image.size(),CV_8U,cv::Scalar(0));
	cv::line(oneline, cv::Point(li[n][0],li[n][1]),cv::Point(li[n][2],li[n][3]),cv::Scalar(255),5);
	cv::bitwise_and(contours,oneline,oneline);
	cv::Mat onelineInv;
	cv::threshold(oneline,onelineInv,128,255,cv::THRESH_BINARY_INV);
	cv::namedWindow("One line");
	cv::imshow("One line",onelineInv);

	std::vector<cv::Point> points;

	// Iterate over the pixels to obtain all point positions
	for( int y = 0; y < oneline.rows; y++ ) {

		uchar* rowPtr = oneline.ptr<uchar>(y);

		for( int x = 0; x < oneline.cols; x++ ) {

			// if on a contour
			if (rowPtr[x]) {

				points.push_back(cv::Point(x,y));
			}
		}
	}

	// find the best fitting line
	cv::Vec4f line;
	cv::fitLine(cv::Mat(points),line,CV_DIST_L2,0,0.01,0.01);

	std::cout << "line: (" << line[0] << "," << line[1] << ")(" << line[2] << "," << line[3] << ")\n"; 

	int x0= line[2];
	int y0= line[3];
	int x1= x0-200*line[0];
	int y1= y0-200*line[1];
	image= cv::imread("../images/road.jpg",0);
	cv::line(image,cv::Point(x0,y0),cv::Point(x1,y1),cv::Scalar(0),3);
	cv::namedWindow("Estimated line");
	cv::imshow("Estimated line",image);

	// eliminate inconsistent lines
	ld.removeLinesOfInconsistentOrientations(ed.getOrientation(),0.4,0.1);

	// Display the detected line image
	image= cv::imread("../images/road.jpg",0);
	ld.drawDetectedLines(image);
	cv::namedWindow("Detected Lines (2)");
	cv::imshow("Detected Lines (2)",image);

	// Create a Hough accumulator
	cv::Mat acc(200,180,CV_8U,cv::Scalar(0));

	// Choose a point
	int x=50, y=30;

	// loop over all angles
	for (int i=0; i<180; i++) {

		double theta= i*PI/180.;

		// find corresponding rho value 
		double rho= x*cos(theta)+y*sin(theta);
		int j= static_cast<int>(rho+100.5);

		std::cout << i << "," << j << std::endl;

		// increment accumulator
		acc.at<uchar>(j,i)++;
	}

	cv::imwrite("hough1.bmp",acc*100);

	// Choose a second point
	x=30, y=10;

	// loop over all angles
	for (int i=0; i<180; i++) {

		double theta= i*PI/180.;
		double rho= x*cos(theta)+y*sin(theta);
		int j= static_cast<int>(rho+100.5);

		acc.at<uchar>(j,i)++;
	}

	cv::namedWindow("Hough Accumulator");
	cv::imshow("Hough Accumulator",acc*100);
	cv::imwrite("hough2.bmp",acc*100);*/
	
	/*
	// Detect circles
	cv::Mat image= cv::imread("../images/chariot.jpg",0);
	if (!image.data)
	{
		cout<<"Cannot Load File!"<<endl;
		return -1;
	}
	cv::GaussianBlur(image,image,cv::Size(5,5),1.5);
	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(image, circles, CV_HOUGH_GRADIENT, 
		2,   // accumulator resolution (size of the image / 2) 
		50,  // minimum distance between two circles
		200, // Canny high threshold 
		100, // minimum number of votes 
		25, 100); // min and max radius

	std::cout << "Circles: " << circles.size() << std::endl;

	// Draw the circles
	image= cv::imread("../images/chariot.jpg",0);
	if (!image.data)
	{
		cout<<"Cannot Load File!"<<endl;
		return -1;
	}
	std::vector<cv::Vec3f>::const_iterator itc= circles.begin();

	while (itc!=circles.end()) {

		cv::circle(image, 
			cv::Point((*itc)[0], (*itc)[1]), // circle centre
			(*itc)[2], // circle radius
			cv::Scalar(255), // color 
			2); // thickness

		++itc;	
	}

	cv::namedWindow("Detected Circles");
	cv::imshow("Detected Circles",image);

	image= cv::imread("../images/chariot.jpg",0);
	if (!image.data)
	{
		cout<<"Cannot Load File!"<<endl;
		return -1;
	}
	cv::GaussianBlur(image,image,cv::Size(3,3),1.5);
	cv::Mat contours;
	cv::Canny(image,contours,125,350);
	cv::Mat contoursInv;
	cv::threshold(contours,contoursInv,128,255,cv::THRESH_BINARY_INV);
	cv::namedWindow("Canny Image");
	cv::imshow("Canny Image",contoursInv);

	cv::HoughCircles(contours, circles, CV_HOUGH_GRADIENT, 
		2,   // accumulator resolution (size of the image / 2) 
		50,  // minimum distance between two circles
		200, // Canny high threshold 
		100, // minimum number of votes 
		25, 100); // min and max radius

	std::cout << "Circles: " << circles.size() << std::endl;

	// Draw the circles
	itc= circles.begin();
	image= cv::imread("../images/chariot.jpg",0);
	if (!image.data)
	{
		cout<<"Cannot Load File!"<<endl;
		return -1;
	}
	while (itc!=circles.end()) {

		cv::circle(image, 
			cv::Point((*itc)[0], (*itc)[1]), // circle centre
			(*itc)[2], // circle radius
			cv::Scalar(255), // color 
			2); // thickness

		++itc;	
	}
	cv::namedWindow("Detected Circles2");
	cv::imshow("Detected Circles2",image);*/

	cv::Mat image = cv::imread("../images/binaryGroup.bmp",0);
	if (!image.data)
	{
		cout<<"Cannot Load File!"<<endl;
		return -1;
	}
	cv::namedWindow("Binary Image");
	cv::imshow("Binary Image",image);

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(image,
		contours,               //轮廓数组
		CV_RETR_EXTERNAL,       //获取外轮廓
		CV_CHAIN_APPROX_NONE);  //获取每个轮廓的每个像素

	cout<<"Contours: "<<contours.size()<<endl;
	std::vector<std::vector<cv::Point>>::const_iterator itContours = contours.begin();
	for (;itContours!=contours.end();++itContours)
	{
		cout<<"Size: "<<itContours->size()<<endl;
	}

	cv::Mat result(image.size(),CV_8U,cv::Scalar(255));
	cv::drawContours(result,contours,
		-1,                          //绘制所有轮廓
		cv::Scalar(0),               //颜色为黑色
		2);                          //线宽为2
	cv::namedWindow("Contours");
	cv::imshow("Contours",result);

	//移除过长或过短的轮廓
	int cmin = 100;
	int cmax = 1000;
	std::vector<std::vector<cv::Point>>::const_iterator itc = contours.begin();
	while (itc!=contours.end())
	{
		if (itc->size()<cmin||itc->size()>cmax)
		{
			itc = contours.erase(itc);
		}
		else
		{
			++itc;
		}
	}
	// draw contours on the original image
	cv::Mat original= cv::imread("../images/group.jpg");
	cv::drawContours(original,contours,
		-1,                          //绘制所有轮廓
		cv::Scalar(255,255,255),     //颜色为白色
		2);                          //线宽为2
	cv::namedWindow("Contours on Animals");
	cv::imshow("Contours on Animals",original);

	// Let's now draw black contours on white image
	result.setTo(cv::Scalar(255));
	cv::drawContours(result,contours,
		-1, // draw all contours
		cv::Scalar(0), // in black
		1); // with a thickness of 1
	image= cv::imread("../images/binaryGroup.bmp",0);

	//测试包围盒
	cv::Rect r0 = cv::boundingRect(cv::Mat(contours[0]));
	cv::rectangle(result,r0,cv::Scalar(0),2);
	//测试最小包围圆
	float radius;
	cv::Point2f center;
	cv::minEnclosingCircle(cv::Mat(contours[1]),center,radius);
	cv::circle(result,cv::Point(center),static_cast<int>(radius),cv::Scalar(0),2);
	//测试多边形近似
	std::vector<cv::Point>poly;
	cv::approxPolyDP(cv::Mat(contours[2]),poly,
		5,            //近似的精确度
		true);        //这是个闭合形状
	//遍历每个片段进行绘制
	std::vector<cv::Point>::const_iterator itp = poly.begin();
	while (itp!=(poly.end()-1))
	{
		cv::line(result,*itp,*(itp+1),cv::Scalar(0),2);
		++itp;
	}
	//首尾直线相连
	cv::line(result,
		*(poly.begin()),
		*(poly.end()-1),cv::Scalar(20),2);
	//测试凸包
	std::vector<cv::Point>hull;
	cv::convexHull(cv::Mat(contours[3]),hull);
	//遍历每个片段进行绘制
	std::vector<cv::Point>::const_iterator ith = hull.begin();
	while (ith!=(hull.end()-1))
	{
		cv::line(result,*ith,*(ith+1),cv::Scalar(0),2);
		++ith;
	}
	//首尾直线相连
	cv::line(result,
		*(hull.begin()),
		*(hull.end()-1),cv::Scalar(20),2);
	//测试力矩
	//遍历所有轮廓
	itc = contours.begin();
	while (itc!=contours.end())
	{
		//计算所有的力矩
		cv::Moments mom = cv::moments(cv::Mat(*itc++));
		//绘制质心
		cv::circle(result,
			//质心坐标转换为整数
			cv::Point(mom.m10/mom.m00,mom.m01/mom.m00),
			2,cv::Scalar(0),2); //绘制黑点
	}
	cv::namedWindow("Some Shape descriptors");
	cv::imshow("Some Shape descriptors",result);

	// New call to findContours but with CV_RETR_LIST flag
	//image= cv::imread("../images/binaryGroup.bmp",0);



	cv::waitKey(0);
	return 0;
}

