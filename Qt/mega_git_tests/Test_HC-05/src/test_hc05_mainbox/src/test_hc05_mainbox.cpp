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

#ifdef QT_DEBUG
    createTestBar();
#endif

    init_serial();

    connect(ui->btn_find_hc05,      &QPushButton::clicked,  this,   &MainBox::find_device);
    connect(ui->btn_get_version,    &QPushButton::clicked,  this,   &MainBox::get_version);
    connect(ui->btn_get_addr,       &QPushButton::clicked,  this,   &MainBox::get_address);
    connect(ui->btn_test,           &QPushButton::clicked,  this,   &MainBox::test);
    connect(ui->btn_reset,          &QPushButton::clicked,  this,   &MainBox::reset);
}
//--------------------------------------------------------------------------------
void MainBox::init_serial(void)
{
    connect(&serial, SIGNAL(readyRead()), this, SLOT(port_read()));
    connect(&serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(port_error(QSerialPort::SerialPortError)));
}
//--------------------------------------------------------------------------------
void MainBox::port_error(QSerialPort::SerialPortError serial_error)
{
    switch(serial_error)
    {
    case QSerialPort::NoError:  break;
    case QSerialPort::DeviceNotFoundError:  emit error("Error: DeviceNotFoundError"); break;
    case QSerialPort::PermissionError:      emit error("Error: PermissionError"); break;
    case QSerialPort::OpenError:            emit error("Error: OpenError"); break;
#ifdef Q_OS_LINUX
    case QSerialPort::ParityError:          emit error("Error: ParityError"); break;
    case QSerialPort::FramingError:         emit error("Error: FramingError"); break;
    case QSerialPort::BreakConditionError:  emit error("Error: BreakConditionError"); break;
#endif
    case QSerialPort::WriteError:           emit error("Error: WriteError"); break;
    case QSerialPort::ReadError:            emit error("Error: ReadError"); break;
    case QSerialPort::ResourceError:        emit error("Error: ResourceError"); break;
    case QSerialPort::UnsupportedOperationError: emit error("Error: UnsupportedOperationError"); break;
    case QSerialPort::UnknownError:         emit error("Error: UnknownError"); break;
    case QSerialPort::TimeoutError:         emit error("Error: TimeoutError"); break;
    case QSerialPort::NotOpenError:         emit error("Error: NotOpenError"); break;
    default:
        emit error(QString("Unknown error %1").arg(serial_error));
        break;
    }
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

    if(serial.isOpen()) serial.close();

    flag_closed = false;
    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
        emit info(QString(tr("Ищем %1 на порту %2"))
                  .arg(DEVICE_NAME)
                  .arg(port.portName()));
        foreach (int speed, speeds)
        {
            if(flag_closed) return;
            emit info(QString("speed %1").arg(speed));

            serial.setPort(port);
            bool ok = serial.setBaudRate(speed);
            if(ok)
            {
                if(serial.open(QIODevice::ReadWrite))
                {
                    ok = send_AT();
                    if(ok)
                    {
                        emit info(QString(tr("%1 найден на скорости %2"))
                                  .arg(DEVICE_NAME)
                                  .arg(speed));
                        return;
                    }
                    serial.close();
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
    if(serial.isOpen() == false)
    {
        emit error("serial not open!");
        return false;
    }

    return send_command("AT");
}
//--------------------------------------------------------------------------------
bool MainBox::reset(void)
{
    // [HC-05_white] addr (00:21:13:03:C4:81)
    // [HC-05_black] addr (00:14:02:10:09:04)

    return send_command("AT+ORGL");
}
//--------------------------------------------------------------------------------
bool MainBox::get_version(void)
{
    // [HC-05_white] addr (00:21:13:03:C4:81)
    // [HC-05_black] addr (00:14:02:10:09:04)

    return send_command("AT+VERSION?");
}
//--------------------------------------------------------------------------------
bool MainBox::get_address(void)
{
    // [HC-05_white] addr (00:21:13:03:C4:81)
    // [HC-05_black] addr (00:14:02:10:09:04)

    return send_command("AT+ADDR?");
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    // [HC-05_white] addr (00:21:13:03:C4:81)
    // [HC-05_black] addr (00:14:02:10:09:04)

    //14,2,100904

    bool ok = false;

#if 1
    ok = send_command("AT+UART=9600,0,0");
    if(!ok) return;
    ok = send_command("AT+NAME=hc05-arduino-nano");
    if(!ok) return;
    ok = send_command("AT+PSWD=\"1234\"");
    if(!ok) return;
    ok = send_command("AT+ROLE=0");
    if(!ok) return;
    ok = send_command("AT+RMAAD");
    if(!ok) return;
    ok = send_command("AT+CMODE=0");
    if(!ok) return;
#else
    ok = send_command("AT+ORGL");
    //if(!ok) return;
    ok = send_command("AT+RMAAD");
    //if(!ok) return;
    ok = send_command("AT+ROLE=1");
    //if(!ok) return;
    ok = send_command("AT+RESET");
    //if(!ok) return;
    ok = send_command("AT+PSWD=1234");
    //if(!ok) return;
    ok = send_command("AT+PAIR=14,2,100904, 5");    // (пример: AT+PAIR=12,6,143117, 5)
    //if(!ok) return;
    ok = send_command("AT+BIND=14,2,100904");       // (пример: AT+BIND=12,6,143117)
    //if(!ok) return;
    ok = send_command("AT+CMODE=0");
    //if(!ok) return;
#endif

    emit info("OK");
}
//--------------------------------------------------------------------------------
bool MainBox::send_command(QString cmd_string)
{
    if(cmd_string.isEmpty())
    {
        emit error("cmd_string is empty");
        return false;
    }

    if(serial.isOpen() == false)
    {
        emit error("serial not open!");
        return false;
    }

    emit info(QString("send: %1").arg(cmd_string));

    QByteArray ba;
    ba.append(cmd_string.toLatin1());
    ba.append(0x0D);
    ba.append(0x0A);

    rs232_data.clear();
    is_ready = false;
    serial.write(ba);

    wait(1000);

    if(is_ready == false)
    {
        emit error("No answer");
        return false;
    }

    if(rs232_data.isEmpty())
    {
        emit error("data is empty");
        return false;
    }

    if(rs232_data.contains("OK") == false)
    {
        emit error("data incorrect");
        return false;
    }

    emit info(rs232_data.replace('\r', "").replace('\n', "").replace("OK", ""));
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::port_read(void)
{
    rs232_data.append(serial.readAll());
    is_ready = true;
}
//--------------------------------------------------------------------------------
void MainBox::wait(int time_msec)
{
#ifdef FAKE
    Q_UNUSED(time_msec)
#else
    QElapsedTimer time;

    time.start();
    while(true)
    {
        QCoreApplication::processEvents();
        if(time.elapsed() > time_msec)
            break;

        if(is_ready)
            break;
    }
#endif
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test", &MainBox::f_test });

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
    auto cmd_it = std::find_if(
        commands.begin(),
        commands.end(),
        [cmd](CMD command){ return command.cmd == cmd; }
    );
    if (cmd_it != commands.end())
    {
        typedef bool (MainBox::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (this->*x)();
        }
        else
        {
            emit error("no func");
        }
    }
}
//--------------------------------------------------------------------------------
bool MainBox::f_test(void)
{
    emit info("Test");

    return true;
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
