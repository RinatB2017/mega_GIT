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

IplImage* image;
IplImage* dst;

// для хранения каналов HSV
IplImage* hsv;
IplImage* h_plane;
IplImage* s_plane;
IplImage* v_plane;
// для хранения каналов HSV после преобразования
IplImage* h_range;
IplImage* s_range;
IplImage* v_range;
// для хранения суммарной картинки
IplImage* hsv_and;

int Hmin = 0;
int Hmax = 256;

int Smin = 0;
int Smax = 256;

int Vmin = 0;
int Vmax = 256;

int HSVmax = 256;

//
// функции-обработчики ползунков
//
void myTrackbarHmin(int pos)
{
	Hmin = pos;
	cvInRangeS(h_plane, cvScalar(Hmin), cvScalar(Hmax), h_range);
}

void myTrackbarHmax(int pos)
{
	Hmax = pos;
	cvInRangeS(h_plane, cvScalar(Hmin), cvScalar(Hmax), h_range);
}

void myTrackbarSmin(int pos)
{
	Smin = pos;
	cvInRangeS(s_plane, cvScalar(Smin), cvScalar(Smax), s_range);
}

void myTrackbarSmax(int pos)
{
	Smax = pos;
	cvInRangeS(s_plane, cvScalar(Smin), cvScalar(Smax), s_range);
}

void myTrackbarVmin(int pos)
{
	Vmin = pos;
	cvInRangeS(v_plane, cvScalar(Vmin), cvScalar(Vmax), v_range);
}

void myTrackbarVmax(int pos)
{
	Vmax = pos;
	cvInRangeS(v_plane, cvScalar(Vmin), cvScalar(Vmax), v_range);
}

int main(int argc, char* argv[])
{
	// имя картинки задаётся первым параметром
    const char* filename = "images/scg1.jpg";
	// получаем картинку
	image = cvLoadImage(filename,1);

	printf("[i] image: %s\n", filename);
	assert( image != 0 );

	// создаём картинки
	hsv = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 3 );
	h_plane = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
	s_plane = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
	v_plane = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
	h_range = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
	s_range = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
	v_range = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
	hsv_and = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
	//  конвертируем в HSV 
	cvCvtColor( image, hsv, CV_BGR2HSV ); 
	// разбиваем на отельные каналы
	cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );

	//
	// определяем минимальное и максимальное значение
	// у каналов HSV
	double framemin=0;
	double framemax=0;

	cvMinMaxLoc(h_plane, &framemin, &framemax);
	printf("[H] %f x %f\n", framemin, framemax );
	Hmin = framemin;
	Hmax = framemax;
	cvMinMaxLoc(s_plane, &framemin, &framemax);
	printf("[S] %f x %f\n", framemin, framemax );
	Smin = framemin;
	Smax = framemax;
	cvMinMaxLoc(v_plane, &framemin, &framemax);
	printf("[V] %f x %f\n", framemin, framemax );
	Vmin = framemin;
	Vmax = framemax;

	// окна для отображения картинки
	cvNamedWindow("original",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("H",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("S",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("V",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("H range",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("S range",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("V range",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("hsv and",CV_WINDOW_AUTOSIZE);

	cvCreateTrackbar("Hmin", "H range", &Hmin, HSVmax, myTrackbarHmin);
	cvCreateTrackbar("Hmax", "H range", &Hmax, HSVmax, myTrackbarHmax);
	cvCreateTrackbar("Smin", "S range", &Smin, HSVmax, myTrackbarSmin);
	cvCreateTrackbar("Smax", "S range", &Smax, HSVmax, myTrackbarSmax);
	cvCreateTrackbar("Vmin", "V range", &Vmin, HSVmax, myTrackbarVmin);
	cvCreateTrackbar("Vmax", "V range", &Vmax, HSVmax, myTrackbarVmax);

	//
	// разместим окна по рабочему столу
	//
    if(image->width <1920/4 && image->height<1080/2)
    {
        cvMoveWindow("original", 0, 0);
        cvMoveWindow("H", image->width+10, 0);
        cvMoveWindow("S", (image->width+10)*2, 0);
        cvMoveWindow("V", (image->width+10)*3, 0);
        cvMoveWindow("hsv and", 0, image->height+30);
        cvMoveWindow("H range", image->width+10, image->height+30);
        cvMoveWindow("S range", (image->width+10)*2, image->height+30);
        cvMoveWindow("V range", (image->width+10)*3, image->height+30);
	}

    while(true)
    {
		// показываем картинку
		cvShowImage("original",image);

		cvShowImage( "H", h_plane );
		cvShowImage( "S", s_plane );
		cvShowImage( "V", v_plane );

		cvShowImage( "H range", h_range );
		cvShowImage( "S range", s_range );
		cvShowImage( "V range", v_range );

		// складываем 
		cvAnd(h_range, s_range, hsv_and);
		cvAnd(hsv_and, v_range, hsv_and);

		cvShowImage( "hsv and", hsv_and );

		char c = cvWaitKey(33);
        if (c == 27) // если нажата ESC - выходим
        {
			break;
		}
	}
	printf("\n[i] Results:\n" );
	printf("[H] %d x %d\n", Hmin, Hmax );
	printf("[S] %d x %d\n", Smin, Smax );
	printf("[V] %d x %d\n", Vmin, Vmax );

	// освобождаем ресурсы
	cvReleaseImage(&image);
	cvReleaseImage(&hsv);
	cvReleaseImage(&h_plane);
	cvReleaseImage(&s_plane);
	cvReleaseImage(&v_plane);
	cvReleaseImage(&h_range);
	cvReleaseImage(&s_range);
	cvReleaseImage(&v_range);
	cvReleaseImage(&hsv_and);
	// удаляем окна
	cvDestroyAllWindows();
	return 0;
}
