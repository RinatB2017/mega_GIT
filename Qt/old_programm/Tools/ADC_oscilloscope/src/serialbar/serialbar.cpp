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
#include <QApplication>
#include <QRadioButton>
#include <QToolButton>
#include <QComboBox>
#include <QLabel>

#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#include "serialbar.hpp"
//--------------------------------------------------------------------------------
SerialBar::SerialBar(const QString &title, QWidget *parent) :
    QToolBar(parent)
{
    setWindowTitle(title);

    port_name = new QComboBox(this);
    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
        port_name->addItem(port.portName());
    }

    port_baud = new QComboBox(this);
    port_baud->addItem("4800");
    port_baud->addItem("9600");
    port_baud->addItem("19200");
    port_baud->addItem("28800");
    port_baud->addItem("38400");
    port_baud->addItem("57600");
    port_baud->addItem("76800");
    port_baud->addItem("115200");

    port_refresh = new QToolButton(this);
    port_refresh->setText(tr("refresh"));
    port_refresh->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));

    port_start = new QToolButton(this);
    port_start->setText(tr("start"));
    port_start->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));

    port_stop  = new QToolButton(this);
    port_stop->setText(tr("stop"));
    port_stop->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop));

    data_1_byte = new QRadioButton(tr("1 byte"), this);
    data_2_byte = new QRadioButton(tr("2 byte"), this);
    data_4_byte = new QRadioButton(tr("4 byte"), this);
    data_float  = new QRadioButton(tr("float"), this);

    data_2_byte->setChecked(true);

    addWidget(port_refresh);
    addWidget(new QLabel(tr("Порт:")));
    addWidget(port_name);
    addWidget(new QLabel(tr("Скорость:")));
    addWidget(port_baud);
    addWidget(port_start);
    addWidget(port_stop);
    addWidget(new QLabel(tr("Размер данных:")));
    addWidget(data_1_byte);
    addWidget(data_2_byte);
    addWidget(data_4_byte);
    addWidget(data_float);
}
//--------------------------------------------------------------------------------
