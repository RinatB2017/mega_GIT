/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
**                                                                              **
**     This program is free software: you can redistribute it and/or modify     **
**     it under the terms of the GNU General Public License as published by     **
**     the Free Software Foundation, either version 3 of the License, or        **
**     (at your option) any later version.                                      **
**                                                                              **
**     This program is distributed in the hope that it will be useful,          **
**     but WITHOUT ANY WARRANTY; without even the implied warranty of           **
**     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **
**     GNU General Public License for more details.                             **
**                                                                              **
**     You should have received a copy of the GNU General Public License        **
**     along with this program.  If not, see http://www.gnu.org/licenses/.      **
**                                                                              **
**********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "other.hpp"
//--------------------------------------------------------------------------------
// http://alexkutsan.blogspot.ru/2012/10/cvseq-opencv.html
// http://snipplr.com/view/40277/iplimage-to-qimage--qimage-to-iplimage/
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

    if(sizeHint().height() > 0)
    {
        setMinimumHeight(sizeHint().height());
    }
}
//--------------------------------------------------------------------------------
QToolButton *MainBox::add_button(QToolBar *tool_bar,
                                 QToolButton *tool_button,
                                 QIcon icon,
                                 const QString &text,
                                 const QString &tool_tip)
{
    if(!tool_bar)       return nullptr;
    if(!tool_button)    return nullptr;

    tool_button->setIcon(icon);
    tool_button->setText(text);
    tool_button->setToolTip(tool_tip);
    tool_button->setObjectName(text);
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw == nullptr)
    {
        return;
    }

    commands.clear();
    commands.append({ 1000, "run_kmines",       &MainBox::run_kmines });
    commands.append({ 1001, "find_circles",     &MainBox::find_circles });
    commands.append({ 1002, "find_rectangles",  &MainBox::find_rectangles });
    commands.append({ 1003, "find_contours",    &MainBox::find_contours });
    commands.append({ 1004, "compare_pic",      &MainBox::compare_pic });

    commands.append({ 2001, "test 0", &MainBox::test_0 });
    commands.append({ 2002, "test 1", &MainBox::test_1 });
    commands.append({ 2003, "test 2", &MainBox::test_2 });
    commands.append({ 2004, "test 3", &MainBox::test_3 });
    commands.append({ 2005, "test 4", &MainBox::test_4 });
    commands.append({ 2006, "test 5", &MainBox::test_5 });
    commands.append({ 2007, "test 6", nullptr });

    QToolBar *toolBar = new QToolBar(tr("testbar"));
    toolBar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    QCheckBox *cb_block = new QCheckBox("block");
    toolBar->addWidget(cb_block);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(Qt::UserRole + command.cmd));
    }

    toolBar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(toolBar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    connect(cb_block, SIGNAL(clicked(bool)), cb_test,           SLOT(setDisabled(bool)));
    connect(cb_block, SIGNAL(clicked(bool)), btn_choice_test,   SLOT(setDisabled(bool)));

    //toolBar->setFixedWidth(toolBar->sizeHint().width());
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef bool (MainBox::*function)(void);
            function x;
            x = command.func;
            if(x)
            {
                (this->*x)();
            }
            else
            {
                emit error("no func");
            }

            return;
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::mouse_click(unsigned int button, QPoint pos)
{
    Display* display = XOpenDisplay( nullptr );
    if( display == nullptr )
    {
        return;
    }

    QCursor::setPos(pos);
    XTestFakeButtonEvent( display, button, true, CurrentTime );

    XFlush( display );
    XCloseDisplay( display );
}
//--------------------------------------------------------------------------------
void MainBox::mouse_release(unsigned int button)
{
    Display* display = XOpenDisplay( nullptr );
    if( display == nullptr )
    {
        return;
    }

    XTestFakeButtonEvent( display, button, false, CurrentTime );

    XFlush( display );
    XCloseDisplay( display );
}
//--------------------------------------------------------------------------------
void MainBox::mouse_move_to(QPoint pos)
{
    Display* display = XOpenDisplay( nullptr );
    if( display == nullptr )
    {
        return;
    }

    XTestFakeMotionEvent( display, 0, pos.x(), pos.y(), CurrentTime );

    XFlush( display );
    XCloseDisplay( display );
}
//--------------------------------------------------------------------------------
bool MainBox::run_kmines(void)
{
    QString program = "kmines";
    QStringList arguments;

    emit info(QString("run %1").arg(program));

    QProcess *myProcess = new QProcess(this);
    myProcess->start(program, arguments);
    bool ok = myProcess->waitForStarted();
    if(ok == false)
    {
        messagebox_critical("Error", "QProcess::waitForStarted return false");
        return false;
    }

    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::find_circles(void)
{
    emit info("find_circles()");

    QString filename = "test_circle.png";

    //---
    //IplImage *temp_image = cvLoadImage(filename.toLocal8Bit().data(), CV_LOAD_IMAGE_GRAYSCALE);
    IplImage *temp_image = cvLoadImage(filename.toLocal8Bit().data(), CV_LOAD_IMAGE_COLOR);

    //IplImage *image = cvLoadImage(filename.toLocal8Bit().data(), CV_LOAD_IMAGE_GRAYSCALE);
    IplImage *image = cvCreateImage( cvGetSize(temp_image), IPL_DEPTH_8U, 1);
    IplImage *new_image = cvCreateImage( cvGetSize(temp_image), temp_image->depth, temp_image->nChannels);

    // конвертируем в градации серого
    cvConvertImage(temp_image, image, CV_BGR2GRAY);
    // находим границы
    //cvCanny(image, image, 50, 200);
    cvCanny(image, image, 10, 100);

    // хранилище памяти для контуров
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* contours;

    // находим контуры
    int contoursCount = cvFindContours( image,
                                        storage,
                                        &contours,
                                        sizeof(CvContour),
                                        CV_RETR_TREE,    //CV_RETR_LIST,
                                        CV_CHAIN_APPROX_SIMPLE,
                                        cvPoint(0,0));

    assert(contours != nullptr);

    emit info(QString("contoursCont %1").arg(contoursCount));

    // обходим все контуры
    for( CvSeq* current = contours; current != nullptr; current = current->h_next )
    {
        double area = fabs(cvContourArea(current));
        double perim = cvContourPerimeter(current);

        // 1/4*CV_PI = 0,079577
        if ( area / (perim * perim) > 0.07 && area / (perim * perim)< 0.087 )
        {
            emit trace(QString("find circle: perim %1 area %2").arg(perim).arg(area));
            // рисуем контур
            cvDrawContours(new_image,
                           current,
                           cvScalar(255,0,0),
                           cvScalar(0,0,255),
                           0, //0
                           1,
                           CV_AA,
                           cvPoint(0,0));
        }
        else
        {
            CvRect Rect;
            Rect = cvBoundingRect( current );
            cvRectangle(new_image,
                        cvPoint( Rect.x, Rect.y ),
                        cvPoint( Rect.x + Rect.width, Rect.y + Rect.height ),
                        CV_RGB(255,255,255),
                        1);
        }
    }

    cvNamedWindow("test", CV_WINDOW_AUTOSIZE);
    cvShowImage("test", new_image);
    cvWaitKey(0);

    //---
    cvReleaseImage(&new_image);
    cvReleaseImage(&temp_image);
    cvReleaseImage(&image);
    cvDestroyAllWindows();
    //---

    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
IplImage *MainBox::QImageToIplImage2(QImage qImage)
{
    int width  = qImage.width();
    int height = qImage.height();

    CvSize size;
    size.width = width;
    size.height = height;

    emit info(QString("QImageToIplImage2: w %1").arg(size.width));
    emit info(QString("QImageToIplImage2: h %1").arg(size.height));

    IplImage *charIplImageBuffer = cvCreateImage(size, IPL_DEPTH_8U, 1);
    char *charTemp = (char *) charIplImageBuffer->imageData;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = y * width + x;
            charTemp[index] = (char) qGray(qImage.pixel(x, y));
        }
    }
    return charIplImageBuffer;
}
//--------------------------------------------------------------------------------
QImage *MainBox::IplImageToQImage(const IplImage *iplImage,
                                  uchar **data,
                                  double mini,
                                  double maxi)
{
    uchar *qImageBuffer = nullptr;
    int width = iplImage->width;

    // Note here that OpenCV image is stored so that each lined is
    // 32-bits aligned thus * explaining the necessity to "skip"
    // the few last bytes of each line of OpenCV image buffer.
    int widthStep = iplImage->widthStep;
    int height = iplImage->height;

    switch (iplImage->depth)
    {
    case IPL_DEPTH_8U:
        if (iplImage->nChannels == 1)
        {
            // IplImage is stored with one byte grey pixel.
            // We convert it to an 8 bit depth QImage.
            qImageBuffer = (uchar *) malloc(width*height*sizeof(uchar));
            uchar *QImagePtr = qImageBuffer;
            const uchar *iplImagePtr = (const uchar *)iplImage->imageData;
            for (int y = 0; y < height; y++)
            {
                // Copy line by line
                memcpy(QImagePtr, iplImagePtr, width);
                QImagePtr += width;
                iplImagePtr += widthStep;
            }
        }
        else if (iplImage->nChannels == 3)
        {
            // IplImage is stored with 3 byte color pixels (3 channels).
            // We convert it to a 32 bit depth QImage.
            qImageBuffer = (uchar *) malloc(width*height*4*sizeof(uchar));
            uchar *QImagePtr = qImageBuffer;
            const uchar *iplImagePtr = (const uchar *) iplImage->imageData;

            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    // We cannot help but copy manually.
                    QImagePtr[0] = iplImagePtr[0];
                    QImagePtr[1] = iplImagePtr[1];
                    QImagePtr[2] = iplImagePtr[2];
                    QImagePtr[3] = 0;

                    QImagePtr += 4;
                    iplImagePtr += 3;
                }
                iplImagePtr += widthStep-3*width;
            }
        }
        else
        {
            qDebug("IplImageToQImage: image format is not supported:\
                   depth=8U and %d channels\n", iplImage->nChannels);
        }
        break;

    case IPL_DEPTH_16U:
        if (iplImage->nChannels == 1)
        {
            // IplImage is stored with 2 bytes grey pixel.
            // We convert it to an 8 bit depth QImage.
            qImageBuffer = (uchar *) malloc(width*height*sizeof(uchar));
            uchar *QImagePtr = qImageBuffer;
            const uint16_t *iplImagePtr = (const uint16_t *)iplImage->imageData;

            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    // We take only the highest part of the 16 bit value.
                    // It is similar to dividing by 256.
                    *QImagePtr++ = ((*iplImagePtr++) >> 8);
                }
                iplImagePtr += widthStep/sizeof(uint16_t)-width;
            }
        }
        else
        {
            qDebug("IplImageToQImage: image format is not supported:\
                   depth=16U and %d channels\n", iplImage->nChannels);
        }
        break;

    case IPL_DEPTH_32F:
        if (iplImage->nChannels == 1)
        {
            // IplImage is stored with float (4 bytes) grey pixel.
            // We convert it to an 8 bit depth QImage.
            qImageBuffer = (uchar *) malloc(width*height*sizeof(uchar));
            uchar *QImagePtr = qImageBuffer;
            const float *iplImagePtr = (const float *) iplImage->imageData;

            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    uchar p;
                    float pf = 255 * ((*iplImagePtr++) - mini) / (maxi - mini);

                    if (pf < 0) p = 0;
                    else if (pf > 255) p = 255;
                    else p = (uchar) pf;

                    *QImagePtr++ = p;
                }
                iplImagePtr += widthStep/sizeof(float)-width;
            }
        }
        else
        {
            qDebug("IplImageToQImage: image format is not supported:\
                   depth=32F and %d channels\n", iplImage->nChannels);
        }
        break;

    case IPL_DEPTH_64F:
        if (iplImage->nChannels == 1)
        {
            // OpenCV image is stored with double (8 bytes) grey pixel.
            // We convert it to an 8 bit depth QImage.
            qImageBuffer = (uchar *) malloc(width*height*sizeof(uchar));
            uchar *QImagePtr = qImageBuffer;
            const double *iplImagePtr = (const double *) iplImage->imageData;

            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    uchar p;
                    double pf = 255 * ((*iplImagePtr++) - mini) / (maxi - mini);

                    if (pf < 0) p = 0;
                    else if (pf > 255) p = 255;
                    else p = (uchar) pf;

                    *QImagePtr++ = p;
                }
                iplImagePtr += widthStep/sizeof(double)-width;
            }
        }
        else
        {
            qDebug("IplImageToQImage: image format is not supported:\
                   depth=64F and %d channels\n", iplImage->nChannels);
        }
        break;

    default:
        qDebug("IplImageToQImage: image format is not supported: depth=%d\
               and %d channels\n", iplImage->depth, iplImage->nChannels);
    }

    QImage *qImage;
    if (iplImage->nChannels == 1)
    {
        QVector<QRgb> colorTable;
        for (int i = 0; i < 256; i++)
        {
            colorTable.push_back(qRgb(i, i, i));
        }
        qImage = new QImage(qImageBuffer, width, height, QImage::Format_Indexed8);
        qImage->setColorTable(colorTable);
    }
    else
    {
        qImage = new QImage(qImageBuffer, width, height, QImage::Format_RGB32);
    }
    *data = qImageBuffer;

    return qImage;
}
//--------------------------------------------------------------------------------
IplImage *MainBox::QImageToIplImage(const QImage *qImage, int *correct_width)
{
    int width  = qImage->width();
    int height = qImage->height();

    CvSize size;
    size.height = height;
    size.width  = width;

    emit info(QString("QImageToIplImage: w %1").arg(size.width));
    emit info(QString("QImageToIplImage: h %1").arg(size.height));

    IplImage *charIplImageBuffer = cvCreateImage(size, IPL_DEPTH_8U, 1);

    emit info(QString("charIplImageBuffer: w %1").arg(charIplImageBuffer->width));
    emit info(QString("charIplImageBuffer: h %1").arg(charIplImageBuffer->height));

    emit info(QString("charIplImageBuffer->widthStep %1").arg(charIplImageBuffer->widthStep));

    uchar *charTemp = (uchar *) charIplImageBuffer->imageData;

    *correct_width = charIplImageBuffer->widthStep - width;

    int index = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            charTemp[index++] = (uchar) qGray(qImage->pixel(x, y));
        }
        index+=(charIplImageBuffer->widthStep - width);
    }
    return charIplImageBuffer;
}
//--------------------------------------------------------------------------------
bool MainBox::find_rectangles(void)
{
    emit info("find_circles()");

    QString kmines_name = "KMines";
    //QString filename = "/dev/shm/test.png";

    bool ok = find_window(kmines_name, &x, &y, &w, &h);
    if(ok == false)
    {
        messagebox_critical("Error", QString("%1 not found!").arg(kmines_name));
        return false;
    }

    emit info(QString("x %1").arg(x));
    emit info(QString("y %1").arg(y));
    emit info(QString("w %1").arg(w));
    emit info(QString("h %1").arg(h));

    QScreen *screen = QGuiApplication::primaryScreen();
    if(screen == nullptr)
    {
        emit error("error screen");
        return false;
    }

    QPixmap screen_shot = screen->grabWindow(0, x, y, w, h);
    //---
    QImage im = screen_shot.toImage();
    emit info(QString("im w %1").arg(im.width()));
    emit info(QString("im h %1").arg(im.height()));

    //---
#if 0
    QLabel *lbl = new QLabel();
    lbl->setWindowTitle("im");
    lbl->setPixmap(QPixmap::fromImage(im,Qt::AutoColor));
    lbl->setFixedSize(im.width(), im.height());
    lbl->show();
#endif
    //---

    int correct_width = 0;
    IplImage *temp_image = QImageToIplImage(&im, &correct_width);
    cvNamedWindow("temp_image", 0);
    cvShowImage("temp_image", temp_image);

    IplImage *image = cvCreateImage( cvGetSize(temp_image), IPL_DEPTH_8U, 1 );
    IplImage *new_image = cvCreateImage( cvGetSize(temp_image), temp_image->depth, temp_image->nChannels );
    cvSet(new_image, cvScalar(0,0,0));    //BGR

    // конвертируем в градации серого
    cvConvertImage(temp_image, image, CV_BGR2GRAY);
    // находим границы
    cvCanny(image, image, 50, 200);
    //cvCanny(image, image, 10, 100);

    // хранилище памяти для контуров
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* contours;

    // находим контуры
    int contoursCount = cvFindContours( image,
                                        storage,
                                        &contours,
                                        sizeof(CvContour),
                                        CV_RETR_LIST,
                                        CV_CHAIN_APPROX_SIMPLE,
                                        cvPoint(0,0) );

    assert(contours != nullptr);

    emit info(QString("contoursCount %1").arg(contoursCount));

    // обходим все контуры
    for( CvSeq* current = contours; current != NULL; current = current->h_next )
    {
        double area = fabs(cvContourArea(current));
        double perim = cvContourPerimeter(current);

        if(area > 100000)
        {
            emit trace(QString("find obj: perim %1 area %2").arg(perim).arg(area));
            // рисуем контур
#if 0
            cvDrawContours(new_image,
                           current,
                           cvScalar(255,0,0),
                           cvScalar(0,0,255),
                           0, //0
                           1,
                           CV_AA,
                           cvPoint(0,0));
#endif

            CvRect Rect;
            Rect = cvBoundingRect( current );
            emit error(QString("Rect.x %1").arg(Rect.x));
            emit error(QString("Rect.y %1").arg(Rect.y));
            emit error(QString("Rect.w %1").arg(Rect.width));
            emit error(QString("Rect.h %1").arg(Rect.height));
            emit error(QString("correct_width %1").arg(correct_width));

            //---
            QPixmap game_shot = screen_shot.copy(Rect.x,
                                                 Rect.y,
                                                 Rect.width,
                                                 Rect.height);

            int inc_w = game_shot.width() / 30;
            int inc_h = game_shot.height() / 16;

            emit info(QString("inc_w %1").arg(inc_w));
            emit info(QString("inc_h %1").arg(inc_h));

            QPainter paint;
            paint.begin(&game_shot);
            paint.setPen(Qt::red);
            for(int x=0; x<30; x++)
            {
                paint.drawLine(x*inc_w, 0, x*inc_w, game_shot.height());
            }
            for(int y=0; y<16; y++)
            {
                paint.drawLine(0, y*inc_h, game_shot.width(), y*inc_h);
            }
            paint.end();

            game_shot.save("/dev/shm/game_shot.png");

            QLabel *lbl = new QLabel();
            QPixmap test_pixmap = game_shot.copy(0, 0, inc_w*30, inc_h*16);

            lbl->setWindowTitle("game_shot");
            lbl->setPixmap(test_pixmap);
            lbl->setFixedSize(game_shot.width(), game_shot.height());
            lbl->show();
            //---
            analize(&test_pixmap, inc_w, inc_h);
            //---

            break;
        }
    }

#if 0
    cvNamedWindow("test", CV_WINDOW_AUTOSIZE);
    cvShowImage("test", new_image);
    cvWaitKey(0);
#endif
    //---
    cvReleaseImage(&new_image);
    cvReleaseImage(&temp_image);
    cvReleaseImage(&image);
    cvDestroyAllWindows();
    //---

    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::find_contours(void)
{
    emit info("find_contours()");

    QString kpatiens_name = "KPatience";

    bool ok = find_window(kpatiens_name, &x, &y, &w, &h);
    if(ok == false)
    {
        messagebox_critical("Error", QString("%1 not found!").arg(kpatiens_name));
        return false;
    }

    emit info(QString("x %1").arg(x));
    emit info(QString("y %1").arg(y));
    emit info(QString("w %1").arg(w));
    emit info(QString("h %1").arg(h));

    QScreen *screen = QGuiApplication::primaryScreen();
    if(screen == nullptr)
    {
        emit error("error screen");
        return false;
    }

    QPixmap screen_shot = screen->grabWindow(0, x, y, w, h);
    //---
    QImage im = screen_shot.toImage();
    emit info(QString("im w %1").arg(im.width()));
    emit info(QString("im h %1").arg(im.height()));
    //---

    int correct_width = 0;
    IplImage *temp_image = QImageToIplImage(&im, &correct_width);
    cvNamedWindow("temp_image", 0);
    cvShowImage("temp_image", temp_image);

    IplImage *image = cvCreateImage( cvGetSize(temp_image), IPL_DEPTH_8U, 1 );
    IplImage *new_image = cvCreateImage( cvGetSize(temp_image), temp_image->depth, temp_image->nChannels );
    cvSet(new_image, cvScalar(0,0,0));    //BGR

    // конвертируем в градации серого
    cvConvertImage(temp_image, image, CV_BGR2GRAY);
    // находим границы
    cvCanny(image, image, 50, 200);
    //cvCanny(image, image, 10, 100);

    // хранилище памяти для контуров
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* contours;

    // находим контуры
    int contoursCount = cvFindContours( image,
                                        storage,
                                        &contours,
                                        sizeof(CvContour),
                                        CV_RETR_LIST,
                                        CV_CHAIN_APPROX_SIMPLE,
                                        cvPoint(0,0));

    if(contours == nullptr)
    {
        messagebox_critical("Error", "contours == 0");
        return false;
    }

    emit info(QString("contoursCount %1").arg(contoursCount));

    // обходим все контуры
    for( CvSeq* current = contours; current != NULL; current = current->h_next )
    {
#if 0
        double area = fabs(cvContourArea(current));
        double perim = cvContourPerimeter(current);
        emit message(QString("find obj: perim %1 area %2").arg(perim).arg(area));
#endif

        // рисуем контур
        cvDrawContours(new_image,
                       current,
                       cvScalar(255,0,0),
                       cvScalar(0,0,255),
                       -1, //0
                       1,
                       CV_AA,
                       cvPoint(0,0));
    }

    cvNamedWindow("test", CV_WINDOW_AUTOSIZE);
    cvShowImage("test", new_image);
    cvWaitKey(0);
    //---
    cvReleaseImage(&new_image);
    cvReleaseImage(&temp_image);
    cvReleaseImage(&image);
    cvDestroyAllWindows();
    //---

    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::analize(QPixmap *pixmap,
                      int inc_x,
                      int inc_y)
{
    if(pixmap == nullptr)
    {
        emit error("pixmap == nullptr");
        return false;
    }

    int max_x = pixmap->width() / inc_x;
    int max_y = pixmap->height() / inc_y;

    emit info(QString("max_x %1").arg(max_x));
    emit info(QString("max_y %1").arg(max_y));

    for(int y=0; y<max_y; y++)
    {
        QString temp;
        for(int x=0; x<max_x; x++)
        {
            temp.append(get_digit(pixmap->copy(x*inc_x, y*inc_y, inc_x, inc_y), x, y));
        }
        emit info(temp);
    }

    return true;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool MainBox::compare_pic(void)
{
    IplImage *src;
    IplImage *templ;
    IplImage *ftmp;

    //---
    src = cvLoadImage("images/1.png", CV_LOAD_IMAGE_COLOR);
    //---
    templ = cvLoadImage("images/1.png", CV_LOAD_IMAGE_COLOR);
    //---
    emit info(QString("src w %1").arg(src->width));
    emit info(QString("src h %1").arg(src->height));
    emit info(QString("templ w %1").arg(templ->width));
    emit info(QString("templ h %1").arg(templ->height));
    //---
    ftmp = cvCreateImage(
                CvSize(src->width-templ->width+1,
                       src->height-templ->height+1),
                IPL_DEPTH_32F,
                1);
    //---
#if 1
    cvNamedWindow("src", CV_WINDOW_AUTOSIZE);
    cvShowImage("src", src);
    //---
    cvNamedWindow("templ", CV_WINDOW_AUTOSIZE);
    cvShowImage("templ", templ);
    //---
    cvNamedWindow("ftmp", CV_WINDOW_AUTOSIZE);
    cvShowImage("ftmp", ftmp);
    //---
    cvWaitKey(0);
#endif
    //---
    cvMatchTemplate(src,
                    templ,
                    ftmp,
                    CV_TM_SQDIFF);

    //---
    cvDestroyAllWindows();
    //---
    return true;
}
//--------------------------------------------------------------------------------
#if 1
QString MainBox::get_digit(QPixmap pixmap, int x, int y)
{
    Q_UNUSED(x)
    Q_UNUSED(y)

    IplImage *src;
    IplImage *templ;
    IplImage *ftmp;

    //---
    CvSize size;
    //---
    size.width  = pixmap.width();
    size.height = pixmap.height();

    emit info(QString("get_digit: w %1").arg(size.width));
    emit info(QString("get_digit: h %1").arg(size.height));

    //---
#if 1
    src = cvCreateImage(size, IPL_DEPTH_32F, 1);
    char *charTemp = (char *) src->imageData;

    QImage image = pixmap.toImage();
    for (int y = 0; y < size.height; y++)
    {
        for (int x = 0; x < size.width; x++)
        {
            int index = y * size.width + x;
            charTemp[index] = (char) image.pixel(x, y);
        }
    }
#else
    size.width  = 23;
    size.height = 23;

    src = cvLoadImage("images/2.png", CV_LOAD_IMAGE_COLOR);
#endif
    //---
    templ = cvLoadImage("images/2.png", CV_LOAD_IMAGE_COLOR);
    //---
    ftmp = cvCreateImage(
                CvSize(src->width-templ->width+1,
                       src->height-templ->height+1),
                IPL_DEPTH_32F,
                1);
    //---
    cvNamedWindow("src", CV_WINDOW_AUTOSIZE);
    cvShowImage("src", src);
    //---
    cvNamedWindow("templ", CV_WINDOW_AUTOSIZE);
    cvShowImage("templ", templ);
    //---
    cvNamedWindow("ftmp", CV_WINDOW_AUTOSIZE);
    cvShowImage("ftmp", ftmp);
    //---
    cvWaitKey(0);
    //---
    cvMatchTemplate(src,
                    templ,
                    ftmp,
                    CV_TM_SQDIFF);

    //---
    cvDestroyAllWindows();
    //---
    return "*";
}
#endif

#if 0
QString MainBox::get_digit(QPixmap pixmap, int x, int y)
{
    Q_UNUSED(x)
    Q_UNUSED(y)

    //QImage image = pixmap.toImage().convertToFormat(QImage::Format_Mono);
    QImage image = pixmap.toImage();

    int CORRELATION_THRESHOLD = pixmap.width() * pixmap.height() * 0.8;
    int sum =0;

    bool ok = false;
    QPixmap pixmap_1;

    //---
    ok = pixmap_1.load("images/1.png");
    if(ok == false)
    {
        return "-";
    }
    sum = 0;
    QImage image_1 = pixmap_1.toImage();
    for(int x=0;x<pixmap.width(); x++)
    {
        for(int y=0; y<pixmap.height(); y++)
        {
            if(image.pixel(x,y) == image_1.pixel(x,y))
            {
                sum++;
            }
        }
        if(sum > CORRELATION_THRESHOLD)
        {
            return "1";
        }
        //        else
        //        {
        //            emit error(QString("sum %1").arg(sum));
        //        }
    }
    //---

    return "X";
}
#endif

#if 0
QString MainBox::get_digit(QPixmap pixmap, int x, int y)
{
#if 0
    pixmap.save(QString("%1_%2.png").arg(x).arg(y));
#else
    Q_UNUSED(x)
    Q_UNUSED(y)
#endif

    int color_r = 0;
    int color_g = 0;
    int color_b = 0;

    //---
#if 0
    QLabel *lbl = new QLabel();
    lbl->setPixmap(pixmap);
    lbl->setFixedSize(pixmap.width(), pixmap.height());
    lbl->show();
#endif
    //---

    QImage image = pixmap.toImage();

    color_r = 0;
    color_g = 0;
    color_b = 0;
    for(int y=1; y<(pixmap.height() - 1); y++)
    {
        for(int x=1; x<(pixmap.width() - 1); x++)
        {
            QRgb rgb = image.pixel(x, y);
            get_color(rgb, &color_r, &color_g, &color_b);
        }
    }
    if((color_r > color_g) && (color_r > color_b))
    {
        return "R"; //R
    }
    if((color_g > color_r) && (color_g > color_b))
    {
        return "G"; //G
    }
    if((color_b > color_r) && (color_b > color_g))
    {
        return "B"; //B
    }
    return "#";
}
#endif
//--------------------------------------------------------------------------------
void MainBox::get_color(QRgb rgb,
                        int *r,
                        int *g,
                        int *b)
{
    int temp_r = qRed(rgb);
    int temp_g = qGreen(rgb);
    int temp_b = qBlue(rgb);

    if((temp_r == temp_g) && (temp_r == temp_b) && (temp_g == temp_b))
    {
        return;
    }
    if((temp_r > temp_g) && (temp_r > temp_b))
    {
        (*r)++;
        return;
    }
    if((temp_g > temp_r) && (temp_g > temp_b))
    {
        (*g)++;
        return;
    }
    if((temp_b > temp_r) && (temp_b > temp_g))
    {
        (*b)++;
        return;
    }

}
//--------------------------------------------------------------------------------
bool MainBox::find_window(const QString programm_title,
                          int *x,
                          int *y,
                          int *width,
                          int *heigth)
{
    Display* display = XOpenDisplay( nullptr );
    if( display == nullptr )
    {
        return false;
    }

    bool is_found = false;
    ulong count = 0;
    Window* wins = findWindows( display, &count );
    char* name;
    QString temp;

    for( ulong i = 0; i < count; ++i )
    {
        Window w = wins[ i ];
        name = getWindowName( display, w );
        if( name )
        {
            temp.clear();
            temp.append(name);
            XFree( name );
        }

        XWindowAttributes attrs;
        if( XGetWindowAttributes( display, w, &attrs ) )
        {
            Window child;
            if( XTranslateCoordinates( display,
                                       w, attrs.root,
                                       0, 0,
                                       &attrs.x, &attrs.y,
                                       &child
                                       ) )
            {
                if(temp.contains(programm_title))
                {
                    *x = attrs.x;
                    *y = attrs.y;
                    *width = attrs.width;
                    *heigth = attrs.height;
                    is_found = true;
                }
            }
        }
    }

    if( wins )
    {
        XFree( wins );
    }

    XCloseDisplay( display );

    return is_found;
}
//--------------------------------------------------------------------------------
int MainBox::get_corr_x1(QPixmap *pix)
{
    emit trace("get_corr_x1");

    QImage image = pix->toImage();
    for(int n=1; n<100; n++)
    {
        QRgb rgb = image.pixel(n, h/2);
        int r = qRed(rgb);
        int g = qGreen(rgb);
        int b = qBlue(rgb);
        emit trace(QString("%1 %2 %3 %4 %5")
                     .arg(rgb)
                     .arg(r)
                     .arg(g)
                     .arg(b)
                     .arg(qAlpha(rgb)));
        if((r!=148 && b!=148 && g!=148) && (r!=136 && b!=136 && g!=136))
        {
            return n;
        }
    }
    return 0;
}
//--------------------------------------------------------------------------------
int MainBox::get_corr_y1(QPixmap *pix)
{
    emit trace("get_corr_y1");

    QImage image = pix->toImage();
    for(int n=1; n<100; n++)
    {
        QRgb rgb = image.pixel(w/2, n);
        int r = qRed(rgb);
        int g = qGreen(rgb);
        int b = qBlue(rgb);
        emit trace(QString("%1 %2 %3 %4 %5")
                     .arg(rgb)
                     .arg(r)
                     .arg(g)
                     .arg(b)
                     .arg(qAlpha(rgb)));
        if((r!=148 && b!=148 && g!=148) && (r!=136 && b!=136 && g!=136))
        {
            return n;
        }
    }
    return 0;
}
//--------------------------------------------------------------------------------
int MainBox::get_corr_x2(QPixmap *pix)
{
    emit trace("get_corr_x2");

    QImage image = pix->toImage();
    for(int n=2; n<100; n++)
    {
        QRgb rgb = image.pixel(w - n - 1, h/2);
        int r = qRed(rgb);
        int g = qGreen(rgb);
        int b = qBlue(rgb);
        emit trace(QString("%1 %2 %3 %4 %5")
                     .arg(rgb)
                     .arg(r)
                     .arg(g)
                     .arg(b)
                     .arg(qAlpha(rgb)));
        if((r!=148 && b!=148 && g!=148) && (r!=136 && b!=136 && g!=136))
        {
            return n;
        }
    }
    return 0;
}
//--------------------------------------------------------------------------------
#if 0
int MainBox::get_corr_y2(QPixmap *pix)
{
    emit message("get_corr_y2");

    QImage image = pix->toImage();
    for(int n=1; n<100; n++)
    {
        QRgb rgb = image.pixel(w/2, h - n - 1);
        int r = qRed(rgb);
        int g = qGreen(rgb);
        int b = qBlue(rgb);
        emit message(QString("%1 %2 %3 %4 %5")
                     .arg(rgb)
                     .arg(r)
                     .arg(g)
                     .arg(b)
                     .arg(qAlpha(rgb)));
        if((r!=148 && b!=148 && g!=148) && (r!=136 && b!=136 && g!=136))
        {
            return n;
        }
    }
    return 0;
}
#else
int MainBox::get_corr_y2(QPixmap *pix)
{
    emit trace("get_corr_y2");

    int begin_y = h /2;
    QImage image = pix->toImage();
    for(int n=0; n<h/2; n++)
    {
        QRgb rgb = image.pixel(w/2, begin_y);
        int r = qRed(rgb);
        int g = qGreen(rgb);
        int b = qBlue(rgb);
        emit info(QString("%1 %2 %3 %4 %5")
                  .arg(rgb)
                  .arg(r)
                  .arg(g)
                  .arg(b)
                  .arg(qAlpha(rgb)));
        if((r==148 && b==148 && g==148) || (r==136 && b==136 && g==136))
        {
            return begin_y;
        }
        begin_y++;
    }
    return 0;
}
#endif
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit info("Test_0()");

    QString kmines_name = "KMines";

    bool ok = find_window(kmines_name, &x, &y, &w, &h);
    if(ok == false)
    {
        messagebox_critical("Error", QString("%1 not found!").arg(kmines_name));
        return false;
    }

    emit info(QString("x %1").arg(x));
    emit info(QString("y %1").arg(y));
    emit info(QString("w %1").arg(w));
    emit info(QString("h %1").arg(h));

    QScreen *screen = QGuiApplication::primaryScreen();
    if(screen == nullptr)
    {
        emit error("error screen");
        return false;
    }

    //костыль
    //y+=66;
    //h-=(66 + 21);

    QPixmap screen_shot = screen->grabWindow(0, x, y, w, h);
    //screen_shot.save("test.png");

    int m_numCols = 30;
    int m_numRows = 16;

    //bool chooseHorizontalSide = (m_numCols+2) / screen_shot.width() > (m_numRows+2) / screen_shot.height();
    //qreal size = 0;

    //int w_size = (int)((float)w / (float)(m_numCols+2) + 0.5f);
    int w_size = w / (m_numCols+2);
    int h_size = h / (m_numRows+2);

    emit info(QString("w_size %1").arg(w_size));
    emit info(QString("h_size %1").arg(h_size));

#if 1
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;
    x1 = (w - w_size * m_numCols) / 2;
    x2 = w - x1;
    y1 = (h - h_size * m_numRows) / 2;
    y2 = h - y1;

    emit info(QString("x1 %1").arg(x1));
    emit info(QString("y1 %1").arg(y1));
    emit info(QString("x2 %1").arg(x2));
    emit info(QString("y2 %1").arg(y2));

    QPainter paint;
    paint.begin(&screen_shot);

    paint.drawLine ( x1, 0, x1, h );
    paint.drawLine ( x2, 0, x2, h );

    paint.drawLine ( 0, y1, w, y1 );
    paint.drawLine ( 0, y2, w, y2 );

    paint.end();
#endif

#if 0
    int x1 = get_corr_x1(&screen_shot);
    int y1 = get_corr_y1(&screen_shot);
    int x2 = get_corr_x2(&screen_shot);
    int y2 = get_corr_y2(&screen_shot);

    QPainter paint;
    paint.begin(&screen_shot);

    paint.drawLine ( x1, 0, x1, h );
    paint.drawLine ( w-x2, 0, w-x2, h );

    paint.drawLine ( 0, y1, w, y1 );
    paint.drawLine ( 0, h+y2, w, h+y2 );

    paint.end();
#endif

#if 0
    int temp_x = 36;
    int temp_y = 91;

    QPainter paint;
    paint.begin(&screen_shot);

    for(int n=0; n<=16; n++)
    {
        paint.drawLine ( 0, temp_y, w, temp_y );
        temp_y+=24;
    }
    for(int n=0; n<=30; n++)
    {
        paint.drawLine ( temp_x, 0, temp_x, h );
        temp_x+=24;
    }

    paint.end();
#endif

#if 1
    QLabel *lbl = new QLabel();
    lbl->setWindowTitle("screen_shot");
    lbl->setPixmap(screen_shot);
    lbl->installEventFilter(this);
    lbl->setFixedSize(screen_shot.width(), screen_shot.height());
    lbl->show();
#endif

    emit info("OK");

#if 0
    QRect available_geom = QDesktopWidget().availableGeometry();
    QPoint pos = QPoint(available_geom.center().x(),
                        available_geom.center().y());
    mouse_click(Qt::LeftButton, pos);
    mouse_release(Qt::LeftButton);
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_1()");

    emit info(QString("w %1").arg(width()));
    emit info(QString("h %1").arg(height()));

    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw == nullptr)
    {
        return false;
    }

    int sb = mw->statusBar()->height();
    int h_cw = mw->centralWidget()->height();

    emit info(QString("mw w %1").arg(mw->width()));
    emit info(QString("mw h %1").arg(mw->height()));
    emit info(QString("mw statusBar h %1").arg(sb));
    emit info(QString("mw cw h %1").arg(h_cw));

    emit info(QString("заголовок %1").arg(mw->height() - h_cw - sb));

    QToolBar tb;
    int res = mw->toolBarArea(&tb);
    if(res != Qt::NoToolBarArea)
    {
        emit info(QString("mw tb h %1").arg(tb.height()));
    }
    else
    {
        emit error(QString("res %1").arg(res));
    }

    //run_kmines();

#if 0
    Display* display = XOpenDisplay( nullptr );
    if( display == nullptr )
    {
        return false;
    }

    QRect available_geom = QDesktopWidget().availableGeometry();
    QPoint pos = QPoint(available_geom.center().x(),
                        available_geom.center().y());

    QPoint new_pos = QPoint(pos.x(), pos.y()+200);
    mouse_click(Qt::LeftButton, pos);
    mouse_move_to(new_pos);
    mouse_release(Qt::LeftButton);
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit info("Test_2()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit info("Test_3()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit info("Test_4()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit info("Test_5()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::eventFilter(QObject*, QEvent* event)
{
    QMouseEvent *mouseEvent = (QMouseEvent *) event;
    if(mouseEvent == nullptr)
    {
        return false;
    }
    //---
    if(mouseEvent->type() == QMouseEvent::MouseButtonPress)
    {
        emit info(QString("%1 %2")
                  .arg(mouseEvent->pos().x())
                  .arg(mouseEvent->pos().y()));
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
void MainBox::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
