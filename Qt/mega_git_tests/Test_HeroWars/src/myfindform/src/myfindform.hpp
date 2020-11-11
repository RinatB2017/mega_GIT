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
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   include <opencv2/core/core.hpp>
#   include <opencv2/core/core_c.h>
#   include <opencv2/objdetect/objdetect.hpp>
#   include <opencv2/imgproc/imgproc.hpp>
#   include <opencv2/opencv.hpp>
#   include <opencv2/highgui.hpp>
#   include <opencv2/highgui/highgui.hpp>
#   include <opencv2/highgui/highgui_c.h>
#   include <opencv2/imgcodecs/legacy/constants_c.h>
#   include <opencv2/videoio/videoio_c.h>

    using namespace cv;
#endif
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

    QString src_file        = "/dev/shm/0/Screenshot.png";

#ifdef Q_OS_LINUX
    QString file_ok          = "/dev/shm/0/ok.png";
    QString file_auto        = "/dev/shm/0/auto.png";
    QString file_auto_active = "/dev/shm/0/auto_active.png";
    QString file_in_battle   = "/dev/shm/0/в бой.png";
#endif

#ifdef Q_OS_WIN
    QString file_ok          = "C:/0/ok.png";
    QString file_auto        = "C:/0/auto.png";
    QString file_auto_active = "C:/0/auto_active.png";
    QString file_in_battle   = "C:/0/в бой.png";
#endif

    QList<QRgb> l_src_file;
    QRect rect_src_file;

    QList<QRgb> l_file_ok;
    QRect rect_file_ok;

    QList<QRgb> l_file_auto;
    QRect rect_file_auto;

    QList<QRgb> l_file_auto_active;
    QRect rect_file_auto_active;

    QList<QRgb> l_file_in_battle;
    QRect rect_file_in_battle;

    QList<QRgb> l_temp;
    QRect rect_temp;

    void show_rect_picture(QString caption, QList<QRgb> array, QRect rect);

    void init(void);
    bool searchObjectByTemplate(const char *imgName,
                                const char *templName,
                                QRect *rect);

    void click(void);
    void fail(void);

    void find_ok(void);
    void find_auto(void);
    void find_auto_active(void);
    void find_programm(void);
    void find_to_battle(void);

    void check_pict(const QString &orig_file,
                    const QString &copy_file);

    bool find_programm_with_title(const QString &title);
    bool find_window(const QString &programm_title,
                     int *x,
                     int *y,
                     int *width,
                     int *heigth);

    void show_res(QString filename, QRect rect);

    void set_src_file(void);

    bool prepare_l(QString filename, QList<QRgb> *list, QRect *rect);
    bool prepare_temp(QString filename,
                      QRect rect,
                      QList<QRgb> *list,
                      QRect *out_rect);

    void mouse_click(unsigned int button, QPoint pos);

    void test(void);
    void add_buttons(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MYFINDFORM_HPP
