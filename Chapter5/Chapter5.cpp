// Chapter5.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>  
#include<opencv2/core/core.hpp>  
#include<opencv2/highgui/highgui.hpp> 
#include<opencv2/imgproc/imgproc.hpp> 
#include "WatershedSegmenter.h"
#include <iostream>
using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
	/*
	cv::Mat image = cv::imread("../images/group.jpg");
	if(!image.data)
	{
		cout<<"Cannot Load File!"<<endl;
		return -1;
	}
	// Display the image
	cv::namedWindow("Original Image");
	cv::imshow("Original Image",image);
	// Get the binary map
	cv::Mat binary;
	binary= cv::imread("../images/binary.bmp",0);
	// Display the binary image
	cv::namedWindow("Binary Image");
	cv::imshow("Binary Image",binary);
	cv::Mat fg;
	cv::erode(binary,fg,cv::Mat(),cv::Point(-1,-1),6);
	// Display the foreground image
	cv::namedWindow("Foreground Image");
	cv::imshow("Foreground Image",fg);
	// Identify image pixels without objects
	cv::Mat bg;
	cv::dilate(binary,bg,cv::Mat(),cv::Point(-1,-1),6);
	cv::threshold(bg,bg,1,128,cv::THRESH_BINARY_INV);
	// Display the background image
	cv::namedWindow("Background Image");
	cv::imshow("Background Image",bg);
	// Show markers image
	cv::Mat markers(binary.size(),CV_8U,cv::Scalar(0));
	markers= fg+bg;
	cv::namedWindow("Markers");
	cv::imshow("Markers",markers);
	// Create watershed segmentation object
	WatershedSegmenter segmenter;
	// Set markers and process
	segmenter.setMarkers(markers);
	//cv::cvtColor(image,image,cv::COLOR_BGR2GRAY);
	segmenter.process(image);
	// Display segmentation result
	cv::namedWindow("Segmentation");
	cv::imshow("Segmentation",segmenter.getSegmentation());
	// Display watersheds
	cv::namedWindow("Watersheds");
	cv::imshow("Watersheds",segmenter.getWatersheds());*/
	/*
	cv::Mat image = cv::imread("../images/group.jpg");
	if(!image.data)
	{
		cout<<"Cannot Load File!"<<endl;
		return -1;
	}
	cv::Rect rectangle(10,100,380,180);
	cv::Mat result;             //分割（四种可能的结果）
	cv::Mat bgModel,fgNodel;    //模型（供内部使用）
	cv::grabCut(image,          //输入图像
		result,                 //分割结果
		rectangle,               //包含前景物体的矩形
		bgModel,fgNodel,         //模型
		5,                      //迭代次数
		cv::GC_INIT_WITH_RECT);  //使用矩形进行初始化
	cv::compare(result,cv::GC_PR_FGD,result,cv::CMP_EQ);
	result= result&1;
	cv::Mat foreground;
	foreground.create(image.size(),CV_8UC3);
	foreground.setTo(cv::Scalar(255,255,255));
	image.copyTo(foreground,result); // bg pixels not copied
	// draw rectangle on original image
	cv::rectangle(image, rectangle, cv::Scalar(255,255,255),1);
	cv::namedWindow("Image 2");
	cv::imshow("Image 2",image);
	// display result
	cv::namedWindow("Foreground objects");
	cv::imshow("Foreground objects",foreground);*/
	/*
	// Open another image
	cv::Mat image = cv::imread("../images/tower.jpg");
	if(!image.data)
	{
		cout<<"Cannot Load File!"<<endl;
		return -1;
	}
	// Identify background pixels
	cv::Mat imageMask(image.size(),CV_8U,cv::Scalar(0));
	cv::rectangle(imageMask,cv::Point(5,5),cv::Point(image.cols-5,image.rows-5),cv::Scalar(255),3);
	// Identify foreground pixels (in the middle of the image)
	cv::rectangle(imageMask,cv::Point(image.cols/2-10,image.rows/2-10),
		cv::Point(image.cols/2+10,image.rows/2+10),cv::Scalar(1),10);
	WatershedSegmenter segmenter;
	// Set markers and process
	segmenter.setMarkers(imageMask);
	segmenter.process(image);

	// Display the image with markers
	cv::rectangle(image,cv::Point(5,5),cv::Point(image.cols-5,image.rows-5),cv::Scalar(255,255,255),3);
	cv::rectangle(image,cv::Point(image.cols/2-10,image.rows/2-10),
		cv::Point(image.cols/2+10,image.rows/2+10),cv::Scalar(1,1,1),10);
	cv::namedWindow("Image with marker");
	cv::imshow("Image with marker",image);

	// Display watersheds
	cv::namedWindow("Watersheds of foreground object");
	cv::imshow("Watersheds of foreground object",segmenter.getWatersheds());*/

	
	// Open another image
	cv::Mat image= cv::imread("../images/tower.jpg");
	if(!image.data)
	{
		cout<<"Cannot Load File!"<<endl;
		return -1;
	}
	// define bounding rectangle 
	cv::Rect rectangle(50,70,image.cols-150,image.rows-180);

	cv::Mat result; // segmentation result (4 possible values)
	cv::Mat bgModel,fgModel; // the models (internally used)
	// GrabCut segmentation
	cv::grabCut(image,    // input image
		result,   // segmentation result
		rectangle,// rectangle containing foreground 
		bgModel,fgModel, // models
		1,        // number of iterations
		cv::GC_INIT_WITH_RECT); // use rectangle

	// Get the pixels marked as likely foreground
	cv::compare(result,cv::GC_PR_FGD,result,cv::CMP_EQ);
	// Generate output image
	cv::Mat foreground(image.size(),CV_8UC3,cv::Scalar(255,255,255));
	image.copyTo(foreground,result); // bg pixels not copied

	// draw rectangle on original image
	cv::rectangle(image, rectangle, cv::Scalar(255,255,255),1);
	cv::namedWindow("Image");
	cv::imshow("Image",image);

	// display result
	cv::namedWindow("Segmented Image");
	cv::imshow("Segmented Image",foreground);

	cv::waitKey(0);
	return 0;
}

