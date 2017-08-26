//
// https://habrahabr.ru/sandbox/20517/
//

#define CV_NO_BACKWARD_COMPATIBILITY

#include <iostream>
#include <vector>

#include "math.h"

#include "cxcore.h"
#include "cv.h"
#include "highgui.h"

using std::vector;
using std::string;

//TODO
#define size_t  int

// функция рисует прямоугольник
void DrawRect(IplImage *src, int x, int y, int width, int height)
{
    cvRectangle(src,
                cvPoint(x, y),
                cvPoint(x + width, y + height),
                CV_RGB(255, 0, 0), 1, 0, 0 );
}

// пересекаются или нет два прямоугольника
bool Intersection(CvRect r1, CvRect r2)
{
    double x1 = r1.x + r1.width / 2.0;
    double y1 = r1.y + r1.height / 2.0;

    double x2 = r2.x + r2.width / 2.0;
    double y2 = r2.y + r2.height / 2.0;

    return abs(x1 - x2) <= (r1.width + r2.width) / 2.0 && abs(y1 - y2)<=(r1.height + r2.height) / 2.0;
}

// функция нахождения объединения двух прямоугольников
CvRect Union(CvRect r1, CvRect r2)
{
    int x = r1.x < r2.x ? r1.x : r2.x;
    int y = r1.y < r2.y ? r1.y : r2.y;
    int w = r1.x + r1.width > r2.x + r2.width ? r1.x + r1.width - x : r2.x + r2.width - x;
    int h = r1.y + r1.height > r2.y + r2.height ? r1.y + r1.height - y : r2.y + r2.height - y;

    return cvRect(x,y,w,h);
}

// поиск всех изображений похожих на templ в изображении src
// используемый алгоритм - поиск по шаблону
// src - исходное изображение
// templ - то что мы ищем в src
// arr - результат
// eps - пороговое значение для алгоритма
void FindObjects(IplImage *src, IplImage *templ, vector *arr, double eps)
{
    // создаем изображение где будем хранить результат сравнения по шаблону
    int iwidth = src->width - templ->width + 1;
    int iheight = src->height - templ->height + 1;
    IplImage* ftmp = cvCreateImage( cvSize( iwidth, iheight ), 32, src->nChannels );

    // ищем по шаблону templ в исходном изображении src
    cvMatchTemplate( src, templ, ftmp, CV_TM_SQDIFF );
    cvNormalize( ftmp, ftmp, 1, 0, CV_MINMAX );

    for(int i = 0; i < ftmp->height; ++i)
    {
        for(int j = 0; j < ftmp->width; ++j)
        {
            CvScalar s = cvGet2D(ftmp, i, j);
            if( s.val[0] < eps )
            {
                CvRect r = cvRect(j, i, templ->width, templ->height);

                bool find = false;
                for(size_t k = 0; k < arr.size(); ++k)
                {
                    find = Intersection(arr[k], r);
                    if(find)
                    {
                        arr[k] = Union(arr[k], r);
                        break;
                    }
                }

                if(!find)
                {
                    arr.push_back;
                }
            }
        }
    }

    cvReleaseImage(&ftmp);
}

// класс функтор для сортировки
// сортировка осуществляется по среднему значению яркости
class SortByPixel
{
private:
    IplImage *_src;

public:
    SortByPixel(IplImage *src) : _src(src)
    {}

    bool operator()( CvRect lhs, CvRect rhs)
    {
        // находим среднее значение яркости в прямоугольнике lhs
        cvSetImageROI(_src, lhs);
        double pix1 = cvSum(_src).val[0] / (double)(lhs.width * lhs.height);
        cvResetImageROI(_src);

        // находим среднее значение яркости в прямоугольнике rhs
        cvSetImageROI(_src, rhs);
        double pix2 = cvSum(_src).val[0] / (double)(rhs.width * rhs.height);
        cvResetImageROI(_src);

        // сравниваем
        return (pix1 < pix2);
    }
};

int main(int argc, char ** argv)
{
    const char* filename = argc >=2 ? argv[1] : "captcha.png";

    // исходное цветное изображение
    IplImage *img = cvLoadImage(filename);
    // исходное grayscale изображение
    IplImage *src = cvLoadImage(filename, 0);
    // изображение собаки
    IplImage *dog = cvLoadImage("dog.png", 0);
    // изображение охотника
    IplImage *hunter = cvLoadImage("hunter.png", 0);

    // найдем всех собак
    vector dogs;
    FindObjects(src, dog, dogs, 0.5);
    // найдем всех охотников
    vector hunters;
    FindObjects(src, hunter, hunters, 0.3);

    // отсортируем найденные объекты по среднему значению яркости
    // таким образом сопоставим по цвету собак и охотников
    SortByPixel pred(src);
    std::sort(dogs.begin(), dogs.end(), pred);
    std::sort(hunters.begin(), hunters.end(), pred);

    // отобразим результаты
    CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.7, 0.7, 0, 2, CV_AA);
    for(size_t i = 0; i < hunters.size(); ++i)
    {
        // преобразование числа в строку
        string s;
        std::stringstream out;
        out << i;
        s = out.str();

        // рисуем текст по которому сопоставим охотников и собак
        cvPutText(img, s.c_str(), cvPoint(dogs[i].x, dogs[i].y + dogs[i].height), &font, cvScalar(0, 0, 0, 0));
        cvPutText(img, s.c_str(), cvPoint(hunters[i].x, hunters[i].y + hunters[i].height), &font, cvScalar(0, 0, 0, 0));

        // рисуем рамки вокруг охотников и собак
        DrawRect(img, dogs[i].x, dogs[i].y, dogs[i].width, dogs[i].height);
        DrawRect(img, hunters[i].x, hunters[i].y, hunters[i].width, hunters[i].height);
    }

    // показываем результат
    cvShowImage("win", img);
    cvWaitKey(0);

    // освобождаем ресурсы
    cvDestroyAllWindows();
    cvReleaseImage(&src);
    cvReleaseImage(&dog);
    cvReleaseImage(&hunter);
    cvReleaseImage(&img);

    return 0;
}
