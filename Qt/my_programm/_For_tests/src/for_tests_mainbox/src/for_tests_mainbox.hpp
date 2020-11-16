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
#include "ui_for_tests_mainbox.h"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
class MyDebug
{
public:
    static void set_v(int v)
    {
        s_value = v;
    }

    static int get_v(void)
    {
        return s_value;
    }

private:
    static int s_value;
};

#include <QWebEngineView>
// https://forum.qt.io/topic/54233/how-to-capture-mouse-events-in-webengineview
class TestBrowser : public QWebEngineView
{
    Q_OBJECT

public:
    TestBrowser()
    {
        rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
//        installEventFilter(this);
        QApplication::instance()->installEventFilter(this);
        setMouseTracking(true);
    }

private:
    QPoint origin;
    QRubberBand *rubberBand = nullptr;

protected:
    bool eventFilter(QObject *object, QEvent *event)
    {
        if (object->parent() == this && event->type() == QEvent::MouseButtonPress)
        {
            origin = static_cast<QMouseEvent *>(event)->pos();
            rubberBand->setGeometry(QRect(origin, QSize()));
            rubberBand->show();

            mousePressEvent(static_cast<QMouseEvent *>(event));
        }
        if (object->parent() == this && event->type() == QEvent::MouseButtonRelease)
        {
            rubberBand->hide();
            mouseReleaseEvent(static_cast<QMouseEvent *>(event));
        }
        if (object->parent() == this && event->type() == QEvent::MouseMove)
        {
            rubberBand->setGeometry(QRect(origin, static_cast<QMouseEvent *>(event)->pos()).normalized());
            mouseMoveEvent(static_cast<QMouseEvent *>(event));
        }

        return false;
    }
};
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class SimpleWidget;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent, MySplashScreen *splash);
    ~MainBox();

    typedef void (MainBox::*saveSlot)(void);
    void inFunc(QPushButton *btn, saveSlot slot);

public slots:
    void choice_test(void);
    bool timer_start(void);
    bool timer_stop(void);
    void show_timer_count(void);

    bool test(void);

    void print_mp(QWidget *widget);
    void delete_string(void);

    void s_inFunc(void);

    void check_in(void);
    void victory(void);

private:
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (MainBox::*func)(void);
    } *cmd_t;
    QList<CMD> commands;

    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;

    QPointer<QComboBox> cb_test;
    QPointer<QTimer> timer;
    int cnt = 0;

    //---
    QDockWidget *main_dock;
    QList<QDockWidget *> l_docks;
    //---

    //MyToolBar *bar = nullptr;
    bool direction = false;

    QPointer<QFileSystemModel> file_model;

    QPointer<QSpinBox>  sb_test;
    QPointer<QCheckBox> cb_block;

    bool set_theme_windows(void);
    bool set_norton_commander(void);
    bool set_styles(void);

    bool inc_push_button(void);
    bool dec_push_button(void);

    void init(void);
    void createTestBar(void);

    void test_validator(void);
    int get_cnt(void);

    void heavy_function(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
