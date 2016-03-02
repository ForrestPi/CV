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
	//��ʾ�ǵ�ǿ�ȵ�32λ������
	cv::Mat cornerStrength;
	//��ʾ��ֵ����Ƕȵ�32λ������ͼ��
	cv::Mat cornerTh;
	//�ֲ�����ֵͼ���ڲ���
	cv::Mat localMax;
	//����ƽ�����������صĳߴ�
	int neighbourhood;
	//�ݶȼ���Ŀ׾���С
	int aperture;        //sobel��Ե��ⴰ�ڴ�С��sobel��ȡ�����ص�x��y����ĻҶȵ�����
	//Harris����
	double k;
	//��ֵ��������ǿ��
	double maxStrength;
    //����õ�����ֵ���ڲ���
	double threshold;
	//�Ǽ���ֵ���Ƶ��������صĳߴ�
	int nonMaxSize;
	//�Ǽ���ֵ���Ƶĺ�
	cv::Mat kernel;

};

