#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdlib>

using namespace cv;
using std::cout;
using std::endl;

//IplImage* image = 0;

int main(void)
{
    char mainWindow[] = "Main";
    char trackbarWindow[] = "Trackbar";
    char thresholdWindow[] = "Threshold";
    int min = 0, max = 1000;
    int hmin = 0, smin = 0, vmin = 0,
            hmax = 255, smax = 255, vmax = 255;
    Mat frame, HSV, threshold, blurred;

    //const char* filename = "scg1.jpg";
    // получаем картинку
    //image = cvLoadImage(filename,1);

    //Создаем окна
    namedWindow(mainWindow, 0);
    namedWindow(trackbarWindow, 0);
    namedWindow(thresholdWindow, 0);

    //Создаем трэкбар
    createTrackbar("H min:", trackbarWindow, &hmin, hmax);
    createTrackbar("H max:", trackbarWindow, &hmax, hmax);
    createTrackbar("S min:", trackbarWindow, &smin, smax);
    createTrackbar("S max:", trackbarWindow, &smax, smax);
    createTrackbar("V min:", trackbarWindow, &vmin, vmax);
    createTrackbar("V max:", trackbarWindow, &vmax, vmax);
    createTrackbar("Size min:", trackbarWindow, &min, max);
    createTrackbar("Size max:", trackbarWindow, &max, max);

#if 1
    VideoCapture capture;
    //Открываем камеру
    capture.open(0);
    if(!capture.isOpened())
    {
        cout << "Камера не может быть открыта." << endl;
        exit(1);
    }
#endif

    //Запускаем цикл чтения с камеры
    for(;;)
    {
        capture >> frame;
        //image >> frame;

        cvtColor(frame, HSV, COLOR_BGR2HSV);
        medianBlur(HSV, blurred, 21);
        inRange(blurred, Scalar(hmin, smin, vmin), Scalar(hmax, smax, vmax), threshold);
        for(int y = 0; y < threshold.rows; y++)
        {
            for(int x = 0; x < threshold.cols; x++)
            {
                int value = threshold.at<uchar>(y, x);
                if(value == 255){
                    Rect rect;
                    int count = floodFill(threshold, Point(x, y), Scalar(200), &rect);
                    if(rect.width >= min && rect.width <= max
                            && rect.height >= min && rect.height <= max)
                    {
                        rectangle(frame, rect, Scalar(255, 0, 255, 4));

                    }
                }
            }
        }
        imshow(mainWindow, frame);
        imshow(thresholdWindow, threshold);
        if(waitKey(33) == 27) break;
    }
    return 0;
}
