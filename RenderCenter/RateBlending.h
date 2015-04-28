#pragma once
#include "stdafx.h"
#include <opencv2/opencv.hpp>
using namespace cv;
class RateBlending
{
public:
	Mat_<Vec3f> left;  
	Mat_<Vec3f> right;
	Mat_<float> blendMask;

	vector<Mat_<Vec3f> > leftRatePyr,rightRatePyr,resultRatePyr;//Laplacian Pyramids  
	Mat leftHighestLevel, rightHighestLevel, resultHighestLevel;
	vector<Mat_<Vec3f> > maskGaussianPyramid;
	int levels; 
	void buildPyramids();
	void buildRatePyramid(const Mat& img, vector<Mat_<Vec3f> >& RatePyr, Mat& HighestLevel);
	Mat_<Vec3f> reconstructImgFromRatePyramid();
	void blendRatePyrs();
public:
	RateBlending(void);
	RateBlending(const Mat_<Vec3f>& _left, const Mat_<Vec3f>& _right,const Mat_<float>& _blendMask, int _levels);//construct function, used in LaplacianBlending lb(l,r,m,4);  
	void buildGaussianPyramid();
	~RateBlending(void);
	Mat_<Vec3f> blend();
	Mat_<Vec3f> RateBlend(const Mat_<Vec3f>& l, const Mat_<Vec3f>& r, const Mat_<float>& m);
};

