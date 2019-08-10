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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "for_tests_mainbox.hpp"
#include "ui_for_tests_mainbox.h"

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
//--------------------------------------------------------------------------------
class TestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TestWidget(int l_width,
                        int l_height,
                        QWidget *parent = nullptr)
        : QWidget(parent)
    {
        cnt_x = l_width;
        cnt_y = l_height;

        d_width  = l_width  * led_width;
        d_height = l_height * led_height;

        for(int n=0; n<l_width * l_height; n++)
        {
            leds.append(Qt::black);
        }

        setFixedSize(d_width, d_height);
        update();
    }

    void set_color(int x, int y, QColor color)
    {
        if(x > cnt_x)
        {
            return;
        }
        if(y > cnt_y)
        {
            return;
        }
        int index = y*cnt_x + x;
        leds[index] = color;

        update();
    }

private:
    int d_width = 0;
    int d_height = 0;

    int cnt_x = 0;
    int cnt_y = 0;

    int led_width = 16;
    int led_height = 16;
    int led_border = 1;

    QList<QColor> leds;

    QColor led_color = Qt::black;
    QColor border_color = Qt::gray;

protected:
    void paintEvent(QPaintEvent *)
    {
        QPainter painter(this);
        painter.setBrush(QBrush(led_color));
        painter.drawRect(0, 0, width(), height());

        painter.setPen(border_color);
        for(int y=0; y<cnt_y; y++)
        {
            painter.drawLine(0, y * led_height, width(), y * led_height);
        }
        for(int x=0; x<cnt_x; x++)
        {
            painter.drawLine(x * led_width, 0, x * led_width, height());
        }

        for(int y=0; y<cnt_y; y++)
        {
            for(int x=0; x<cnt_x; x++)
            {
                int index = y*cnt_x + x;
                painter.setBrush(QBrush(leds[index]));
                painter.drawRect(x*led_width,
                                 y*led_height,
                                 led_width,
                                 led_height);
            }
        }
    }
};
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    ~MainBox();

    typedef void (MainBox::*saveSlot)(void);
    void inFunc(QPushButton *btn, saveSlot slot);

    bool test_assert(int value);

public slots:
    void choice_test(void);
    bool test_0(void);
    bool test_1(void);
    bool test_2(void);
    bool test_3(void);
    bool test_4(void);
    bool test_5(void);

    void s_inFunc(void);

    void check_in(void);
    void victory(void);

private:
    enum {
        ID_TEST_0 = 1000,
        ID_TEST_1,
        ID_TEST_2,
        ID_TEST_3,
        ID_TEST_4,
        ID_TEST_5,
        ID_TEST_6
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

    QComboBox *cb_test2;

    //---
    void test_validator(void);
    //---
    template<typename T>
    void temp_test(T *obj, int x)
    {
        obj->setText(QString("x = %1").arg(x));
        cb_test->setCurrentIndex(2);
    }
    //---

    void init(void);
    void createTestBar(void);

    bool create_color_block(int width,
                            int height,
                            int w_border,
                            QColor color_border,
                            QColor color,
                            QString path,
                            QString filename);

    int get_cnt(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
