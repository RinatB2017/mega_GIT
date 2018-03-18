/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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
#include <QtWidgets>
//--------------------------------------------------------------------------------
#include "modbusasciiwidget.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
ModbusAsciiWidget::ModbusAsciiWidget(QWidget *parent) :
    MyWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
void ModbusAsciiWidget::init(void)
{
    clean_data.clear();

    serial = new QSerialPort(this);
    connect(serial, SIGNAL(readyRead()), this, SLOT(port_read()));
    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(port_error(QSerialPort::SerialPortError)));

    port_name = new QComboBox(this);
    refresh();

    port_baud = new QComboBox(this);
    port_baud->addItem(QString("%1").arg(QSerialPort::Baud1200), Qt::UserRole + QSerialPort::Baud1200);
    port_baud->addItem(QString("%1").arg(QSerialPort::Baud2400), Qt::UserRole + QSerialPort::Baud1200);
    port_baud->addItem(QString("%1").arg(QSerialPort::Baud4800), Qt::UserRole + QSerialPort::Baud4800);
    port_baud->addItem(QString("%1").arg(QSerialPort::Baud9600), Qt::UserRole + QSerialPort::Baud9600);
    port_baud->addItem(QString("%1").arg(QSerialPort::Baud19200), Qt::UserRole + QSerialPort::Baud19200);
    port_baud->addItem(QString("%1").arg(QSerialPort::Baud38400), Qt::UserRole + QSerialPort::Baud38400);
    port_baud->addItem(QString("%1").arg(QSerialPort::Baud57600), Qt::UserRole + QSerialPort::Baud57600);
    port_baud->addItem(QString("%1").arg(QSerialPort::Baud115200), Qt::UserRole + QSerialPort::Baud115200);
    port_baud->setCurrentText(QString("%1").arg(QSerialPort::Baud9600));

    port_databits = new QComboBox(this);
    port_databits->addItem(QString("5 bit"), Qt::UserRole + QSerialPort::Data5);
    port_databits->addItem(QString("6 bit"), Qt::UserRole + QSerialPort::Data6);
    port_databits->addItem(QString("7 bit"), Qt::UserRole + QSerialPort::Data7);
    port_databits->addItem(QString("8 bit"), Qt::UserRole + QSerialPort::Data8);
    port_databits->setCurrentIndex(3);

    port_parity = new QComboBox(this);
    port_parity->addItem(QString("NoParity"), Qt::UserRole + QSerialPort::NoParity);
    port_parity->addItem(QString("EvenParity"), Qt::UserRole + QSerialPort::EvenParity);
    port_parity->addItem(QString("OddParity"), Qt::UserRole + QSerialPort::OddParity);
    port_parity->addItem(QString("SpaceParity"), Qt::UserRole + QSerialPort::SpaceParity);
    port_parity->addItem(QString("MarkParity"), Qt::UserRole + QSerialPort::MarkParity);

    port_stopbits = new QComboBox(this);
    port_stopbits->addItem(QString("OneStop"), Qt::UserRole + QSerialPort::OneStop);
    port_stopbits->addItem(QString("OneAndHalfStop"), Qt::UserRole + QSerialPort::OneAndHalfStop);
    port_stopbits->addItem(QString("TwoStop"), Qt::UserRole + QSerialPort::TwoStop);

    port_flowcontrol = new QComboBox(this);
    port_flowcontrol->addItem(QString("NoFlow"), Qt::UserRole + QSerialPort::NoFlowControl);
    port_flowcontrol->addItem(QString("Hardware"), Qt::UserRole + QSerialPort::HardwareControl);
    port_flowcontrol->addItem(QString("Software"), Qt::UserRole + QSerialPort::SoftwareControl);

    btn_refresh = new QToolButton(this);
    btn_refresh->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));

    btn_start = new QToolButton(this);
    btn_start->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));

    btn_stop = new QToolButton(this);
    btn_stop->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop));

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(new QLabel(tr("Порт:")));
    hbox->addWidget(port_name);
    hbox->addWidget(port_baud);
    hbox->addWidget(port_databits);
    hbox->addWidget(port_parity);
    hbox->addWidget(port_stopbits);
    hbox->addWidget(port_flowcontrol);

    hbox->addWidget(btn_refresh);
    hbox->addWidget(btn_start);
    hbox->addWidget(btn_stop);

    connect(btn_refresh, SIGNAL(clicked()), this, SLOT(refresh()));
    connect(btn_start, SIGNAL(clicked()), this, SLOT(open()));
    connect(btn_stop, SIGNAL(clicked()), this, SLOT(close()));

    setLayout(hbox);
}
//--------------------------------------------------------------------------------
void ModbusAsciiWidget::port_read(void)
{
    QByteArray ba;
    ba = serial->readAll();
    for(int n=0; n<ba.count(); n++)
    {
        char temp = ba.at(n);
        switch(temp)
        {
        case ':':
            clean_data.clear();
            break;

        case 0x0A:
            processing();
            break;

        default:
            clean_data.append(temp);
            break;
        }
    }
}
//--------------------------------------------------------------------------------
bool ModbusAsciiWidget::processing(void)
{
    if((clean_data.count() % 8) == 0)
    {
        emit get_data(clean_data);
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
void ModbusAsciiWidget::port_error(QSerialPort::SerialPortError serial_error)
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
void ModbusAsciiWidget::refresh(void)
{
    port_name->clear();
    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
        port_name->addItem(port.portName());
    }
}
//--------------------------------------------------------------------------------
bool ModbusAsciiWidget::open(void)
{
    if(serial->isOpen())
    {
        emit info(tr("port is open"));
        return true;
    }
    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
        if(port_name->currentText() == port.portName())
        {
            serial->setPort(port);
            int speed = 9600;
            bool ok = serial->setBaudRate(speed);
            if(ok)
            {
                if(serial->open(QIODevice::ReadWrite))
                {
                    return true;
                }
            }
            return false;
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
void ModbusAsciiWidget::close(void)
{
    emit info("close");
    serial->close();
}
//--------------------------------------------------------------------------------
void ModbusAsciiWidget::updateText(void)
{

}
//--------------------------------------------------------------------------------
