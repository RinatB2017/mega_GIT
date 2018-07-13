/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Copyright (C) 2013 BlackBerry Limited. All rights reserved.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtBluetooth module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "service.h"

#include <qbluetoothaddress.h>
#include <qbluetoothservicediscoveryagent.h>
#include <qbluetoothserviceinfo.h>
#include <qbluetoothlocaldevice.h>
#include <qbluetoothdeviceinfo.h>
#include <qbluetoothuuid.h>

#include <qbluetoothsocket.h>


ServiceDiscoveryDialog::ServiceDiscoveryDialog(const QString &name,
                                               QWidget *parent)
    :   QDialog(parent), ui(new Ui_ServiceDiscovery)
{
    ui->setupUi(this);

    //Using default Bluetooth adapter
    QBluetoothLocalDevice localDevice;
    QBluetoothAddress adapterAddress = localDevice.address();

    /*
     * In case of multiple Bluetooth adapters it is possible to
     * set which adapter will be used by providing MAC Address.
     * Example code:
     *
     * QBluetoothAddress adapterAddress("XX:XX:XX:XX:XX:XX");
     * discoveryAgent = new QBluetoothServiceDiscoveryAgent(adapterAddress);
     */

    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(adapterAddress);
    setWindowTitle(name);
    connect(discoveryAgent, SIGNAL(finished()), this, SLOT(serviceDiscoverFinished()));
    discoveryAgent->start();
}

void ServiceDiscoveryDialog::serviceDiscoverFinished()
{
    ui->list->addItem("Device discover finished");

    QList<QBluetoothDeviceInfo> listOfDevices = discoveryAgent->discoveredDevices();

    if (listOfDevices.isEmpty())
    {
        ui->list->addItem("No devices found");
        return;
    }

    ui->list->addItem("Found new devices:");

    for (int i = 0; i < listOfDevices.size(); i++)
    {
        QString temp;
        temp.append(listOfDevices.at(i).name().trimmed());
        temp.append(" (");
        temp.append(listOfDevices.at(i).address().toString().trimmed());
        temp.append(")");
        ui->list->addItem(temp);

        socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
        socket->connectToService(QBluetoothAddress(listOfDevices.at(i).address()),
                                 QBluetoothUuid(QBluetoothUuid::SerialPort));
        connect(socket, SIGNAL(error(QBluetoothSocket::SocketError)),   this, SLOT(socketError(QBluetoothSocket::SocketError)));
        connect(socket, SIGNAL(connected()),    this, SLOT(socketConnected()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
        connect(socket, SIGNAL(readyRead()),    this, SLOT(socketRead()));
        ui->status->clear();

        return;
    }
    ui->status->clear();
}

//---
void ServiceDiscoveryDialog::socketError(QBluetoothSocket::SocketError error)
{
    switch(error)
    {
    case QBluetoothSocket::NoSocketError:               ui->list->addItem("NoSocketError");             break;
    case QBluetoothSocket::UnknownSocketError:          ui->list->addItem("UnknownSocketError");        break;
    case QBluetoothSocket::RemoteHostClosedError:       ui->list->addItem("RemoteHostClosedError");     break;
    case QBluetoothSocket::HostNotFoundError:           ui->list->addItem("HostNotFoundError");         break;
    case QBluetoothSocket::ServiceNotFoundError:        ui->list->addItem("ServiceNotFoundError");      break;
    case QBluetoothSocket::NetworkError:                ui->list->addItem("NetworkError");              break;
    case QBluetoothSocket::UnsupportedProtocolError:    ui->list->addItem("UnsupportedProtocolError");  break;
    case QBluetoothSocket::OperationError:              ui->list->addItem("OperationError");            break;
    default:
        ui->list->addItem("unknown error");
        break;
    }
}
void ServiceDiscoveryDialog::socketConnected(void)
{
    ui->list->addItem("socketConnected");
}
void ServiceDiscoveryDialog::socketDisconnected(void)
{
    ui->list->addItem("socketDisconnected");
}
void ServiceDiscoveryDialog::socketRead(void)
{
    ui->list->addItem("socketRead");
}
//---

ServiceDiscoveryDialog::~ServiceDiscoveryDialog()
{
    delete discoveryAgent;
    delete ui;
}

void ServiceDiscoveryDialog::addService(const QBluetoothDeviceInfo &info)
{
    if (info.name().isEmpty())
        return;

    QString line = QString("device name [%1]").arg(info.name());
    ui->list->addItem(line);
}
