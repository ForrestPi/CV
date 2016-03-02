#pragma once
#include <opencv2/opencv.hpp>  
#include<opencv2/core/core.hpp>  
#include<opencv2/highgui/highgui.hpp> 
#include<opencv2/imgproc/imgproc.hpp> 
#include <opencv2/features2d/features2d.hpp>
class HarrisDetector
{
public:
	HarrisDetector(void);
	~HarrisDetector(void);
	void detect(const cv::Mat& image);
	cv::Mat getCornerMap(double qualityLevel);
	void setLoaclMaxWindowSize( int size );
	void getCorners(std::vector<cv::Point>&points,
		double qualityLevel);
	void getCorners(std::vector<cv::Point>&points, const cv::Mat& cornerMap);
	void drawOnImage(cv::Mat& image,const std::vector<cv::Point>&points,
		cv::Scalar color = cv::Scalar(255,255,255),int radius=3, int thickness=2);
	
private:
	//表示角点强度的32位浮点数
	cv::Mat cornerStrength;
	//表示阈值化后角度的32位浮点数图像
	cv::Mat cornerTh;
	//局部极大值图像（内部）
	cv::Mat localMax;
	//导数平滑的相邻像素的尺寸
	int neighbourhood;
	//梯度计算的孔径大小
	int aperture;        //sobel边缘检测窗口大小（sobel获取各像素点x，y方向的灰度导数）
	//Harris参数
	double k;
	//阈值计算的最大强度
	double maxStrength;
    //计算得到的阈值（内部）
	double threshold;
	//非极大值抑制的相邻像素的尺寸
	int nonMaxSize;
	//非极大值抑制的核
	cv::Mat kernel;

};

