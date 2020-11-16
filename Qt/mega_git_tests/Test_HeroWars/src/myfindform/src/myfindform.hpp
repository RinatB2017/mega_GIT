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

    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        void (MyFindForm::*func)(void);
    } *cmd_t;
    QList<CMD> commands;
    QPointer<QComboBox> cb_test;

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

    void init(void);
    void createTestBar(void);
    void choice_test(void);
    bool searchObjectByTemplate(const char *imgName,
                                const char *templName,
                                QRect *rect);

    void fail(void);

    void check_pict(const QString &orig_file,
                    const QString &copy_file);

    void show_res(QString filename, QRect rect);
    void mouse_click(unsigned int button, QPoint pos);

    void test(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MYFINDFORM_HPP
