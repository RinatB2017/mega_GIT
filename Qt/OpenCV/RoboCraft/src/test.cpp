//
// конвертирует изображение в HSV
// и позволяет подобрать параметры
// Hmin, Hmax, Smin, Smax, Vmin, Vmax
// для выделения нужного объекта
//
// robocraft.ru
//
#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <stdio.h>

#define cvCvtPixToPlane cvSplit

IplImage* image = 0;

// для хранения каналов HSV
IplImage* hsv = 0;

IplImage *frame = 0;

int main(void)
{
    // имя картинки задаётся первым параметром
    const char* filename = "scg1.jpg";
    // получаем картинку
    image = cvLoadImage(filename,1);

    printf("[i] image: %s\n", filename);
    assert( image != 0 );

    // создаём картинки
    hsv = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 3 );
    frame = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
    //  конвертируем в HSV
    cvCvtColor( image, hsv, CV_BGR2HSV );

    // окна для отображения картинки
    cvNamedWindow("original",   CV_WINDOW_AUTOSIZE);
    cvNamedWindow("hsv",        CV_WINDOW_AUTOSIZE);
    cvNamedWindow("frame",      CV_WINDOW_AUTOSIZE);

    cvInRangeS(hsv, cvScalar(200,100,100), cvScalar(240,255,255), frame);

    // показываем картинку
    cvShowImage("original", image);
    cvShowImage("hsv", hsv);
    cvShowImage("frame", frame);

    cvWaitKey(0);

    // освобождаем ресурсы
    cvReleaseImage(&image);
    // удаляем окна
    cvDestroyAllWindows();
    return 0;
}
