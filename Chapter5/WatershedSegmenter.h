#pragma once
#include <opencv2/opencv.hpp>  
#include<opencv2/core/core.hpp>  
#include<opencv2/highgui/highgui.hpp> 
#include<opencv2/imgproc/imgproc.hpp>  

class WatershedSegmenter
{
public:
	WatershedSegmenter(void);
	void setMarkers(const cv::Mat& markerImage);
	cv::Mat process(const cv::Mat &image);
	~WatershedSegmenter(void);
	cv::Mat getSegmentation();
	cv::Mat getWatersheds();
private:
	cv::Mat markers;
};

