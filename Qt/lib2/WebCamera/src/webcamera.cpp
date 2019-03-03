/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   include </usr/include/stdlib.h>
#endif
//--------------------------------------------------------------------------------
#define FEAT_FACE_FILE  "xml/haarcascade_frontalface_default.xml"
#define FEAT_EYE_FILE   "xml/haarcascade_mcs_eyepair_big.xml"
#define FEAT_NOSE_FILE  "xml/haarcascade_mcs_nose.xml"
#define FEAT_MOUTH_FILE "xml/haarcascade_mcs_mouth.xml"
//--------------------------------------------------------------------------------
#include "ui_webcamera.h"
#include "webcamera.hpp"
#include "sleeper.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
using namespace std;
//--------------------------------------------------------------------------------
// https://habrahabr.ru/post/148692/
// http://illumium.org/node/58
// http://recog.ru/blog/opencv/57.html
// http://robocraft.ru/blog/computervision/3046.html
//--------------------------------------------------------------------------------
WebCamera::WebCamera(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::WebCamera)
{
    init();
}
//--------------------------------------------------------------------------------
WebCamera::~WebCamera()
{
    if(mCameraEventId)
    {
        killTimer(mCameraEventId);
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void WebCamera::init(void)
{
    ui->setupUi(this);

    ui->sl_brightness->setRange(1, 100);
    ui->sl_contrast->setRange(1, 100);
    ui->sl_saturation->setRange(1, 100);
    ui->sl_hue->setRange(1, 100);

    ui->btn_start->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_stop->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop));

    ui->btn_start->setToolTip("start");
    ui->btn_stop->setToolTip("stop");

    //---
    ui->checkBox_eyes->setChecked(true);
    ui->checkBox_nose->setChecked(true);
    ui->checkBox_mouth->setChecked(true);
    //---

    //ui->le_device->setText("rtsp://192.168.0.66:554/av0_1");
    ui->le_device->setText("/dev/video0");

    set_brightness(1);
    set_contrast(1);
    set_saturation(1);
    set_hue(1);

    connect(ui->btn_start,  SIGNAL(clicked(bool)),  this,   SLOT(start()));
    connect(ui->btn_stop,   SIGNAL(clicked(bool)),  this,   SLOT(stop()));

    connect(ui->btn_test,   SIGNAL(clicked(bool)),  this,   SLOT(test()));
    //connect(ui->btn_test,   SIGNAL(clicked(bool)),  this,   SLOT(test1()));
    //connect(ui->btn_test,   SIGNAL(clicked(bool)),  this,   SLOT(test2()));
    //connect(ui->btn_test,   SIGNAL(clicked(bool)),  this,   SLOT(test3()));
    //connect(ui->btn_test,   SIGNAL(clicked(bool)),  this,   SLOT(test4()));
    //connect(ui->btn_test,   SIGNAL(clicked(bool)),  this,   SLOT(test5()));

    connect(ui->sl_brightness,  SIGNAL(valueChanged(int)),  this,   SLOT(set_brightness(int)));
    connect(ui->sl_contrast,    SIGNAL(valueChanged(int)),  this,   SLOT(set_contrast(int)));
    connect(ui->sl_saturation,  SIGNAL(valueChanged(int)),  this,   SLOT(set_saturation(int)));
    connect(ui->sl_hue,         SIGNAL(valueChanged(int)),  this,   SLOT(set_hue(int)));
    //connect(ui->cb_autofocus,   SIGNAL(toggled(bool)),      this,   SLOT(set_autofocus(bool)));
}
//--------------------------------------------------------------------------------
bool WebCamera::create_detectors(void)
{
    emit trace(Q_FUNC_INFO);
    QString file;

    if(mFaceDetector.empty())
    {
        file = QString("%1/%2").arg(QApplication::applicationDirPath()).arg(FEAT_FACE_FILE);
        if(!mFaceDetector.load(file.toLatin1().constData()))
        {
            emit error(QString("Cannot find %1").arg(file));
            return false;
        }
    }

    if(mEyeDetector.empty())
    {
        file = QString("%1/%2").arg(QApplication::applicationDirPath()).arg(FEAT_EYE_FILE);
        if(!mEyeDetector.load(file.toLatin1().constData()))
        {
            emit error(QString("Cannot find %1").arg(file));
            return false;
        }
    }

    if(mNoseDetector.empty())
    {
        file = QString("%1/%2").arg(QApplication::applicationDirPath()).arg(FEAT_NOSE_FILE);
        if(!mNoseDetector.load(file.toLatin1().constData()))
        {
            emit error(QString("Cannot find %1").arg(file));
            return false;
        }
    }

    if(mMouthDetector.empty())
    {
        file = QString("%1/%2").arg(QApplication::applicationDirPath()).arg(FEAT_MOUTH_FILE);
        if(!mMouthDetector.load(file.toLatin1().constData()))
        {
            emit error(QString("Cannot find %1").arg(file));
            return false;
        }
    }
    return true;
}
//--------------------------------------------------------------------------------
void WebCamera::start(void)
{
    emit trace(Q_FUNC_INFO);
    bool ok = create_detectors();
    if(ok == false)
    {
        return;
    }

    QString dev_name = ui->le_device->text();
    if(dev_name.isEmpty())
    {
        messagebox_critical("Ошибка", "Не задано имя");
        return;
    }
    ok = false;

    mCapture.release();
    int num = dev_name.toInt(&ok);
    if(ok)
    {
        mCapture.open(num);
    }
    else
    {
        mCapture.open(dev_name.toLatin1().data());
    }

    //mCapture.open("rtsp://192.168.0.66:554/av0_0");
    //mCapture.open("rtsp://192.168.0.66:554/av0_1");
    //mcapture("rtsp://USER:PASS@xxx.xxx.xxx.xxx/axis-media/media.amp?camera=2");
    //mCapture.open(0);
    if(mCapture.isOpened())
    {
        ui->sl_brightness->setValue(static_cast<int>(mCapture.get(CV_CAP_PROP_BRIGHTNESS) * 100.0));
        ui->sl_contrast->setValue(static_cast<int>(mCapture.get(CV_CAP_PROP_CONTRAST) * 100.0));
        ui->sl_saturation->setValue(static_cast<int>(mCapture.get(CV_CAP_PROP_SATURATION) * 100.0));
        ui->sl_hue->setValue(static_cast<int>(mCapture.get(CV_CAP_PROP_HUE) * 100.0));

        //TODO
        mCapture.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
        mCapture.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
        //set_autofocus(true);
        mCapture.set(CV_CAP_PROP_AUTOFOCUS, 1);

        int w = static_cast<int>(mCapture.get(CV_CAP_PROP_FRAME_WIDTH));
        int h = static_cast<int>(mCapture.get(CV_CAP_PROP_FRAME_HEIGHT));

        emit info(QString("resolution %1*%2").arg(w).arg(h));

        ui->sl_brightness->setDisabled(mCapture.get(CV_CAP_PROP_BRIGHTNESS) == 0.0);
        ui->sl_contrast->setDisabled(mCapture.get(CV_CAP_PROP_CONTRAST) == 0.0);
        ui->sl_saturation->setDisabled(mCapture.get(CV_CAP_PROP_SATURATION) == 0.0);
        ui->sl_hue->setDisabled(mCapture.get(CV_CAP_PROP_HUE) == 0.0);

        ui->cameraWidget->setFixedSize(w, h);
        ui->main_frame->setFixedWidth(ui->cameraWidget->width() + ui->frame->width());

        mCameraEventId = startTimer(0);    //50
    }
    else
    {
        QMessageBox::critical(this, tr("error"), tr("Camera not found!"));
    }
}
//--------------------------------------------------------------------------------
void WebCamera::set_brightness(int value)
{
    emit trace(Q_FUNC_INFO);
    if(value < 1)   return;
    if(value > 100) return;

    ui->lbl_brightness->setText(QString("%1").arg(ui->sl_brightness->value()));
    if(mCapture.isOpened())
    {
        mCapture.set(CV_CAP_PROP_BRIGHTNESS, static_cast<double>(value) / 100.0);
    }
}
//--------------------------------------------------------------------------------
void WebCamera::set_contrast(int value)
{
    emit trace(Q_FUNC_INFO);
    if(value < 1)   return;
    if(value > 100) return;

    ui->lbl_contrast->setText(QString("%1").arg(ui->sl_contrast->value()));
    if(mCapture.isOpened())
    {
        mCapture.set(CV_CAP_PROP_CONTRAST, static_cast<double>(value) / 100.0);
    }
}
//--------------------------------------------------------------------------------
void WebCamera::set_saturation(int value)
{
    emit trace(Q_FUNC_INFO);
    if(value < 1)   return;
    if(value > 100) return;

    ui->lbl_saturation->setText(QString("%1").arg(ui->sl_saturation->value()));
    if(mCapture.isOpened())
    {
        mCapture.set(CV_CAP_PROP_SATURATION, static_cast<double>(value) / 100.0);
    }
}
//--------------------------------------------------------------------------------
void WebCamera::set_hue(int value)
{
    emit trace(Q_FUNC_INFO);
    if(value < 1)   return;
    if(value > 100) return;

    ui->lbl_hue->setText(QString("%1").arg(ui->sl_hue->value()));
    if(mCapture.isOpened())
    {
        mCapture.set(CV_CAP_PROP_HUE, static_cast<double>(value) / 100.0);
    }
}
//--------------------------------------------------------------------------------
void WebCamera::set_autofocus(bool state)
{
    emit trace(Q_FUNC_INFO);
    //ui->cb_autofocus->setChecked(state);
    if(mCapture.isOpened())
    {
        if(state)
            mCapture.set(CV_CAP_PROP_AUTOFOCUS, 1);
        else
            mCapture.set(CV_CAP_PROP_AUTOFOCUS, 0);
    }
}
//--------------------------------------------------------------------------------
void WebCamera::stop(void)
{
    emit trace(Q_FUNC_INFO);
    killTimer(mCameraEventId);

    mCapture.release();
}
//--------------------------------------------------------------------------------
void WebCamera::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == mCameraEventId)
    {
        // Stop Timer to stop receiving data from Camera during elaboration
        killTimer(mCameraEventId);

        mCapture >> mOrigImage;

        mOrigImage.copyTo(mElabImage);

        vector< Rect > faceVec;
        //Rect rectVec;

        double scaleFactor = 3.0; // Change Scale Factor to change speed
        mFaceDetector.detectMultiScale(mOrigImage, faceVec, scaleFactor);
        //mFaceDetector.detectSingleScale(mOrigImage, rectVec);

        if(faceVec.size() > 0)
        {
            //emit info(QString("faceVec.size() = %1").arg(faceVec.size()));
            ui->lbl_result->setText("Found");
        }
        else
        {
            //emit error("Not found!");
            ui->lbl_result->setText("NOT Found");
        }
        for(size_t i=0; i<faceVec.size(); i++)
        {
            rectangle(mElabImage, faceVec[i], CV_RGB(255,0,0), 2);

            Mat face = mOrigImage(faceVec[i]);

            // ---> Eye Detection
            if(ui->checkBox_eyes->isChecked())
            {
                vector< Rect > eyeVec;

                mEyeDetector.detectMultiScale(face, eyeVec);

                for(size_t j=0; j<eyeVec.size(); j++)
                {
                    Rect rect = eyeVec[j];
                    rect.x += faceVec[i].x;
                    rect.y += faceVec[i].y;

                    rectangle(mElabImage, rect, CV_RGB(0,255,0), 2);
                }
            }
            // <--- Eye Detection

            // ---> Nose Detection
            if(ui->checkBox_nose->isChecked())
            {
                vector< Rect > noseVec;

                mNoseDetector.detectMultiScale(face, noseVec, 3);

                for(size_t j=0; j<noseVec.size(); j++)
                {
                    Rect rect = noseVec[j];
                    rect.x += faceVec[i].x;
                    rect.y += faceVec[i].y;

                    rectangle(mElabImage, rect, CV_RGB(0,0,255), 2);
                }
            }
            // <--- Nose Detection

            // ---> Mouth Detection
            // [Searched in the bottom half face]
            if(ui->checkBox_mouth->isChecked())
            {
                vector< Rect > mouthVec;
                Rect halfRect = faceVec[i];
                halfRect.height /= 2;
                halfRect.y += halfRect.height;

                Mat halfFace = mOrigImage(halfRect);

                mMouthDetector.detectMultiScale(halfFace, mouthVec, 3);

                for(size_t j=0; j<mouthVec.size(); j++)
                {
                    Rect rect = mouthVec[j];
                    rect.x += halfRect.x;
                    rect.y += halfRect.y;

                    rectangle(mElabImage, rect, CV_RGB(255,255,255), 2);
                }
            }
            // <--- Mouth Detection
        }

        ui->cameraWidget->showImage(mElabImage);

        // Timer reactivation
        mCameraEventId = startTimer(0);
    }
}
//--------------------------------------------------------------------------------
void WebCamera::show_image_cv(void)
{
    emit trace(Q_FUNC_INFO);
    CvMat *cv = cvLoadImageM("test_photo.jpg");

    // создаём окошко
    cvNamedWindow("Hello World", 0);
    // показываем картинку в созданном окне
    cvShowImage("Hello World", cv);

    // ждём нажатия клавиши
    cvWaitKey(0);

    // освобождаем ресурсы
    cvDestroyWindow("Hello World");
}
//--------------------------------------------------------------------------------
void WebCamera::show_image_hw(void)
{
    emit trace(Q_FUNC_INFO);
    IplImage *hw = cvLoadImage("test_photo.jpg");

    // создаём окошко
    cvNamedWindow("Hello World", 0);
    // показываем картинку в созданном окне
    cvShowImage("Hello World", hw);

    // ждём нажатия клавиши
    cvWaitKey(0);

    // освобождаем ресурсы
    cvReleaseImage(&hw);
    cvDestroyWindow("Hello World");
}
//--------------------------------------------------------------------------------
void WebCamera::test(void)
{
    emit trace(Q_FUNC_INFO);
    vector< Rect > faceVec;
    vector< Rect > eyeVec;
    vector< Rect > noseVec;
    vector< Rect > mouthVec;

    QString filename ;
    QFileDialog *dlg;

    dlg = new QFileDialog;
    dlg->setNameFilter("Image Files (*.png *.jpg *.jpeg *.bmp)");
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    //dlg->setDirectory(QDir::currentPath());
    dlg->setDirectory("/dev/shm/0");
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        filename = files.at(0);
    }
    if(filename.isEmpty())
    {
        return;
    }

    emit info(QString("filename [%1]").arg(filename.toLocal8Bit().data()));
    mOrigImage = imread(filename.toLocal8Bit().data(), CV_LOAD_IMAGE_COLOR);
    mOrigImage.copyTo(mElabImage);

    double scaleFactor = 3.0; // Change Scale Factor to change speed
    mFaceDetector.detectMultiScale(mOrigImage, faceVec, scaleFactor);
    for(size_t i=0; i<faceVec.size(); i++)
    {
        rectangle(mElabImage, faceVec[i], CV_RGB(255,0,0), 5);
        Mat face = mOrigImage(faceVec[i]);

        //---
        mEyeDetector.detectMultiScale(face, eyeVec);

        for(size_t j=0; j<eyeVec.size(); j++)
        {
            Rect rect = eyeVec[j];
            rect.x += faceVec[i].x;
            rect.y += faceVec[i].y;

            rectangle(mElabImage, rect, CV_RGB(0,255,0), 5);
        }
        //---
        mNoseDetector.detectMultiScale(face, noseVec, 3);

        for(size_t j=0; j<noseVec.size(); j++)
        {
            Rect rect = noseVec[j];
            rect.x += faceVec[i].x;
            rect.y += faceVec[i].y;

            rectangle(mElabImage, rect, CV_RGB(0,0,255), 5);
        }
#if 1
        Rect halfRect = faceVec[i];
        Mat halfFace = mOrigImage(halfRect);
        mMouthDetector.detectMultiScale(halfFace, mouthVec, 3);

        for(size_t j=0; j<mouthVec.size(); j++)
        {
            Rect rect = mouthVec[j];
            rect.x += halfRect.x;
            rect.y += halfRect.y;

            rectangle(mElabImage, rect, CV_RGB(255,255,255), 2);
        }
#endif
    }

    double k = static_cast<double>(ui->cameraWidget->height()) / static_cast<double>(mElabImage.rows);
