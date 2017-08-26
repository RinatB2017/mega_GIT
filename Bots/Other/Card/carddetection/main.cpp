#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//const string basePath = "/home/pi/opencv/test/";
const string basePath = "/Users/elviss/Projects/cards/";
const int camera = 1;

const int WINDOW_WIDTH = 800;

enum Suit
{
    SUIT_UNKNOWN = 0,
    SUIT_HEARTS = 1,
    SUIT_DIAMONDS = 2,
    SUIT_SPADES = 3,
    SUIT_CLUBS = 4
};

enum Rank
{
    RANK_UNKNOWN = 0,
    RANK_ACE = 1,
    RANK_TWO = 2,
    RANK_THREE = 3,
    RANK_FOUR = 4,
    RANK_FIVE = 5,
    RANK_SIX = 6,
    RANK_SEVEN = 7,
    RANK_EIGHT = 8,
    RANK_NINE = 9,
    RANK_TEN = 10,
    RANK_JACK = 11,
    RANK_QUEEN = 12,
    RANK_KING = 13
};

Mat suits[5];
Mat ranks[14];

struct Card
{
    Suit suit;
    Rank rank;
};

RNG rng(12345);

const Scalar contoursColor = Scalar(255, 0, 0, 255);
const Scalar dotColor = Scalar(0, 128, 0);

const Scalar rectColor = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
const Scalar circleColor = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
const Scalar textColor = Scalar(0, 0, 0);

// parameters
int minCannyThreshold = 50;
const int maxCannyThreshold = 200;
const int kernelSize = 3;

int threshholdValue = 200;

int rho = 1;
const double theta = CV_PI / 180.0;
int linesThreshold = 100.0;
int minLineLength = 50;
int maxLineGap = 10;

const Size2f cardSize = Size2f(630.0f, 870.0f); // 630mm x 870mm

Mat resizeToWidth(const Mat& mat, int width)
{
    if (mat.cols > width)
    {
        Mat result;
        resize(mat, result, Size(width, mat.rows * ((double)width / mat.cols)), INTER_LANCZOS4);
        
        return result;
    }
    else
    {
        return mat;
    }
}

void mouseCallback(int event, int x, int y, int flags, void* userdata)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        cout << "Left button down at " << x << "," << y << endl;
    }
}

void buttonCallback(int state, void* userdata)
{
    cout << "Button" << endl;
}

void vectorToPoints(const vector<Point>& vec, vector<Point2f>& out)
{
    out.clear();

    for (size_t i = 0; i < vec.size(); ++i)
    {
        out.push_back(vec[i]);
    }
}

bool compare(const Mat& img1, const Mat& img2, double& result)
{
    Mat img1Test = img1;

    Mat img2Test;
    resize(img2, img2Test, img1.size());

    /*
	int hBins = 50; int sBins = 60;
	int histSize[] = { hBins, sBins };

	float hRanges[] = { 0, 180 };
	float sRanges[] = { 0, 256 };

	const float* ranges[] = { hRanges, sRanges };

	int channels[] = { 0, 1 };

	MatND histImg1;
	MatND histImg2;

	calcHist(&img1Test, 1, channels, Mat(), histImg1, 2, histSize, ranges, true, false);
	normalize(histImg1, histImg1, 0, 1, NORM_MINMAX, -1, Mat());

	calcHist(&img2Test, 1, channels, Mat(), histImg2, 2, histSize, ranges, true, false);
	normalize(histImg2, histImg2, 0, 1, NORM_MINMAX, -1, Mat());

	result = compareHist(histImg1, histImg2, CV_COMP_CORREL);

    return result < 0.12f;*/

    double error = norm(img1Test, img2Test);
    result = error / (double)(img1Test.rows * img1Test.cols);

    return result < 6;
}

string suitToString(Suit suit)
{
    switch (suit)
    {
        case SUIT_UNKNOWN:
            return "unknown";
        case SUIT_HEARTS:
            return "hearts";
        case SUIT_DIAMONDS:
            return "diamonds";
        case SUIT_SPADES:
            return "spades";
        case SUIT_CLUBS:
            return "clubs";
    }

    return "unknown";
}

string rankToString(Rank rank)
{
    switch (rank)
    {
        case RANK_UNKNOWN:
            return "unknown";
        case RANK_ACE:
            return "ace";
        case RANK_TWO:
            return "two";
        case RANK_THREE:
            return "three";
        case RANK_FOUR:
            return "four";
        case RANK_FIVE:
            return "five";
        case RANK_SIX:
            return "six";
        case RANK_SEVEN:
            return "seven";
        case RANK_EIGHT:
            return "eight";
        case RANK_NINE:
            return "nine";
        case RANK_TEN:
            return "ten";
        case RANK_JACK:
            return "jack";
        case RANK_QUEEN:
            return "queen";
        case RANK_KING:
            return "king";
    }

    return "unknown";
}

