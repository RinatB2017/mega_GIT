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
#ifndef MYFINDFORM_HPP
#define MYFINDFORM_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   include </usr/include/opencv2/core/core.hpp>
#   include </usr/include/opencv2/objdetect/objdetect.hpp>
#   include </usr/include/opencv2/imgproc/imgproc.hpp>
#   include </usr/include/opencv2/highgui/highgui.hpp>
#   include </usr/include/opencv2/opencv.hpp>
#else
#   include <opencv2/core/core.hpp>
#   include <opencv2/objdetect/objdetect.hpp>
#   include <opencv2/imgproc/imgproc.hpp>
#   include <opencv2/highgui/highgui.hpp>
#   include <opencv2/highgui/highgui_c.h>
#   include <opencv2/opencv.hpp>
#   include <opencv2/highgui.hpp>
#   include <opencv2/imgcodecs/legacy/constants_c.h>
#   include <opencv2/videoio/videoio_c.h>
#endif
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
using namespace cv;
//--------------------------------------------------------------------------------
namespace Ui {
    class MyFindForm;
}
//--------------------------------------------------------------------------------
class MyFindForm : public MyWidget
{
    Q_OBJECT

public:
    explicit MyFindForm(QWidget *parent = nullptr);
    ~MyFindForm();

private:
    Ui::MyFindForm *ui;

    QString src_file        = "/dev/shm/0/Screenshot_5.png";
    QString file_ok         = "/dev/shm/0/ok.png";
    QString file_auto       = "/dev/shm/0/auto.png";
    QString file_in_battle  = "/dev/shm/0/в бой.png";

//    QString src_file        = "";
//    QString file_ok         = ":/targets/ok.png";
//    QString file_auto       = ":/targets/auto.png";
//    QString file_in_battle  = ":/targets/в бой.png";

    QList<QRgb> l_src_file;
    QList<QRgb> l_file_ok;
    QList<QRgb> l_file_auto;
    QList<QRgb> l_file_in_battle;
    QList<QRgb> l_temp;

    void init(void);
    void test(void);
    bool searchObjectByTemplate(QString srcImgName,
                                QString templImgName,
                                QRect *rect);

    void fail(void);

    void find_ok(void);
    void find_auto(void);
    void find_programm(void);
    void find_to_battle(void);

    void show_res(QString filename, QRect rect);

    void set_src_file(void);

    bool prepare_l(QString filename, QList<QRgb> *list);
    bool prepare_temp(QString filename, QRect rect, QList<QRgb> *list);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MYFINDFORM_HPP
