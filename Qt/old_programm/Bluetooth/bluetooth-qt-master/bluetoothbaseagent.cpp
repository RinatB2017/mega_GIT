/*  -*- Mode: C++ -*-
 *
 * meego handset bluetooth
 * Copyright © 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include "bluetoothbaseagent.h"

#include "agentadaptor.h"
#include "bluemanager.h"
#include "blueadapter.h"
#include <QTimer>

BluetoothBaseAgent::BluetoothBaseAgent(QString path, QObject *parent):QObject(parent),requestAccepted(false),m_path(path)
{
	new AgentAdaptor(this);
	QDBusConnection::systemBus().registerObject(path,this);
}

void BluetoothBaseAgent::registerAgent()
{
    OrgBluezManagerInterface manager(
                    "org.bluez",
                    "/", QDBusConnection::systemBus());

    QDBusObjectPath adapterpath = manager.DefaultAdapter();

    OrgBluezAdapterInterface adapter(
                    "org.bluez",
                    adapterpath.path(),
                    QDBusConnection::systemBus());

    adapter.RegisterAgent(QDBusObjectPath(m_path),"");
    qDebug()<<"last error: "<<adapter.lastError().message();
}

void BluetoothBaseAgent::authorize(OrgBluezDeviceInterface &device, QString uuid)
{
	Q_UNUSED(uuid);
	Q_UNUSED(device);
	qDebug("authorize");
}

void BluetoothBaseAgent::confirmModeChange(QString mode)
{
	qDebug()<<"mode changed "<<mode;
}

void BluetoothBaseAgent::displayPasskey(OrgBluezDeviceInterface &device, uint key, uint entered)
{
	qDebug()<<"display key "<<device.path()<<" "<<key;

	///create and return back an empty reply:
	QDBusMessage reply = message().createReply();
	connection().send(reply);

    Q_UNUSED(entered)
}

void BluetoothBaseAgent::release()
{
	qDebug("release called");
}

void BluetoothBaseAgent::requestConfirmation(OrgBluezDeviceInterface &device, uint key)
{
	QVariantMap props = device.GetProperties();

	QString alias = props["Alias"].toString();

	qDebug()<<"Pairing with "<< alias << " with key: "+QString::number(key);
}

uint BluetoothBaseAgent::requestPasskey(OrgBluezDeviceInterface &device)
{
	Q_UNUSED(device);
	qDebug("requestKey");
	return 0;
}

QString BluetoothBaseAgent::requestPidCode(OrgBluezDeviceInterface &device)
{
	Q_UNUSED(device);
	return "";
}

