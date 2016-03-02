#include "stdafx.h"
#include "RobustMatcher.h"


RobustMatcher::RobustMatcher(void):ratio(0.65f),refineF(true),confidence(0.99),distance(3.0)
{
	//SURFΪĬ������
	detector = new cv::SurfFeatureDetector();
	extrator = new cv::SurfDescriptorExtractor();
}


RobustMatcher::~RobustMatcher(void)
{
}
//�������������
void RobustMatcher::setFeatureDetector(cv::Ptr<cv::FeatureDetector>& detect)
{
	detector = detect;
}
//������������ȡ��
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

//ʹ�öԳ��Բ����Լ�RANSACƥ��������
//���ػ�������
cv::Mat RobustMatcher::match(cv::Mat& image1,
							 cv::Mat& image2,  //����ͼ��
							 //���ƥ�估������
							 std::vector<cv::DMatch>& matches,
							 std::vector<cv::KeyPoint>& keypoints1,
							 std::vector<cv::KeyPoint>& keypoints2)
{
	//1a.��SURF����
	detector->detect(image1,keypoints1);
	detector->detect(image2,keypoints2);
	//1b.ȡSURF������
	cv::Mat descriptor1,descriptor2;
	extrator->compute(image1,keypoints1,descriptor1);
	extrator->compute(image2,keypoints2,descriptor2);
	//2.������ͼ���������
	//����ƥ��ͼ
	cv::BruteForceMatcher<cv::L2<float>>matcher;
	//��ͼ1->ͼ2��k�����(K=2)
	std::vector<std::vector<cv::DMatch>>matches1;
	matcher.knnMatch(descriptor1,descriptor2,
		matches1,          //ƥ������������ÿ��������ֵ��
		2);               //�������������
	//��ͼ2->ͼ1��k�����(K=2)
	std::vector<std::vector<cv::DMatch>>matches2;
	matcher.knnMatch(descriptor2,descriptor1,
		matches2,          //ƥ������������ÿ��������ֵ��
		2);               //�������������
	//3.�Ƴ�NN���ʴ�����ֵ��ƥ��
	//����ͼ2->ͼ1��ƥ��
	int removed = ratioTest(matches1);
	//4.�Ƴ��ǶԳƵ�ƥ��
	std::vector<cv::DMatch>symMatches;
	symmetryTest(matches1,matches2,symMatches);
	//5.ʹ��RANSAC����������֤
	cv::Mat fundemental = ransacTest(symMatches,keypoints1,keypoints2,matches);
	//�����ҵ��Ļ�������
	return fundemental;
}

