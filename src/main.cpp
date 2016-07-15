#include <stdio.h>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/xfeatures2d.hpp"
#include <algorithm>
#include "feature.hpp"

#define GOOD_MATCH_NUM 20 
#define NEG_MATCH_NUM 5
using namespace cv;
using namespace cv::xfeatures2d;
void readme();
/* @function main */
int main( int argc, char** argv )
{
		if( argc != 3 )
		{ return -1; }
		Mat img_1 = imread( argv[1]);
		Mat img_2 = imread( argv[2]);
		if( !img_1.data || !img_2.data )
		{ return -1; }
		std::vector<std::array<cv::Point2f, 2> > pos_matches, neg_matches;
		std::vector<double> pos_match_dist, neg_match_dist;
		sift_match(img_1, img_2, GOOD_MATCH_NUM, NEG_MATCH_NUM, pos_matches, pos_match_dist, neg_matches, neg_match_dist);
		
}
