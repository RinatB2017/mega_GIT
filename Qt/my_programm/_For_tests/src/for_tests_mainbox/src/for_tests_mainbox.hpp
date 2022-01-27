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
#include <QtMath>
//--------------------------------------------------------------------------------
#include "for_tests_mainbox_gui.hpp"
#include "mysplashscreen.hpp"
#include "mymainwindow.hpp"
#include "myfiledialog.hpp"
#include "mywidget.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
class TestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TestWidget(QWidget *parent = nullptr) :
        QWidget(parent)
    {
        setFixedSize(300, 300);
    }

private:
    void calc_line(qreal center_x,
                   qreal center_y,
                   qreal angle,
                   qreal radius,
                   qreal *end_x,
                   qreal *end_y)
    {
        qreal A = radius;
        qreal B = qCos(qDegreesToRadians(angle)) * A;
        qreal C = qSin(qDegreesToRadians(angle)) * A;

        *end_x = center_x + B;
        *end_y = center_y + C;
    }

protected:
    void paintEvent(QPaintEvent *)
    {
        QPainter painter(this);

        const int count = 12; //6 зубцов

        bool flag = false;
        qreal p_x = 0.0;
        qreal p_y = 0.0;
        qreal inc_angle = 360.0 / count;
        qreal angle = 0.0;
        qreal radius = 10.0;
        qreal center_x = 150;
        qreal center_y = 150;

        // отзеркалим по вертикали
        painter.translate(0, height());
        painter.scale(1.0, -1.0);

        painter.drawLine(5, 5, 25, 25); // для теста

        for(int n=0; n<count; n++)
        {
            flag = !flag;
            if(flag)
                radius = 50.0;
            else
                radius = 25.0;

            calc_line(0,
                      0,
                      angle,
                      radius,
                      &p_x,
                      &p_y);
            angle += inc_angle;
            painter.drawLine(center_x,
                             center_y,
                             p_x + 150.0,
                             p_y + 150.0);
        }
    }
};
//--------------------------------------------------------------------------------
class FractalWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FractalWidget(QWidget *parent = nullptr) :
        QWidget(parent)
    {
        qreal w = 600.0;
        qreal h = 600.0;
        qreal c_x = w / 2.0;
        qreal c_y = h / 2.0;

        setFixedSize(w, h);

        lines.append(QLineF(c_x,
                            c_y,
                            c_x,
                            c_y + 100));
        show_line(c_x,
                  c_y,
                  c_x,
                  c_y + 200,
                  90.0);
    }

private:
    int cnt = 0;
    int max_cnt = 100;
    QList<QLineF> lines;

    void calc_line(qreal center_x,
                   qreal center_y,
                   qreal angle,
                   qreal radius,
                   qreal *end_x,
                   qreal *end_y)
    {
        qreal A = radius;
        qreal B = qCos(qDegreesToRadians(angle)) * A;
        qreal C = qSin(qDegreesToRadians(angle)) * A;

        *end_x = center_x + B;
        *end_y = center_y + C;
    }

protected:
    void show_line(qreal x1,
                   qreal y1,
                   qreal x2,
                   qreal y2,
                   qreal angle)
    {
        if(cnt > max_cnt) return;
        cnt++;

        qreal center_x = x1 + (x2 - x1) / 2.0;
        qreal center_y = y1 + (y2 - y1) / 2.0;

        qreal end_x;
        qreal end_y;

        qreal radius = qSqrt(qPow(x2 - x1, 2) + qPow(y2 - y1, 2)) * 0.9;
        qreal new_angle;

        //---
#if 1
        new_angle = angle + 45.0;
        calc_line(center_x,
                  center_y,
                  new_angle,
                  radius,
                  &end_x,
                  &end_y);
        lines.append(QLineF(center_x,
                            center_y,
                            end_x,
                            end_y));
#else
        new_angle = angle - 45.0;
        calc_line(center_x,
                  center_y,
                  new_angle,
                  radius,
                  &end_x,
                  &end_y);
        lines.append(QLineF(center_x,
                            center_y,
                            end_x,
                            end_y));
#endif
        //---
        show_line(center_x,
                  center_y,
                  end_x,
                  end_y,
                  new_angle);
    }

    void paintEvent(QPaintEvent *)
    {
        // отзеркалим по вертикали
        QPainter painter(this);
        painter.translate(0, height());
        painter.scale(1.0, -1.0);

        painter.setPen(QColor(Qt::red));
        foreach (QLineF line, lines)
        {
            painter.drawLine(line);
        }
    }
};
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
//--------------------------------------------------------------------------------
class MainBox : public MainBox_GUI
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent, MySplashScreen *splash);
    virtual ~MainBox();

public slots:
    void choice_test(void);

    bool test(void);
    bool test2(void);

private:
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (MainBox::*func)(void);
    } *cmd_t;
    QList<CMD> commands;

    QPointer<MySplashScreen> splash;

    QPointer<QComboBox> cb_test;
    QPointer<QSpinBox>  sb_test;
    QPointer<QCheckBox> cb_block;

    bool set_theme_windows(void);
    bool set_norton_commander(void);
    bool set_styles(void);

    void init(void);
    void createTestBar(void);

    template<typename T1, typename T2>
    void test_template(void);

    void heavy_function(int x);

    template<class T>
    void test_function(int delay);
    void test_function2(bool (MainBox::*func)(void));

    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
