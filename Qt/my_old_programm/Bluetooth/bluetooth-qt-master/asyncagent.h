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

#ifndef ASYNCAGENT_H
#define ASYNCAGENT_H

#include "bluetoothbaseagent.h"
#include "bluetoothdevice.h"

class AsyncAgent : public BluetoothBaseAgent
{
    Q_OBJECT
public:
	explicit AsyncAgent(QString path, QObject *parent = 0);

	BluetoothDevice* device() { return deviceToPair; }

	void requestConfirmation(OrgBluezDeviceInterface &device, uint key);
	uint requestPasskey(OrgBluezDeviceInterface &device);
	QString requestPidCode(OrgBluezDeviceInterface &device);
	void release();

signals:

public slots:
	void replyRequestConfirmation(bool confirmed);
	void replyPasskey(uint passkey);
	void replyRequestPidCode(QString pidCode);

private:
	QDBusMessage pendingMessage;
	QDBusConnection m_connection;

	BluetoothDevice* deviceToPair;

};

#endif // ASYNCAGENT_H
