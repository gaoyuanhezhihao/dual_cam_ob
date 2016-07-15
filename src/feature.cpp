#include "feature.hpp"
/*sift_match: match two images using sift feature.
 * @img_1: first image.
 * @img_2: second_image.
 * @pos_match_num: number of positive match to find. 
 * 		positive match is the match between up half of the down_camera and image of up_camera.
 * @neg_match_num: number of negative match to find.
 * 		negative match is the match between down half of
 * 		the down_camera and image of up_camera.
 * @pos_match: vector of positive matches.
 * @pos_match_dist: vector of distances of  matches.
 * @neg_match: vector of negative matches.
 * @neg_match_dist: vector of distances of matches.
 **/
int sift_match(cv::Mat &img1, cv::Mat & img_2, int pos_match_num,
				int neg_match_num, std::vector<std::array<cv::Point2f, 2> > & pos_matches,
				vector<double>& pos_match_dist, vector<array<cv::Point2f, 2> > & neg_matches,
				vector<double>& neg_match_dist)
{
		if( !img1.data || !img_2.data )
		{ return -1; }
		cv::Mat img1_copy;
		cv::Mat img2_copy;
		cv::cvtColor(img1, img1_copy, cv::COLOR_BGR2GRAY);
		cv::cvtColor(img_2, img2_copy, cv::COLOR_BGR2GRAY);
		std::swap(img1, img1_copy);
		std::swap(img_2, img2_copy);
		cv::Rect up_half_rect(0, 0, img1.cols, img1.rows/2);
		cv::Rect down_half_rect(0, img1.rows/2, img1.cols, img1.rows/2);
		cv::Mat img_1_up_half= img1(up_half_rect);
		cv::Mat img_1_down_half = img1(down_half_rect);
		cout << "img_1_up_half: "<<img_1_up_half.cols<<','<<img_1_up_half.rows<<endl;
		cout << "img_1_down_half: "<<img_1_down_half.cols<<','<<img_1_down_half.rows<<endl;
		cout << "img_1"<<img1.cols << ','<<img1.rows<<endl;
		cout << "img_2"<<img_2.cols << ','<<img_2.rows<<endl;
		//-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
		Ptr<Feature2D> detector = xfeatures2d::SIFT::create();
		std::vector<KeyPoint> keypoints_1_up, keypoints_1_down, keypoints_2;
		Mat descriptors_1_up, descriptors_1_down, descriptors_2;
		detector->detect(img_1_up_half, keypoints_1_up);
		detector->detect(img_1_down_half, keypoints_1_down);
		detector->detect( img_2, keypoints_2);

		detector->compute(img_1_up_half, keypoints_1_up, descriptors_1_up);
		detector->compute(img_1_down_half, keypoints_1_down, descriptors_1_down);
		detector->compute(img_2, keypoints_2, descriptors_2);
		//-- Step 2: Matching descriptor vectors with a brute force matcher
		BFMatcher matcher(NORM_L2);
		std::vector< DMatch > matches_up;
		std::vector< DMatch > matches_down;
		matcher.match( descriptors_1_up, descriptors_2, matches_up);
		matcher.match(descriptors_1_down, descriptors_2, matches_down);
				
		//-- Step 2.1: Find out the best N matches.
		std::vector< DMatch > good_pos_match(pos_match_num);
		std::vector< DMatch > good_neg_match(neg_match_num);

		auto sort_op = [](const DMatch &left, const DMatch &right){
				return left.distance < right.distance;
		};
		std::sort(matches_up.begin(), matches_up.end(),sort_op );
		std::copy(matches_up.cbegin(), matches_up.cbegin()+pos_match_num, good_pos_match.begin());
		std::sort(matches_down.begin(), matches_down.end(),sort_op );
		std::copy(matches_down.cbegin(), matches_down.cbegin()+neg_match_num, good_neg_match.begin());
		/*-- Step 3: transform the result to output format.*/
		int i = 0;
		int q_id = -1;
		int t_id = -1;
		array<Point2f, 2> tmp_match_pair;
		Point2f tmp_pt;
		for(i=0;i<pos_match_num;++i) {
				q_id = matches_up[i].queryIdx;
				t_id = matches_up[i].trainIdx;
				tmp_match_pair[0] = keypoints_1_up[q_id].pt;
				tmp_match_pair[1] = keypoints_2[t_id].pt;
				pos_matches.push_back(tmp_match_pair);
				pos_match_dist.push_back(matches_up[i].distance);
				cv::circle(img1_copy, tmp_match_pair[0], 3, Scalar(0,0,255), -1);
				cv::circle(img2_copy, tmp_match_pair[1], 3, Scalar(0, 0, 255), -1);
		}
		for(i = 0;i< neg_match_num;++i) {
			q_id = matches_down[i].queryIdx;
			t_id = matches_down[i].trainIdx;
			tmp_pt = tmp_match_pair[0] = keypoints_1_down[q_id].pt;
			tmp_match_pair[1] = keypoints_2[t_id].pt;
			neg_matches.push_back(tmp_match_pair);
			neg_match_dist.push_back(matches_down[i].distance);
			tmp_pt.y += img1_copy.rows/2;	
			cv::circle(img1_copy, tmp_pt, 3, Scalar(0,255,0), -1);
			cv::circle(img2_copy, tmp_match_pair[1], 3, Scalar(0, 255, 0), -1);
		}
		
		//-- Draw matches
		Mat img_matches_up, img_matches_down;
		drawMatches( img_1_up_half, keypoints_1_up, img_2, keypoints_2, good_pos_match,img_matches_up);
		drawMatches(img_1_down_half, keypoints_1_down, img_2, keypoints_2, good_neg_match, img_matches_down);
		//-- Show detected matches
		imshow("Up Matches", img_matches_up);
		imshow("Down Matches", img_matches_down);
		imshow("img1_copy", img1_copy);
		imshow("img2_copy", img2_copy);
		waitKey(0);
		return 0;
}
