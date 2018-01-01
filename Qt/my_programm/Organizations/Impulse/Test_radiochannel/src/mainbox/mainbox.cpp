/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#include <QCoreApplication>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QSpinBox>
#include <QToolBar>
#include <QLabel>
#include <QDebug>
#include <QTime>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "grapherbox.hpp"

#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "sleeper.h"
//--------------------------------------------------------------------------------
#define MAX_LEN_DATA    10

union PACKET_QUESTION
{
    struct {
        unsigned char address;
        unsigned char test_data[MAX_LEN_DATA];
        unsigned char channel;
    } data;
    unsigned char buf[sizeof(data)];
};
union PACKET_ANSWER
{
    struct {
        unsigned char address;
        unsigned char test_data[MAX_LEN_DATA];
    } data;
    unsigned char buf[sizeof(data)];
};
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox),
    test_is_started(false)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    test_is_started = false;
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

    connect(&serial, SIGNAL(readyRead()), this, SLOT(port_read()));
    connect(&serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(port_error(QSerialPort::SerialPortError)));

    QColor color = Qt::red;
    QColor color_background = Qt::yellow;
    QwtSymbol *symbol = new QwtSymbol(QwtSymbol::Ellipse,
                                      QBrush(color_background),
                                      QPen(color, 2),
                                      QSize(8, 8));

    grapher = new GrapherBox(this);
    grapher->set_title("test");
    grapher->set_title_axis_X("x");
    grapher->set_title_axis_Y("y");
    grapher->set_axis_scale_x(0, 1000);
    grapher->set_axis_scale_y(0, 300);

    grapher->add_curve("data", symbol);

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(grapher);
    setLayout(hbox);
}
//--------------------------------------------------------------------------------
void MainBox::port_read(void)
{
    //emit debug("port read");
    data_multimeter.append(serial.readAll());
    is_ready = true;
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
    case QSerialPort::ParityError:          emit error("Error: ParityError"); break;
    case QSerialPort::FramingError:         emit error("Error: FramingError"); break;
    case QSerialPort::BreakConditionError:  emit error("Error: BreakConditionError"); break;
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
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    sb_address = new QSpinBox(this);
    sb_address->setMinimum(0);
    sb_address->setMaximum(0xFF);
    sb_address->setObjectName("sb_address");
    //sb_address->setValue(7);

    sb_channel = new QSpinBox(this);
    sb_channel->setMinimum(1);
    sb_channel->setMaximum(15);
    sb_channel->setObjectName("sb_channel");
    //sb_channel->setValue(4);

    sb_time_multimeter = new QSpinBox(this);
    sb_time_multimeter->setMinimum(1000);
    sb_time_multimeter->setMaximum(0xFFFF);
    sb_time_multimeter->setObjectName("sb_time_multimeter");
    //sb_time_multimeter->setValue(3000);

    testbar->addWidget(new QLabel(tr("Адрес:")));
    testbar->addWidget(sb_address);
    testbar->addWidget(new QLabel(tr("Канал:")));
    testbar->addWidget(sb_channel);
    testbar->addWidget(new QLabel(tr("Max time msec:")));
    testbar->addWidget(sb_time_multimeter);

    QToolButton *btn_test_start = add_button(testbar,
                                             new QToolButton(this),
                                             qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                             "start",
                                             "start");
    QToolButton *btn_test_stop = add_button(testbar,
                                            new QToolButton(this),
                                            qApp->style()->standardIcon(QStyle::SP_MediaStop),
                                            "stop",
                                            "stop");

    connect(btn_test_start, SIGNAL(clicked()), this, SLOT(test_start()));
    connect(btn_test_stop,  SIGNAL(clicked()), this, SLOT(test_stop()));

    testbar->setFixedSize(testbar->sizeHint());
}
//--------------------------------------------------------------------------------
bool MainBox::find_device(void)
{
    int speed = 57600;
    if(serial.isOpen())
    {
        serial.close();
    }
    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
        emit info(QString(tr("Порт %1")).arg(port.portName()));
        serial.setPort(port);
        bool ok = serial.setBaudRate(speed);
        if(ok)
        {
            if(serial.open(QIODevice::ReadWrite))
            {
                bool ok = check_breloque();
                if(ok)
                {
                    emit info(QString(tr("USB приемо-передатчик В7-86/2 найден на порту %1"))
                              .arg(port.portName()));
                    return true;
                }
                serial.close();
            }
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
bool MainBox::check_breloque(void)
{
    if(serial.isOpen() == false)
    {
        emit error(tr("Порт USB приемо-передатчика не открыт!"));
        return false;
    }

    emit info(tr("Тест USB приемо-передатчика"));

    QByteArray temp;
    temp.clear();

    temp.append(":");
    temp.append("FF55AA55FF");
    temp.append((char)0x0D);

    data_multimeter.clear();
    is_ready = false;
    serial.write(temp);

    QTime time;
    time.start();
    wait_msec(5000, sizeof(breloque_answer));
    emit debug(QString(tr("Прошло времени %1 ms")).arg(time.elapsed()));

    if(data_multimeter.isEmpty())
    {
        emit error(tr("USB приемо-передатчик не отвечает!"));
        return false;
    }

    //emit debug(data_radiomodule);
    if(data_multimeter.data() != breloque_answer)
    {
        emit error(tr("Это не USB приемо-передатчик!"));
        return false;
    }
    emit info("Ok");
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::test_start(void)
{
    emit info("test start");
    if(!find_device()) return;

    test_is_started = true;
    while(test_is_started)
    {
        QCoreApplication::processEvents();
        send_data();
        Sleeper::msleep(1000);
        //0720010203040506070809
    }
    emit info("test end");
}
//--------------------------------------------------------------------------------
void MainBox::test_stop(void)
{
    test_is_started = false;
}
//--------------------------------------------------------------------------------
void MainBox::send_data(void)
{
    PACKET_QUESTION question;
    PACKET_ANSWER   answer;

    if(serial.isOpen() == false)
    {
        emit error(tr("Порт USB приемо-передатчика не открыт!"));
        return;
    }

    //---
    question.data.channel = sb_channel->value();
    question.data.address = sb_address->value();
    for(int n=0; n<MAX_LEN_DATA; n++)
    {
        question.data.test_data[n] = n;
    }

    answer.data.address = sb_address->value();
    for(int n=0; n<MAX_LEN_DATA; n++)
    {
        answer.data.test_data[n] = n;
    }
    //---

    QByteArray ba;
    ba.clear();
    for(unsigned int n=0; n<sizeof(PACKET_QUESTION); n++)
    {
        ba.append(question.buf[n]);
    }

    QByteArray temp;
    temp.clear();
    temp.append(':');
    temp.append(ba.toHex());
    temp.append((char)0x0D);

    //emit debug(temp.toUpper());

    data_multimeter.clear();
    is_ready = false;
    serial.write(temp);

    QTime time;
    time.start();
    wait_msec(sb_time_multimeter->value(), sizeof(PACKET_ANSWER));
    //emit debug(QString(tr("Прошло времени %1 ms")).arg(time.elapsed()));

    //emit info(QString("data [%1]").arg(data_multimeter.toHex().data()));
    QByteArray ba_answer;
    ba_answer.clear();
    for(unsigned int n=0; n<sizeof(PACKET_ANSWER); n++)
    {
        ba_answer.append(answer.buf[n]);
    }

    if(data_multimeter.isEmpty())
    {
        emit error(QString("empty %1").arg(QTime::currentTime().toString()));
        grapher->add_curve_data(0, 0);
    }
    else
    {
        if(data_multimeter == ba_answer)
        {
            //emit info("ok");
            grapher->add_curve_data(0, 255);
        }
        else
        {
            emit error(QString("fail %1 [%2]")
                       .arg(QTime::currentTime().toString())
                       .arg(data_multimeter.toHex().data()));
            grapher->add_curve_data(0, 127);
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::wait_msec(int timeout_msec, int max_len_data)
{
    QTime time;

    if(timeout_msec < 1)
        return;

    time.start();
    while(time.elapsed() < timeout_msec)
    {
        QCoreApplication::processEvents();
        if(data_multimeter.size() >= max_len_data)
        {
            return;
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
