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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "ui_test_modbus_mainbox.h"
//--------------------------------------------------------------------------------
#include "test_modbus_mainbox.hpp"
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "crc.h"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_widgets(APPNAME);
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);
    createTestBar();

    init_serial();

    //---
    for(int n=0; n<10; n++)
    {
        QRadioButton *rb = new QRadioButton(this);
        rb->setObjectName(QString("rb_%1").arg(n));
        rb->setProperty("index", n);
        rb->setText(QString("%1").arg(n));

        QLineEdit *le_name = new QLineEdit(this);
        le_name->setObjectName(QString("le_name_%1").arg(n));
        le_name->setProperty("index", n);

        QLineEdit *le_answer = new QLineEdit(this);
        le_answer->setObjectName(QString("le_answer_%1").arg(n));
        le_answer->setProperty("index", n);

        ui->grid->addWidget(rb,          n, 0);
        ui->grid->addWidget(le_name,     n, 1);
        ui->grid->addWidget(le_answer,   n, 2);
    }
    //---

#if 1
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
#else
    if(sizeHint().height() > 0)
    {
        setMinimumHeight(sizeHint().height());
    }
#endif

    load_widgets(APPNAME);
}
//--------------------------------------------------------------------------------
bool MainBox::get_le_name(int index, QString *result)
{
    QList<QLineEdit *> allobj = findChildren<QLineEdit *>();
    foreach(QLineEdit *obj, allobj)
    {
        bool ok = false;
        int f_index = obj->property("index").toInt(&ok);
        if(ok)
        {
            if(index == f_index)
            {
                (*result) = obj->text();
                return true;
            }
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
bool MainBox::le_answer(int index, QString *result)
{
    QList<QLineEdit *> allobj = findChildren<QLineEdit *>();
    foreach(QLineEdit *obj, allobj)
    {
        bool ok = false;
        int f_index = obj->property("index").toInt(&ok);
        if(ok)
        {
            if(index == f_index)
            {
                (*result) = obj->text();
                return true;
            }
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
void MainBox::init_serial(void)
{
    ui->serial_widget->set_fix_baudrate(115200);
    connect(this,               SIGNAL(send(QByteArray)),   ui->serial_widget,  SLOT(input(QByteArray)));
    connect(ui->serial_widget,  SIGNAL(output(QByteArray)), this,               SLOT(read_data(QByteArray)));
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &MainBox::test_0 });
    commands.append({ ID_TEST_1, "test 1", &MainBox::test_1 });
    commands.append({ ID_TEST_2, "test 2", &MainBox::test_2 });
    commands.append({ ID_TEST_3, "test 3", &MainBox::test_3 });
    commands.append({ ID_TEST_4, "test 4", &MainBox::test_4 });
    commands.append({ ID_TEST_5, "test 5", &MainBox::test_5 });
    commands.append({ ID_TEST_6, "test 6", nullptr });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok)
    {
        return;
    }
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef bool (MainBox::*my_mega_function)(void);
            my_mega_function x;
            x = command.func;
            if(x)
            {
                (this->*x)();
            }
            else
            {
                emit error("no func");
            }

            return;
        }
    }
}
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit info("Test_0()");

    send_answer();

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_1()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit info("Test_2()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit info("Test_3()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit info("Test_4()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit info("Test_5()");
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray ba_data)
{
    if(ba_data.isEmpty())
    {
        return;
    }

    QString temp = QString("%1").arg(ba_data.data()).replace("\r", "").replace("\n", "");
    emit info(QString("read_data [%1]").arg(temp));

    for(int n=0; n<ba_data.length(); n++)
    {
        char s = ba_data.at(n);
        switch(s)
        {
        case ':':
            clean_packet.clear();
            break;

        case '\r':
        case '\n':
            analize_packet();
            clean_packet.clear();
            break;

        default:
            clean_packet.append(s);
            break;
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::analize_packet(void)
{
    emit info("analize_packet");
    send_answer();
}
//--------------------------------------------------------------------------------
void MainBox::send_answer(void)
{
    QList<QRadioButton *> allobj = findChildren<QRadioButton *>();
    foreach(QRadioButton *btn, allobj)
    {
        if(btn->isChecked())
        {
            emit info(btn->objectName());
            return;
        }
    }

    QByteArray packet;
#if 0
    if(ui->rb_1->isChecked())
    {
        packet.clear();
        packet.append(":");
        packet.append(ui->le_1->text());
        packet.append(0x0D);
    }
    if(ui->rb_2->isChecked())
    {
        packet.clear();
        packet.append(":");
        packet.append(ui->le_2->text());
        packet.append(0x0D);
    }
    if(ui->rb_3->isChecked())
    {
        packet.clear();
        packet.append(":");
        packet.append(ui->le_3->text());
        packet.append(0x0D);
    }
    if(ui->rb_4->isChecked())
    {
        packet.clear();
        packet.append(":");
        packet.append(ui->le_4->text());
        packet.append(0x0D);
    }
    if(ui->rb_5->isChecked())
    {
        packet.clear();
        packet.append(":");
        packet.append(ui->le_5->text());
        packet.append(0x0D);
    }
#endif
    emit send(packet);
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
