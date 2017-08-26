#include "ObjectDetector.h"

ObjectDetector::ObjectDetector()
{
	// Data is collected from both hue and saturation.
	dims = 2;

	// The number of subdivisions in each dim.
	// Hue bins represents the x-axis and saturation bins the y-axis.
	bins[0] = 80;
	bins[1] = 25;
	
	// The limits for the values to be measured [inclusive, exclusive].
	h_range[0] = 0;
	h_range[1] = 180;
	s_range[0] = 0;
	s_range[1] = 256;
	ranges[0] = h_range;
	ranges[1] = s_range;
	
	// Hue (color) and saturation (greyness). Value (brightness) is not calculated.
	channels[0] = 0;
	channels[1] = 1;
}

ObjectDetector::~ObjectDetector() { }

//cv::Mat ObjectDetector::get_scene_image() const
//{
//	return scene_image;
//}
//
//void ObjectDetector::set_scene_image(const cv::Mat& scene_image)
//{
//	this->scene_image = scene_image;
//}

void ObjectDetector::set_scene_image_path(const std::string& scene_image_path, const cv::ImreadModes& mode)
{
	this->scene_image_path = scene_image_path;

	scene_image = cv::imread(scene_image_path, mode);
}

//cv::Mat ObjectDetector::get_object_image() const
//{
//	return object_image;
//}
//
//void ObjectDetector::set_object_image(const cv::Mat& object_image)
//{
//	this->object_image = object_image;
//}

void ObjectDetector::set_object_image_path(const std::string& object_image_path, const cv::ImreadModes& mode)
{
	this->object_image_path = object_image_path;

	object_image = cv::imread(object_image_path, mode);
}

