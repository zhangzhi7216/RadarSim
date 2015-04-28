#include "StdAfx.h"
#include "LaplacianBlending.h"


LaplacianBlending::LaplacianBlending(void)
{
}
void LaplacianBlending::buildPyramids() 
{  
	buildLaplacianPyramid(left,leftLapPyr,leftHighestLevel);  
	buildLaplacianPyramid(right,rightLapPyr,rightHighestLevel);  

}    
void LaplacianBlending::buildLaplacianPyramid(const Mat& img, vector<Mat_<Vec3f> >& lapPyr, Mat& HighestLevel) 
{  
	lapPyr.clear();  
	Mat currentImg = img;  
	for (int l=0; l<levels; l++) {  
		Mat down,up;  
		pyrDown(currentImg, down);  
		pyrUp(down, up,currentImg.size());  
		Mat lap = currentImg - up;  
		lapPyr.push_back(lap);  
		currentImg = down;  
	}  
	currentImg.copyTo(HighestLevel);  
}  
Mat_<Vec3f> LaplacianBlending::reconstructImgFromLapPyramid() {  
	//将左右laplacian图像拼成的resultLapPyr金字塔中每一层  
	//从上到下插值放大并相加，即得blend图像结果  
	Mat currentImg = resultHighestLevel;  
	for (int l=levels-1; l>=0; l--) {  
		Mat up;   
		pyrUp(currentImg, up, resultLapPyr[l].size());  
		currentImg = up + resultLapPyr[l];  
	}  
	return currentImg;  
}  
void LaplacianBlending::blendLapPyrs() 
{  
	//获得每层金字塔中直接用左右两图Laplacian变换拼成的图像resultLapPyr     

	Mat_<Vec3f> blendedLevel;		
	resultHighestLevel=0.5*leftHighestLevel+0.5*rightHighestLevel;
	//	min(leftHighestLevel,rightHighestLevel,resultHighestLevel);
	for (int l=0; l<levels; l++)
	{    
		int nr=leftLapPyr[l].rows;
		int nc=leftLapPyr[l].cols ;
		Mat_<Vec3f> blendedLevel=leftLapPyr[l];
		for(int i=0;i<nr;++i)				
		{
			for(int j=0;j<nc;++j)
			{	
				for(int k=0;k<3;k++)
				{
					if(abs(leftLapPyr[l](i,j)[k])>abs(rightLapPyr[l](i,j)[k])) 						   
						blendedLevel(i,j)[k]=leftLapPyr[l](i,j)[k];
					else
						blendedLevel(i,j)[k]=rightLapPyr[l](i,j)[k];

				}

			}

		}

		resultLapPyr.push_back(blendedLevel);
	} 

}  
LaplacianBlending::LaplacianBlending(const Mat_<Vec3f>& _left, const Mat_<Vec3f>& _right, int _levels)://construct function, used in LaplacianBlending lb(l,r,m,4);  
left(_left),right(_right),levels(_levels)  
{  
	assert(_left.size() == _right.size());  

	buildPyramids();  //construct Laplacian Pyramid and Gaussian Pyramid  
	blendLapPyrs();   //blend left & right Pyramids into one Pyramid  
};  
Mat_<Vec3f> LaplacianBlending::blend()
{  
	return reconstructImgFromLapPyramid();//reconstruct Image from Laplacian Pyramid  
} 
Mat_<Vec3f> LaplacianBlending::LaplacianBlend(const Mat_<Vec3f>& l, const Mat_<Vec3f>& r) 
{  
	LaplacianBlending lb(l,r,4);  
	return lb.blend();  
}  
LaplacianBlending::~LaplacianBlending(void)
{
}
