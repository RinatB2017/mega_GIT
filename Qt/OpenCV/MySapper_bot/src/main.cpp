//
// пример cvMatchTemplate()
// сравнение изображение с шаблоном
//

#include <highgui.h>
#include <stdlib.h>
#include <stdio.h>
#include <cv.h>

#include <QDebug>

void test_0(void)
{
    IplImage *image;
    IplImage *templ;
    IplImage *res;

    image = cvLoadImage("images/1.png", CV_LOAD_IMAGE_COLOR);
    templ = cvLoadImage("images/2.png", CV_LOAD_IMAGE_COLOR);

    cvNamedWindow("origianl",   CV_WINDOW_AUTOSIZE);
    cvNamedWindow("template",   CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Match",      CV_WINDOW_AUTOSIZE);
    cvNamedWindow("res",        CV_WINDOW_AUTOSIZE);

    // размер шаблона
    //int width = templ->width;
    //int height = templ->height;

    // оригинал и шаблон
    cvShowImage( "origianl", image);
    cvShowImage( "template", templ);

    // изображение для хранения результата сравнения
    // размер результата: если image WxH и templ wxh, то result = (W-w+1)x(H-h+1)
    res = cvCreateImage(
                cvSize( (image->width-templ->width+1),
                        (image->height-templ->height+1)),
                IPL_DEPTH_32F,
                1);

    // сравнение изображения с шаблоном
    cvMatchTemplate(image,
                    templ,
                    res,
                    CV_TM_SQDIFF);

    // покажем что получили
    cvShowImage( "res", res);

    // определение лучшее положение для сравнения
    // (поиск минимумов и максимумов на изображении)
    double  minval, maxval;
    CvPoint minloc, maxloc;
    cvMinMaxLoc(res,
                &minval, &maxval,
                &minloc, &maxloc,
                nullptr);

    // нормализуем
    cvNormalize(res, res, 1, 0, CV_MINMAX);
    cvNamedWindow("res norm", CV_WINDOW_AUTOSIZE);
    cvShowImage("res norm", res);

    // выделим область прямоугольником
    cvRectangle(image,
                cvPoint(minloc.x, minloc.y),
                cvPoint(minloc.x+templ->width-1, minloc.y+templ->height-1),
                CV_RGB(255, 0, 0), 1, 8);

    // показываем изображение
    cvShowImage("Match", image);

    // ждём нажатия клавиши
    cvWaitKey(0);

    // освобождаем ресурсы
    cvReleaseImage( &image );
    cvReleaseImage( &templ );
    cvReleaseImage( &res );
    cvDestroyAllWindows();

    qDebug() << "OK";
}

bool test_1(void)
{
    IplImage *image;
    IplImage *templ;
    IplImage *res;

    image = cvLoadImage("images/1.png", CV_LOAD_IMAGE_COLOR);
    templ = cvLoadImage("images/2.png", CV_LOAD_IMAGE_COLOR);

    cvNamedWindow("origianl", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("template", CV_WINDOW_AUTOSIZE);

    cvShowImage( "origianl", image);
    cvShowImage( "template", templ);

    res = cvCreateImage(
                CvSize(image->width-templ->width+1,
                       image->height-templ->height+1),
                IPL_DEPTH_32F,
                1);

    cvWaitKey(0);
    cvMatchTemplate(image,
                    templ,
                    res,
                    CV_TM_SQDIFF);

    cvReleaseImage( &image );
    cvReleaseImage( &templ );
    cvReleaseImage( &res );
    cvDestroyAllWindows();

    qDebug() << "OK";
    return true;
}

int main(void)
{
    test_0();
    //test_1();
    return 0;
}
