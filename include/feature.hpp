#ifndef FEATURE_HPP
#define FEATURE_HPP 
#include <stdio.h>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <algorithm>
#include <vector>
#include <array>
using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

int sift_match(cv::Mat &img1, cv::Mat & img_2, int pos_match_num,
				int neg_match_num, std::vector<std::array<cv::Point2f, 2> > & pos_matches,
				vector<double>& pos_match_dist, vector<array<cv::Point2f, 2> > & neg_matches,
				vector<double>& neg_match_dist);
#endif // FEATURE_HPP
