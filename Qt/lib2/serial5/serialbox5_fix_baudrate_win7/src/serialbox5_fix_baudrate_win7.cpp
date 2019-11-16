/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#ifdef QT_DEBUG
#   include <QDebug>
#endif
#include <QThread>
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "serialbox5_fix_baudrate_win7.hpp"
#include "ui_serialbox5_fix_baudrate_win7.h"
//--------------------------------------------------------------------------------
#ifdef RS232_SEND
#   include "sendbox5.hpp"
#endif
//--------------------------------------------------------------------------------
#include "logbox.hpp"
//--------------------------------------------------------------------------------
//#define MAX_TIME_MSEC   100
//--------------------------------------------------------------------------------
typedef enum
{
    E_NO_ERROR = 0,
    E_PORT_NOT_INIT,
    E_PORT_NOT_OPEN
} ERRORS;
//--------------------------------------------------------------------------------
SerialBox5_fix_baudrate_win7::SerialBox5_fix_baudrate_win7(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::SerialBox5_fix_baudrate_win7),
    caption("no name"),
    flag_in_hex(false),
    flag_byte_by_byte(false)
{
    init();
}
//--------------------------------------------------------------------------------
SerialBox5_fix_baudrate_win7::SerialBox5_fix_baudrate_win7(QWidget *parent,
                                                           const QString &caption,
                                                           const QString &o_name) :
    MyWidget(parent),
    ui(new Ui::SerialBox5_fix_baudrate_win7),
    caption(caption),
    o_name(o_name),
    flag_in_hex(false),
    flag_byte_by_byte(false)
{
    init();
}
//--------------------------------------------------------------------------------
SerialBox5_fix_baudrate_win7::~SerialBox5_fix_baudrate_win7()
{
#ifdef RS232_SEND
    if(sendBox5)
    {
        sendBox5->disconnect();
        sendBox5->close();
        sendBox5->deleteLater();
    }
#endif
    delete ui;
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::set_caption(QString value)
{
    caption = value;
    o_name = value;
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::set_fix_baudrate(int value)
{
    emit port_set_baudrate(value);
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::write(const char *data)
{
    emit port_write(data);
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::init(void)
{
    ui->setupUi(this);

    createWidgets();
    initThread();
    setCloseState();

    //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    //setFixedWidth(sizeHint().width());
    setFixedHeight(sizeHint().height());
    updateText();
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::createWidgets(void)
{
    ui->gridLayout->setMargin(0);
    ui->gridLayout->setSpacing(0);

    ui->PortBox->setProperty(NO_BLOCK, true);
    ui->PortBox->setMinimumWidth(150);

    ui->btn_power->setProperty(NO_BLOCK, true);
    ui->btn_refresh->setProperty(NO_BLOCK, true);
    ui->btn_power->setIcon(QIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay)));

    ui->btn_refresh->setToolTip("Обновить список портов");

    connect(ui->btn_power,      SIGNAL(clicked(bool)),  this,   SLOT(btnOpenPortClicked()));
    connect(ui->btn_refresh,    SIGNAL(clicked(bool)),  this,   SLOT(refresh()));

    connect(this, SIGNAL(output(QByteArray)), this, SLOT(drawData(QByteArray)));

#ifdef RS232_SEND
    sendBox5 = new SendBox5(this);
    connect(sendBox5, SIGNAL(sendData(QByteArray)), this, SLOT(sendData(QByteArray)));
    ui->layout_SEND->addWidget(sendBox5);
#endif
}
//--------------------------------------------------------------------------------
#ifndef RS232_NO_FRAME
void SerialBox5_fix_baudrate_win7::add_frame_text(QFrame *parent,
                                                  const QString &text)
{
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->setMargin(0);
    hbox->setSpacing(0);
    QLabel *label = new QLabel(text);
    label->setAlignment(Qt::AlignHCenter);
    hbox->addWidget(label);
    parent->setLayout(hbox);
    parent->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
}
#endif
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::refresh(void)
{
    ui->PortBox->clear();
    foreach (QSerialPortInfo p_info, QSerialPortInfo::availablePorts())
    {
        ui->PortBox->addItem(p_info.portName());
    }
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::thread_is_finished(void)
{
    emit info("thread_is_finished");
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::change_icon(bool state)
{
    if(state)
    {
        ui->btn_power->setIcon(QIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop)));
    }
    else
    {
        ui->btn_power->setIcon(QIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay)));
    }
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::getStatus(const QString &status, QDateTime current)
{
    emit info(QString("%1 %2")
              .arg(status)
              .arg(current.toString()));

    get_parameter();
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::setCloseState(void)
{
    //ui->btn_power->setIcon(QIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop)));

    ui->btn_refresh->setEnabled(true);
    ui->PortBox->setEnabled(true);
    ui->btn_power->setChecked(false);
    emit state(false);
#ifdef RS232_SEND
    sendBox5->block_interface(true);
#endif
    ui->btn_power->setToolTip("Старт");
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::setOpenState()
{
    //ui->btn_power->setIcon(QIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay)));

    ui->btn_refresh->setEnabled(false);
    ui->PortBox->setEnabled(false);
    ui->btn_power->setChecked(true);
    emit state(true);
#ifdef RS232_SEND
    sendBox5->block_interface(false);
#endif
    ui->btn_power->setToolTip("Стоп");
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::btnOpenPortClicked()
{
    // emit trace(Q_FUNC_INFO);

    if(is_open)
    {
        emit port_close();
    }
    else
    {
        emit port_set_name(ui->PortBox->currentText());
        emit port_open();
        emit port_set_baudrate(fix_baudrate);
        emit port_set_bits(QSerialPort::Data8);
        emit port_set_parity(QSerialPort::NoParity);
        emit port_set_stop_bits(QSerialPort::OneStop);
        emit port_set_flow_control(QSerialPort::NoFlowControl);
    }
}
//--------------------------------------------------------------------------------
int SerialBox5_fix_baudrate_win7::input(const QByteArray &sending_data)
{
    if(flag_byte_by_byte)
    {
        emit debug("flag_byte_by_byte");
        for(int n=0; n<sending_data.length(); n++)
            emit port_write(sending_data.constData()+n, 1);
    }
    else
    {
        QString temp = sending_data;
        bool containsNonASCII = temp.contains(QRegularExpression(QStringLiteral("[^\\x{0000}-\\x{007F}]")));
        if(containsNonASCII == false)
        {
            if(temp.isEmpty() == false)
            {
                QString x_str = temp.remove('\r').remove('\n');
                //emit debug(QString("send: %1").arg(x_str));
            }
        }
        else
        {
            emit debug(QString("send [%1]").arg(sending_data.toHex().toUpper().data()));
        }
        emit port_write(sending_data);
    }
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int SerialBox5_fix_baudrate_win7::input(const QString &data)
{
    QByteArray sending_data;
    sending_data.clear();
    sending_data.append(data);
    if(flag_byte_by_byte)
    {
        for(int n=0; n<sending_data.length(); n++)
        {
            emit port_write(sending_data.constData()+n, 1);
        }
    }
    else
    {
        emit port_write(sending_data);
    }
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::procSerialDataReceive(void)
{
//    //TODO было
//    // emit output(serial5->readAll());

    emit output(serial_data);

//    if (!timer->isActive())
//        timer->singleShot(MAX_TIME_MSEC, this, SLOT(timer_stop()));
//    else
//        timer->stop();
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::timer_stop(void)
{
    emit output(serial_data);
}
//--------------------------------------------------------------------------------
QString SerialBox5_fix_baudrate_win7::ByteArrayToHex(const QByteArray &data)
{
    return data.toHex();
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::sendData(const QByteArray &sending_data)
{
    if(flag_byte_by_byte)
    {
        for(int n=0; n<sending_data.length(); n++)
        {
            emit port_write(sending_data.constData()+n, 1);
        }
    }
    else
    {
        emit port_write(sending_data);
    }
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::drawData(const QByteArray &data)
{
    // qDebug() << "drawData[" << data << "]";
#ifdef RS232_LOG
    if(flag_in_hex)
    {
        emit info(ByteArrayToHex(data));
    }
    else
    {
        emit info(data.data());
    }
    if(flag_in_hex)
    {
        emit info(ByteArrayToHex(data));
    }
    else
    {
        //emit info(data);
    }
#else
    Q_UNUSED(data)
#endif
}
//--------------------------------------------------------------------------------
bool SerialBox5_fix_baudrate_win7::isOpen(void)
{
    return is_open;
}
//--------------------------------------------------------------------------------
bool SerialBox5_fix_baudrate_win7::add_menu(int index)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    QMenu *menu = new QMenu(caption);
    Q_CHECK_PTR(menu);

    QAction *action_flag_in_hex = new QAction(menu);
    QAction *action_flag_byte_by_byte = new QAction(menu);

    action_flag_in_hex->setCheckable(true);
    action_flag_byte_by_byte->setCheckable(true);

    action_flag_in_hex->setText("in HEX");
    action_flag_byte_by_byte->setText("byte to byte");

    menu->addAction(action_flag_in_hex);
    menu->addAction(action_flag_byte_by_byte);

    connect(action_flag_in_hex,         SIGNAL(triggered(bool)),    this,   SLOT(set_flag_in_hex(bool)));
    connect(action_flag_byte_by_byte,   SIGNAL(triggered(bool)),    this,   SLOT(set_flag_byte_by_byte(bool)));

    //---
    mw->add_optionsmenu_menu(index, menu);
    //---

    return false;
}
//--------------------------------------------------------------------------------
bool SerialBox5_fix_baudrate_win7::add_menu(int index, const QString &title)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    QMenu *menu = new QMenu(title);
    Q_CHECK_PTR(menu);

    QAction *action_flag_in_hex = new QAction(menu);
    QAction *action_flag_byte_by_byte = new QAction(menu);

    action_flag_in_hex->setCheckable(true);
    action_flag_byte_by_byte->setCheckable(true);

    action_flag_in_hex->setText("in HEX");
    action_flag_byte_by_byte->setText("byte to byte");

    menu->addAction(action_flag_in_hex);
    menu->addAction(action_flag_byte_by_byte);

    connect(action_flag_in_hex, SIGNAL(triggered(bool)), this, SLOT(set_flag_in_hex(bool)));
    connect(action_flag_byte_by_byte, SIGNAL(triggered(bool)), this, SLOT(set_flag_byte_by_byte(bool)));

    mw->add_menu(index, menu);

    return true;
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::set_flag_in_hex(bool state)
{
    emit debug(QString("state is %1").arg(state ? "true" : "false"));
    flag_in_hex = state;
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::set_flag_byte_by_byte(bool state)
{
    emit debug(QString("state is %1").arg(state ? "true" : "false"));
    flag_byte_by_byte = state;
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::get_parameter(void)
{
//    QString temp;
//    temp.clear();
//#if 1
//    emit info(QString("baudRate %1").arg(worker->baudRate()));
//    emit info(QString("dataBits %1").arg(worker->dataBits()));
//    emit info(QString("parity %1").arg(worker->parity()));
//    emit info(QString("stopBits %1").arg(worker->stopBits()));
//    emit info(QString("flowControl %1").arg(worker->flowControl()));
//#else
//    temp = QString("%1, %2, %3, %4, %5")
//            .arg(serial5->baudRate())
//            .arg(serial5->dataBits())
//            .arg(serial5->parity())
//            .arg(serial5->stopBits())
//            .arg(serial5->flowControl());
//#endif
//    emit info(temp);
}
//--------------------------------------------------------------------------------
QByteArray SerialBox5_fix_baudrate_win7::readAll(void)
{
    return serial_data;
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::initThread(void)
{
    emit info("thread_is_started");

    QThread *thread = new QThread;
    SerialBox5_thread *worker = new SerialBox5_thread;
    worker->moveToThread(thread);

    connect(worker, SIGNAL(info(QString)),      this, SIGNAL(info(QString)));
    connect(worker, SIGNAL(debug(QString)),     this, SIGNAL(debug(QString)));
    connect(worker, SIGNAL(error(QString)),     this, SIGNAL(error(QString)));
    connect(worker, SIGNAL(trace(QString)),     this, SIGNAL(trace(QString)));

    connect(this,   SIGNAL(port_open()),                                        worker, SLOT(port_open()),                                      Qt::DirectConnection);
    connect(this,   SIGNAL(port_close()),                                       worker, SLOT(port_close()),                                     Qt::DirectConnection);
    connect(this,   SIGNAL(port_set_name(QString)),                             worker, SLOT(port_set_name(QString)),                           Qt::DirectConnection);
    connect(this,   SIGNAL(port_set_baudrate(qint32)),                          worker, SLOT(port_set_baudrate(qint32)),                        Qt::DirectConnection);
    connect(this,   SIGNAL(port_set_bits(QSerialPort::DataBits)),               worker, SLOT(port_set_bits(QSerialPort::DataBits)),             Qt::DirectConnection);
    connect(this,   SIGNAL(port_set_stop_bits(QSerialPort::StopBits)),          worker, SLOT(port_set_stop_bits(QSerialPort::StopBits)),        Qt::DirectConnection);
    connect(this,   SIGNAL(port_set_parity(QSerialPort::Parity)),               worker, SLOT(port_set_parity(QSerialPort::Parity)),             Qt::DirectConnection);
    connect(this,   SIGNAL(port_set_flow_control(QSerialPort::FlowControl)),    worker, SLOT(port_set_flow_control(QSerialPort::FlowControl)),  Qt::DirectConnection);
    connect(this,   SIGNAL(port_write(const char *)),                           worker, SLOT(port_write(const char*)),                          Qt::DirectConnection);
    connect(this,   SIGNAL(port_write(const char *, qint64)),                   worker, SLOT(port_write(const char *, qint64)),                 Qt::DirectConnection);

    connect(worker, SIGNAL(port_bytes_avialable()),                             this,   SLOT(port_bytes_avialable(void)));
    connect(worker, SIGNAL(port_get_state(bool)),                               this,   SLOT(port_get_state(bool)));
    connect(worker, SIGNAL(port_get_name(QString)),                             this,   SLOT(port_get_name(QString)));
    connect(worker, SIGNAL(port_get_baudrate(qint32)),                          this,   SLOT(port_get_baudrate(qint32)));
    connect(worker, SIGNAL(port_get_bits(QSerialPort::DataBits)),               this,   SLOT(port_getbits(QSerialPort::DataBits)));
    connect(worker, SIGNAL(port_get_stop_bits(QSerialPort::StopBits)),          this,   SLOT(port_get_stop_bits(QSerialPort::StopBits)));
    connect(worker, SIGNAL(port_get_patity(QSerialPort::Parity)),               this,   SLOT(port_get_patity(QSerialPort::Parity)));
    connect(worker, SIGNAL(port_get_flow_control(QSerialPort::FlowControl)),    this,   SLOT(port_get_flow_control(QSerialPort::FlowControl)));
    connect(worker, SIGNAL(port_read_all(QByteArray)),                          this,   SLOT(port_read_all(QByteArray)));
    connect(worker, SIGNAL(port_ready_read(bool)),                              this,   SLOT(port_ready_read(bool)));
    connect(worker, SIGNAL(port_error(QSerialPort::SerialPortError)),           this,   SLOT(port_error(QSerialPort::SerialPortError)));

    connect(thread, SIGNAL(started()),  worker, SLOT(process()));

    connect(worker, SIGNAL(readyRead()),            this,   SIGNAL(readyRead()));
    connect(worker, SIGNAL(readChannelFinished()),  this,   SIGNAL(readChannelFinished()));

    connect(worker, SIGNAL(readyRead()), this, SLOT(procSerialDataReceive()));
//    connect(worker, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(serial5_error(QSerialPort::SerialPortError)));

    connect(thread, SIGNAL(finished()), this, SLOT(thread_is_finished()));

    thread->start();

    connect(ui->btn_power,  SIGNAL(toggled(bool)),  this,   SLOT(change_icon(bool)));

    refresh();
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::port_bytes_avialable(void)
{

}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::port_get_state(bool state)
{
    is_open = state;
    emit debug(QString("State: %1").arg(state ? "true" : "false"));

    if(is_open)
    {
        setOpenState();
    }
    else
    {
        setCloseState();
    }
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::port_get_name(QString port_name)
{
    emit info(QString("Name: %1").arg(port_name));
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::port_get_baudrate(qint32 value)
{
    emit info(QString("BaudRate: %1").arg(value));
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::port_getbits(QSerialPort::DataBits value)
{
    emit info(QString("DataBits: %1").arg(value));
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::port_get_stop_bits(QSerialPort::StopBits value)
{
    emit info(QString("StopBits: %1").arg(value));
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::port_get_patity(QSerialPort::Parity value)
{
    emit info(QString("Parity: %1").arg(value));
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::port_get_flow_control(QSerialPort::FlowControl value)
{
    emit info(QString("FlowControl: %1").arg(value));
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::port_read_all(QByteArray ba)
{
    // emit debug(QString("ReadAll: [%1]").arg(ba.data()));
    serial_data = ba;
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::port_ready_read(bool state)
{
    emit info(QString("ReadRead: %1").arg(state ? "true" : "false"));
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::port_error(QSerialPort::SerialPortError err)
{
    switch(err)
    {
    case QSerialPort::NoError:                      emit error("NoError");                      break;
    case QSerialPort::DeviceNotFoundError:          emit error("DeviceNotFoundError");          break;
    case QSerialPort::PermissionError:              emit error("PermissionError");              break;
    case QSerialPort::OpenError:                    emit error("OpenError");                    break;
    case QSerialPort::ParityError:                  emit error("ParityError");                  break;
    case QSerialPort::FramingError:                 emit error("FramingError");                 break;
    case QSerialPort::BreakConditionError:          emit error("BreakConditionError");          break;
    case QSerialPort::WriteError:                   emit error("WriteError");                   break;
    case QSerialPort::ReadError:                    emit error("ReadError");                    break;
    case QSerialPort::ResourceError:                emit error("ResourceError");                break;
    case QSerialPort::UnsupportedOperationError:    emit error("UnsupportedOperationError");    break;
    case QSerialPort::UnknownError:                 emit error("UnknownError");                 break;
    case QSerialPort::TimeoutError:                 emit error("TimeoutError");                 break;
    case QSerialPort::NotOpenError:                 emit error("NotOpenError");                 break;
    }
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::updateText(void)
{
    ui->retranslateUi(this);

#ifdef RS232_SEND
    sendBox5->updateText();
#endif
}
//--------------------------------------------------------------------------------
bool SerialBox5_fix_baudrate_win7::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate_win7::save_setting(void)
{

}
//--------------------------------------------------------------------------------
