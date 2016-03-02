#pragma once
#include <opencv2/opencv.hpp>  
#include<opencv2/core/core.hpp>  
#include<opencv2/highgui/highgui.hpp> 
#include<opencv2/imgproc/imgproc.hpp> 
#include <opencv2/features2d/features2d.hpp>
#include<opencv2/nonfree/features2d.hpp>
#include<opencv2/nonfree/nonfree.hpp>
#include<opencv2/legacy/legacy.hpp>

class RobustMatcher
{
public:
	RobustMatcher(void);
	~RobustMatcher(void);
	void setFeatureDetector(cv::Ptr<cv::FeatureDetector>& detect);
	void setDescriptorExtractor(cv::Ptr<cv::DescriptorExtractor>& desc);
	void setMinDistanceToEpipolar(double d);
	void setConfidenceLevel(double c);
	void setRatio(float r);
	void refineFundamental(bool flag);
	cv::Mat match(cv::Mat& image1, cv::Mat& image2, std::vector<cv::DMatch>& matches, std::vector<cv::KeyPoint>& keypoints1, std::vector<cv::KeyPoint>& keypoints2);
	int ratioTest(std::vector<std::vector<cv::DMatch>>&matches);
	cv::Mat ransacTest( const std::vector<cv::DMatch>& matches, const std::vector<cv::KeyPoint>& Keypoints1, const std::vector<cv::KeyPoint>& Keypoints2, std::vector<cv::DMatch>& outMatches);
	void symmetryTest( const std::vector<std::vector<cv::DMatch>>& matches1, const std::vector<std::vector<cv::DMatch>>& matches2, std::vector<cv::DMatch>& symMatches);
	
private:
	//指定特征检测器的智能指针
	cv::Ptr<cv::FeatureDetector>detector;
	//指向描述子提取器的智能指针
	cv::Ptr<cv::DescriptorExtractor>extrator;
	float ratio;             //第一个以及第二个最近邻之间的最大比率
	bool refineF;            //是否改善F矩阵
	double distance;         //到极线的最小距离
	double confidence;       //置信等级（概率）

};

