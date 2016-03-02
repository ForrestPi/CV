#include "stdafx.h"
#include "VideoProcessor.h"



VideoProcessor::VideoProcessor(void):callIt(true),delay(0),fnumber(0),stop(false),frameToStop(-1)
{
}


VideoProcessor::~VideoProcessor(void)
{
}
//设置回调函数
void VideoProcessor::setFrameProcessor(
	void(*frameProcessingCallback)(cv::Mat&,cv::Mat&))
{
	// invalidate frame processor class instance
	frameProcessor= 0;
	process=frameProcessingCallback;
}


// set the instance of the class that implements the FrameProcessor interface
void VideoProcessor::setFrameProcessor(FrameProcessor* frameProcessorPtr) {

	// invalidate callback function
	process= 0;
	// this is the frame processor instance that will be called
	frameProcessor= frameProcessorPtr;
	callProcess();
}

//设置视频文件的名称
bool VideoProcessor::setInput(std::string filename)
{
	fnumber=0;
	//释放之前打开过的资源
	capture.release();
	images.clear();
	//打开视频文件
	return capture.open(filename);
}
// set the camera ID
bool VideoProcessor::setInput(int id) {

	fnumber= 0;
	// In case a resource was already 
	// associated with the VideoCapture instance
	capture.release();
	images.clear();

	// Open the video file
	return capture.open(id);
}

// set the vector of input images
bool VideoProcessor::setInput(const std::vector<std::string>& imgs) {

	fnumber= 0;
	// In case a resource was already 
	// associated with the VideoCapture instance
	capture.release();

	// the input will be this vector of images
	images= imgs;
	itImg= images.begin();

	return true;
}

//创建输入窗口
void VideoProcessor::displayInput(std::string wn)
{
	windowNameInput=wn;
	cv::namedWindow(windowNameInput);
}
void VideoProcessor::displayOutput(std::string wn)
{
	windowNameOutput=wn;
	cv::namedWindow(windowNameInput);
}
//不再显示处理后的帧
void VideoProcessor::dontDisplay()
{
	cv::destroyWindow(windowNameInput);
	cv::destroyWindow(windowNameOutput);
	windowNameInput.clear();
	windowNameOutput.clear();
}

// set the output video file
// by default the same parameters than input video will be used
bool VideoProcessor::setOutput(const std::string &filename, int codec, double framerate, bool isColor) {

	outputFile= filename;
	extension.clear();

	if (framerate==0.0) 
		framerate= getFrameRate(); // same as input

	char c[4];
	// use same codec as input
	if (codec==0) { 
		codec= getCodec(c);
	}

	// Open output video
	return writer.open(outputFile, // filename
		codec, // codec to be used 
		framerate,      // frame rate of the video
		getFrameSize(), // frame size
		isColor);       // color video?
}

// set the output as a series of image files
// extension must be ".jpg", ".bmp" ...
bool VideoProcessor::setOutput(const std::string &filename, // filename prefix
			   const std::string &ext, // image file extension 
			   int numberOfDigits,   // number of digits
			   int startIndex) {     // start index

				   // number of digits must be positive
				   if (numberOfDigits<0)
					   return false;

				   // filenames and their common extension
				   outputFile= filename;
				   extension= ext;

				   // number of digits in the file numbering scheme
				   digits= numberOfDigits;
				   // start numbering at this index
				   currentIndex= startIndex;

				   return true;
}





// stop streaming at this frame number
void VideoProcessor::stopAtFrameNo(long frame) {

	frameToStop= frame;
}

// process callback to be called
void VideoProcessor::callProcess() {

	callIt= true;
}

// do not call process callback
void VideoProcessor::dontCallProcess() {

	callIt= false;
}

// to get the next frame 
// could be: video file; camera; vector of images
bool VideoProcessor::readNextFrame(cv::Mat& frame) {

	if (images.size()==0)
		return capture.read(frame);
	else {

		if (itImg != images.end()) {

			frame= cv::imread(*itImg);
			itImg++;
			return frame.data != 0;
		}
	}
}

// to write the output frame 
// could be: video file or images
void VideoProcessor::writeNextFrame(cv::Mat& frame) {

	if (extension.length()) { // then we write images

		std::stringstream ss;
		ss << outputFile << std::setfill('0') << std::setw(digits) << currentIndex++ << extension;
		cv::imwrite(ss.str(),frame);

	} else { // then write video file

		writer.write(frame);
	}
}


// set a delay between each frame
// 0 means wait at each frame
// negative means no delay
void VideoProcessor::setDelay(int d) {

	delay= d;
}

// a count is kept of the processed frames
long VideoProcessor::getNumberOfProcessedFrames() {

	return fnumber;
}

