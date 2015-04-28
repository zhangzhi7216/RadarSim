#pragma once
#include "stdafx.h"
#include <opencv2/opencv.hpp>
using namespace cv;
class LaplacianBlending
{
private:
	Mat_<Vec3f> left;  
	Mat_<Vec3f> right; 

	vector<Mat_<Vec3f> > leftLapPyr,rightLapPyr,resultLapPyr;//Laplacian Pyramids  
	Mat leftHighestLevel, rightHighestLevel, resultHighestLevel;    
	int levels; 
	void buildPyramids();
	void buildLaplacianPyramid(const Mat& img, vector<Mat_<Vec3f> >& lapPyr, Mat& HighestLevel);
	Mat_<Vec3f> reconstructImgFromLapPyramid();
	void blendLapPyrs();
public:
	LaplacianBlending(void);
	LaplacianBlending(const Mat_<Vec3f>& _left, const Mat_<Vec3f>& _right, int _levels);//construct function, used in LaplacianBlending lb(l,r,m,4);  

	~LaplacianBlending(void);
	Mat_<Vec3f> blend();
	Mat_<Vec3f> LaplacianBlend(const Mat_<Vec3f>& l, const Mat_<Vec3f>& r);
};

