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
#include <QVideoProbe>
#include <QPointer>
//--------------------------------------------------------------------------------
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio.hpp>
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
    void get_frame(QVideoFrame frame);

private:
    Ui::MyMediaRecorder *ui;

    QString filename = "output.avi";
    VideoWriter out;

    void init(void);

    void set_filename(void);
    void start(void);
    void stop(void);

    void processFrame(QVideoFrame const &frame);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MYMEDIARECORDER_HPP
