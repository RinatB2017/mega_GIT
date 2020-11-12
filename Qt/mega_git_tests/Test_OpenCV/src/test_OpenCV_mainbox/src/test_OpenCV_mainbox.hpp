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
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include <QtGlobal>
//--------------------------------------------------------------------------------
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
//--------------------------------------------------------------------------------
#include <opencv2/opencv.hpp>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#ifdef Q_OS_WIN
#   include <windows.h>
#endif
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox( QWidget* parent = nullptr );
    ~MainBox();

private slots:
    void onLoad(void);
    void refreshHSV(void);

    void set_scaleFactor(int value);
    void set_scaleFactor(double value);
    void set_minNeighbors(int value);

    void find_faces(void);

    void s_test(void);

private:
    Ui::MainBox *ui;
    cv::Mat mOrigImage;
    cv::Mat mElabImage;

    QString m_lastLoadPath = ".";

    // ---> Face detectors
    bool create_detectors(void);

    cv::Mat grayFrames;
    cv::CascadeClassifier faceCade;
    std::vector<cv::Rect> faces;

    qreal scaleFactor = 1.0;
    int minNeighbors = 0;

    cv::CascadeClassifier mFaceDetector;
    cv::CascadeClassifier mEyeDetector;
    cv::CascadeClassifier mMouthDetector;
    cv::CascadeClassifier mNoseDetector;
    // <--- Face detectors

    void init(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
