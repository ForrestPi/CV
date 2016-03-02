// Chapter10.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <opencv2/opencv.hpp>  
#include<opencv2/core/core.hpp>  
#include<opencv2/highgui/highgui.hpp> 
#include<opencv2/imgproc/imgproc.hpp> 
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/video/video.hpp>
#include <iostream>
#include "VideoProcessor.h"

void draw(cv::Mat& img, cv::Mat& out) {

	img.copyTo(out);
	cv::circle(out, cv::Point(100,100),5,cv::Scalar(255,0,0),2);
}

void canny(cv::Mat& img, cv::Mat& out) {

	// Convert to gray
	cv::cvtColor(img,out,CV_BGR2GRAY);
	// Compute Canny edges
	cv::Canny(out,out,100,200);
	// Invert the image
	cv::threshold(out,out,128,255,cv::THRESH_BINARY_INV);
}



int _tmain(int argc, _TCHAR* argv[])
{
	/*
	//����Ƶ�ļ�
	cv::VideoCapture capture("../images/bike.avi");
	//�����Ƶ�Ƿ�ɹ���
	if (!capture.isOpened())
	{
		std::cout<<"Cannot Load File!"<<std::endl;
		return -1;
	}
	//��ȡ֡��
	double rate = capture.get(CV_CAP_PROP_FPS);
	bool stop(false);
	cv::Mat frame;  //��ǰ��Ƶ֡
	cv::namedWindow("Extrated Frame");
	//ÿһ֮֡����ӳ�
	//����Ƶ��֡�����Ӧ
	int delay = 1000/rate;
	//����ÿһ֡
	while(!stop)
	{
		//���Զ�ȡ��һ֡
		if (!capture.read(frame))
		{
			break;
		}
		cv::imshow("Extrated Frame",frame);
		//�����ӳ�
		//Ҳ��ͨ������ֹͣ
		if (cv::waitKey(delay)>=0)
		{
			stop=true;
		}
	}
	//�ر���Ƶ�ļ�
	//���������������ã���˷Ǳ���
	capture.release();*/

	/*
	// Create instance
	VideoProcessor processor;
	// Open video file
	processor.setInput("../images/bike.avi");
	// Declare a window to display the video
	processor.displayInput("Input Video");
	processor.displayOutput("Output Video");
	// Play the video at the original frame rate
	processor.setDelay(1000./processor.getFrameRate());
	// Set the frame processor callback function
	processor.setFrameProcessor(canny);
	// Start the process
	processor.run();*/

	
	// Second test
	// Create instance
	VideoProcessor processor;
	// Open video file
	processor.setInput("../images/bike.avi");

	// Get basic info about video file
	cv::Size size= processor.getFrameSize();
	std::cout << size.width << " " << size.height << std::endl;
	std::cout << processor.getFrameRate() << std::endl;
	std::cout << processor.getTotalFrameCount() << std::endl;
	std::cout << processor.getFrameNumber() << std::endl;
	std::cout << processor.getPositionMS() << std::endl;

	// No processing
	processor.dontCallProcess();
	// Output filename
	//	processor.setOutput("../output/bikeOut",".jpg");
	char codec[4];
	processor.setOutput("../output/bike.avi",processor.getCodec(codec),processor.getFrameRate());
	std::cout << "Codec: " << codec[0] << codec[1] << codec[2] << codec[3] << std::endl;

	// Position the stream at frame 300
	//	processor.setFrameNumber(300);
	//	processor.stopAtFrameNo(120);

	// Declare a window to display the video
	processor.displayInput("Current Frame");
	processor.displayOutput("Output Frame");

	// Play the video at the original frame rate
	processor.setDelay(1000./processor.getFrameRate());

	// Start the process
	processor.run();

	std::cout << processor.getFrameNumber() << std::endl;
	std::cout << processor.getPositionMS() << std::endl;

	cv::waitKey(0);
	return 0;
}

