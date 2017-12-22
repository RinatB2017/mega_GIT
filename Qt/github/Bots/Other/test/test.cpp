//
// for testing
//
// robocraft.ru
//

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	IplImage* image=0, *dst=0;

	// имя картинки
	char filename[] = "Image0.jpg";

	// получаем картинку
	image = cvLoadImage(filename, 1);

	printf("[i] image: %s\n", filename);
	assert( image != 0 );

	// покажем изображение
	cvNamedWindow( "image");
	cvShowImage( "image", image );

	// ждём нажатия клавиши
	cvWaitKey(0);

	// освобождаем ресурсы
	cvReleaseImage(& image);
	cvReleaseImage(&dst);

	// удаляем окна
	cvDestroyAllWindows();
	return 0;
}