Suit getSuit(const Mat& img, float& diff)
{
	Suit suit = SUIT_UNKNOWN;;

	double lowest = DBL_MAX;
	double current;

	for (int i = SUIT_HEARTS; i <= SUIT_CLUBS; ++i)
    {
        compare(img, suits[i], current);
        cout << suitToString((Suit)i) << " " << current << "\n";
        if (current < lowest) { lowest = current; suit = (Suit)i; }
    }
    
    diff = lowest;

	if (lowest > 5.0)
	{
		suit = SUIT_UNKNOWN;
	}

    cout << "Result " << suitToString(suit) << " " << lowest << "\n";

    return suit;
}

Rank getRank(const Mat& img, float& diff)
{
    Rank rank = RANK_UNKNOWN;

	double lowest = DBL_MAX;
	double current;

	for (int i = RANK_ACE; i <= RANK_KING; ++i)
    {
        compare(img, ranks[i], current);
        //cout << rankToString((Rank)i) << " " << current << "\n";
        if (current < lowest) { lowest = current; rank = (Rank)i; }
    }
    
    diff = lowest;

	if (lowest > 5.0)
	{
		rank = RANK_UNKNOWN;
	}

    cout << "Result " << rankToString(rank) << " " << lowest << "\n";

    return rank;
}

Card processCard(Mat& cardImg, Mat& threshOut)
{
    Card card;
    card.suit = SUIT_UNKNOWN;
    card.rank = RANK_UNKNOWN;

    vector<vector<Point>  > contours;
    vector<Vec4i> hierarchy;

    /*Mat grayOut;
    cvtColor(cardImg, grayOut, COLOR_BGR2GRAY);

    Mat blurOut;
    GaussianBlur(grayOut, blurOut, Size(1, 1), 1000);

    Mat threshOut;
    threshold(blurOut, threshOut, threshholdValue, 255, THRESH_BINARY);*/

    //Mat threshOut = cardImg.clone();

    Mat thresh = threshOut.clone();
    findContours(threshOut, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

    vector<vector<Point> > approxPoly(contours.size());
    vector<Rect> boundRect(contours.size());

    Mat drawing = cardImg.clone();

    float lowestSuitDiff = FLT_MAX;
    float lowestRankDiff = FLT_MAX;
    
    for (size_t i = 0; i < contours.size(); ++i)
    {
        double peri = arcLength(contours[i], true);
        //cout << "Perimeter: " << peri << endl;

        if (peri > 500.0 && peri < 2000) // perimeter big enough
        {
            approxPolyDP(Mat(contours[i]), approxPoly[i], 0.02 * peri, true);
            boundRect[i] = boundingRect(Mat(approxPoly[i]));

            rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), rectColor, 2, 8, 0);

            Mat test = thresh.clone()(boundRect[i]);
            //cvtColor(test, test, CV_GRAY2BGR);
            //cvtColor(test, test, CV_BGR2HSV);

            float currentSuitDiff;
            Suit suit = getSuit(test, currentSuitDiff);

            if (suit != SUIT_UNKNOWN && currentSuitDiff < lowestSuitDiff)
            {
            	card.suit = suit;
                lowestSuitDiff = currentSuitDiff;
            }
            
            float currentRankDiff;
            
            Rank rank = getRank(test, currentRankDiff);
            if (rank != RANK_UNKNOWN && currentRankDiff < lowestRankDiff)
            {
                card.rank = rank;
                lowestRankDiff = currentRankDiff;
            }

            /*if (i == 0)
            {
                Rect roi(Point(0, 0), test.size());
                test.copyTo(cardImg(roi));
            }*/
        }
    }

    //Rect roi(Point(200, 0), hearts.size());
    //hearts.copyTo(cardImg(roi));

    imshow("Card", resizeToWidth(drawing, WINDOW_WIDTH));
    //imshow("Card thresh", resizeToWidth(thresh, WINDOW_WIDTH));

    return card;
}

template<class T>
Point2f computeIntersect(const Vec<T, 4>& a, const Vec<T, 4>& b)
{
    float x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3];
    float x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];

    if (float d = ((float)(x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4)))
    {
         Point2f pt;
         pt.x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / d;
         pt.y = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / d;
         return pt;
    }
    else
    {
        return Point2f(-1.0f, -1.0f);
    }
}

