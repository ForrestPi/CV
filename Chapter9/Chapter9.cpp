// Chapter9.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>  
#include<opencv2/core/core.hpp>  
#include<opencv2/highgui/highgui.hpp> 
#include<opencv2/imgproc/imgproc.hpp> 
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "CameraCalibrator.h"
#include "RobustMatcher.h"
#include <iomanip>

int _tmain(int argc, _TCHAR* argv[])
{
	/*
	cv::namedWindow("Image");
	cv::Mat image;
	std::vector<std::string> filelist;
	//generate list of chessboard image filename
	for (int i=1;i<=20;i++)
	{
		std::stringstream str;
		str<<"../images/chessboards/chessboard"<<std::setw(2) << std::setfill('0')<<i<<".jpg";
		std::cout<<str.str()<<std::endl;
		filelist.push_back(str.str());
		image = cv::imread(str.str(),0);
		cv::imshow("Image",image);
		cv::waitKey(100);
	}
	CameraCalibrator cameraCalibrator;
	cv::Size boardSize(6,4);
	cameraCalibrator.addChessboardPoints(
		filelist,         //filename of the chessboard image
		boardSize);        //size of chessboard
	//calibrate the camera
	cameraCalibrator.calibrate(image.size());
	//Image Undistortion
	image = cv::imread(filelist[6]);
	cv::Mat uImage = cameraCalibrator.remap(image);
	//display camera matrix
	cv::Mat cameraMatrix = cameraCalibrator.getCameraMatrix();
	std::cout<<"Camera intrinsic: "<<cameraMatrix.rows<<"x"<<cameraMatrix.cols<<std::endl;
	std::cout<<cameraMatrix.at<double>(0,0)<<" "<<cameraMatrix.at<double>(0,1)<<" "<<cameraMatrix.at<double>(0,2)<<std::endl;
	std::cout<<cameraMatrix.at<double>(1,0)<<" "<<cameraMatrix.at<double>(1,1)<<" "<<cameraMatrix.at<double>(1,2)<<std::endl;
	std::cout<<cameraMatrix.at<double>(2,0)<<" "<<cameraMatrix.at<double>(2,1)<<" "<<cameraMatrix.at<double>(2,2)<<std::endl;
	imshow("Original Image", image);
	imshow("Undistorted Image", uImage);*/

	/*
	// Read input images
	cv::Mat image1= cv::imread("../images/parliament1.bmp",0);
	cv::Mat image2= cv::imread("../images/parliament2.bmp",0);
	if (!image1.data || !image2.data)
		return 0; 
	// Display the images
	cv::namedWindow("Image 1");
	cv::imshow("Image 1",image1);
	cv::namedWindow("Image 2");
	cv::imshow("Image 2",image2);
	//准备匹配器
	RobustMatcher rmacher;
	rmacher.setConfidenceLevel(0.98);
	rmacher.setMinDistanceToEpipolar(1.0);
	rmacher.setRatio(0.65f);
	cv::Ptr<cv::FeatureDetector>pfd=new cv::SurfFeatureDetector(10);
	rmacher.setFeatureDetector(pfd);
	//匹配两幅图像
	std::vector<cv::DMatch>matches;
	std::vector<cv::KeyPoint>keypoints1,keypoints2;
	cv::Mat fundemental = rmacher.match(image1,image2,matches,keypoints1,keypoints2);
	// draw the matches
	cv::Mat imageMatches;
	cv::drawMatches(image1,keypoints1,  // 1st image and its keypoints
		image2,keypoints2,              // 2nd image and its keypoints
		matches,			            // the matches
		imageMatches,		            // the image produced
		cv::Scalar(255,255,255));       // color of the lines
	cv::namedWindow("Matches");
	cv::imshow("Matches",imageMatches);


	//Convert keypoints into Point2f
	std::vector<cv::Point2f>points1,points2;
	for (std::vector<cv::DMatch>::const_iterator it=matches.begin();it!=matches.end();++it)
	{
		//Get the position of left keypoints
		float x=keypoints1[it->queryIdx].pt.x;
		float y=keypoints1[it->queryIdx].pt.y;
		points1.push_back(cv::Point2f(x,y));
		//Get the position of right keypoints
		x=keypoints2[it->trainIdx].pt.x;
		y=keypoints2[it->trainIdx].pt.y;
		points2.push_back(cv::Point2f(x,y));
	}
	std::cout << points1.size() << " " << points2.size() << std::endl; 
	//找到两个图之间的单应矩阵
	std::vector<uchar> inliers(points1.size(),0);
	cv::Mat homography=cv::findHomography(
		cv::Mat(points1),cv::Mat(points2),   //对应的点集
		inliers,                             //输出的正确值
		CV_RANSAC,                           //RANSAC 算法
		1.);                                 //到反射投影点的最大距离
	//Draw the inlier points
	std::vector<cv::Point2f>::const_iterator itPts=points1.begin();
	std::vector<uchar>::const_iterator itIn=inliers.begin();
	while (itPts!=points1.end())
	{
		//draw a circle at each inliers location
		if (*itIn)
		{
			cv::circle(image1,*itPts,3,cv::Scalar(255,255,255),2);
		}
		++itPts;
		++itIn;
	}
	itPts= points2.begin();
	itIn= inliers.begin();
	while (itPts!=points2.end())
	{
		// draw a circle at each inlier location
		if (*itIn) 
			cv::circle(image2,*itPts,3,cv::Scalar(255,255,255),2);

		++itPts;
		++itIn;
	}
	//Display the images with points
	cv::namedWindow("Image 1 Homography Points");
	cv::imshow("Image 1 Homography Points",image1);
	cv::namedWindow("Image 2 Homography Points");
	cv::imshow("Image 2 Homography Points",image2);
	// 歪曲image1到image2
	cv::Mat result;
	cv::warpPerspective(image1, // 输入图像
		result,			        // 输出图像
		homography,		        // homography
		cv::Size(2*image1.cols,image1.rows)); // size of output image

	// 赋值图1到整幅图的前半部分
	cv::Mat half(result,cv::Rect(0,0,image2.cols,image2.rows));
	image2.copyTo(half);  //复制图2到图1的ROI区域

	// Display the warp image
	cv::namedWindow("After warping");
	cv::imshow("After warping",result);*/

	
	// Read input images
	cv::Mat image1= cv::imread("../images/canal1.jpg",0);
	cv::Mat image2= cv::imread("../images/canal2.jpg",0);
	if (!image1.data || !image2.data)
		return 0; 

	// Display the images
	cv::namedWindow("Right Image");
	cv::imshow("Right Image",image1);
	cv::namedWindow("Left Image");
	cv::imshow("Left Image",image2);

	// Prepare the matcher
	RobustMatcher rmatcher;
	rmatcher.setConfidenceLevel(0.98);
	rmatcher.setMinDistanceToEpipolar(1.0);
	rmatcher.setRatio(0.65f);
	cv::Ptr<cv::FeatureDetector> pfd= new cv::SurfFeatureDetector(10); 
	rmatcher.setFeatureDetector(pfd);

	// Match the two images
	std::vector<cv::DMatch> matches;
	std::vector<cv::KeyPoint> keypoints1, keypoints2;
	cv::Mat fundemental= rmatcher.match(image1,image2,matches, keypoints1, keypoints2);

	// draw the matches
	cv::Mat imageMatches;
	cv::drawMatches(image1,keypoints1,  // 1st image and its keypoints
		image2,keypoints2,  // 2nd image and its keypoints
		matches,			// the matches
		imageMatches,		// the image produced
		cv::Scalar(255,255,255)); // color of the lines
	cv::namedWindow("Matches");
	cv::imshow("Matches",imageMatches);

	// Convert keypoints into Point2f	
	std::vector<cv::Point2f> points1, points2;

	for (std::vector<cv::DMatch>::const_iterator it= matches.begin();
		it!= matches.end(); ++it) {

			// Get the position of left keypoints
			float x= keypoints1[it->queryIdx].pt.x;
			float y= keypoints1[it->queryIdx].pt.y;
			points1.push_back(cv::Point2f(x,y));
			cv::circle(image1,cv::Point(x,y),3,cv::Scalar(255,255,255),3);
			// Get the position of right keypoints
			x= keypoints2[it->trainIdx].pt.x;
			y= keypoints2[it->trainIdx].pt.y;
			cv::circle(image2,cv::Point(x,y),3,cv::Scalar(255,255,255),3);
			points2.push_back(cv::Point2f(x,y));
	}

	// Draw the epipolar lines
	std::vector<cv::Vec3f> lines1; 
	cv::computeCorrespondEpilines(cv::Mat(points1),1,fundemental,lines1);

	for (std::vector<cv::Vec3f>::const_iterator it= lines1.begin();
		it!=lines1.end(); ++it) {

			cv::line(image2,cv::Point(0,-(*it)[2]/(*it)[1]),
				cv::Point(image2.cols,-((*it)[2]+(*it)[0]*image2.cols)/(*it)[1]),
				cv::Scalar(255,255,255));
	}

	std::vector<cv::Vec3f> lines2; 
	cv::computeCorrespondEpilines(cv::Mat(points2),2,fundemental,lines2);

	for (std::vector<cv::Vec3f>::const_iterator it= lines2.begin();
		it!=lines2.end(); ++it) {

			cv::line(image1,cv::Point(0,-(*it)[2]/(*it)[1]),
				cv::Point(image1.cols,-((*it)[2]+(*it)[0]*image1.cols)/(*it)[1]),
				cv::Scalar(255,255,255));
	}

	// Display the images with epipolar lines
	cv::namedWindow("Right Image Epilines (RANSAC)");
	cv::imshow("Right Image Epilines (RANSAC)",image1);
	cv::namedWindow("Left Image Epilines (RANSAC)");
	cv::imshow("Left Image Epilines (RANSAC)",image2);


	


	cv::waitKey();
	return 0;
}

