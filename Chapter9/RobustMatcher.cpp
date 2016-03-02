#include "stdafx.h"
#include "RobustMatcher.h"


RobustMatcher::RobustMatcher(void):ratio(0.65f),refineF(true),confidence(0.99),distance(3.0)
{
	//SURF为默认特征
	detector = new cv::SurfFeatureDetector();
	extrator = new cv::SurfDescriptorExtractor();
}


RobustMatcher::~RobustMatcher(void)
{
}
//设置特征检测器
void RobustMatcher::setFeatureDetector(cv::Ptr<cv::FeatureDetector>& detect)
{
	detector = detect;
}
//设置描述子提取器
void RobustMatcher::setDescriptorExtractor(cv::Ptr<cv::DescriptorExtractor>& desc)
{
	extrator = desc;
}
// Set the minimum distance to epipolar in RANSAC
void RobustMatcher::setMinDistanceToEpipolar(double d) {

	distance= d;
}

// Set confidence level in RANSAC
void RobustMatcher::setConfidenceLevel(double c) {

	confidence= c;
}

// Set the NN ratio
void RobustMatcher::setRatio(float r) {

	ratio= r;
}

// if you want the F matrix to be recalculated
void RobustMatcher::refineFundamental(bool flag) {

	refineF= flag;
}

//使用对称性测试以及RANSAC匹配特征点
//返回基础矩阵
cv::Mat RobustMatcher::match(cv::Mat& image1,
							 cv::Mat& image2,  //输入图像
							 //输出匹配及特征点
							 std::vector<cv::DMatch>& matches,
							 std::vector<cv::KeyPoint>& keypoints1,
							 std::vector<cv::KeyPoint>& keypoints2)
{
	//1a.测SURF特征
	detector->detect(image1,keypoints1);
	detector->detect(image2,keypoints2);
	//1b.取SURF描述子
	cv::Mat descriptor1,descriptor2;
	extrator->compute(image1,keypoints1,descriptor1);
	extrator->compute(image2,keypoints2,descriptor2);
	//2.配两幅图像的描述子
	//创建匹配图
	cv::BruteForceMatcher<cv::L2<float>>matcher;
	//从图1->图2的k最近邻(K=2)
	std::vector<std::vector<cv::DMatch>>matches1;
	matcher.knnMatch(descriptor1,descriptor2,
		matches1,          //匹配结果的向量（每项有两个值）
		2);               //返回两个最近邻
	//从图2->图1的k最近邻(K=2)
	std::vector<std::vector<cv::DMatch>>matches2;
	matcher.knnMatch(descriptor2,descriptor1,
		matches2,          //匹配结果的向量（每项有两个值）
		2);               //返回两个最近邻
	//3.移除NN比率大于阈值的匹配
	//清理图2->图1的匹配
	int removed = ratioTest(matches1);
	//4.移除非对称的匹配
	std::vector<cv::DMatch>symMatches;
	symmetryTest(matches1,matches2,symMatches);
	//5.使用RANSAC进行最终验证
	cv::Mat fundemental = ransacTest(symMatches,keypoints1,keypoints2,matches);
	//返回找到的基础矩阵
	return fundemental;
}

