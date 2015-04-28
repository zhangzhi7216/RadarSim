#include "StdAfx.h"
#include "RateBlending.h"



RateBlending::RateBlending(void)
{
}
 void RateBlending::buildPyramids() 
	{  
        buildRatePyramid(left,leftRatePyr,leftHighestLevel);  
        buildRatePyramid(right,rightRatePyr,rightHighestLevel);  
        buildGaussianPyramid();
    } 
 void RateBlending::buildGaussianPyramid()
 {
	 assert(leftRatePyr.size()>0);  
  
        maskGaussianPyramid.clear();  
        Mat currentImg;  
        cvtColor(blendMask, currentImg, CV_GRAY2BGR);//store color img of blend mask into maskGaussianPyramid  
        maskGaussianPyramid.push_back(currentImg); //0-level  
  
        currentImg = blendMask;  
        for (int l=1; l<levels+1; l++) {  
            Mat _down;  
            if (leftRatePyr.size() > l)  
                pyrDown(currentImg, _down, leftRatePyr[l].size());  
            else  
                pyrDown(currentImg, _down, leftHighestLevel.size()); //lowest level  
  
            Mat down;  
            cvtColor(_down, down, CV_GRAY2BGR);  
            maskGaussianPyramid.push_back(down);//add color blend mask into mask Pyramid  
            currentImg = _down;  
        }  
 
 }
void RateBlending::buildRatePyramid(const Mat& img, vector<Mat_<Vec3f> >& RatePyr, Mat& HighestLevel) 
	{  
        RatePyr.clear();  
        Mat currentImg = img;  
        for (int l=0; l<levels; l++) {  
            Mat down,up;  
            pyrDown(currentImg, down);  
            pyrUp(down, up,currentImg.size());  
            Mat lap;
			divide(currentImg,up,lap);  
            RatePyr.push_back(lap);  
            currentImg = down;  
        }  
        currentImg.copyTo(HighestLevel);  
    }  
Mat_<Vec3f> RateBlending::reconstructImgFromRatePyramid() {  
        //将左右laplacian图像拼成的resultLapPyr金字塔中每一层  
        //从上到下插值放大并相加，即得blend图像结果  
        Mat currentImg = resultHighestLevel;  
        for (int l=levels-1; l>=0; l--) {  
            Mat up;   
            pyrUp(currentImg, up, resultRatePyr[l].size()); 
           
			currentImg=up .mul( resultRatePyr[l]);
        }  
        return currentImg;  
    }  
void RateBlending::blendRatePyrs() 
	{  
        //获得每层金字塔中直接用左右两图Laplacian变换拼成的图像resultLapPyr     
			
		resultHighestLevel=0.5*leftHighestLevel+0.5*rightHighestLevel;
	//	max(leftHighestLevel,rightHighestLevel,resultHighestLevel);
		for (int l=0; l<levels; l++)
		{    
			int nr=leftRatePyr[l].rows;
			int nc=leftRatePyr[l].cols ;
			Mat_<Vec3f> blendedLevel=leftRatePyr[l];
			for(int i=0;i<nr;++i)				
			{
				for(int j=0;j<nc;++j)
				{	
					for(int k=0;k<3;k++)
					{
					  if(abs(leftRatePyr[l](i,j)[k]-1)>abs(rightRatePyr[l](i,j)[k]-1)) 						   
				        blendedLevel(i,j)[k]=leftRatePyr[l](i,j)[k];
					  else
						 blendedLevel(i,j)[k]=rightRatePyr[l](i,j)[k];
				  
					}
				  
				}
				
			}
			 
          resultRatePyr.push_back(blendedLevel);
        } 

	/* resultHighestLevel = leftHighestLevel.mul(maskGaussianPyramid.back()) +  
            rightHighestLevel.mul(Scalar(1.0,1.0,1.0) - maskGaussianPyramid.back());  
        for (int l=0; l<levels; l++) {  
            Mat A = leftRatePyr[l].mul(maskGaussianPyramid[l]);  
            Mat antiMask = Scalar(1.0,1.0,1.0) - maskGaussianPyramid[l];  
            Mat B = rightRatePyr[l].mul(antiMask);  
            Mat_<Vec3f> blendedLevel = A + B;  
  
            resultRatePyr.push_back(blendedLevel);  
        }  */
		 
	}  
RateBlending::RateBlending(const Mat_<Vec3f>& _left, const Mat_<Vec3f>& _right,const Mat_<float>& _blendMask, int _levels)://construct function, used in LaplacianBlending lb(l,r,m,4);  
      left(_left),right(_right),blendMask(_blendMask),levels(_levels)  
      {  
          assert(_left.size() == _right.size());  
          assert(_left.size() == _blendMask.size()); 
          buildPyramids();  //construct Laplacian Pyramid and Gaussian Pyramid  
          blendRatePyrs();   //blend left & right Pyramids into one Pyramid  
      };  
Mat_<Vec3f> RateBlending::blend()
	  {  
          return reconstructImgFromRatePyramid();//reconstruct Image from Laplacian Pyramid  
      } 
Mat_<Vec3f> RateBlending::RateBlend(const Mat_<Vec3f>& l, const Mat_<Vec3f>& r,const Mat_<float>& m) 
{  
    RateBlending lb(l,r,m,4);  
    return lb.blend();  
}  
RateBlending::~RateBlending(void)
{
}
