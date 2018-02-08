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
#include <QtWidgets>
//--------------------------------------------------------------------------------
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
class My_class
{
public:
    inline void operator =(const quint32 &value)
    {
        val  = value;
        c[0] = (val >> 24) & 0xFF;
        c[1] = (val >> 16) & 0xFF;
        c[2] = (val >> 8)  & 0xFF;
        c[3] = val         & 0xFF;
    }
    inline quint8 &operator [](int index) { return c[index]; }
    inline quint8 operator [](int index) const { return c[index]; }

    inline quint32 get_value(void) { return val; }

private:
    quint32 val = 0;
    quint8 c[4];
};
//--------------------------------------------------------------------------------
class MyMegaWidget : public QWidget
{
    Q_OBJECT

public:
    MyMegaWidget(QWidget *parent) : QWidget(parent)
    {
        setStyleSheet("background:red;");
        setFixedSize(10, 10);
    }

private:
};
//--------------------------------------------------------------------------------
class MySplashScreen;
class QToolButton;
class QComboBox;
class QToolBar;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent,
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

    MySplashScreen *splash = 0;
    Ui::MainBox *ui = 0;

    QComboBox *cb_test = 0;
    QList<CMD> commands;

    bool split_address(const QString address,
                       int *a,
                       int *b,
                       int *c,
                       int *d,
                       int *port);

    void init(void);
    void createTestBar(void);
    void updateText(void);

    quint32 test(const QByteArray ba);
    bool test2(int a, int b, int *c, int *d);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
