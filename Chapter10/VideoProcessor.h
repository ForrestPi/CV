#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iomanip>

// The frame processor interface
class FrameProcessor {

public:
	// processing method
	virtual void process(cv:: Mat &input, cv:: Mat &output)= 0;
};

class VideoProcessor
{
public:
	VideoProcessor(void);
	~VideoProcessor(void);
	
	bool setInput(std::string filename);
	bool setInput(int id);
	bool setInput(const std::vector<std::string>& imgs);
	void displayInput(std::string wn);
	void displayOutput(std::string wn);
	void dontDisplay();
	bool setOutput(const std::string &filename, int codec=0, double framerate=0.0, bool isColor=true);
	bool setOutput(const std::string &filename, const std::string &ext,int numberOfDigits=3, int startIndex=0);
	void setFrameProcessor(void(*fameProcessingCallback)(cv::Mat&,cv::Mat&));
	void setFrameProcessor(FrameProcessor* frameProcessorPtr);
	void stopAtFrameNo(long frame);
	void callProcess();
	void dontCallProcess();
	void setDelay(int d);
	long getNumberOfProcessedFrames();
	cv::Size getFrameSize();
	long getFrameNumber();
	double getPositionMS();
	double getFrameRate();
	long getTotalFrameCount();
	int getCodec(char codec[4]);
	bool setFrameNumber(long pos);
	bool setPositionMS(double pos);
	bool setRelativePosition(double pos);
	void stopIt();
	bool isStopped();
	bool isOpened();
	void run();

private:
	//opencv��Ƶ��׽����
	cv::VideoCapture capture;
	//ÿ֡���õĻص�����
	void(*process)(cv::Mat&,cv::Mat&);
	
	// the pointer to the class implementing 
	// the FrameProcessor interface
	FrameProcessor *frameProcessor;
	//ȷ���Ƿ���ûص�����bool����
	bool callIt;
	//���봰�ڵ�����
	std::string windowNameInput;
	//������ڵ�����
	std::string windowNameOutput;
	//�ӳ�
	int delay;
	//�Ѵ����֡��
	long fnumber;
	//�ڸ�֡��ֹͣ
	long frameToStop;
	//�Ƿ�ֹͣ����
	bool stop;
	// vector of image filename to be used as input
	std::vector<std::string> images; 
	// image vector iterator
	std::vector<std::string>::const_iterator itImg;

	// the OpenCV video writer object
	cv::VideoWriter writer;
	// output filename
	std::string outputFile;

	// current index for output images
	int currentIndex;
	// number of digits in output image filename
	int digits;
	// extension of output images
	std::string extension;

	bool readNextFrame(cv::Mat& frame);
	void writeNextFrame(cv::Mat& frame);
	
};

