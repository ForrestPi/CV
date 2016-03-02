// Chapter6.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>  
#include<opencv2/core/core.hpp>  
#include<opencv2/highgui/highgui.hpp> 
#include<opencv2/imgproc/imgproc.hpp> 
#include <iostream>
using namespace std;
#include "LaplacianZC.h"
#include <iomanip>

int _tmain(int argc, _TCHAR* argv[])
{
	/*
	// Read input image
	cv::Mat image= cv::imread("../images/boldt.jpg",0);
	if (!image.data)
	{
		cout<<"Cannot Load File!"<<endl;
		return -1; 
	}
	// Display the image
	cv::namedWindow("Original Image");
	cv::imshow("Original Image",image);

	// Blur the image
	cv::Mat result;
	cv::GaussianBlur(image,result,cv::Size(5,5),1.5);

	// Display the blurred image
	cv::namedWindow("Gaussian filtered Image");
	cv::imshow("Gaussian filtered Image",result);

	// Get the gaussian kernel (1.5)
	cv::Mat gauss= cv::getGaussianKernel(9,1.5,CV_32F);

	// Display kernel values
	cv::Mat_<float>::const_iterator it= gauss.begin<float>();  
	cv::Mat_<float>::const_iterator itend= gauss.end<float>();  
	std::cout << "[";
	for ( ; it!= itend; ++it) {
		std::cout << *it << " ";
	}
	std::cout << "]" << std::endl;

	// Get the gaussian kernel (0.5)
	gauss= cv::getGaussianKernel(9,0.5,CV_32F);

	// Display kernel values
	it= gauss.begin<float>();  
	itend= gauss.end<float>();  
	std::cout << "[";
	for ( ; it!= itend; ++it) {
		std::cout << *it << " ";
	}
	std::cout << "]" << std::endl;

	// Get the gaussian kernel (2.5)
	gauss= cv::getGaussianKernel(9,2.5,CV_32F);

	// Display kernel values
	it= gauss.begin<float>();  
	itend= gauss.end<float>();  
	std::cout << "[";
	for ( ; it!= itend; ++it) {
		std::cout << *it << " ";
	}
	std::cout << "]" << std::endl;

	// Get the Deriv kernel (2.5)
	cv::Mat kx, ky;
	cv::getDerivKernels(kx,ky,2,2,7,true);

	// Display kernel values
	cv::Mat_<float>::const_iterator kit= kx.begin<float>();  
	cv::Mat_<float>::const_iterator kitend= kx.end<float>();  
	std::cout << "[";
	for ( ; kit!= kitend; ++kit) {
		std::cout << *kit << " ";
	}
	std::cout << "]" << std::endl;

	// Blur the image with a mean filter
	cv::blur(image,result,cv::Size(5,5));

	// Display the blurred image
	cv::namedWindow("Mean filtered Image");
	cv::imshow("Mean filtered Image",result);*/

	/*
	// Read input image with salt&pepper noise
	cv::Mat image= cv::imread("../images/salted.bmp",0);
	if (!image.data)
	{
		cout<<"Cannot Load File!"<<endl;
		return -1; 
	}

	// Display the S&P image
	cv::namedWindow("S&P Image");
	cv::imshow("S&P Image",image);
	cv::Mat result;
	// Blur the image with a mean filter
	cv::blur(image,result,cv::Size(5,5));

	// Display the blurred image
	cv::namedWindow("Mean filtered S&P Image");
	cv::imshow("Mean filtered S&P Image",result);

	// Applying a median filter
	cv::medianBlur(image,result,5);

	// Display the blurred image
	cv::namedWindow("Median filtered S&P Image");
	cv::imshow("Median filtered S&P Image",result);

	// Reduce by 4 the size of the image (the wrong way)
	image= cv::imread("../images/boldt.jpg",0);
	if (!image.data)
	{
		cout<<"Cannot Load File!"<<endl;
		return -1; 
	}
	cv::Mat reduced(image.rows/2,image.cols/2,CV_8U);

	for (int i=0; i<reduced.rows; i++)
		for (int j=0; j<reduced.cols; j++)
			reduced.at<uchar>(i,j)= image.at<uchar>(i*2,j*2);

	// Display the reduced image
	cv::namedWindow("Badly reduced Image");
	cv::imshow("Badly reduced Image",reduced);*/

	// Read input image
	cv::Mat image= cv::imread("../images/boldt.jpg",0);
	if (!image.data)
	{
		cout<<"Cannot Load File!"<<endl;
		return -1; 
	} 
	// Display the image
	cv::namedWindow("Original Image");
	cv::imshow("Original Image",image);

	// Compute Sobel X derivative
	cv::Mat sobelX;
	cv::Sobel(image,sobelX,CV_8U,1,0,3,0.4,128);

	// Display the image
	//cv::namedWindow("Sobel X Image");
	//cv::imshow("Sobel X Image",sobelX);

	// Compute Sobel Y derivative
	cv::Mat sobelY;
	cv::Sobel(image,sobelY,CV_8U,0,1,3,0.4,128);

	// Display the image
	//cv::namedWindow("Sobel Y Image");
	//cv::imshow("Sobel Y Image",sobelY);

	// Compute norm of Sobel
	cv::Sobel(image,sobelX,CV_16S,1,0);
	cv::Sobel(image,sobelY,CV_16S,0,1);
	cv::Mat sobel;
	//compute the L1 norm
	sobel= abs(sobelX)+abs(sobelY);

	double sobmin, sobmax;
	cv::minMaxLoc(sobel,&sobmin,&sobmax);
	std::cout << "sobel value range: " << sobmin << "  " << sobmax << std::endl;

	// Print window pixel values
	for (int i=0; i<12; i++) {
		for (int j=0; j<12; j++)
			std::cout << std::setw(5) << static_cast<int>(sobel.at<short>(i+135,j+362)) << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

	//Sobel必须以浮点数格式进行计算
	cv::Sobel(image,sobelX,CV_32F,1,0);
	cv::Sobel(image,sobelY,CV_32F,0,1);

	//计算L2范式及梯度方向
	//cv::Mat norm,dir;
	//cv::cartToPolar(sobelX,sobelY,norm,dir);
	//cv::namedWindow("norm Image");
	//cv::imshow("norm Image",norm);
	//norm = abs(norm);
	//dir = abs(dir);
	//cv::namedWindow("dir Image");
	//cv::imshow("dir Image",dir);

	// Conversion to 8-bit image
	// sobelImage = -alpha*sobel + 255
	cv::Mat sobelImage;
	sobel.convertTo(sobelImage,CV_8U,-255./sobmax,255);

	// Display the image
	cv::namedWindow("Sobel Image");
	cv::imshow("Sobel Image",sobelImage);

	// Apply threshold to Sobel norm (low threshold value)
	cv::Mat sobelThresholded;
	cv::threshold(sobelImage, sobelThresholded, 225, 255, cv::THRESH_BINARY);

	// Display the image
	cv::namedWindow("Binary Sobel Image (low)");
	cv::imshow("Binary Sobel Image (low)",sobelThresholded);

	// Apply threshold to Sobel norm (high threshold value)
	cv::threshold(sobelImage, sobelThresholded, 190, 255, cv::THRESH_BINARY);

	// Display the image
	cv::namedWindow("Binary Sobel Image (high)");
	cv::imshow("Binary Sobel Image (high)",sobelThresholded);

	// Compute Laplacian 3x3
	cv::Mat laplace;
	cv::Laplacian(image,laplace,CV_8U,1,1,128);

	// Display the image
	cv::namedWindow("Laplacian Image");
	cv::imshow("Laplacian Image",laplace);

	// Print window pixel values
	for (int i=0; i<12; i++) {
		for (int j=0; j<12; j++)
			std::cout << std::setw(5) << static_cast<int>(laplace.at<uchar>(i+135,j+362))-128 << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

	// Compute Laplacian 7x7
	cv::Laplacian(image,laplace,CV_8U,7,0.01,128);

	// Display the image 
	cv::namedWindow("Laplacian Image");
	cv::imshow("Laplacian Image",laplace);

	// Print window pixel values
	for (int i=0; i<12; i++) {
		for (int j=0; j<12; j++)
			std::cout << std::setw(5) << static_cast<int>(laplace.at<uchar>(i+135,j+362))-128 << " ";
		std::cout << std::endl;
	}

	// Extract small window
	cv::Mat window(image,cv::Rect(362,135,12,12));
	cv::namedWindow("Image window");
	cv::imshow("Image window",window);
	cv::imwrite("window.bmp",window);

	// Compute Laplacian using LaplacianZC class
	LaplacianZC laplacian;
	laplacian.setAperture(7);
	cv::Mat flap= laplacian.computeLaplacian(image);
	double lapmin, lapmax;
	cv::minMaxLoc(flap,&lapmin,&lapmax);
	std::cout << "Laplacian value range=[" << lapmin << "," << lapmax << "]\n";
	laplace= laplacian.getLaplacianImage();
	cv::namedWindow("Laplacian Image (7x7)");
	cv::imshow("Laplacian Image (7x7)",laplace);

	// Print Laplacian values
	std::cout << std::endl;
	for (int i=0; i<12; i++) {
		for (int j=0; j<12; j++)
			std::cout << std::setw(5) << static_cast<int>(flap.at<float>(i+135,j+362)/100) << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;

	// Compute and display the zero-crossing points
	cv::Mat zeros;
	zeros= laplacian.getZeroCrossings(lapmax);
	cv::namedWindow("Zero-crossings");
	cv::imshow("Zero-crossings",zeros);

	// Compute and display the zero-crossing points (Sobel version)
	zeros= laplacian.getZeroCrossings();
	zeros= laplacian.getZeroCrossingsWithSobel(50);
	cv::namedWindow("Zero-crossings (2)");
	cv::imshow("Zero-crossings (2)",zeros);

	// Print window pixel values
	for (int i=0; i<12; i++) {
		for (int j=0; j<12; j++)
			std::cout << std::setw(2) << static_cast<int>(zeros.at<uchar>(i+135,j+362)) << " ";
		std::cout << std::endl;
	}

	// Display the image with window
	cv::rectangle(image,cv::Point(362,135),cv::Point(374,147),cv::Scalar(255,255,255));
	cv::namedWindow("Original Image with window");
	cv::imshow("Original Image with window",image);

	cv::waitKey(0);
	return 0;
}

