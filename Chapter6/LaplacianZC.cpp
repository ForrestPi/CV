#include "stdafx.h"
#include "LaplacianZC.h"


LaplacianZC::LaplacianZC(void):aperture(3)
{
}


LaplacianZC::~LaplacianZC(void)
{
}

void LaplacianZC::setAperture( int a )
{
	aperture= a;
}

int LaplacianZC::getAperture() const
{
	return aperture;
}

cv::Mat LaplacianZC::computeLaplacian( const cv::Mat& image )
{
	// Compute Laplacian
	cv::Laplacian(image,laplace,CV_32F,aperture);

	// Keep local copy of the image
	// (used for zero-crossings)
	img= image.clone();

	return laplace;
}

cv::Mat LaplacianZC::getLaplacianImage( double scale/*=-1.0*/ )
{
	if (scale<0) {

		double lapmin, lapmax;
		cv::minMaxLoc(laplace,&lapmin,&lapmax);

		scale= 127/ std::max(-lapmin,lapmax);
	}

	cv::Mat laplaceImage;
	laplace.convertTo(laplaceImage,CV_8U,scale,128);

	return laplaceImage;
}

cv::Mat LaplacianZC::getZeroCrossings( float threshold/*=1.0*/ )
{
	// Create the iterators
	cv::Mat_<float>::const_iterator it= laplace.begin<float>()+laplace.step1();
	cv::Mat_<float>::const_iterator itend= laplace.end<float>();
	cv::Mat_<float>::const_iterator itup= laplace.begin<float>();

	// Binary image initialize to white
	cv::Mat binary(laplace.size(),CV_8U,cv::Scalar(255));
	cv::Mat_<uchar>::iterator itout= binary.begin<uchar>()+binary.step1();

	// negate the input threshold value
	threshold *= -1.0;

	for ( ; it!= itend; ++it, ++itup, ++itout) {

		// if the product of two adjascent pixel is negative
		// then there is a sign change
		if (*it * *(it-1) < threshold)
			*itout= 0; // horizontal zero-crossing
		else if (*it * *itup < threshold)
			*itout= 0; // vertical zero-crossing
	}

	return binary;
}

cv::Mat LaplacianZC::getZeroCrossingsWithSobel( float threshold )
{
	cv::Mat sx;
	cv::Sobel(img,sx,CV_32F,1,0,1);
	cv::Mat sy;
	cv::Sobel(img,sy,CV_32F,0,1,1);

	// Create the iterators
	cv::Mat_<float>::const_iterator it= laplace.begin<float>()+laplace.step1();
	cv::Mat_<float>::const_iterator itend= laplace.end<float>();
	cv::Mat_<float>::const_iterator itup= laplace.begin<float>();
	cv::Mat_<float>::const_iterator itx= sx.begin<float>()+sx.step1();
	cv::Mat_<float>::const_iterator ity= sy.begin<float>()+sy.step1();

	// Binary image initialize to white
	cv::Mat binary(laplace.size(),CV_8U,cv::Scalar(255));
	cv::Mat_<uchar>::iterator itout= binary.begin<uchar>()+binary.step1();

	for ( ; it!= itend; ++it, ++itup, ++itout, ++itx, ++ity) {

		// if the product of two adjacent pixel is negative
		// then there is a sign change
		if (*it * *(it-1) < 0.0 && fabs(*ity) > threshold)
			*itout= 0; // horizontal zero-crossing
		else if (*it * *itup < 0.0 && fabs(*ity) > threshold)
			*itout= 0; // vertical zero-crossing
	}

	return binary;
}
