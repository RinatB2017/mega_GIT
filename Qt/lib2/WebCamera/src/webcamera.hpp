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
#include <QInputDialog>
//--------------------------------------------------------------------------------
#include <opencv2/core/core.hpp>
#include <opencv2/core/core_c.h>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs/legacy/constants_c.h>
#include <opencv2/videoio/videoio_c.h>

#include <opencv2/videoio.hpp>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#define FACE_DETECT
//--------------------------------------------------------------------------------
using namespace cv;
//--------------------------------------------------------------------------------
namespace Ui {
    class WebCamera;
}
//--------------------------------------------------------------------------------
class WebCamera : public MyWidget
{
    Q_OBJECT

public:
    explicit WebCamera(QWidget *parent);
    virtual ~WebCamera();

    bool searchObjectByTemplate(const char *srcImgName, const char *templName, QRect *rect);

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
    void set_device(void);

private:
    Ui::WebCamera *ui;

    int mCameraEventId = 0;

    Mat mOrigImage;
    Mat mElabImage;

    VideoCapture mCapture;

    // ---> Face detectors
#ifdef FACE_DETECT
    bool create_detectors(void);
    CascadeClassifier mFaceDetector;
    CascadeClassifier mEyeDetector;
    CascadeClassifier mMouthDetector;
    CascadeClassifier mNoseDetector;
#endif
    // <--- Face detectors

    void init(void);
    void show_image_cv(void);
    void show_image_hw(void);

    void check_xml_files(void);
    void copy_file(QString filename);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);

protected:
    void timerEvent(QTimerEvent *);
};
//--------------------------------------------------------------------------------
#endif // WEBCAMERA_HPP
