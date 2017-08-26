#ifndef OBJECT_DETECTOR_H
#define OBJECT_DETECTOR_H

#include <queue>
#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>
#include "ImageMatcher.h"

class ObjectDetector
{
private:
	int dims;
	int bins[2];
	float h_range[2];
	float s_range[2];
	const float* ranges[2];
	int channels[2];
	std::string scene_image_path;
	std::string object_image_path;
	cv::Mat scene_image;
	cv::Mat object_image;
	cv::Mat histogram;
	cv::Mat back_projection;
	std::queue<cv::Rect> matches;

	void get_histogram();
	void get_back_projection();
	
public:
	ObjectDetector();
	~ObjectDetector();

	cv::Mat get_scene_image() const;
	void set_scene_image(const cv::Mat& scene_image);
	void set_scene_image_path(const std::string& scene_image_path, const cv::ImreadModes& mode);
	cv::Mat get_object_image() const;
	void set_object_image(const cv::Mat& object_image);
	void set_object_image_path(const std::string& object_image_path, const cv::ImreadModes& mode);
	void find_contours(const std::string& scene_image_path, const cv::Point& point);
	void color_match(const std::string& scene_image_path, const std::string& object_image_path);
	void feature_match(const std::string& scene_image_path, const std::string& object_image_path);
	void template_match(const std::string& scene_image_path, double threshold1 = 0.6, double threshold2 = 0.7);
	std::queue<cv::Rect> get_matches() const;
	void remove_match();
};

#endif