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

    bool test_assert(int value);

public slots:
    void choice_test(void);
    bool test_0(void);
    bool test_1(void);
    bool test_2(void);
    bool test_3(void);
    bool test_4(void);
    bool test_5(void);

    void print_mp(QWidget *widget);
    bool load_property(QWidget *widget, const QString &property_name);
    bool save_property(QWidget *widget, const QString &property_name, QVariant value);
    void delete_string(void);

    void s_inFunc(void);

    void check_in(void);
    void victory(void);

    void set_time(void);
    void run_timer(void);

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

    //---
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (MainBox::*func)(void);
    } CMD_t;
    QList<CMD> commands;
    //---

    MySplashScreen *splash;
    Ui::MainBox *ui;

    QComboBox *cb_test;

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
