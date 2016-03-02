#pragma once
#include <opencv2/opencv.hpp>  
#include<opencv2/core/core.hpp>  
#include<opencv2/highgui/highgui.hpp> 
#include<opencv2/imgproc/imgproc.hpp> 
#define PI 3.1415926

class EdgeDetector
{
public:
	EdgeDetector(void);
	void setAperture(int a);
	int getAperture() const;
	void computeSobel(const cv::Mat& image, cv::Mat &sobelX=cv::Mat(), cv::Mat &sobelY=cv::Mat());
	cv::Mat getMagnitude();
	cv::Mat getOrientation();
	cv::Mat getBinaryMap(double threshold);
	cv::Mat getSobelImage();
	cv::Mat getSobelOrientationImage();

	~EdgeDetector(void);
private:
	// original image
	cv::Mat img;
	// 16-bit signed int image
	cv::Mat sobel;
	// Aperture size of the Sobel kernel
	int aperture;
	// Sobel magnitude
	cv::Mat sobelMagnitude;
	// Sobel orientation
	cv::Mat sobelOrientation;
};

