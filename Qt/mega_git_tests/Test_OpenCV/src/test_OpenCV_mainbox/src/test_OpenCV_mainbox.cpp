/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#define FEAT_FACE_FILE  "xml/haarcascade_frontalface_default.xml"
#define FEAT_EYE_FILE   "xml/haarcascade_mcs_eyepair_big.xml"
#define FEAT_NOSE_FILE  "xml/haarcascade_mcs_nose.xml"
#define FEAT_MOUTH_FILE "xml/haarcascade_mcs_mouth.xml"
//--------------------------------------------------------------------------------
#include "test_OpenCV_mainbox.hpp"
#include "ui_test_OpenCV_mainbox.h"
#include "myfiledialog.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget* parent) :
    MyWidget(parent),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    foreach(QSlider* slider, findChildren< QSlider* >())
    {
        connect(slider, SIGNAL(sliderMoved(int)), SLOT(refreshHSV()));
        QString sliderName = slider->objectName();
        if(QSpinBox* sp = findChild< QSpinBox* >("sp" + sliderName.mid(2)))
        {
            sp->setMinimum(slider->minimum());
            sp->setMaximum(slider->maximum());
            sp->setValue(slider->value());
            connect(sp, SIGNAL(valueChanged(int)), SLOT(refreshHSV()));
        }
    }

    foreach(const QRadioButton* rb, findChildren< QRadioButton* >())
    {
        connect(rb, &QRadioButton::clicked, this,   &MainBox::refreshHSV);
    }

    ui->sl_scale_factor->setRange(10, 50);
    ui->dsb_scalefactor->setSingleStep(0.01);
    ui->sl_minNeighbors->setRange(1, 10);

    connect(ui->sl_scale_factor,    &QSlider::sliderMoved,
            this,                   static_cast<void (MainBox::*)(double)>(&MainBox::set_scaleFactor));
    connect(ui->dsb_scalefactor,    static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this,                   static_cast<void (MainBox::*)(double)>(&MainBox::set_scaleFactor));
    connect(ui->sl_minNeighbors,    &QSlider::sliderMoved,          this,   &MainBox::set_minNeighbors);
    connect(ui->bnLoad,             &QPushButton::clicked,          this,   &MainBox::onLoad);
    connect(ui->btn_test,           &QPushButton::clicked,          this,   &MainBox::s_test);

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::onLoad(void)
{
    MyFileDialog *dlg;

    dlg = new MyFileDialog("load_image", "load_image");
    dlg->setNameFilter("Images (*.jpg *.jpeg *.png *.bmp)");
    dlg->setOption(MyFileDialog::DontUseNativeDialog, true);
//    dlg->setDirectory(m_lastLoadPath);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        QString imgName = files.at(0);
        m_lastLoadPath = QFileInfo(imgName).absolutePath();
        mOrigImage = cv::imread(imgName.toStdString());
        if(!mOrigImage.empty())
        {
            cv::cvtColor(mOrigImage, mOrigImage, cv::COLOR_BGR2RGB);
        }

        refreshHSV();
    }
    dlg->deleteLater();
}
//--------------------------------------------------------------------------------
void MainBox::refreshHSV(void)
{
    if(mOrigImage.empty())
    {
        return;
    }

    QImage resultImg;

    if(ui->rb_Original->isChecked())
    {
        resultImg = QImage(mOrigImage.data,
                           mOrigImage.cols,
                           mOrigImage.rows,
                           static_cast<int>(mOrigImage.step),
                           QImage::Format_RGB888).copy();
    }
    else
    {
        int hueFrom = ui->sl_HueFrom->value();
        int hueTo = std::max(hueFrom, ui->sl_HueTo->value());

        int saturationFrom = ui->sl_SaturationFrom->value();
        int saturationTo = std::max(saturationFrom, ui->sl_SaturationTo->value());

        int valueFrom = ui->sl_ValueFrom->value();
        int valueTo = std::max(valueFrom, ui->sl_ValueTo->value());

        cv::Mat thresholdedMat;
        cv::cvtColor(mOrigImage, thresholdedMat, cv::COLOR_RGB2HSV);
        // Отфильтровываем только то, что нужно, по диапазону цветов
        cv::inRange(thresholdedMat,
                    cv::Scalar(hueFrom, saturationFrom, valueFrom),
                    cv::Scalar(hueTo, saturationTo, valueTo),
                    thresholdedMat
                    );

        // Убираем шум
        cv::erode(thresholdedMat,
                  thresholdedMat,
                  cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
        cv::dilate(thresholdedMat,
                   thresholdedMat,
                   cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5))
                   );

        // Замыкаем оставшиеся крупные объекты
        cv::dilate(thresholdedMat,
                   thresholdedMat,
                   cv::getStructuringElement( cv::MORPH_ELLIPSE, cv::Size( 5, 5 ))
                   );
        cv::erode(thresholdedMat,
                  thresholdedMat,
                  cv::getStructuringElement( cv::MORPH_ELLIPSE, cv::Size( 5, 5 ))
                  );

        if(ui->rb_Canny->isChecked())
        {
            // Визуально выделяем границы
            cv::Canny(thresholdedMat, thresholdedMat, 100, 50, 5 );
        }

        if(ui->rbResult->isChecked())
        {
            // Находим контуры
            std::vector< std::vector< cv::Point > > countours;
            std::vector< cv::Vec4i > hierarchy;
            cv::findContours(thresholdedMat,
                             countours,
                             hierarchy,
                             cv::RETR_TREE,
                             cv::CHAIN_APPROX_SIMPLE,
                             cv::Point( 0, 0 )
                             );

            std::vector< cv::Rect > rects;
            for(uint i = 0; i < countours.size(); ++i)
            {
                // Пропускаем внутренние контуры
                if(0 <= hierarchy[i][3])
                {
                    continue;
                }
                rects.push_back( cv::boundingRect( countours[ i ]));
            }

            resultImg = QImage(mOrigImage.data,
                               mOrigImage.cols,
                               mOrigImage.rows,
                               static_cast<int>(mOrigImage.step),
                               QImage::Format_RGB888
                               ).copy();

            QPainter p;
            p.begin( &resultImg );
            p.setPen( QPen( Qt::green, 2 ));
            foreach( const cv::Rect& r, rects )
            {
                p.drawRect( r.x, r.y, r.width, r.height );
            }
            p.end();

        }
        else
        {
            resultImg = QImage(thresholdedMat.data,
                               thresholdedMat.cols,
                               thresholdedMat.rows,
                               static_cast<int>(thresholdedMat.step),
                               QImage::Format_Indexed8
                               ).copy();
        }
    }

    ui->lbView->setPixmap(
                QPixmap::fromImage(resultImg).scaled(ui->lbView->size(),
                                                     Qt::KeepAspectRatio,
                                                     Qt::SmoothTransformation
                                                     )
                );
}
//--------------------------------------------------------------------------------
bool MainBox::create_detectors(void)
{
    QString file;

    if(mFaceDetector.empty())
    {
        file = QString("%1/%2").arg(QApplication::applicationDirPath()).arg(FEAT_FACE_FILE);
        if(!mFaceDetector.load(file.toLatin1().constData()))
        {
            emit error(QString("error load %1").arg(file));
            return false;
        }
    }

    if(mEyeDetector.empty())
    {
        file = QString("%1/%2").arg(QApplication::applicationDirPath()).arg(FEAT_EYE_FILE);
        if(!mEyeDetector.load(file.toLatin1().constData()))
        {
            emit error(QString("error load %1").arg(file));
            return false;
        }
    }

    if(mNoseDetector.empty())
    {
        file = QString("%1/%2").arg(QApplication::applicationDirPath()).arg(FEAT_NOSE_FILE);
        if(!mNoseDetector.load(file.toLatin1().constData()))
        {
            emit error(QString("error load %1").arg(file));
            return false;
        }
    }

    if(mMouthDetector.empty())
    {
        file = QString("%1/%2").arg(QApplication::applicationDirPath()).arg(FEAT_MOUTH_FILE);
        if(!mMouthDetector.load(file.toLatin1().constData()))
        {
            emit error(QString("error load %1").arg(file));
            return false;
        }
    }
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::find_faces(void)
{
    if(mOrigImage.data == nullptr)
    {
        emit error("find_faces: no data");
        return;
    }

    if(scaleFactor == 1.0)
    {
        emit error("FIXME: scaleFactor == 1.0");
        return;
    }

    mOrigImage.copyTo(mElabImage);

    cv::cvtColor(mOrigImage, grayFrames, cv::COLOR_BGR2GRAY);
    equalizeHist(grayFrames, grayFrames);

    bool ok = faceCade.load(FEAT_FACE_FILE);
    if(!ok)
    {
        emit error(QString("error load %1").arg(FEAT_FACE_FILE));
        return;
    }

    //faceCade.detectMultiScale(grayFrames, faces, 1.01, 3.0);
    faceCade.detectMultiScale(grayFrames, faces, scaleFactor, minNeighbors);
    
    //---
    // https://docs.opencv.org/2.4/doc/tutorials/objdetect/cascade_classifier/cascade_classifier.html
    //faceCade.detectMultiScale(grayFrames, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
    //---

    for(unsigned int i=0; i<faces.size(); i++)
    {
        cv::rectangle(mElabImage, faces[i], CV_RGB(255,0,0), 5);
    }

    //---
    QImage resultImg = QImage(mElabImage.data,
                              mElabImage.cols,
                              mElabImage.rows,
                              static_cast<int>(mElabImage.step),
                              QImage::Format_RGB888 ).copy();

    ui->lbView->setPixmap(QPixmap::fromImage(resultImg).scaled(
                              ui->lbView->size(),
                              Qt::KeepAspectRatio,
                              Qt::SmoothTransformation
                              )
                          );
}
//--------------------------------------------------------------------------------
void MainBox::s_test(void)
{
    find_faces();
}
//--------------------------------------------------------------------------------
void MainBox::set_scaleFactor(int value)
{
    scaleFactor = static_cast<double>(value) / 10.0;
    ui->dsb_scalefactor->setValue(scaleFactor);

    find_faces();
}
//--------------------------------------------------------------------------------
void MainBox::set_scaleFactor(double value)
{
    scaleFactor = value;
    ui->sl_scale_factor->setValue(static_cast<int>(scaleFactor * 10.0));

    find_faces();
}
//--------------------------------------------------------------------------------
void MainBox::set_minNeighbors(int value)
{
    minNeighbors = value;
    ui->sb_minNeighbors->setValue(minNeighbors);

    find_faces();
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