//�Ƴ�NN���ʴ�����ֵ��ƥ��
//�����Ƴ������Ŀ
//(��Ӧ������㣬���ߴ罫Ϊ0)
int RobustMatcher::ratioTest(std::vector<std::vector<cv::DMatch>>&matches){
	int removed = 0;
	for (std::vector<std::vector<cv::DMatch>>::iterator matchIterator=matches.begin();
		matchIterator!=matches.end();++matchIterator)
	{
		//���ʶ�����������
		if (matchIterator->size()>1)
		{
			//���������
			if ((*matchIterator)[0].distance/(*matchIterator)[1].distance>ratio)
			{
				matchIterator->clear();  //�Ƴ�ƥ��
				removed++;
			}
		}else
		{//���������������
			matchIterator->clear();   //�Ƴ�ƥ��
			removed++;
		}
	}
	return removed;
}
//��synMatches�����в���Գ�ƥ��
void RobustMatcher::symmetryTest(
	const std::vector<std::vector<cv::DMatch>>& matches1,
	const std::vector<std::vector<cv::DMatch>>& matches2,
	std::vector<cv::DMatch>& symMatches){
		//����ͼ1->ͼ2������ƥ��
		for (std::vector<std::vector<cv::DMatch>>::const_iterator matchIterator1=matches1.begin();
			matchIterator1!=matches1.end();++matchIterator1)
		{
			//���Ա�ɾ����ƥ��
			if (matchIterator1->size()<2)
			{
				continue;
			}
			//����ͼ2->ͼ1������ƥ��
			for (std::vector<std::vector<cv::DMatch>>::const_iterator matchIterator2=matches2.begin();
				matchIterator2!=matches2.end();++matchIterator2)
			{
				//���Ա�ɾ����ƥ��
				if (matchIterator2->size()<2)
				{
					continue;
				}
				//�Գ��Բ���
				if ((*matchIterator1)[0].queryIdx==(*matchIterator2)[0].trainIdx &&
					(*matchIterator2)[0].queryIdx==(*matchIterator1)[0].trainIdx)
				{//��ӶԳƵ�ƥ��
					symMatches.push_back(
						cv::DMatch((*matchIterator1)[0].queryIdx,(*matchIterator1)[0].trainIdx,
						(*matchIterator1)[0].distance));
					break;  //ͼ2->ͼ2�е���һ��ƥ��
				}
			}
		}
}
//����RANSACʶ������ƥ��
//���ػ�������
cv::Mat RobustMatcher::ransacTest(
	const std::vector<cv::DMatch>& matches,
	const std::vector<cv::KeyPoint>& Keypoints1,
	const std::vector<cv::KeyPoint>& Keypoints2,
	std::vector<cv::DMatch>& outMatches)
{
		//ת��KeyPoint���͵�Point2f
		std::vector<cv::Point2f>points1,points2;
		for (std::vector<cv::DMatch>::const_iterator it = matches.begin();it!=matches.end();++it)
		{
			//�õ���������������
			float x=Keypoints1[it->queryIdx].pt.x;
			float y=Keypoints1[it->queryIdx].pt.y;
			points1.push_back(cv::Point2f(x,y));
			//�õ��ұ������������
			x=Keypoints2[it->queryIdx].pt.x;
			y=Keypoints2[it->queryIdx].pt.y;
			points2.push_back(cv::Point2f(x,y));
		}
		//����RANSAC����F����
		std::vector<uchar>inliers(points1.size(),0);
		cv::Mat fundamental = cv::findFundamentalMat(
			cv::Mat(points1),cv::Mat(points2),   //ƥ���
			inliers,                             //ƥ��״̬(inlier or outlier)
			CV_FM_RANSAC,                        //RANCAC����
			distance,                            //�����ߵľ���
			confidence);                         //���Ÿ���
		//��ȡͨ����ƥ��
		std::vector<uchar>::const_iterator itIn=inliers.begin();
		std::vector<cv::DMatch>::const_iterator itM=matches.begin();
		//��������ƥ��
		for (;itIn!=inliers.end();++itIn,++itM)
		{
			if (*itIn)//Ϊ��Чƥ��
			{
				outMatches.push_back(*itM);
			}
		}
		if (refineF)
		{
			//F����ʹ�����н��յ�ƥ�����¼���
			//ת��KeyPoint���͵�Point2f
			//׼���������յ�F����
			points1.clear();
			points2.clear();
			for (std::vector<cv::DMatch>::const_iterator it = outMatches.begin();it!=outMatches.end();++it)
			{
				//�õ���������������
				float x = Keypoints1[it->queryIdx].pt.x;
				float y = Keypoints1[it->queryIdx].pt.y;
				points1.push_back(cv::Point2f(x,y));
				//�õ��ұ������������
				x = Keypoints2[it->trainIdx].pt.x;
				y = Keypoints2[it->trainIdx].pt.y;
				points2.push_back(cv::Point2f(x,y));
			}
			//�����н��ܵ�ƥ���м���8��F
			fundamental=cv::findFundamentalMat(cv::Mat(points1),cv::Mat(points2),//ƥ��
				CV_FM_8POINT);  //8�㷨
		}
		return fundamental;
}