#include "StdAfx.h"
#include "FeatureMatch.h"

FeatureMatch::FeatureMatch(void)
{
}


FeatureMatch::~FeatureMatch(void)
{
}
int FeatureMatch::getMatcherFilterType(const string& str)
{
	if( str == "NoneFilter" )
		return NONE_FILTER;
	if( str == "CrossCheckFilter" )
		return CROSS_CHECK_FILTER;
	CV_Error(CV_StsBadArg, "Invalid filter name");
	return -1;
}
void FeatureMatch::simpleMatching( Ptr<DescriptorMatcher>& descriptorMatcher,
	const Mat& descriptors1, const Mat& descriptors2,
	vector<DMatch>& matches12 )
{
	vector<DMatch> matches;
	descriptorMatcher->match( descriptors1, descriptors2, matches12 );
}
void FeatureMatch::crossCheckMatching(Ptr<DescriptorMatcher>& descriptorMatcher,
	const Mat& descriptors1, const Mat& descriptors2,
	vector<DMatch>& filteredMatches12, int knn=1)
{
	filteredMatches12.clear();
	vector<vector<DMatch> > matches12, matches21;
	descriptorMatcher->knnMatch( descriptors1, descriptors2, matches12, knn );
	descriptorMatcher->knnMatch( descriptors2, descriptors1, matches21, knn );
	for( size_t m = 0; m < matches12.size(); m++ )
	{
		bool findCrossCheck = false;
		for( size_t fk = 0; fk < matches12[m].size(); fk++ )
		{
			DMatch forward = matches12[m][fk];

			for( size_t bk = 0; bk < matches21[forward.trainIdx].size(); bk++ )
			{
				DMatch backward = matches21[forward.trainIdx][bk];
				if( backward.trainIdx == forward.queryIdx )
				{
					filteredMatches12.push_back(forward);
					findCrossCheck = true;
					break;
				}
			}
			if( findCrossCheck ) break;
		}
	}
}
void FeatureMatch::warpPerspectiveRand( const Mat& src, Mat& dst, Mat& H, RNG& rng )
{
	H.create(3, 3, CV_32FC1);
	H.at<float>(0,0) = rng.uniform( 0.8f, 1.2f);
	H.at<float>(0,1) = rng.uniform(-0.1f, 0.1f);
	H.at<float>(0,2) = rng.uniform(-0.1f, 0.1f)*src.cols;
	H.at<float>(1,0) = rng.uniform(-0.1f, 0.1f);
	H.at<float>(1,1) = rng.uniform( 0.8f, 1.2f);
	H.at<float>(1,2) = rng.uniform(-0.1f, 0.1f)*src.rows;
	H.at<float>(2,0) = rng.uniform( -1e-4f, 1e-4f);
	H.at<float>(2,1) = rng.uniform( -1e-4f, 1e-4f);
	H.at<float>(2,2) = rng.uniform( 0.8f, 1.2f);

	warpPerspective( src, dst, H, src.size() );
}
Mat FeatureMatch::doIteration(const Mat& img1, Mat& img2, bool isWarpPerspective,
	vector<KeyPoint>& keypoints1, const Mat& descriptors1,
	Ptr<FeatureDetector>& detector, Ptr<DescriptorExtractor>& descriptorExtractor,
	Ptr<DescriptorMatcher>& descriptorMatcher, int matcherFilter, bool eval,
	double ransacReprojThreshold, RNG& rng)
{
	assert( !img1.empty() );
	Mat H12;
	if( isWarpPerspective )
		warpPerspectiveRand(img1, img2, H12, rng );
	else
		assert( !img2.empty() );
	vector<KeyPoint> keypoints2;
	detector->detect( img2, keypoints2 );
	if( !H12.empty() && eval )
	{		
		float repeatability;
		int correspCount;
		evaluateFeatureDetector( img1, img2, H12, &keypoints1, &keypoints2, repeatability, correspCount );

	}
	Mat descriptors2;
	descriptorExtractor->compute( img2, keypoints2, descriptors2 );
	vector<DMatch> filteredMatches;
	switch( matcherFilter )
	{
	case CROSS_CHECK_FILTER :
		crossCheckMatching( descriptorMatcher, descriptors1, descriptors2, filteredMatches, 1 );
		break;
	default :
		simpleMatching( descriptorMatcher, descriptors1, descriptors2, filteredMatches );
	}
	if( !H12.empty() && eval )
	{

		vector<Point2f> curve;
		Ptr<GenericDescriptorMatcher> gdm = new VectorDescriptorMatcher( descriptorExtractor, descriptorMatcher );
		evaluateGenericDescriptorMatcher( img1, img2, H12, keypoints1, keypoints2, 0, 0, curve, gdm );

		Point2f firstPoint = *curve.begin();
		Point2f lastPoint = *curve.rbegin();
		int prevPointIndex = -1;

		for( float l_p = 0; l_p <= 1 + FLT_EPSILON; l_p+=0.05f )
		{
			int nearest = getNearestPoint( curve, l_p );
			if( nearest >= 0 )
			{
				Point2f curPoint = curve[nearest];
				if( curPoint.x > firstPoint.x && curPoint.x < lastPoint.x && nearest != prevPointIndex )
				{

					prevPointIndex = nearest;
				}
			}
		}

	}
	vector<int> queryIdxs( filteredMatches.size() ), trainIdxs( filteredMatches.size() );
	for( size_t i = 0; i < filteredMatches.size(); i++ )
	{
		queryIdxs[i] = filteredMatches[i].queryIdx;
		trainIdxs[i] = filteredMatches[i].trainIdx;
	}
	if( !isWarpPerspective && ransacReprojThreshold >= 0 )
	{

		vector<Point2f> points1; KeyPoint::convert(keypoints1, points1, queryIdxs);
		vector<Point2f> points2; KeyPoint::convert(keypoints2, points2, trainIdxs);
		H12 = findHomography( Mat(points1), Mat(points2), CV_RANSAC, ransacReprojThreshold );

	}
	if( !H12.empty() ) 
	{
		vector<char> matchesMask( filteredMatches.size(), 0 );
		vector<Point2f> points1; KeyPoint::convert(keypoints1, points1, queryIdxs);
		vector<Point2f> points2; KeyPoint::convert(keypoints2, points2, trainIdxs);
		Mat points1t; perspectiveTransform(Mat(points1), points1t, H12);

		double maxInlierDist = ransacReprojThreshold < 0 ? 3 : ransacReprojThreshold;
		for( size_t i1 = 0; i1 < points1.size(); i1++ )
		{
			if( norm(points2[i1] - points1t.at<Point2f>((int)i1,0)) <= maxInlierDist ) // inlier
				matchesMask[i1] = 1;
		}
		Mat imageturn;
		warpPerspective(img1,imageturn,H12,img1.size());
		return imageturn;
	}
}