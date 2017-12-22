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

#ifndef BLUETOOTHBASEAGENT_H
#define BLUETOOTHBASEAGENT_H

#include <QObject>
#include <QDBusContext>
#include "bluedevice.h"

class BluetoothBaseAgent: public QObject, public QDBusContext
{
	Q_OBJECT
public:
	BluetoothBaseAgent(QString path="", QObject *parent=0);
public slots:
	virtual void authorize(OrgBluezDeviceInterface &device, QString);
	virtual void registerAgent();
	virtual void cancel()
	{
		sendErrorReply("org.bluez.Error.Canceled","The request was canceled");
	}
	virtual void confirmModeChange(QString);
	virtual void displayPasskey(OrgBluezDeviceInterface &device, uint key, uint entered);
	virtual void release();
	virtual void requestConfirmation(OrgBluezDeviceInterface &device, uint key);
	virtual uint requestPasskey(OrgBluezDeviceInterface &device);
	virtual QString requestPidCode(OrgBluezDeviceInterface &device);

private:
	bool requestAccepted;
	QString m_path;
};

#endif // BLUETOOTHBASEAGENT_H