// return the size of the video frame
cv::Size VideoProcessor::getFrameSize() {

	if (images.size()==0) {

		// get size of from the capture device
		int w= static_cast<int>(capture.get(CV_CAP_PROP_FRAME_WIDTH));
		int h= static_cast<int>(capture.get(CV_CAP_PROP_FRAME_HEIGHT));

		return cv::Size(w,h);

	} else { // if input is vector of images

		cv::Mat tmp= cv::imread(images[0]);
		if (!tmp.data) return cv::Size(0,0);
		else return tmp.size();
	}
}

// return the frame number of the next frame
long VideoProcessor::getFrameNumber() {

	if (images.size()==0) {

		// get info of from the capture device
		long f= static_cast<long>(capture.get(CV_CAP_PROP_POS_FRAMES));
		return f; 

	} else { // if input is vector of images

		return static_cast<long>(itImg-images.begin());
	}
}

// return the position in ms
double VideoProcessor::getPositionMS() {

	// undefined for vector of images
	if (images.size()!=0) return 0.0;

	double t= capture.get(CV_CAP_PROP_POS_MSEC);
	return t; 
}

// return the frame rate
double VideoProcessor::getFrameRate() {

	// undefined for vector of images
	if (images.size()!=0) return 0;

	double r= capture.get(CV_CAP_PROP_FPS);
	return r; 
}

// return the number of frames in video
long VideoProcessor::getTotalFrameCount() {

	// for vector of images
	if (images.size()!=0) return images.size();

	long t= capture.get(CV_CAP_PROP_FRAME_COUNT);
	return t; 
}

// get the codec of input video
int VideoProcessor::getCodec(char codec[4]) {

	// undefined for vector of images
	if (images.size()!=0) return -1;

	union {
		int value;
		char code[4]; } returned;

		returned.value= static_cast<int>(capture.get(CV_CAP_PROP_FOURCC));

		codec[0]= returned.code[0];
		codec[1]= returned.code[1];
		codec[2]= returned.code[2];
		codec[3]= returned.code[3];

		return returned.value;
}

// go to this frame number
bool VideoProcessor::setFrameNumber(long pos) {

	// for vector of images
	if (images.size()!=0) {

		// move to position in vector
		itImg= images.begin() + pos;
		// is it a valid position?
		if (pos < images.size())
			return true;
		else
			return false;

	} else { // if input is a capture device

		return capture.set(CV_CAP_PROP_POS_FRAMES, pos);
	}
}

// go to this position
bool VideoProcessor::setPositionMS(double pos) {

	// not defined in vector of images
	if (images.size()!=0) 
		return false;
	else 
		return capture.set(CV_CAP_PROP_POS_MSEC, pos);
}

// go to this position expressed in fraction of total film length
bool VideoProcessor::setRelativePosition(double pos) {

	// for vector of images
	if (images.size()!=0) {

		// move to position in vector
		long posI= static_cast<long>(pos*images.size()+0.5);
		itImg= images.begin() + posI;
		// is it a valid position?
		if (posI < images.size())
			return true;
		else
			return false;

	} else { // if input is a capture device

		return capture.set(CV_CAP_PROP_POS_AVI_RATIO, pos);
	}
}

// Stop the processing
void VideoProcessor::stopIt() {

	stop= true;
}

// Is the process stopped?
bool VideoProcessor::isStopped() {

	return stop;
}

// Is a capture device opened?
bool VideoProcessor::isOpened() {

	return capture.isOpened() || !images.empty();
}

// to grab (and process) the frames of the sequence
void VideoProcessor::run() {

	// current frame
	cv::Mat frame;
	// output frame
	cv::Mat output;

	// if no capture device has been set
	if (!isOpened())
		return;

	stop= false;

	while (!isStopped()) {

		// read next frame if any
		if (!readNextFrame(frame))
			break;

		// display input frame
		if (windowNameInput.length()!=0) 
			cv::imshow(windowNameInput,frame);

		// calling the process function or method
		if (callIt) {

			// process the frame
			if (process)
				process(frame, output);
			else if (frameProcessor) 
				frameProcessor->process(frame,output);
			// increment frame number
			fnumber++;

		} else {

			output= frame;
		}

		// write output sequence
		if (outputFile.length()!=0)
			writeNextFrame(output);

		// display output frame
		if (windowNameOutput.length()!=0) 
			cv::imshow(windowNameOutput,output);

		// introduce a delay
		if (delay>=0 && cv::waitKey(delay)>=0)
			stopIt();

		// check if we should stop
		if (frameToStop>=0 && getFrameNumber()==frameToStop)
			stopIt();
	}
}