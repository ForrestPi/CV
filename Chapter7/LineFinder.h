#pragma once
#include <opencv2/opencv.hpp>  
#include<opencv2/core/core.hpp>  
#include<opencv2/highgui/highgui.hpp> 
#include<opencv2/imgproc/imgproc.hpp> 
#define PI 3.1415926

class LineFinder
{
public:
	LineFinder(void);
	~LineFinder(void);
	void setAccResolution(double dRho, double dTheta);
	void setMinVote(int minv);
	void setLineLengthAndGap(double length, double gap);
	std::vector<cv::Vec4i> findLines(cv::Mat& binary);
	void drawDetectedLines(cv::Mat &image, cv::Scalar color=cv::Scalar(255,255,255));
	std::vector<cv::Vec4i> removeLinesOfInconsistentOrientations( const cv::Mat &orientations, double percentage, double delta);
private:
	// original image
	cv::Mat img;
	// vector containing the end points 
	// of the detected lines
	std::vector<cv::Vec4i> lines;
	// accumulator resolution parameters
	double deltaRho;
	double deltaTheta;
	// minimum number of votes that a line 
	// must receive before being considered
	int minVote;
	// min length for a line
	double minLength;
	// max allowed gap along the line
	double maxGap;
};

