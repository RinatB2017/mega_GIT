#ifndef IMAGE_MATCHER_H
#define IMAGE_MATCHER_H

#include <opencv2\calib3d.hpp>

class ImageMatcher
{
private:
	cv::Ptr<cv::FeatureDetector> detector;
	cv::Ptr<cv::DescriptorExtractor> extractor;
	cv::Ptr<cv::DescriptorMatcher> matcher;
	float ratio;
	bool refine_fundamental_matrix;
	double distance;
	double confidence_level;

	int ratio_test(std::vector<std::vector<cv::DMatch>>& matches) const;
	void symmetry_test(const std::vector<std::vector<cv::DMatch>>& matches1, const std::vector<std::vector<cv::DMatch>>& matches2, std::vector<cv::DMatch>& symmetrical_matches) const;
	cv::Mat ransac_test(const std::vector<cv::DMatch>& symmetrical_matches, const std::vector<cv::KeyPoint>& keypoints1, const std::vector<cv::KeyPoint>& keypoints2, std::vector<cv::DMatch>& matches) const;
public:
	ImageMatcher();
	~ImageMatcher();
	
	void set_feature_detector(cv::Ptr<cv::FeatureDetector>& detector);
	void set_descriptor_extractor(cv::Ptr<cv::DescriptorExtractor>& extractor);
	void set_descriptor_matcher(cv::Ptr<cv::DescriptorMatcher>& matcher);
	void set_ratio(float ratio);
	void set_refine_fundamental_matrix(bool refine_fundamental_matrix);
	void set_distance(double distance);
	void set_confidence_level(double confidence_level);
	cv::Mat match(const cv::Mat& image1, const cv::Mat& image2, std::vector<cv::KeyPoint>& keypoints1, std::vector<cv::KeyPoint>& keypoints2, std::vector<cv::DMatch>& matches) const;
};

#endif