#include "stdafx.h"
#include "EdgeDetector.h"


EdgeDetector::EdgeDetector(void):aperture(3)
{
}


EdgeDetector::~EdgeDetector(void)
{
}

// Set the aperture size of the kernel
void EdgeDetector::setAperture(int a) {

	aperture= a;
}

// Get the aperture size of the kernel
int EdgeDetector::getAperture() const {

	return aperture;
}

// Compute the Sobel
void EdgeDetector::computeSobel(const cv::Mat& image, cv::Mat &sobelX, cv::Mat &sobelY) {

	// Compute Sobel
	cv::Sobel(image,sobelX,CV_32F,1,0,aperture);
	cv::Sobel(image,sobelY,CV_32F,0,1,aperture);
	// Compute magnitude and orientation
	cv::cartToPolar(sobelX,sobelY,sobelMagnitude,sobelOrientation);
}

// Get Sobel magnitude
// Make a copy if called more than once
cv::Mat EdgeDetector::getMagnitude() {

	return sobelMagnitude;
}

// Get Sobel orientation
// Make a copy if called more than once
cv::Mat EdgeDetector::getOrientation() {

	return sobelOrientation;
}

// Get a thresholded binary map
cv::Mat EdgeDetector::getBinaryMap(double threshold) {

	cv::Mat bin;		  
	cv::threshold(sobelMagnitude,bin,threshold,255,cv::THRESH_BINARY_INV);

	return bin;
}

// Get a CV_8U image of the Sobel
cv::Mat EdgeDetector::getSobelImage() {

	cv::Mat bin;

	double minval, maxval;
	cv::minMaxLoc(sobelMagnitude,&minval,&maxval);
	sobelMagnitude.convertTo(bin,CV_8U,255/maxval);

	return bin;
}

// Get a CV_8U image of the Sobel orientation
// 1 gray-level = 2 degrees
cv::Mat EdgeDetector::getSobelOrientationImage() {

	cv::Mat bin;
	sobelOrientation.convertTo(bin,CV_8U,90/PI);
	return bin;
}