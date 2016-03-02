#include "stdafx.h"
#include "CameraCalibrator.h"





CameraCalibrator::~CameraCalibrator(void)
{
}

// 打开棋盘图像并提取角点
int CameraCalibrator::addChessboardPoints( const std::vector<std::string>& filelist, cv::Size & boardSize )
{
	//棋盘上的点的两种坐标
	std::vector<cv::Point2f>imageCorners;
	std::vector<cv::Point3f>objectCorners;
	// 3D Scene Points:
	// Initialize the chessboard corners 
	// in the chessboard reference frame
	// The corners are at 3D location (X,Y,Z)= (i,j,0)
	for (int i=0; i<boardSize.height; i++) {
		for (int j=0; j<boardSize.width; j++) {

			objectCorners.push_back(cv::Point3f(i, j, 0.0f));
		}
	}
	//2D 图像中的点：
	cv::Mat image;  //为了保存棋盘图像
	int suceesses = 0;
	//for all viewpoints
	for(int i=0;i<filelist.size();i++){
		//打开图像
		image = cv::imread(filelist[i],0);
		//得到角点
		bool found = cv::findChessboardCorners(image,boardSize,imageCorners);
		//获取亚像素精度
		cv::cornerSubPix(image,imageCorners,
			cv::Size(5,5),
			cv::Size(-1,-1),
			cv::TermCriteria(cv::TermCriteria::MAX_ITER+cv::TermCriteria::EPS,
			30,      //最大迭代数目
			0.1));   //最小精度
		//如果角点数目满足要求，那么将它加入数据
		if (imageCorners.size()==boardSize.area())
		{
			//添加一个视角中的图像点及场景点
			addPoints(imageCorners,objectCorners);
			suceesses++;
		}
		//Draw the corners
		cv::drawChessboardCorners(image, boardSize, imageCorners, found);
		cv::imshow("Corners on Chessboard", image);
		cv::waitKey(100);
	}
	return suceesses;
}

//添加场景点与对应的图像点
void CameraCalibrator::addPoints( const std::vector<cv::Point2f>& imageCorners, const std::vector<cv::Point3f>& objectCorners )
{
	//2D图像点
	imagePoints.push_back(imageCorners);
	//对应3D场景中的点
	objectPoints.push_back(objectCorners);
}

//进行标定，返回重投影误差(re-projection error)
double CameraCalibrator::calibrate( cv::Size &imageSize )
{
	//必须重新进行去畸变
	mustInitUndistort = true;
	//输出旋转和平移
	std::vector<cv::Mat> rveces,tvecs;
	//开始标定
	return calibrateCamera(objectPoints,//3D点
		imagePoints,                    //图像点
		imageSize,                      //图像尺寸
		cameraMatrix,                   //输出的相机矩阵
		distCoeffs,                     //输出的畸变矩阵
		rveces,tvecs,                   //旋转和平移
		flag);                          //额外选项
}


// 标定后去除图像中的畸变
cv::Mat CameraCalibrator::remap(const cv::Mat &image) {
	cv::Mat undistorted;
	if (mustInitUndistort)
	{
		//每次标定只需要初始化一次
		cv::initUndistortRectifyMap(
			cameraMatrix,  //计算得到的相机矩阵
			distCoeffs,    //计算得到的畸变矩阵
			cv::Mat(),     //可选的rectification矩阵（此处为空）
			cv::Mat(),     //用于生成undistorted对象的相机矩阵
			image.size(),  //undistorted对象的尺寸
			CV_32FC1,      //输出的映射图像的类型
			map1,map2);    //x坐标和y坐标影射函数
		mustInitUndistort = false;
	}
	//应用映射函数
	cv::remap(image,undistorted,map1,map2,
		cv::INTER_LINEAR);    //插值类型
	return undistorted;
}