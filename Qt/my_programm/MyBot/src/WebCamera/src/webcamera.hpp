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
#ifndef WEBCAMERA_HPP
#define WEBCAMERA_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include </usr/include/opencv2/core/core.hpp>
#include </usr/include/opencv2/objdetect/objdetect.hpp>
#include </usr/include/opencv2/imgproc/imgproc.hpp>
#include </usr/include/opencv2/highgui/highgui.hpp>
#include </usr/include/opencv2/opencv.hpp>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class WebCamera;
}
//--------------------------------------------------------------------------------
class WebCamera : public MyWidget
{
    Q_OBJECT

public:
    WebCamera(QWidget *parent = 0);
    ~WebCamera();

private slots:
    void start(void);
    void stop(void);

    void test(void);
    void test2(void);
    void test3(void);
    void test4(void);
    void test5(void);

    void set_brightness(int value);
    void set_contrast(int value);
    void set_saturation(int value);
    void set_hue(int value);
    void set_autofocus(bool state);

private:
    Ui::WebCamera *ui = 0;

    int mCameraEventId;

    cv::Mat mOrigImage;
    cv::Mat mElabImage;

    cv::VideoCapture mCapture;

    // ---> Face detectors
    bool create_detectors(void);
    cv::CascadeClassifier mFaceDetector;
    cv::CascadeClassifier mEyeDetector;
    cv::CascadeClassifier mMouthDetector;
    cv::CascadeClassifier mNoseDetector;
    // <--- Face detectors

    void init(void);
    void show_image_cv(void);
    void show_image_hw(void);

    bool searchObjectByTemplate(QString srcImgName, QString templImgName, QRect *rect);

    void updateText(void);

protected:
    void timerEvent(QTimerEvent *);

};
//--------------------------------------------------------------------------------
#endif // WEBCAMERA_HPP
