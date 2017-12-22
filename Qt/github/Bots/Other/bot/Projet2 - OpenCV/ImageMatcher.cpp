#include "ImageMatcher.h"

ImageMatcher::ImageMatcher()
{
	detector = cv::ORB::create(2000);
	extractor = cv::ORB::create(2000);
	matcher = cv::makePtr<cv::BFMatcher>(cv::NORM_HAMMING);
	
	// Threshold used in the ratio test.
	ratio = 0.75;

	// Determines if the fundamental matrix will be recomputed with all accepted matches.
	refine_fundamental_matrix = true;

	// It is the maximum distance from a point to an epipolar line in pixels, beyond which the point is
	// considered an outlier and is not used for computing the final fundamental matrix. It can be set to something
	// like 1-3, depending on the accuracy of the point localization, image resolution, and the image noise.
	distance = 3;

	// It specifies a desirable level of confidence (probability) that the estimated matrix is correct by
	// determining the number of iterations to be made.
	confidence_level = 0.99;
}

ImageMatcher::~ImageMatcher() { }

void ImageMatcher::set_feature_detector(cv::Ptr<cv::FeatureDetector>& detector)
{
	this->detector = detector;
}

void ImageMatcher::set_descriptor_extractor(cv::Ptr<cv::DescriptorExtractor>& extractor)
{
	this->extractor = extractor;
}

void ImageMatcher::set_descriptor_matcher(cv::Ptr<cv::DescriptorMatcher>& matcher)
{
	this->matcher = matcher;
}

void ImageMatcher::set_ratio(float ratio)
{
	this->ratio = ratio;
}

void ImageMatcher::set_refine_fundamental_matrix(bool refine_fundamental_matrix)
{
	this->refine_fundamental_matrix = refine_fundamental_matrix;
}

void ImageMatcher::set_distance(double distance)
{
	this->distance = distance;
}

void ImageMatcher::set_confidence_level(double confidence_level)
{
	this->confidence_level = confidence_level;
}

// Feature detection refers to making local decisions at every image point whether there is an image feature of a given type
// at that point or not. The resulting features will be, often in the form of isolated points, continuous curves or connected regions.
// Feature extraction is used to represent the interesting points found with a detector to compare them with other
// features in a given image.
cv::Mat ImageMatcher::match(const cv::Mat& image1, const cv::Mat& image2, std::vector<cv::KeyPoint>& keypoints1, std::vector<cv::KeyPoint>& keypoints2, std::vector<cv::DMatch>& matches) const
{
	cv::Mat descriptors1, descriptors2;

	// Detection of features.
	detector->detect(image1, keypoints1);
	detector->detect(image2, keypoints2);

	// Extraction of the descriptors.
	extractor->compute(image1, keypoints1, descriptors1);
	extractor->compute(image2, keypoints2, descriptors2);

	if (descriptors1.data && descriptors2.data)
	{
		// A DMatch object contains the original keypoint, the matched keypoint and the distance between them.
		std::vector<std::vector<cv::DMatch>> matches1, matches2;
		std::vector<cv::DMatch> symmetrical_matches;
		
		// Find the two best matching points for each feature and direction.
		matcher->knnMatch(descriptors1, descriptors2, matches1, 2);
		matcher->knnMatch(descriptors2, descriptors1, matches2, 2);

		ratio_test(matches1);
		ratio_test(matches2);

		symmetry_test(matches1, matches2, symmetrical_matches);

		return ransac_test(symmetrical_matches, keypoints1, keypoints2, matches);
	}

	else
	{
		throw std::runtime_error("Insufficient number of features.");
	}
}

// Clear matches for which NN ratio is bigger than the threshold. Returns the number of removed points.
int ImageMatcher::ratio_test(std::vector<std::vector<cv::DMatch>>& matches) const
{
	int removed = 0;

	for (std::vector<std::vector<cv::DMatch>>::iterator iterator = matches.begin(); iterator != matches.end(); ++iterator)
	{
		// Check if 2 NN has been identified.
		if (iterator->size() > 1)
		{
			// Check distance ratio.
			if ((*iterator)[0].distance / (*iterator)[1].distance > ratio)
			{
				// The match does not meet the ratio.
				iterator->clear();
				++removed;
			}
		}

		else
		{
			// The match does not have two neighbours.
			iterator->clear();
			++removed;
		}
	}

	return removed;
}

