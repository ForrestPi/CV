// Chapter8.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>  
#include<opencv2/core/core.hpp>  
#include<opencv2/highgui/highgui.hpp> 
#include<opencv2/imgproc/imgproc.hpp> 
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include<opencv2/legacy/legacy.hpp>
#include "HarrisDetector.h"

#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	/*
	cv::Mat image= cv::imread("../images/church01.jpg",0);
	if (!image.data)
	{
		cout<<"Cannot Load File"<<endl;
		return -1;
	}
	// Display the image
	cv::namedWindow("Original Image");
	cv::imshow("Original Image",image);
	//���Harris�ǵ�
	cv::Mat cornerStrength;
	cv::cornerHarris(image,cornerStrength,
		3,                //�������صĳߴ�
		3,                //�˲����Ŀ׾���С
		0.01);            //Harris����
	//�ǵ�ǿ�ȵ���ֵ
	cv::Mat harrisCorners;
	double threshold  = 0.0001;
	cv::threshold(cornerStrength,harrisCorners,
				  threshold,255,cv::THRESH_BINARY_INV);
	// Display the corners
	cv::namedWindow("Harris Corner Map");
	cv::imshow("Harris Corner Map",harrisCorners);

	//����Harris������Ķ���
	HarrisDetector harris;
	//����Harrisֵ
	harris.detect(image);
	//���Harris�ǵ�
	std::vector<cv::Point>pts;
	harris.getCorners(pts,0.01);
	//����Harris�ǵ�
	harris.drawOnImage(image,pts);
	//Display the corners
	cv::namedWindow("Harris Corners");
	cv::imshow("Harris Corners",image);*/
	
	/*
	// Read input image
	cv::Mat image= cv::imread("../images/church01.jpg",0);
	if (!image.data)
	{
		cout<<"Cannot Load File"<<endl;
		return -1;
	}
	//�����ʺϸ��ٵ���������
	std::vector<cv::Point2f>corners;
	cv::goodFeaturesToTrack(image,corners,
		500,          //���ص������������Ŀ
		0.01,         //�����ȼ�
		10);          //����֮�����С�������
	//for all corners
		std::vector<cv::Point2f>::const_iterator it= corners.begin();
	while (it!=corners.end()) {

		// draw a circle at each corner location
		cv::circle(image,*it,3,cv::Scalar(255,255,255),2);
		++it;
	}
	// Display the corners
	cv::namedWindow("Good Features to Track");
	cv::imshow("Good Features to Track",image);*/

	/*
	// Read input image
	cv::Mat image= cv::imread("../images/church01.jpg",0);
	if (!image.data)
	{
		cout<<"Cannot Load File"<<endl;
		return -1;
	}
	// ����������
	std::vector<cv::KeyPoint> keypoints;
	// ������Ĺ��캯�� 
	cv::GoodFeaturesToTrackDetector gftt(
		500,	// maximum number of corners to be returned
		0.01,	// quality level
		10);	// minimum allowed distance between points
	// point detection using FeatureDetector method
	gftt.detect(image,keypoints);

	cv::drawKeypoints(image,		// original image
		keypoints,					// vector of keypoints
		image,						// the resulting image
		cv::Scalar(255,255,255),	// color of the points
		cv::DrawMatchesFlags::DRAW_OVER_OUTIMG); //drawing flag

	// Display the corners
	cv::namedWindow("Good Features to Track Detector");
	cv::imshow("Good Features to Track Detector",image);*/
	
	/*
	// Read input image
	cv::Mat image= cv::imread("../images/church01.jpg",0);
	if (!image.data)
	{
		cout<<"Cannot Load File"<<endl;
		return -1;
	}
	// ����������
	std::vector<cv::KeyPoint> keypoints;
	//����FAST��������
	cv::FastFeatureDetector fast(40);//�����ֵ
	//���м��
	fast.detect(image,keypoints);
	cv::drawKeypoints(image,keypoints,image,cv::Scalar(255,255,255),cv::DrawMatchesFlags::DRAW_OVER_OUTIMG);
	// Display the corners
	cv::namedWindow("FAST Features");
	cv::imshow("FAST Features",image);*/

	/*
	// Read input image
	cv::Mat image= cv::imread("../images/church01.jpg",0);
	if (!image.data)
	{
		cout<<"Cannot Load File"<<endl;
		return -1;
	}
	// ����������
	std::vector<cv::KeyPoint> keypoints;
	// ����SURF���������
	cv::SurfFeatureDetector surf(2500);
	// ���SURF����
	surf.detect(image,keypoints);

	cv::Mat featureImage;
	//���������㣬���ϳ߶��뷽����Ϣ
	cv::drawKeypoints(image,                         //ԭʼͼ��
		keypoints,                                   //�����������
		featureImage,                                //����ͼ��
		cv::Scalar(255,255,255),                     //���������ɫ
		cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);  //��־λ

	// Display the corners
	cv::namedWindow("SURF Features");
	cv::imshow("SURF Features",featureImage);
	// Read input image
	image= cv::imread("../images/church03.jpg",0);
	if (!image.data)
	{
		cout<<"Cannot Load File"<<endl;
		return -1;
	}
	keypoints.clear();
	// Detect the SURF features
	surf.detect(image,keypoints);
	cv::drawKeypoints(image,keypoints,featureImage,cv::Scalar(255,255,255),cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	// Display the corners
	cv::namedWindow("SURF Features2");
	cv::imshow("SURF Features2",featureImage);*/

	/*
	cv::Mat image= cv::imread("../images/church01.jpg",0);
	if (!image.data)
	{
		cout<<"Cannot Load File"<<endl;
		return -1;
	}
	// ����������
	std::vector<cv::KeyPoint> keypoints;
	// Construct the SURF feature detector object
	cv::SiftFeatureDetector sift(
		0.03,  // feature threshold
		10.);  // ���ڽ���ֱ�����жȵ���ֵ
	// Detect the SURF features
	sift.detect(image,keypoints);
	cv::Mat featureImage;
	cv::drawKeypoints(image,keypoints,featureImage,cv::Scalar(255,255,255),cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	// Display the corners
	cv::namedWindow("SIFT Features");
	cv::imshow("SIFT Features",featureImage);*/

	/*
	cv::Mat image= cv::imread("../images/church01.jpg",0);
	if (!image.data)
	{
		cout<<"Cannot Load File"<<endl;
		return -1;
	}
	// ����������
	std::vector<cv::KeyPoint> keypoints;
	cv::MserFeatureDetector mser;
	mser.detect(image,keypoints);
	cv::Mat featureImage;
	// Draw the keypoints with scale and orientation information
	cv::drawKeypoints(image,		// original image
		keypoints,					// vector of keypoints
		featureImage,				// the resulting image
		cv::Scalar(255,255,255),	// color of the points
		cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS); //drawing flag
	// Display the corners
	cv::namedWindow("MSER Features");
	cv::imshow("MSER Features",featureImage);*/

	// Read input images
	cv::Mat image1= cv::imread("../images/church01.jpg",0);
	cv::Mat image2= cv::imread("../images/church02.jpg",0);
	if (!image1.data||!image2.data)
	{
		cout<<"Cannot Load File"<<endl;
		return -1;
	}
	// Display the images
	cv::namedWindow("Right Image");
	cv::imshow("Right Image",image1);
	cv::namedWindow("Left Image");
	cv::imshow("Left Image",image2);
	// vector of keypoints
	std::vector<cv::KeyPoint> keypoints1;
	std::vector<cv::KeyPoint> keypoints2;
	//����SURF���������
	cv::SurfFeatureDetector surf(3000);
	surf.detect(image1,keypoints1);
	surf.detect(image2,keypoints2);
	std::cout << "Number of SURF points (1): " << keypoints1.size() << std::endl;
	std::cout << "Number of SURF points (2): " << keypoints2.size() << std::endl;
	// Draw the kepoints
	cv::Mat imageKP;
	cv::drawKeypoints(image1,keypoints1,imageKP,cv::Scalar(255,255,255),cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	cv::namedWindow("Right SURF Features");
	cv::imshow("Right SURF Features",imageKP);
	cv::drawKeypoints(image2,keypoints2,imageKP,cv::Scalar(255,255,255),cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	cv::namedWindow("Left SURF Features");
	cv::imshow("Left SURF Features",imageKP);
	// Construction of the SURF descriptor extractor 
	cv::SurfDescriptorExtractor surfDesc;
	// Extraction of the SURF descriptors
	cv::Mat descriptors1, descriptors2;
	surfDesc.compute(image1,keypoints1,descriptors1);
	surfDesc.compute(image2,keypoints2,descriptors2);
	std::cout << "descriptor matrix size: " << descriptors1.rows << " by " << descriptors1.cols << std::endl;
	// Construction of the matcher 
	cv::BruteForceMatcher<cv::L2<float>> matcher;

	// Match the two image descriptors
	std::vector<cv::DMatch> matches;
	matcher.match(descriptors1,descriptors2, matches);

	std::cout << "Number of matched points: " << matches.size() << std::endl;

	std::nth_element(matches.begin(),    // initial position
		matches.begin()+24, // position of the sorted element
		matches.end());     // end position
	// remove all elements after the 25th
	matches.erase(matches.begin()+25, matches.end()); 

	cv::Mat imageMatches;
	cv::drawMatches(image1,keypoints1,  // 1st image and its keypoints
		image2,keypoints2,  // 2nd image and its keypoints
		matches,			// the matches
		imageMatches,		// the image produced
		cv::Scalar(255,255,255)); // color of the lines
	cv::namedWindow("Matches");
	cv::imshow("Matches",imageMatches);


	cv::waitKey(0);
	return 0;
}

