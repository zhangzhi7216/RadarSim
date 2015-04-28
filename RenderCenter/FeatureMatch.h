#pragma once
#include "stdafx.h"
#define DRAW_RICH_KEYPOINTS_MODE     0
#define DRAW_OUTLIERS_MODE           0
enum { NONE_FILTER = 0, CROSS_CHECK_FILTER = 1 };
class FeatureMatch
{
public:
	FeatureMatch(void);
	~FeatureMatch(void);
	int getMatcherFilterType( const string& str );
	void simpleMatching( Ptr<DescriptorMatcher>& descriptorMatcher,
		const Mat& descriptors1, const Mat& descriptors2,
		vector<DMatch>& matches12 );
	void crossCheckMatching( Ptr<DescriptorMatcher>& descriptorMatcher,
		const Mat& descriptors1, const Mat& descriptors2,
		vector<DMatch>& filteredMatches12, int knn );
	void warpPerspectiveRand( const Mat& src, Mat& dst, Mat& H, RNG& rng );
	Mat doIteration( const Mat& img1, Mat& img2, bool isWarpPerspective,
		vector<KeyPoint>& keypoints1, const Mat& descriptors1,
		Ptr<FeatureDetector>& detector, Ptr<DescriptorExtractor>& descriptorExtractor,
		Ptr<DescriptorMatcher>& descriptorMatcher, int matcherFilter, bool eval,
		double ransacReprojThreshold, RNG& rng );
};

