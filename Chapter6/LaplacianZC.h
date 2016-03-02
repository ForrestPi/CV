#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
class LaplacianZC
{
public:
	LaplacianZC(void);
	void setAperture(int a);
	int getAperture() const;
	cv::Mat computeLaplacian(const cv::Mat& image);
	cv::Mat getLaplacianImage(double scale=-1.0);
	cv::Mat getZeroCrossings(float threshold=1.0);
	cv::Mat getZeroCrossingsWithSobel(float threshold);
	~LaplacianZC(void);
private:
	// original image
	cv::Mat img;
	// 32-bit float image containing the Laplacian
	cv::Mat laplace;
	// Aperture size of the laplacian kernel
	int aperture;
};