// For a match pair to be accepted, both points must be the best matching feature of the other.
void ImageMatcher::symmetry_test(const std::vector<std::vector<cv::DMatch>>& matches1, const std::vector<std::vector<cv::DMatch>>& matches2, std::vector<cv::DMatch>& symmetrical_matches) const
{
	// For all matches image1 -> image2.
	for (std::vector<std::vector<cv::DMatch>>::const_iterator iterator1 = matches1.begin(); iterator1 != matches1.end(); ++iterator1)
	{
		// Ignore deleted matches.
		if (iterator1->size() < 2)
		{
			continue;
		}

		// For all matches image2 -> image1.
		for (std::vector<std::vector<cv::DMatch>>::const_iterator iterator2 = matches2.begin(); iterator2 != matches2.end(); ++iterator2)
		{
			// Ignore deleted matches.
			if (iterator2->size() < 2)
			{
				continue;
			}

			if ((*iterator1)[0].queryIdx == (*iterator2)[0].trainIdx && (*iterator2)[0].queryIdx == (*iterator1)[0].trainIdx)
			{
				symmetrical_matches.push_back(cv::DMatch((*iterator1)[0].queryIdx, (*iterator1)[0].trainIdx, (*iterator1)[0].distance));
				break; // Next match in image1 -> image2.
			}
		}
	}
}

// A filter that uses the fundamental matrix in order to reject matches that do not obey the epipolar constraint.
cv::Mat ImageMatcher::ransac_test(const std::vector<cv::DMatch>& symmetrical_matches, const std::vector<cv::KeyPoint>& keypoints1, const std::vector<cv::KeyPoint>& keypoints2, std::vector<cv::DMatch>& matches) const
{
	std::vector<cv::Point2f> points1, points2;

	// Convert keypoints into point2f.
	for (std::vector<cv::DMatch>::const_iterator iterator = symmetrical_matches.begin(); iterator != symmetrical_matches.end(); ++iterator)
	{
		points1.push_back(cv::Point2f(keypoints1[iterator->queryIdx].pt.x, keypoints1[iterator->queryIdx].pt.y));
		points2.push_back(cv::Point2f(keypoints2[iterator->trainIdx].pt.x, keypoints2[iterator->trainIdx].pt.y));
	}

	// Compute the fundamental matrix using RANSAC.
	std::vector<uchar> inliers(points1.size(), 0);
	cv::Mat fundamental = cv::findFundamentalMat(cv::Mat(points1), cv::Mat(points2), inliers, CV_FM_RANSAC, distance, confidence_level);

	// Extract the surviving (inliers) matches.
	std::vector<uchar>::const_iterator iterator_inliers = inliers.begin();
	std::vector<cv::DMatch>::const_iterator iterator_matches = symmetrical_matches.begin();

	// Loop through all matches.
	for (; iterator_inliers != inliers.end(); ++iterator_inliers, ++iterator_matches)
	{
		// Check if it is a valid match.
		if (*iterator_inliers)
		{
			matches.push_back(*iterator_matches);
		}
	}

	// The fundamental matrix will be recomputed with all accepted matches.
	if (refine_fundamental_matrix)
	{
		// Convert keypoints into point2f for the final fundamental matrix computation.
		points1.clear();
		points2.clear();

		for (std::vector<cv::DMatch>::const_iterator iterator = matches.begin(); iterator != matches.end(); ++iterator)
		{
			points1.push_back(cv::Point2f(keypoints1[iterator->queryIdx].pt.x, keypoints1[iterator->queryIdx].pt.y));
			points2.push_back(cv::Point2f(keypoints2[iterator->trainIdx].pt.x, keypoints2[iterator->trainIdx].pt.y));
		}

		// Compute the 8-point fundamental matrix from all accepted matches.
		fundamental = cv::findFundamentalMat(cv::Mat(points1), cv::Mat(points2), CV_FM_8POINT);
	}

	return fundamental;
}