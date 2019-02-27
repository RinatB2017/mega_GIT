/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
//#define LEN_SIDE    800
//#define LEN_SIDE    3465
//#define SMALL_R     (LEN_SIDE / 8)
#define FAST_PIC
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class QToolButton;
class QToolBar;
class QComboBox;
class QCheckBox;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent,
            MySplashScreen *splash);
    ~MainBox();

private slots:
    void choice_test(void);
    bool test_0(void);
    bool test_1(void);
    bool test_2(void);
    bool test_3(void);
    bool test_4(void);
    bool test_5(void);

    bool s_create_orig_image(void);
    bool s_load_orig_image(void);
    void s_show_orig_image(void);

    bool s_create_new_image(void);
    bool s_create_new_image_2(void);
    void s_show_new_image(void);

    void set_pic_width(int value);
    void set_pic_height(int value);

    void f_test(void);
    void f_test_2(void);

private:
    enum {
        ID_TEST_0 = 1000,
        ID_TEST_1,
        ID_TEST_2,
        ID_TEST_3,
        ID_TEST_4,
        ID_TEST_5
    };
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (MainBox::*func)(void);
    } CMD_t;

    MySplashScreen *splash;
    Ui::MainBox *ui;

    QComboBox *cb_test;
    QList<CMD> commands;

    QImage *orig_image;
    QImage *new_image;
    long cnt_sin = 0;
    long cnt_cos = 0;

    qreal pic_width = 0;
    qreal pic_height = 0;

    void show_image(QImage *image,
                    int min_x,
                    int min_y);

    void init(void);
    void createTestBar(void);
    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