void sortCorners(vector<Point2f>& corners, Point2f center)
{
    vector<Point2f> top, bottom;

    for (size_t i = 0; i < corners.size(); ++i)
    {
        if (corners[i].y < center.y)
        {
            top.push_back(corners[i]);
        }
        else
        {
            bottom.push_back(corners[i]);
        }
    }

    corners.clear();

    if (top.size() == 2 && bottom.size() == 2)
    {
        Point2f topLeft = top[0].x > top[1].x ? top[1] : top[0];
        Point2f topRight = top[0].x > top[1].x ? top[0] : top[1];
        Point2f bottomLeft = bottom[0].x > bottom[1].x ? bottom[1] : bottom[0];
        Point2f bottomRight = bottom[0].x > bottom[1].x ? bottom[0] : bottom[1];

        corners.push_back(topLeft);
        corners.push_back(topRight);
        corners.push_back(bottomRight);
        corners.push_back(bottomLeft);
    }
}

Point2f calculateCenter(const vector<Point2f>& corners)
{
    Point2f result;
    
    for (size_t i = 0; i < corners.size(); ++i)
    {
        result += corners[i];
    }
    result *= (1.0 / corners.size());
    
    return result;
}

int main()
{
    suits[SUIT_HEARTS] = imread(basePath + "cards/hearts.png");
    suits[SUIT_DIAMONDS] = imread(basePath + "cards/diamonds.png");
    suits[SUIT_SPADES] = imread(basePath + "cards/spades.png");
    suits[SUIT_CLUBS] = imread(basePath + "cards/clubs.png");
    
    ranks[RANK_ACE] = imread(basePath + "cards/ace.png");
    ranks[RANK_TWO] = imread(basePath + "cards/2.png");
    ranks[RANK_THREE] = imread(basePath + "cards/3.png");
    ranks[RANK_FOUR] = imread(basePath + "cards/4.png");
    ranks[RANK_FIVE] = imread(basePath + "cards/5.png");
    ranks[RANK_SIX] = imread(basePath + "cards/6.png");
    ranks[RANK_SEVEN] = imread(basePath + "cards/7.png");
    ranks[RANK_EIGHT] = imread(basePath + "cards/8.png");
    ranks[RANK_NINE] = imread(basePath + "cards/9.png");
    ranks[RANK_TEN] = imread(basePath + "cards/10.png");
    ranks[RANK_JACK] = imread(basePath + "cards/jack.png");
    ranks[RANK_QUEEN] = imread(basePath + "cards/queen.png");
    ranks[RANK_KING] = imread(basePath + "cards/king.png");

	ColorConversionCodes cardConvert = COLOR_BGR2GRAY;

    for (int i = SUIT_HEARTS; i <= SUIT_CLUBS; ++i)
    {
        cvtColor(suits[i], suits[i], cardConvert);
    }

    for (int i = RANK_ACE; i <= RANK_KING; ++i)
    {
        cvtColor(ranks[i], ranks[i], cardConvert);
    }

    VideoCapture cap(camera);

    if (!cap.isOpened())
    {
        cout << "Failed to open camera" << endl;
        return 1;
    }

    double width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    double height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

    cout << "Size: " << width << "x" << height << endl;

    namedWindow("Original", CV_WINDOW_AUTOSIZE);
    //namedWindow("Card", CV_WINDOW_AUTOSIZE);

    setMouseCallback("Processed", mouseCallback, NULL);

    createTrackbar("Threshold", "Original", &threshholdValue, 255);


    createTrackbar("Min canny threshold", "Original", &minCannyThreshold, maxCannyThreshold);


    createTrackbar("rho", "Original", &rho, 100);
    createTrackbar("line threshold", "Original", &linesThreshold, 255);
    createTrackbar("minLineLength", "Original", &minLineLength, 1000);

    //createButton("OK", buttonCallback, "OK");

    bool quit = false;

    while (!quit)
    {
        Mat frame;

        bool success = cap.read(frame);

        if (!success)
        {
            cout << "Failed to read frame" << endl;
            break;
        }

        Mat drawing = frame.clone();

        //Mat dst;
        //erode(frame, dst, Mat(), Point(-1, -1), sliderIterations);

        Mat grayOut;
        cvtColor(frame, grayOut, COLOR_BGR2GRAY);

        Mat gaussianBlurOut;
        GaussianBlur(grayOut, gaussianBlurOut, Size(1, 1), 1000);
        //imshow("Gray out", resizeToWidth(grayOut, WINDOW_WIDTH));

        Mat threshOut;
        threshold(gaussianBlurOut, threshOut, threshholdValue, 255, THRESH_BINARY);
        imshow("Thresh out", resizeToWidth(threshOut, WINDOW_WIDTH));
        
        Mat thresh = threshOut.clone();

        vector<vector<Point>  > contours;
        vector<Vec4i> hierarchy;
        findContours(threshOut, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

        vector<vector<Point> > approxPolys(contours.size());
        vector<Rect> boundRects(contours.size());

        if (!hierarchy.empty())
        {
            for (size_t contourIndex = 0; contourIndex < contours.size(); ++contourIndex)
            {
                vector<Point> contour = contours[contourIndex];
                
                double peri = arcLength(contour, true);
                //cout << "Perimeter: " << peri << "\n";

                if (peri > 1000.0 && hierarchy[contourIndex][3] == -1) // has no parent and perimeter big enough
                {
                    approxPolyDP(Mat(contour), approxPolys[contourIndex], 0.001 * peri, true);
                    boundRects[contourIndex] = boundingRect(Mat(approxPolys[contourIndex]));
                    
                    vector<Point> poly = approxPolys[contourIndex];

                    if (poly.size() >= 4)
                    {
                        vector<uint32_t> lineLengths;
                        
                        for (size_t pointIndex = 0; pointIndex < poly.size(); ++pointIndex)
                        {
                            Point current = poly[pointIndex];
                            Point previous = (pointIndex == 0) ? poly[poly.size() - 1] : poly[pointIndex - 1];
                            
                            uint32_t length = norm(current - previous);
                            
                            lineLengths.push_back(length);
                            
                            //circle(drawing, current, 1, Scalar());
                        }
                        
                        std::sort(lineLengths.begin(), lineLengths.end(), std::greater<int>());
                        
                        int shortestLine = lineLengths[3];
                        
                        vector<Vec4f> lines;
                        
                        for (size_t pointIndex = 0; pointIndex < poly.size(); ++pointIndex)
                        {
                            Point current = poly[pointIndex];
                            Point previous = (pointIndex == 0) ? poly[poly.size() - 1] : poly[pointIndex - 1];
                            
                            uint32_t length = norm(current - previous);
                            
                            if (length >= shortestLine)
                            {
                                //line(drawing, previous, current, contoursColor, 5);
                                
                                Vec4f currentLine;
                                
                                // start x
                                currentLine[0] = 0;
                                
                                // start y
                                currentLine[1] = ((float)previous.y - current.y) / (previous.x - current.x) * -previous.x + previous.y;
                                
                                // end x
                                currentLine[2] = frame.cols;
                                
                                // end y
                                currentLine[3] = ((float)previous.y - current.y) / (previous.x - current.x) * (frame.cols - current.x) + current.y;

                                lines.push_back(currentLine);
                                
                                //line(drawing, Point2f(currentLine[0], currentLine[1]), Point2f(currentLine[2], currentLine[3]), contoursColor, 5);
                            }
                        }
                        
                        vector<Point2f> corners;
                        
                        for (size_t i = 0; i < lines.size(); ++i)
                        {
                            for (size_t j = i + 1; j < lines.size(); ++j)
                            {
                                Point2f pt = computeIntersect(lines[i], lines[j]);
                                if (pt.x >= 0 && pt.y >= 0)
                                {
                                    corners.push_back(pt);
                                    
                                    circle(drawing, pt, 10, dotColor);
                                }
                            }
                        }
                        
                        Point2f center = calculateCenter(corners);
                        circle(drawing, center, 20, dotColor);
                        
                        sortCorners(corners, center);
                        
                        if (corners.size() == 4)
                        {
                            line(drawing, corners[0], corners[1], contoursColor, 4);
                            line(drawing, corners[1], corners[2], contoursColor, 4);
                            line(drawing, corners[2], corners[3], contoursColor, 4);
                            line(drawing, corners[3], corners[0], contoursColor, 4);
                            
                            //630mm x 870mm
                            vector<Point2f> np;
                            np.push_back(Point(0, 0));
                            np.push_back(Point(cardSize.width, 0));
                            np.push_back(Point(cardSize.width, cardSize.height));
                            np.push_back(Point(0, cardSize.height));
                            
                            Mat transform = getPerspectiveTransform(corners, np);
                            
                            Mat warp;
                            warpPerspective(frame, warp, transform, Point(cardSize.width, cardSize.height));
                            
                            Mat warp2;
                            warpPerspective(thresh, warp2, transform, Point(cardSize.width, cardSize.height));
                            
                            if (!warp.empty() && !warp2.empty())
                            {
                                Card card = processCard(warp, warp2);
                                
                                stringstream s;
                                s << rankToString(card.rank) << " of " << suitToString(card.suit);
                                
                                putText(drawing, s.str(), center, FONT_HERSHEY_SIMPLEX, 2.0, textColor, 3, 8, false);
                            }
                        }
                    }

                    rectangle(drawing, boundRects[contourIndex].tl(), boundRects[contourIndex].br(), rectColor, 2, 8, 0);
                }
            }
        }

        imshow("Original", resizeToWidth(drawing, WINDOW_WIDTH));
        //imshow("Processed", resizeToWidth(threshOut, WINDOW_WIDTH));

        int key = waitKey(30);

        switch (key)
        {
            case 27:
                quit = true;
            break;
        }
    }

    destroyWindow("Test");

	return 0;
}