// Detecting edges in object recognition is possible with these algorithms: Sobel derivatives, Laplace operator, Canny algorithm.
// Simple binary edge edge maps suffer from two main drawbacks. First, the edges are too thick. Second, it's difficult
// to find a threshold that is low enough to detect all important edges, but not sufficiently high to not include
// too many insignifient edges. This is why the canny edge detector was chosen.
void ObjectDetector::find_contours(const std::string& scene_image_path, const cv::Point& point)
{
	set_scene_image_path(scene_image_path, cv::IMREAD_UNCHANGED);

	if (!scene_image.empty())
	{
		cv::Mat scene_image_gray, scene_image_filtered, canny_output;
		std::vector<std::vector<cv::Point>> contours, matches;

		// Convert the image from BGR to grayscale color space.
		cv::cvtColor(scene_image, scene_image_gray, cv::COLOR_BGR2GRAY);

		// A morphological gradient is the difference between the dilation and the erosion of an image.
		// It is useful for finding the outline of an object.
		cv::morphologyEx(scene_image_gray, scene_image_gray, cv::MORPH_GRADIENT, cv::Mat(5, 5, CV_8U));

		// Smooth an image with a low-pass filter, useful for removing noise.
		// A bilateral filter can reduce unwanted noise very well while keeping edges fairly sharp.
		cv::bilateralFilter(scene_image_gray, scene_image_filtered, 5, 50, 50);
		
		// Canny recommands an upper:lower ratio between 2:1 and 3:1.
		cv::Canny(scene_image_filtered, canny_output, 75, 150, 3);
		cv::findContours(canny_output, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

		// DEBUG. Show the canny output.
		cv::namedWindow("Canny Output", CV_WINDOW_AUTOSIZE);
		cv::imshow("Canny Output", canny_output);

		for (auto iterator = contours.begin(); iterator != contours.end(); ++iterator)
		{
			// Check if the point is inside (= 1) or on the edge (= 0) of the contour.
			if (cv::pointPolygonTest((*iterator), point, false) >= 0)
			{
				matches.push_back((*iterator));
			}
		}

		if (!matches.empty())
		{
			// Create the minimum-area bounding rectangle from the last point set.
			cv::RotatedRect box = cv::minAreaRect(cv::Mat(matches.back()));
			cv::Rect region_interest;

			// Set the region of interest around the contour that is a match.
			region_interest.x = box.center.x - (box.size.width / 2);
			region_interest.y = box.center.y - (box.size.height / 2);
			region_interest.width = box.size.width;
			region_interest.height = box.size.height;

			// Copy the actual image data.
			object_image = scene_image(region_interest);

			// DEBUG. Show the object image.
			cv::namedWindow("Object Image", CV_WINDOW_AUTOSIZE);
			cv::imshow("Object Image", object_image);
		}

		else
		{
			throw std::runtime_error("No matches found.");
		}
	}

	else
	{
		throw std::runtime_error("Invalid image data.");
	}
}

void ObjectDetector::color_match(const std::string& scene_image_path, const std::string& object_image_path)
{
	set_scene_image_path(scene_image_path, cv::IMREAD_UNCHANGED);
	set_object_image_path(object_image_path, cv::IMREAD_UNCHANGED);

	if (scene_image.data && object_image.data)
	{
		// DEBUG. Show the object image.
		cv::namedWindow("Object image", CV_WINDOW_AUTOSIZE);
		cv::imshow("Object image", object_image);

		// Calculate the histogram of the region of interest.
		get_histogram();
		
		// Calculate the back projection of the new image using the histogram of the region of interest.
		get_back_projection();

		// DEBUG. Show the back projection image.
		cv::namedWindow("Back projection image", CV_WINDOW_AUTOSIZE);
		cv::imshow("Back projection image", back_projection);
	}

	else
	{
		throw std::runtime_error("Invalid image data.");
	}
}

// Feature detection and extraction with the ORB algorithm and a BruteForce matcher.
void ObjectDetector::feature_match(const std::string& scene_image_path, const std::string& object_image_path)
{
	set_scene_image_path(scene_image_path, cv::IMREAD_COLOR);
	set_object_image_path(object_image_path, cv::IMREAD_COLOR);

	if (scene_image.data && object_image.data)
	{
		cv::Mat scene_image_gray, object_image_gray;
		cv::Mat result_image;

		ImageMatcher matcher;
		std::vector<cv::KeyPoint> scene_keypoints, object_keypoints;
		std::vector<cv::DMatch> matches;

		// Convert images to grayscale for the detector.
		cv::cvtColor(scene_image, scene_image_gray, cv::COLOR_BGR2GRAY);
		cv::cvtColor(object_image, object_image_gray, cv::COLOR_BGR2GRAY);

		try
		{
			// Try to detect the object.
			matcher.match(scene_image_gray, object_image_gray, scene_keypoints, object_keypoints, matches);
		}

		catch (const std::exception& exception)
		{
			// If the detection fails, rethrow the error so another algorithm can be used.
			throw;
		}

		cv::drawMatches(scene_image_gray, scene_keypoints, object_image_gray, object_keypoints, matches, result_image);
			
		// DEBUG. Show the result image.
		cv::namedWindow("Result image", CV_WINDOW_FREERATIO);
		cv::imshow("Result image", result_image);
	}

	else
	{
		throw std::runtime_error("Invalid image data.");
	}
}

// Compares a template against overlapped image regions. The first threshold is used to segment the image in black and white areas.
// The second threshold is used to compare against the maximum value of the intensity of the thresholded image.
void ObjectDetector::template_match(const std::string& scene_image_path, double threshold1, double threshold2)
{
	set_scene_image_path(scene_image_path, cv::IMREAD_COLOR);
	//set_object_image_path(object_image_path, cv::IMREAD_COLOR);

	if (!scene_image.empty() && !object_image.empty())
	{
		cv::Mat scene_image_gray, object_image_gray, result;
		double minimum_value, maximum_value;
		cv::Point minimum_location, maximum_location;

		// Convert images to grayscale for thresholding.
		cv::cvtColor(scene_image, scene_image_gray, CV_BGR2GRAY);
		cv::cvtColor(object_image, object_image_gray, CV_BGR2GRAY);

		// Create the image to analyze.
		result.create(scene_image_gray.rows - object_image_gray.rows + 1, scene_image_gray.cols - object_image_gray.cols + 1, CV_32FC1);

		// Match the object on the scene. Treshold the result in order to find the most probable location which has the object.
		cv::matchTemplate(scene_image_gray, object_image_gray, result, CV_TM_CCOEFF_NORMED);
		cv::threshold(result, result, threshold1, 1., CV_THRESH_TOZERO);

		do
		{
			// Find the global maximum and minimum in the thresholded image.
			cv::minMaxLoc(result, &minimum_value, &maximum_value, &minimum_location, &maximum_location);

			if (maximum_value >= threshold2)
			{
				// If a match is found, get his location and then fill it with black color so it can't be detected again.
				matches.push(cv::Rect(maximum_location, cv::Point(maximum_location.x + object_image.cols, maximum_location.y + object_image.rows)));
				cv::rectangle(scene_image, maximum_location, cv::Point(maximum_location.x + object_image.cols, maximum_location.y + object_image.rows), CV_RGB(0, 255, 0), 2);
				cv::floodFill(result, maximum_location, cv::Scalar(0), 0, cv::Scalar(0.1), cv::Scalar(1));
			}
		} while (maximum_value >= threshold2);

		// DEBUG. Show the scene image.
		cv::namedWindow("Scene Image", cv::WINDOW_AUTOSIZE);
		cv::imshow("Scene Image", scene_image);
	}

	else
	{
		throw std::runtime_error("Invalid image data.");
	}
}

std::queue<cv::Rect> ObjectDetector::get_matches() const
{
	return matches;
}

void ObjectDetector::remove_match()
{
	matches.pop();
}

// An image histogram is a graphical representation of an image feature we want to measure
// (gradients, directions, intensities, etc.). It quantifies the number of pixels for each value considered.
void ObjectDetector::get_histogram()
{
	cv::Mat hsv, mask;
	
	// Check if there is an alpha channel.
	if (object_image.channels() == 4)
	{
		// By default, OpenCV uses BGRA channel order.
		std::vector<cv::Mat> bgra_channels;
		cv::split(object_image, bgra_channels);

		// If the values in the alpha channel are bigger than 0, put 255 in the mask. Otherwise, put 0.
		cv::threshold(bgra_channels[3], mask, 0, 255, cv::THRESH_BINARY);
	}

	// Convert the image from BGR to HSV color space.
	cv::cvtColor(object_image, hsv, CV_BGR2HSV);

	// Calculate the 2D-HSV (hue, saturation) histogram.
	// A mask is used in order to calculate the histogram for only one area of the image.
	// All pixels for which the mask value is 0 (black) are ignored.
	cv::calcHist(&hsv, 1, channels, mask, histogram, dims, bins, ranges, true, false);

	// Improve the contrast of the image by stretching the image to the available range of colors.
	cv::normalize(histogram, histogram, 0, 255, cv::NORM_MINMAX, -1, cv::Mat());
}

// Back projection is a way of recording how well the pixels of a given image fit the distribution of pixels in a histogram model.
void ObjectDetector::get_back_projection()
{
	cv::Mat hsv;

	// Convert the image from BGR to HSV color space.
	cv::cvtColor(scene_image, hsv, CV_BGR2HSV);

	// The function computes probability of each element value in respect with the empirical probability
	// distribution represented by the histogram.
	cv::calcBackProject(&hsv, 1, channels, histogram, back_projection, ranges, 1, true);
}