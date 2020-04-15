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
#include "ui_for_tests_mainbox.h"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
//class A {

//public:
//    A()
//    {
//        init();
//    }
//    void init(void)
//    {
//        qDebug() << "A: init";
//        widget = new QWidget();
//    }
//    void test(void)
//    {
//        qDebug() << "A: test";
//    }

//    QWidget *widget = nullptr;
//};

//class B : A {

//public:
//    B()
//    {
//        init();
//    }
//    void init(void)
//    {
//        qDebug() << "B: init";
//        widget = new QSpinBox();
//        test();
//    }

//    QSpinBox *widget = nullptr;
//};
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

signals:

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    ~MainBox();

    typedef void (MainBox::*saveSlot)(void);
    void inFunc(QPushButton *btn, saveSlot slot);

public slots:
    void choice_test(void);
    bool test(void);
    bool print_property(void);
    bool timer_start(void);
    bool timer_stop(void);
    void show_timer_count(void);

    void print_mp(QWidget *widget);
    bool load_property(QWidget *widget, const QString &property_name);
    bool save_property(QWidget *widget, const QString &property_name, QVariant value);
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
    } CMD_t;
    QList<CMD> commands;

    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;

    QPointer<QComboBox> cb_test;
    QPointer<SimpleWidget> sw;
    QPointer<QTimer> timer;
    int cnt = 0;

    void init(void);
    void createTestBar(void);

    void test_validator(void);
    int get_cnt(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
