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
#include "ui_test_hc05_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_hc05_mainbox.hpp"
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
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);
    createTestBar();

    init_serial();

    connect(ui->btn_find_hc05,      SIGNAL(clicked(bool)),  this,   SLOT(find_device()));
    connect(ui->btn_get_version,    SIGNAL(clicked(bool)),  this,   SLOT(get_version()));

#if 1
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
#else
    if(sizeHint().height() > 0)
    {
        setMinimumHeight(sizeHint().height());
    }
#endif
}
//--------------------------------------------------------------------------------
void MainBox::init_serial(void)
{
    serial = new SerialBox5_fix_baudrate(this, "RS232", "RS232");

    serial->add_menu(2);

    ui->serial_layout->addWidget(serial);

    connect(this,   SIGNAL(send(QByteArray)),   serial,     SLOT(input(QByteArray)));
    connect(serial, SIGNAL(output(QByteArray)), this,       SLOT(read_data(QByteArray)));
}
//--------------------------------------------------------------------------------
void MainBox::find_device(void)
{
    QList<int> speeds;

    speeds.clear();
    speeds.append(4800);
    speeds.append(9600);
    speeds.append(19200);
    speeds.append(28800);
    speeds.append(38400);
    speeds.append(57600);
    speeds.append(76800);
    speeds.append(115200);

    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
        emit info(QString(tr("Ищем %1 на порту %2"))
                  .arg(DEVICE_NAME)
                  .arg(port.portName()));
        foreach (int speed, speeds)
        {
            emit info(QString("speed %1").arg(speed));
            bool ok = serial->set_fix_baudrate(speed);
            if(ok)
            {
                bool ok = send_AT();
                if(ok)
                {
                    emit info(QString(tr("%1 найден на скорости %2"))
                              .arg(DEVICE_NAME)
                              .arg(speed));
                    device_speed = speed;
                    return;
                }
            }
            else
            {
                emit error("set_fix_baudrate return false");
            }
        }
    }
    emit error(QString("%1 не найден").arg(DEVICE_NAME));
}
//--------------------------------------------------------------------------------
bool MainBox::send_AT(void)
{
    QByteArray ba;
    ba.append("AT");

    rs232_data.clear();
    is_ready = false;
    emit send(ba);

    wait(1000);

    if(is_ready == false)
    {
        return false;
    }

    if(rs232_data.contains("OK"))
    {
        emit info("OK");
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
void MainBox::get_version(void)
{
    QByteArray ba;
    //ba.append("AT");
    ba.append("AT+VERSION?");
    //ba.append(0x0D);
    ba.append(0x0A);

    rs232_data.clear();
    is_ready = false;
    emit send(ba);

    wait(1000);

    if(is_ready == false)
    {
        emit error("No answer");
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray ba)
{
    emit debug(QString("read_data: [%1]").arg(ba.data()));
    rs232_data.append(ba);
    is_ready = true;
}
//--------------------------------------------------------------------------------
void MainBox::wait(int time_msec)
{
#ifdef FAKE
    Q_UNUSED(time_msec)
#else
    QTime time;

    time.start();
    while(true)
    {
        QCoreApplication::processEvents();
        if(time.elapsed() > time_msec)
            break;
    }
#endif
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
    commands.append({ ID_TEST_6, "test 6", 0 });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_block = new QCheckBox("block", this);
    testbar->addWidget(cb_block);

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

    connect(cb_block, SIGNAL(clicked(bool)), cb_test,           SLOT(setDisabled(bool)));
    connect(cb_block, SIGNAL(clicked(bool)), btn_choice_test,   SLOT(setDisabled(bool)));

    //testbar->setFixedWidth(toolBar->sizeHint().width());
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
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
