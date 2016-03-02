#include "stdafx.h"
#include "HarrisDetector.h"


HarrisDetector::HarrisDetector(void):neighbourhood(3),aperture(3),k(0.01),maxStrength(0.0),threshold(0.01),nonMaxSize(3)
{
	//创建非极大值抑制的核
	setLoaclMaxWindowSize(nonMaxSize);
}


HarrisDetector::~HarrisDetector(void)
{
}

//计算Harris角点
void HarrisDetector::detect( const cv::Mat& image )
{
	//harris计算
	cv::cornerHarris(image,cornerStrength,
		neighbourhood,      //neighbourhood size
		aperture,           //滤波器的孔径大小
		k);                 //Harris参数
	//内部阈值计算
	double minStrength;
	cv::minMaxLoc(cornerStrength,&minStrength,&maxStrength);
	//局部极大值检测
	cv::Mat dilated;    //临时图像
	//默认3*3核膨胀，膨胀之后，除了局部最大值点和原来相同，其它非局部最大值点被  
	//3*3邻域内的最大值点取代
	cv::dilate(cornerStrength,dilated,cv::Mat());
	//与原图相比，只剩下和原图值相同的点，这些点都是局部最大值点，保存到localMax  
	cv::compare(cornerStrength,dilated,
		localMax,cv::CMP_EQ);
}

//由Harris值获取角点图
cv::Mat HarrisDetector::getCornerMap( double qualityLevel )
{
	cv::Mat cornerMap;
	//对角点图像进行阈值化
	threshold = qualityLevel*maxStrength;
	cv::threshold(cornerStrength,cornerTh,
		threshold,255,cv::THRESH_BINARY);
	//转换为8位图像
	cornerTh.convertTo(cornerMap,CV_8U);
	//非极大值抑制
	cv::bitwise_and(cornerMap,localMax,cornerMap);
	return cornerMap;
}

void HarrisDetector::setLoaclMaxWindowSize( int size )
{
	nonMaxSize= size;
	kernel.create(nonMaxSize,nonMaxSize,CV_8U);
}

//由Harris值获取特征点
void HarrisDetector::getCorners( std::vector<cv::Point>&points, double qualityLevel )
{
	//得到角点图
	cv::Mat cornerMap = getCornerMap(qualityLevel);
	//得到角点
	getCorners(points,cornerMap);
}

//由角点图获取特征点
void HarrisDetector::getCorners( std::vector<cv::Point>&points, const cv::Mat& cornerMap )
{
	//遍历像素得到的所有特征
	for (int y=0;y<cornerMap.rows;y++)
	{
		const uchar* rowPtr = cornerMap.ptr<uchar>(y);
		for (int x=0;x<cornerMap.cols;x++)
		{
			//如果是特征点
			if (rowPtr[x])
			{
				points.push_back(cv::Point(x,y));
			}
		}
	}
}

//在特征点的位置绘制圆
void HarrisDetector::drawOnImage( cv::Mat& image,
							   const std::vector<cv::Point>&points,
							   cv::Scalar color /*= cv::Scalar(255,255,255)*/,
							   int radius/*=3*/, int thickness/*=2*/ )
{
	std::vector<cv::Point>::const_iterator it = points.begin();
	//对于所有角点
	while (it!=points.end())
	{
		//绘制一个圆
		cv::circle(image,*it,radius,color,thickness);
		++it;
	}
}
