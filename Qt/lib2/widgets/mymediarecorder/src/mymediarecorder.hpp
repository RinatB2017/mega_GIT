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
#ifndef MYMEDIARECORDER_HPP
#define MYMEDIARECORDER_HPP
//--------------------------------------------------------------------------------
#include <QMediaPlayer>
#include <QPointer>
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#   include <QVideoProbe>
#endif
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   include <opencv2/imgproc/imgproc.hpp>
#   include <opencv2/videoio.hpp>
#endif
#ifdef Q_OS_WIN
#   include "imgproc/imgproc.hpp"
#   include "videoio.hpp"
#endif
//--------------------------------------------------------------------------------
#include "myfiledialog.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
using namespace cv;
//--------------------------------------------------------------------------------
namespace Ui {
    class MyMediaRecorder;
}
//--------------------------------------------------------------------------------
class MyMediaRecorder : public MyWidget
{
    Q_OBJECT

public:
    explicit MyMediaRecorder(QWidget *parent = nullptr);
    virtual ~MyMediaRecorder();

public slots:
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    void get_frame(QVideoFrame frame);
#endif

private:
    Ui::MyMediaRecorder *ui;

    QString filename = "output.avi";
    VideoWriter out;

    qreal frames_per_second = 30.0f;

    void init(void);

    void set_filename(void);
    void start(void);
    void stop(void);

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    void processFrame(QVideoFrame const &frame);
#endif

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MYMEDIARECORDER_HPP
