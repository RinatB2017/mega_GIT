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
class TestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TestWidget(QWidget *parent) : QWidget(parent)
    {
        setFixedSize(250, 250);
    }

    void save(QString filename)
    {
        QPixmap *pixmap = new QPixmap(this->size());
        this->render(pixmap);
        pixmap->save(filename);
    }

protected:
    void paintEvent(QPaintEvent *)
    {
        //---
        QColor begin_color = Qt::lightGray;
        QColor end_color   = Qt::darkGray;
        //---
        QColor begin_border_color = Qt::lightGray;
        QColor end_border_color   = Qt::darkGray;
        //---
        int border = 50;
        //---

        QPainter painter(this);

        painter.setBrush(QBrush(begin_color));
        painter.drawRect(0, 0, width(), height());

        painter.setBrush(QBrush(end_color));
        painter.drawRect(border, border, width() - border, height() - border);

        //---
        QLinearGradient shader_left(QPointF(0, height() / 2.0),
                                    QPointF(border, height() / 2.0));
        shader_left.setColorAt(0, begin_border_color);
        shader_left.setColorAt(1, end_border_color);
        //---
        QLinearGradient shader_up(QPointF(width() / 2.0, 0),
                                  QPointF(width() / 2.0, border));
        shader_up.setColorAt(0, begin_border_color);
        shader_up.setColorAt(1, end_border_color);
        //---
        QLinearGradient shader_right(QPointF(width(), height() / 2.0),
                                     QPointF(width() - border, height() / 2.0));
        shader_right.setColorAt(0, begin_border_color);
        shader_right.setColorAt(1, end_border_color);
        //---
        QLinearGradient shader_down(QPointF(width() / 2.0, height()),
                                    QPointF(width() / 2.0, height() - border));
        shader_down.setColorAt(0, begin_border_color);
        shader_down.setColorAt(1, end_border_color);
        //---

        //---
        QLinearGradient shader_ul(QPointF(0, 0),
                                  QPointF(border, border));
        shader_ul.setColorAt(0, begin_border_color);
        shader_ul.setColorAt(1, end_border_color);
        //---
        QLinearGradient shader_ur(QPointF(border, 0),
                                  QPointF(0, border));
        shader_ur.setColorAt(0, begin_border_color);
        shader_ur.setColorAt(1, end_border_color);
        //---
        QLinearGradient shader_dl(QPointF(0, border),
                                  QPointF(border, 0));
        shader_dl.setColorAt(0, begin_border_color);
        shader_dl.setColorAt(1, end_border_color);
        //---
        QLinearGradient shader_dr(QPointF(border, border),
                                  QPointF(0, 0));
        shader_dr.setColorAt(0, begin_border_color);
        shader_dr.setColorAt(1, end_border_color);
        //---

        //---
        // left
        painter.fillRect(0,
                         border,
                         border,
                         height() - 2 * border,
                         shader_left);
        //---
        // right
        painter.fillRect(width() - border,
                         border,
                         width() - border,
                         height() - 2 * border,
                         shader_right);
        //---
        // up
        painter.fillRect(border,
                         0,
                         width() - 2 * border,
                         border,
                         shader_up);
        //---
        // down
        painter.fillRect(border,
                         height() - border,
                         width() - 2 * border,
                         border,
                         shader_down);
        //---
        // ul
        painter.fillRect(0,
                         0,
                         border,
                         border,
                         shader_ul);
        //---
        // ur
        painter.fillRect(width() - border,
                         0,
                         border,
                         border,
                         shader_ur);
        //---
        // dl
        painter.fillRect(0,
                         width() - border,
                         border,
                         border,
                         shader_dl);
        //---
        // dr
        painter.fillRect(width() - border,
                         width() - border,
                         border,
                         border,
                         shader_dr);
    }
};
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
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
    void updateText(void);

    int get_cnt(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
