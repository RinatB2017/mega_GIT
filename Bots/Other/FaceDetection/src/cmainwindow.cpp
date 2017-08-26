
#include "cmainwindow.h"
#include "ui_cmainwindow.h"
//--------------------------------------------------------------------------------
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
//--------------------------------------------------------------------------------
#include </usr/include/stdlib.h>
#include </usr/include/vector.h>
//--------------------------------------------------------------------------------
#include </usr/include/opencv2/core/core.hpp>
#include </usr/include/opencv2/objdetect/objdetect.hpp>
#include </usr/include/opencv2/imgproc/imgproc.hpp>
#include </usr/include/opencv2/highgui/highgui.hpp>
#include </usr/include/opencv2/opencv.hpp>
//--------------------------------------------------------------------------------
#define FEAT_FACE_FILE  "xml/haarcascade_frontalface_default.xml"
#define FEAT_EYE_FILE   "xml/haarcascade_mcs_eyepair_big.xml"
#define FEAT_NOSE_FILE  "xml/haarcascade_mcs_nose.xml"
#define FEAT_MOUTH_FILE "xml/haarcascade_mcs_mouth.xml"
//--------------------------------------------------------------------------------
using namespace std;
//--------------------------------------------------------------------------------
// https://habrahabr.ru/post/148692/
//--------------------------------------------------------------------------------
CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMainWindow)
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
    //ui->btn_test->setToolTip("test");

    //---
    ui->checkBox_eyes->setChecked(true);
    ui->checkBox_nose->setChecked(true);
    ui->checkBox_mouth->setChecked(true);
    //---

    create_detectors();

    connect(ui->btn_start,  SIGNAL(clicked(bool)),  this,   SLOT(start()));
    connect(ui->btn_stop,   SIGNAL(clicked(bool)),  this,   SLOT(stop()));
    //connect(ui->btn_test,   SIGNAL(clicked(bool)),  this,   SLOT(test()));

    connect(ui->sl_brightness,  SIGNAL(valueChanged(int)),  this,   SLOT(set_brightness(int)));
    connect(ui->sl_contrast,    SIGNAL(valueChanged(int)),  this,   SLOT(set_contrast(int)));
    connect(ui->sl_saturation,  SIGNAL(valueChanged(int)),  this,   SLOT(set_saturation(int)));
    connect(ui->sl_hue,         SIGNAL(valueChanged(int)),  this,   SLOT(set_hue(int)));

    setWindowTitle("MainWindow");
}
//--------------------------------------------------------------------------------
CMainWindow::~CMainWindow()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void CMainWindow::create_detectors(void)
{
    QString file;

    if(mFaceDetector.empty())
    {
        file = tr("%1/%2").arg(QApplication::applicationDirPath()).arg(FEAT_FACE_FILE);
        if(!mFaceDetector.load(file.toLatin1().constData()))
        {
            qDebug() << tr("Cannot find %1").arg(file);
        }
    }

    if(mEyeDetector.empty())
    {
        file = tr("%1/%2").arg(QApplication::applicationDirPath()).arg(FEAT_EYE_FILE);
        if(!mEyeDetector.load(file.toLatin1().constData()))
        {
            qDebug() << tr("Cannot find %1").arg(file);
        }
    }

    if(mNoseDetector.empty())
    {
        file = tr("%1/%2").arg(QApplication::applicationDirPath()).arg(FEAT_NOSE_FILE);
        if(!mNoseDetector.load(file.toLatin1().constData()))
        {
            qDebug() << tr("Cannot find %1").arg(file);
        }
    }

    if(mMouthDetector.empty())
    {
        file = tr("%1/%2").arg(QApplication::applicationDirPath()).arg(FEAT_MOUTH_FILE);
        if(!mMouthDetector.load(file.toLatin1().constData()))
        {
            qDebug() << tr("Cannot find %1").arg(file);
        }
    }
}
//--------------------------------------------------------------------------------
void CMainWindow::start(void)
{
    if(!mCapture.isOpened())
    {
        bool ok = mCapture.open(0);
        if(ok)
        {
            ui->sl_brightness->setValue(mCapture.get(CV_CAP_PROP_BRIGHTNESS) * 100.0);
            ui->sl_contrast->setValue(mCapture.get(CV_CAP_PROP_CONTRAST) * 100.0);
            ui->sl_saturation->setValue(mCapture.get(CV_CAP_PROP_SATURATION) * 100.0);
            ui->sl_hue->setValue(mCapture.get(CV_CAP_PROP_HUE) * 100.0);

            int w = mCapture.get(CV_CAP_PROP_FRAME_WIDTH);
            if(w < 320) w = 320;
            int h = mCapture.get(CV_CAP_PROP_FRAME_HEIGHT);
            if(h < 200) h = 200;

            ui->cameraWidget->setFixedSize(w, h);

            qDebug() << ui->main_frame->width() << ui->cameraWidget->width() << ui->frame->width();
            qDebug() << ui->centralWidget->width();

            ui->main_frame->setFixedWidth(ui->cameraWidget->width() + ui->frame->width());
            ui->centralWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

            mCameraEventId = startTimer(50);
        }
        else
        {
            QMessageBox::critical(this, tr("error"), tr("camera not found!"));
        }
    }
}
//--------------------------------------------------------------------------------
void CMainWindow::set_brightness(int value)
{
    if(value < 1)   return;
    if(value > 100) return;

    if(mCapture.isOpened())
    {
        mCapture.set(CV_CAP_PROP_BRIGHTNESS, (double)value / 100.0);
        ui->lbl_brightness->setText(QString("%1").arg(ui->sl_brightness->value()));
    }
}
//--------------------------------------------------------------------------------
void CMainWindow::set_contrast(int value)
{
    if(value < 1)   return;
    if(value > 100) return;

    if(mCapture.isOpened())
    {
        mCapture.set(CV_CAP_PROP_CONTRAST, (double)value / 100.0);
        ui->lbl_contrast->setText(QString("%1").arg(ui->sl_contrast->value()));
    }
}
//--------------------------------------------------------------------------------
void CMainWindow::set_saturation(int value)
{
    if(value < 1)   return;
    if(value > 100) return;

    if(mCapture.isOpened())
    {
        mCapture.set(CV_CAP_PROP_SATURATION, (double)value / 100.0);
        ui->lbl_saturation->setText(QString("%1").arg(ui->sl_saturation->value()));
    }
}
//--------------------------------------------------------------------------------
void CMainWindow::set_hue(int value)
{
    if(value < 1)   return;
    if(value > 100) return;

    if(mCapture.isOpened())
    {
        mCapture.set(CV_CAP_PROP_HUE, (double)value / 100.0);
        ui->lbl_hue->setText(QString("%1").arg(ui->sl_hue->value()));
    }
}
//--------------------------------------------------------------------------------
void CMainWindow::stop(void)
{
    killTimer(mCameraEventId);

    mCapture.release();
}
//--------------------------------------------------------------------------------
void CMainWindow::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == mCameraEventId)
    {
        // Stop Timer to stop receiving data from camera during elaboration
        killTimer(mCameraEventId);

        mCapture >> mOrigImage;

        mOrigImage.copyTo(mElabImage);

        vector< cv::Rect > faceVec;
        //cv::Rect rectVec;

        float scaleFactor = 3.0f; // Change Scale Factor to change speed
        mFaceDetector.detectMultiScale(mOrigImage, faceVec, scaleFactor);
        //mFaceDetector.detectSingleScale(mOrigImage, rectVec);

        for(size_t i=0; i<faceVec.size(); i++)
        {
            cv::rectangle(mElabImage, faceVec[i], CV_RGB(255,0,0), 2);

            cv::Mat face = mOrigImage(faceVec[i]);

            // ---> Eye Detection
            if(ui->checkBox_eyes->isChecked())
            {
                vector< cv::Rect > eyeVec;

                mEyeDetector.detectMultiScale(face, eyeVec);

                for(size_t j=0; j<eyeVec.size(); j++)
                {
                    cv::Rect rect = eyeVec[j];
                    rect.x += faceVec[i].x;
                    rect.y += faceVec[i].y;

                    cv::rectangle(mElabImage, rect, CV_RGB(0,255,0), 2);
                }
            }
            // <--- Eye Detection

            // ---> Nose Detection
            if(ui->checkBox_nose->isChecked())
            {
                vector< cv::Rect > noseVec;

                mNoseDetector.detectMultiScale(face, noseVec, 3);

                for(size_t j=0; j<noseVec.size(); j++)
                {
                    cv::Rect rect = noseVec[j];
                    rect.x += faceVec[i].x;
                    rect.y += faceVec[i].y;

                    cv::rectangle(mElabImage, rect, CV_RGB(0,0,255), 2);
                }
            }
            // <--- Nose Detection

            // ---> Mouth Detection
            // [Searched in the bottom half face]
            if(ui->checkBox_mouth->isChecked())
            {
                vector< cv::Rect > mouthVec;
                cv::Rect halfRect = faceVec[i];
                halfRect.height /= 2;
                halfRect.y += halfRect.height;

                cv::Mat halfFace = mOrigImage(halfRect);

                mMouthDetector.detectMultiScale(halfFace, mouthVec, 3);

                for(size_t j=0; j<mouthVec.size(); j++)
                {
                    cv::Rect rect = mouthVec[j];
                    rect.x += halfRect.x;
                    rect.y += halfRect.y;

                    cv::rectangle(mElabImage, rect, CV_RGB(255,255,255), 2);
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
void CMainWindow::show_image_cv(void)
{
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
void CMainWindow::show_image_hw(void)
{
    IplImage * hw = cvLoadImage("test_photo.jpg");

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
void CMainWindow::test(void)
{
    vector< cv::Rect > faceVec;
    vector< cv::Rect > eyeVec;
    vector< cv::Rect > noseVec;
    vector< cv::Rect > mouthVec;

    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),
                                                    QDir::currentPath(),
                                                    tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
    if(filename.isEmpty())
    {
        return;
    }

    mOrigImage = cv::imread(filename.toLocal8Bit().data(), CV_LOAD_IMAGE_COLOR);
    mOrigImage.copyTo(mElabImage);

    float scaleFactor = 3.0f; // Change Scale Factor to change speed
    mFaceDetector.detectMultiScale(mOrigImage, faceVec, scaleFactor);
    for(size_t i=0; i<faceVec.size(); i++)
    {
        cv::rectangle(mElabImage, faceVec[i], CV_RGB(255,0,0), 5);
        cv::Mat face = mOrigImage(faceVec[i]);

        //---
        mEyeDetector.detectMultiScale(face, eyeVec);

        for(size_t j=0; j<eyeVec.size(); j++)
        {
            cv::Rect rect = eyeVec[j];
            rect.x += faceVec[i].x;
            rect.y += faceVec[i].y;

            cv::rectangle(mElabImage, rect, CV_RGB(0,255,0), 5);
        }
        //---
        mNoseDetector.detectMultiScale(face, noseVec, 3);

        for(size_t j=0; j<noseVec.size(); j++)
        {
            cv::Rect rect = noseVec[j];
            rect.x += faceVec[i].x;
            rect.y += faceVec[i].y;

            cv::rectangle(mElabImage, rect, CV_RGB(0,0,255), 5);
        }
#if 1
        cv::Rect halfRect = faceVec[i];
        cv::Mat halfFace = mOrigImage(halfRect);
        mMouthDetector.detectMultiScale(halfFace, mouthVec, 3);

        for(size_t j=0; j<mouthVec.size(); j++)
        {
            cv::Rect rect = mouthVec[j];
            rect.x += halfRect.x;
            rect.y += halfRect.y;

            cv::rectangle(mElabImage, rect, CV_RGB(255,255,255), 2);
        }
#endif
    }

    double k = (double)ui->cameraWidget->height() / (double)mElabImage.rows;
    qDebug() << "mElabImage" << mElabImage.cols << mElabImage.rows;
    qDebug() << "cameraWidget" << ui->cameraWidget->width() << ui->cameraWidget->height();
    qDebug() << "K" << k;
    ui->cameraWidget->setMinimumWidth(k * (double)mElabImage.cols); // ui->cameraWidget->width());

    ui->cameraWidget->showImage(mElabImage);
    ui->cameraWidget->adjustSize();
}
//--------------------------------------------------------------------------------