//移除NN比率大于阈值的匹配
//返回移除点的数目
//(对应的项被清零，即尺寸将为0)
int RobustMatcher::ratioTest(std::vector<std::vector<cv::DMatch>>&matches){
	int removed = 0;
	for (std::vector<std::vector<cv::DMatch>>::iterator matchIterator=matches.begin();
		matchIterator!=matches.end();++matchIterator)
	{
		//如果识别两个最近邻
		if (matchIterator->size()>1)
		{
			//检查距离比率
			if ((*matchIterator)[0].distance/(*matchIterator)[1].distance>ratio)
			{
				matchIterator->clear();  //移除匹配
				removed++;
			}
		}else
		{//不包含两个最近邻
			matchIterator->clear();   //移除匹配
			removed++;
		}
	}
	return removed;
}
//在synMatches向量中插入对称匹配
void RobustMatcher::symmetryTest(
	const std::vector<std::vector<cv::DMatch>>& matches1,
	const std::vector<std::vector<cv::DMatch>>& matches2,
	std::vector<cv::DMatch>& symMatches){
		//遍历图1->图2的所有匹配
		for (std::vector<std::vector<cv::DMatch>>::const_iterator matchIterator1=matches1.begin();
			matchIterator1!=matches1.end();++matchIterator1)
		{
			//忽略被删除的匹配
			if (matchIterator1->size()<2)
			{
				continue;
			}
			//遍历图2->图1的所有匹配
			for (std::vector<std::vector<cv::DMatch>>::const_iterator matchIterator2=matches2.begin();
				matchIterator2!=matches2.end();++matchIterator2)
			{
				//忽略被删除的匹配
				if (matchIterator2->size()<2)
				{
					continue;
				}
				//对称性测试
				if ((*matchIterator1)[0].queryIdx==(*matchIterator2)[0].trainIdx &&
					(*matchIterator2)[0].queryIdx==(*matchIterator1)[0].trainIdx)
				{//添加对称的匹配
					symMatches.push_back(
						cv::DMatch((*matchIterator1)[0].queryIdx,(*matchIterator1)[0].trainIdx,
						(*matchIterator1)[0].distance));
					break;  //图2->图2中的下一个匹配
				}
			}
		}
}
//基于RANSAC识别优质匹配
//返回基础矩阵
cv::Mat RobustMatcher::ransacTest(
	const std::vector<cv::DMatch>& matches,
	const std::vector<cv::KeyPoint>& Keypoints1,
	const std::vector<cv::KeyPoint>& Keypoints2,
	std::vector<cv::DMatch>& outMatches)
{
		//转换KeyPoint类型到Point2f
		std::vector<cv::Point2f>points1,points2;
		for (std::vector<cv::DMatch>::const_iterator it = matches.begin();it!=matches.end();++it)
		{
			//得到左边特征点的坐标
			float x=Keypoints1[it->queryIdx].pt.x;
			float y=Keypoints1[it->queryIdx].pt.y;
			points1.push_back(cv::Point2f(x,y));
			//得到右边特征点的坐标
			x=Keypoints2[it->queryIdx].pt.x;
			y=Keypoints2[it->queryIdx].pt.y;
			points2.push_back(cv::Point2f(x,y));
		}
		//基于RANSAC计算F矩阵
		std::vector<uchar>inliers(points1.size(),0);
		cv::Mat fundamental = cv::findFundamentalMat(
			cv::Mat(points1),cv::Mat(points2),   //匹配点
			inliers,                             //匹配状态(inlier or outlier)
			CV_FM_RANSAC,                        //RANCAC方法
			distance,                            //到极线的距离
			confidence);                         //置信概率
		//提取通过的匹配
		std::vector<uchar>::const_iterator itIn=inliers.begin();
		std::vector<cv::DMatch>::const_iterator itM=matches.begin();
		//遍历所有匹配
		for (;itIn!=inliers.end();++itIn,++itM)
		{
			if (*itIn)//为有效匹配
			{
				outMatches.push_back(*itM);
			}
		}
		if (refineF)
		{
			//F矩阵将使用所有接收的匹配重新计算
			//转换KeyPoint类型到Point2f
			//准备计算最终的F矩阵
			points1.clear();
			points2.clear();
			for (std::vector<cv::DMatch>::const_iterator it = outMatches.begin();it!=outMatches.end();++it)
			{
				//得到左边特征点的坐标
				float x = Keypoints1[it->queryIdx].pt.x;
				float y = Keypoints1[it->queryIdx].pt.y;
				points1.push_back(cv::Point2f(x,y));
				//得到右边特征点的坐标
				x = Keypoints2[it->trainIdx].pt.x;
				y = Keypoints2[it->trainIdx].pt.y;
				points2.push_back(cv::Point2f(x,y));
			}
			//从所有接受的匹配中计算8点F
			fundamental=cv::findFundamentalMat(cv::Mat(points1),cv::Mat(points2),//匹配
				CV_FM_8POINT);  //8点法
		}
		return fundamental;
}