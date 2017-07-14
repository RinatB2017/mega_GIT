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

#include "agentadaptor.h"

AgentAdaptor::AgentAdaptor(BluetoothBaseAgent *parent) :
    QDBusAbstractAdaptor(parent), agent(parent)
{

}

AgentAdaptor::~AgentAdaptor()
{

}

void AgentAdaptor::Authorize(const QDBusObjectPath &deviceObject,
                             const QString &uuid)
{
    if (!agent)
    {
        return;
    }

    OrgBluezDeviceInterface device("org.bluez",
                                   deviceObject.path(),
                                   QDBusConnection::systemBus());

    agent->authorize(device, uuid);
}

void AgentAdaptor::Cancel()
{
    if(!agent)
    {
        return;
    }

    agent->cancel();
}

void AgentAdaptor::ConfirmModeChange(const QString &mode)
{
    if(!agent)
    {
        return;
    }

    agent->confirmModeChange(mode);
}

void AgentAdaptor::DisplayPasskey(const QDBusObjectPath &deviceObject,
                                  uint passkey,
                                  uint entered)
{
    if(!agent)
    {
        return;
    }

    OrgBluezDeviceInterface device("org.bluez",
                                   deviceObject.path(),
                                   QDBusConnection::systemBus());

    agent->displayPasskey(device, passkey, entered);
}

void AgentAdaptor::Release()
{
    if(!agent)
    {
        return;
    }

    agent->release();
}

void AgentAdaptor::RequestConfirmation(const QDBusObjectPath &deviceObject,
                                       uint passkey)
{
    if(!agent)
    {
        return;
    }

    OrgBluezDeviceInterface device("org.bluez",
                                   deviceObject.path(),
                                   QDBusConnection::systemBus());

    agent->requestConfirmation(device, passkey);
}

uint AgentAdaptor::RequestPasskey(const QDBusObjectPath &deviceObject)
{
    if(!agent)
    {
        return 0;
    }

    OrgBluezDeviceInterface device("org.bluez",
                                   deviceObject.path(),
                                   QDBusConnection::systemBus());

    return agent->requestPasskey(device);
}

QString AgentAdaptor::RequestPinCode(const QDBusObjectPath &deviceObject)
{
    if(!agent)
    {
        return QString();
    }

    OrgBluezDeviceInterface device("org.bluez", deviceObject.path(), QDBusConnection::systemBus());

    return agent->requestPidCode(device);
}
