#include "stdafx.h"
#include "HarrisDetector.h"


HarrisDetector::HarrisDetector(void):neighbourhood(3),aperture(3),k(0.01),maxStrength(0.0),threshold(0.01),nonMaxSize(3)
{
	//�����Ǽ���ֵ���Ƶĺ�
	setLoaclMaxWindowSize(nonMaxSize);
}


HarrisDetector::~HarrisDetector(void)
{
}

//����Harris�ǵ�
void HarrisDetector::detect( const cv::Mat& image )
{
	//harris����
	cv::cornerHarris(image,cornerStrength,
		neighbourhood,      //neighbourhood size
		aperture,           //�˲����Ŀ׾���С
		k);                 //Harris����
	//�ڲ���ֵ����
	double minStrength;
	cv::minMaxLoc(cornerStrength,&minStrength,&maxStrength);
	//�ֲ�����ֵ���
	cv::Mat dilated;    //��ʱͼ��
	//Ĭ��3*3�����ͣ�����֮�󣬳��˾ֲ����ֵ���ԭ����ͬ�������Ǿֲ����ֵ�㱻  
	//3*3�����ڵ����ֵ��ȡ��
	cv::dilate(cornerStrength,dilated,cv::Mat());
	//��ԭͼ��ȣ�ֻʣ�º�ԭͼֵ��ͬ�ĵ㣬��Щ�㶼�Ǿֲ����ֵ�㣬���浽localMax  
	cv::compare(cornerStrength,dilated,
		localMax,cv::CMP_EQ);
}

//��Harrisֵ��ȡ�ǵ�ͼ
cv::Mat HarrisDetector::getCornerMap( double qualityLevel )
{
	cv::Mat cornerMap;
	//�Խǵ�ͼ�������ֵ��
	threshold = qualityLevel*maxStrength;
	cv::threshold(cornerStrength,cornerTh,
		threshold,255,cv::THRESH_BINARY);
	//ת��Ϊ8λͼ��
	cornerTh.convertTo(cornerMap,CV_8U);
	//�Ǽ���ֵ����
	cv::bitwise_and(cornerMap,localMax,cornerMap);
	return cornerMap;
}

void HarrisDetector::setLoaclMaxWindowSize( int size )
{
	nonMaxSize= size;
	kernel.create(nonMaxSize,nonMaxSize,CV_8U);
}

//��Harrisֵ��ȡ������
void HarrisDetector::getCorners( std::vector<cv::Point>&points, double qualityLevel )
{
	//�õ��ǵ�ͼ
	cv::Mat cornerMap = getCornerMap(qualityLevel);
	//�õ��ǵ�
	getCorners(points,cornerMap);
}

//�ɽǵ�ͼ��ȡ������
void HarrisDetector::getCorners( std::vector<cv::Point>&points, const cv::Mat& cornerMap )
{
	//�������صõ�����������
	for (int y=0;y<cornerMap.rows;y++)
	{
		const uchar* rowPtr = cornerMap.ptr<uchar>(y);
		for (int x=0;x<cornerMap.cols;x++)
		{
			//�����������
			if (rowPtr[x])
			{
				points.push_back(cv::Point(x,y));
			}
		}
	}
}

//���������λ�û���Բ
void HarrisDetector::drawOnImage( cv::Mat& image,
							   const std::vector<cv::Point>&points,
							   cv::Scalar color /*= cv::Scalar(255,255,255)*/,
							   int radius/*=3*/, int thickness/*=2*/ )
{
	std::vector<cv::Point>::const_iterator it = points.begin();
	//�������нǵ�
	while (it!=points.end())
	{
		//����һ��Բ
		cv::circle(image,*it,radius,color,thickness);
		++it;
	}
}
