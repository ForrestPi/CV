#include "stdafx.h"
#include "CameraCalibrator.h"





CameraCalibrator::~CameraCalibrator(void)
{
}

// ������ͼ����ȡ�ǵ�
int CameraCalibrator::addChessboardPoints( const std::vector<std::string>& filelist, cv::Size & boardSize )
{
	//�����ϵĵ����������
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
	//2D ͼ���еĵ㣺
	cv::Mat image;  //Ϊ�˱�������ͼ��
	int suceesses = 0;
	//for all viewpoints
	for(int i=0;i<filelist.size();i++){
		//��ͼ��
		image = cv::imread(filelist[i],0);
		//�õ��ǵ�
		bool found = cv::findChessboardCorners(image,boardSize,imageCorners);
		//��ȡ�����ؾ���
		cv::cornerSubPix(image,imageCorners,
			cv::Size(5,5),
			cv::Size(-1,-1),
			cv::TermCriteria(cv::TermCriteria::MAX_ITER+cv::TermCriteria::EPS,
			30,      //��������Ŀ
			0.1));   //��С����
		//����ǵ���Ŀ����Ҫ����ô������������
		if (imageCorners.size()==boardSize.area())
		{
			//���һ���ӽ��е�ͼ��㼰������
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

//��ӳ��������Ӧ��ͼ���
void CameraCalibrator::addPoints( const std::vector<cv::Point2f>& imageCorners, const std::vector<cv::Point3f>& objectCorners )
{
	//2Dͼ���
	imagePoints.push_back(imageCorners);
	//��Ӧ3D�����еĵ�
	objectPoints.push_back(objectCorners);
}

//���б궨��������ͶӰ���(re-projection error)
double CameraCalibrator::calibrate( cv::Size &imageSize )
{
	//�������½���ȥ����
	mustInitUndistort = true;
	//�����ת��ƽ��
	std::vector<cv::Mat> rveces,tvecs;
	//��ʼ�궨
	return calibrateCamera(objectPoints,//3D��
		imagePoints,                    //ͼ���
		imageSize,                      //ͼ��ߴ�
		cameraMatrix,                   //������������
		distCoeffs,                     //����Ļ������
		rveces,tvecs,                   //��ת��ƽ��
		flag);                          //����ѡ��
}


// �궨��ȥ��ͼ���еĻ���
cv::Mat CameraCalibrator::remap(const cv::Mat &image) {
	cv::Mat undistorted;
	if (mustInitUndistort)
	{
		//ÿ�α궨ֻ��Ҫ��ʼ��һ��
		cv::initUndistortRectifyMap(
			cameraMatrix,  //����õ����������
			distCoeffs,    //����õ��Ļ������
			cv::Mat(),     //��ѡ��rectification���󣨴˴�Ϊ�գ�
			cv::Mat(),     //��������undistorted������������
			image.size(),  //undistorted����ĳߴ�
			CV_32FC1,      //�����ӳ��ͼ�������
			map1,map2);    //x�����y����Ӱ�亯��
		mustInitUndistort = false;
	}
	//Ӧ��ӳ�亯��
	cv::remap(image,undistorted,map1,map2,
		cv::INTER_LINEAR);    //��ֵ����
	return undistorted;
}