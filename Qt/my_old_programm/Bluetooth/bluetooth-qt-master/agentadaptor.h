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



#ifndef AGENTADAPTOR_H
#define AGENTADAPTOR_H

#include <QDBusAbstractAdaptor>
#include "bluetoothbaseagent.h"

class AgentAdaptor : public QDBusAbstractAdaptor
{
Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "org.bluez.Agent");
public:
	AgentAdaptor(BluetoothBaseAgent *parent = NULL);
	virtual ~AgentAdaptor();

signals:

public slots:
	void Authorize(const QDBusObjectPath &device, const QString &uuid);
	void Cancel();
	void ConfirmModeChange(const QString &mode);
	void DisplayPasskey(const QDBusObjectPath &device, uint passkey, uint entered);
	void Release();
	void RequestConfirmation(const QDBusObjectPath &device, uint passkey);
	uint RequestPasskey(const QDBusObjectPath &device);
	QString RequestPinCode(const QDBusObjectPath &device);

private:
	BluetoothBaseAgent *agent;
};

#endif // AGENTADAPTOR_H