#ifdef QT_DEBUG
    qDebug() << "mElabImage" << mElabImage.cols << mElabImage.rows;
    qDebug() << "cameraWidget" << ui->cameraWidget->width() << ui->cameraWidget->height();
    qDebug() << "K" << k;
#endif
    ui->cameraWidget->setMinimumWidth(static_cast<int>(k * static_cast<double>(mElabImage.cols))); // ui->cameraWidget->width());

    ui->cameraWidget->showImage(mElabImage);
    ui->cameraWidget->adjustSize();
}
//--------------------------------------------------------------------------------
void WebCamera::test2(void)
{
    emit trace(Q_FUNC_INFO);
    IplImage *src, *templ, *result;
    double min, max;
    CvPoint minpos, maxpos;

    src     = cvLoadImage("lena.png", 1);
    templ   = cvLoadImage("lena_eye.png", 1);
    result  = cvCreateImage(cvSize(src->width - templ->width + 1, src->height - templ->height + 1), 32, 1);

    cvMatchTemplate( src, templ, result, CV_TM_CCOEFF_NORMED);
    cvNormalize( result, result, 1, 0, CV_MINMAX );

    cvMinMaxLoc(result, &min, &max, &minpos, &maxpos);

    //TODO
#if 1
    cvRectangle(result,
                cvPoint(maxpos.x-5, maxpos.y-5),
                cvPoint(maxpos.x+5, maxpos.y+5),
                CV_RGB(255,0,0), 1);
    emit info(QString("%1; x = %2; y = %3").arg(max).arg(maxpos.x).arg(maxpos.y));
#else
    cvRectangle(result,
                cvPoint(maxpos.x, maxpos.y),
                cvPoint(minpos.x, minpos.y),
                CV_RGB(255,0,0), 1);
    emit info(QString("%1; x = %2; y = %3").arg(max).arg(minpos.x).arg(minpos.y));
    emit info(QString("%1; x = %2; y = %3").arg(max).arg(maxpos.x).arg(maxpos.y));
#endif

    //---
    cvNamedWindow("Image", 0);
    cvShowImage( "Image", result );
    cvWaitKey(0);
    cvReleaseImage(&result);
    cvDestroyWindow("Image");
    //---
}
//--------------------------------------------------------------------------------
void WebCamera::test3(void)
{
    emit trace(Q_FUNC_INFO);
    QStringList sl;
    sl << "cards/0_2.png";
    sl << "cards/0_3.png";
    sl << "cards/0_4.png";
    sl << "cards/0_5.png";
    sl << "cards/0_6.png";
    sl << "cards/0_7.png";
    sl << "cards/0_8.png";
    sl << "cards/0_9.png";
    sl << "cards/0_10.png";
    sl << "cards/0_D.png";
    sl << "cards/0_K.png";
    sl << "cards/0_T.png";
    sl << "cards/0_V.png";

    sl << "cards/1_2.png";
    sl << "cards/1_3.png";
    sl << "cards/1_4.png";
    sl << "cards/1_5.png";
    sl << "cards/1_6.png";
    sl << "cards/1_7.png";
    sl << "cards/1_8.png";
    sl << "cards/1_9.png";
    sl << "cards/1_10.png";
    sl << "cards/1_D.png";
    sl << "cards/1_K.png";
    sl << "cards/1_T.png";
    sl << "cards/1_V.png";

    sl << "cards/2_2.png";
    sl << "cards/2_3.png";
    sl << "cards/2_4.png";
    sl << "cards/2_5.png";
    sl << "cards/2_6.png";
    sl << "cards/2_7.png";
    sl << "cards/2_8.png";
    sl << "cards/2_9.png";
    sl << "cards/2_10.png";
    sl << "cards/2_D.png";
    sl << "cards/2_K.png";
    sl << "cards/2_T.png";
    sl << "cards/2_V.png";

    sl << "cards/3_2.png";
    sl << "cards/3_3.png";
    sl << "cards/3_4.png";
    sl << "cards/3_5.png";
    sl << "cards/3_6.png";
    sl << "cards/3_7.png";
    sl << "cards/3_8.png";
    sl << "cards/3_9.png";
    sl << "cards/3_10.png";
    sl << "cards/3_D.png";
    sl << "cards/3_K.png";
    sl << "cards/3_T.png";
    sl << "cards/3_V.png";

    QRect rect;
    bool ok = false;

    foreach (QString filename, sl)
    {
        ok = searchObjectByTemplate("KPatience.png", filename, &rect);
        if(ok)
        {
            emit info(QString("%1 \t %2 %3 %4 %5")
                      .arg(filename)
                      .arg(rect.x())
                      .arg(rect.y())
                      .arg(rect.width())
                      .arg(rect.height()));
        }
        Sleeper::msleep(10);
        //QCoreApplication::processEvents();
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void WebCamera::test4(void)
{
    emit trace(Q_FUNC_INFO);
    block_this_button(true);

    QRect rect;
    bool ok = false;
    //ok = searchObjectByTemplate("lena.png", "lena_eye.png", &rect);
    //ok = searchObjectByTemplate("lena.png", "lena_eye_bad.png", &rect);
    ok = searchObjectByTemplate("lena.png", "rect.png", &rect);
    if(ok)
    {
        emit info(QString("%1 %2 %3 %4")
                  .arg(rect.x())
                  .arg(rect.y())
                  .arg(rect.width())
                  .arg(rect.height()));

        //IplImage *result = cvLoadImage("lena.png", CV_LOAD_IMAGE_GRAYSCALE);
        IplImage *result = cvLoadImage("lena.png", CV_LOAD_IMAGE_COLOR);
        cvRectangle(result,
                    cvPoint(rect.x(), rect.y()),
                    cvPoint(rect.x()+rect.width(), rect.y()+rect.height()),
                    CV_RGB(0,0,0), 1);

        cvNamedWindow("Image", CV_WINDOW_AUTOSIZE);
        cvShowImage("Image", result);
        cvWaitKey(0);
        cvReleaseImage(&result);
        cvDestroyWindow("Image");
    }
    emit info("OK");

    block_this_button(false);
}
//--------------------------------------------------------------------------------
void WebCamera::test5(void)
{
    emit trace(Q_FUNC_INFO);
    IplImage* image = nullptr;
    IplImage* templ = nullptr;

    // получаем картинку
    image = cvLoadImage("lena.png", CV_LOAD_IMAGE_COLOR);
    //image = cvLoadImage("KPatience.png", CV_LOAD_IMAGE_COLOR);

    // шаблон
    templ = cvLoadImage("rect.png", CV_LOAD_IMAGE_COLOR);
    //templ = cvLoadImage("lena_eye.png", CV_LOAD_IMAGE_COLOR);
    //templ = cvLoadImage("lena_eye_bad.png", CV_LOAD_IMAGE_COLOR);
    //templ = cvLoadImage("_3_5.png", CV_LOAD_IMAGE_COLOR);
    //templ = cvLoadImage("small_3_5.png", CV_LOAD_IMAGE_COLOR);

    // изображение для хранения результата сравнения
    // размер результата: если image WxH и templ wxh, то result = (W-w+1)x(H-h+1)
    IplImage *res = cvCreateImage( cvSize( (image->width-templ->width+1), (image->height-templ->height+1)), IPL_DEPTH_32F, 1 );

    // сравнение изображения с шаблоном
    cvMatchTemplate(image, templ, res, CV_TM_SQDIFF);

    // определение лучшее положение для сравнения
    // (поиск минимумов и максимумов на изображении)
    double  minval, maxval;
    CvPoint minloc, maxloc;
    cvMinMaxLoc(res, &minval, &maxval, &minloc, &maxloc, nullptr);

    // выделим область прямоугольником
    cvRectangle(image,
                cvPoint(minloc.x, minloc.y),
                cvPoint(minloc.x+templ->width-1, minloc.y+templ->height-1),
                CV_RGB(255, 0, 0), 2, 8);

    // показываем изображение
    cvShowImage("Match", image);

    // показываем шаблон
    cvShowImage("Template", templ);

    // ждём нажатия клавиши
    cvWaitKey(0);

    // освобождаем ресурсы
    cvReleaseImage( &image );
    cvReleaseImage( &templ );
    cvReleaseImage( &res );
    cvDestroyAllWindows();
}
//--------------------------------------------------------------------------------
bool WebCamera::searchObjectByTemplate(QString srcImgName, QString templImgName, QRect *rect)
{
    emit trace(Q_FUNC_INFO);

    QRect CCORR_result;
    QRect SQDIFF_result;
    QPair <QRect, QRect> resultPair;

    resultPair.first = QRect(0, 0, 0, 0);
    resultPair.second = QRect(1, 1, 0, 0);

    double min,max;
    CvPoint minpos, maxpos;

    char* imgName   = srcImgName.toLocal8Bit().data();
    char* templName = templImgName.toLocal8Bit().data();

    IplImage *src    = nullptr;
    IplImage *templ  = nullptr;
    IplImage *result = nullptr;

    src = cvLoadImage(imgName, CV_LOAD_IMAGE_GRAYSCALE);
    //src = cvLoadImage(imgName, CV_LOAD_IMAGE_COLOR);
    if(!src)
    {
        emit error(QString("Error load %1").arg(srcImgName));
        return false;
    }

    templ = cvLoadImage(templName, CV_LOAD_IMAGE_GRAYSCALE);
    //templ = cvLoadImage(templName, CV_LOAD_IMAGE_COLOR);
    if(!templ)
    {
        emit error(QString("Error load %1").arg(templImgName));
        return false;
    }

    result = cvCreateImage(cvSize(src->width - templ->width + 1, src->height - templ->height + 1), 32, 1);

    cvMatchTemplate(src, templ, result, CV_TM_CCORR_NORMED);
    cvNormalize(result, result, 1, 0, CV_MINMAX );
    cvMinMaxLoc(result, &min, &max, &minpos, &maxpos);

    CCORR_result = QRect(maxpos.x, maxpos.y, templ->width, templ->height);

    cvMatchTemplate(src, templ, result, CV_TM_SQDIFF_NORMED);
    cvNormalize(result, result, 1, 0, CV_MINMAX );
    cvMinMaxLoc(result, &min, &max, &minpos, &maxpos);

    SQDIFF_result = QRect(minpos.x, minpos.y, templ->width, templ->height);

    resultPair.first = CCORR_result;
    resultPair.second = SQDIFF_result;

    if(resultPair.first.x() == resultPair.second.x() &&
            resultPair.first.x() == resultPair.second.x())
    {
        *rect = resultPair.first;
    }

    cvReleaseImage(&src);
    cvReleaseImage(&templ);
    cvReleaseImage(&result);

    return true;
}
//--------------------------------------------------------------------------------
void WebCamera::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
