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

#include "bluetoothdevice.h"
#include "blueadapter.h"
#include "bluemanager.h"
#include "btprofiles.h"
#include "audiosink.h"
#include "audiosource.h"
#include "headset.h"
#include "audio.h"
#include "input.h"

BluetoothDevice::BluetoothDevice(QDBusObjectPath path, QObject *parent) :
    QObject(parent),
    m_device(new OrgBluezDeviceInterface("org.bluez",path.path(),QDBusConnection::systemBus(),this)),
    audio(NULL),
    input(NULL)
{
    if(!m_device->isValid())
        return;
    QObject::connect(m_device,SIGNAL(PropertyChanged(QString,QDBusVariant)),
                     this,SLOT(propertyChanged(QString,QDBusVariant)));
}

void BluetoothDevice::unpair()
{
    OrgBluezManagerInterface manager(
                "org.bluez",
                "/",
                QDBusConnection::systemBus());

    QDBusObjectPath adapterpath = manager.DefaultAdapter();

    if(adapterpath.path().isEmpty()) return;

    OrgBluezAdapterInterface adapter(
                "org.bluez",
                adapterpath.path(),
                QDBusConnection::systemBus());

    adapter.RemoveDevice(QDBusObjectPath(m_device->path()));
}

void BluetoothDevice::connectAudio()
{
    if(isProfileSupported(BluetoothProfiles::a2sink) || isProfileSupported(BluetoothProfiles::hs))
    {
        if(!audio) audio = new OrgBluezAudioInterface("org.bluez", m_device->path(), QDBusConnection::systemBus(), this);

        audio->Connect();

        connect(audio,SIGNAL(PropertyChanged(QString,QDBusVariant)),this,SLOT(audioPropertiesChanged(QString,QDBusVariant)));
    }
}
void BluetoothDevice::connectAudioSrc()
{

    if(isProfileSupported(BluetoothProfiles::a2src))
    {
        OrgBluezAudioSourceInterface source("org.bluez",
                                            m_device->path(),
                                            QDBusConnection::systemBus());
        source.Connect();
    }
}

QString BluetoothDevice::connectSerial()
{
    if(isProfileSupported(BluetoothProfiles::spp))
    {
        QDBusInterface interface("org.bluez",m_device->path(),"org.bluez.Serial",QDBusConnection::systemBus());
        QDBusReply<QString> reply = interface.call(QDBus::AutoDetect, "Connect","spp");

        if(reply.isValid()) return reply;
        else qDebug()<<"Error connecting spp profile: "<<reply.error().message();
    }

    return "";
}

void BluetoothDevice::connectInput()
{
    if(isProfileSupported(BluetoothProfiles::hid))
    {
        if(!input)
        {
            input = new OrgBluezInputInterface("org.bluez",
                                               m_device->path(),
                                               QDBusConnection::systemBus(), this);
            connect(input,SIGNAL(PropertyChanged(QString,QDBusVariant)),this,
                    SLOT(inputPropertiesChanged(QString,QDBusVariant)));
        }
        input->Connect();
    }
}

void BluetoothDevice::disconnect()
{
    m_device->Disconnect();
}

void BluetoothDevice::disconnectAudio()
{
    if(!audio) audio = new OrgBluezAudioInterface("org.bluez", m_device->path(), QDBusConnection::systemBus(), this);

    audio->Disconnect();
}

QStringList BluetoothDevice::profiles()
{
    QVariantMap props = m_device->GetProperties();

    QStringList uuidlist = props["UUIDs"].toStringList();

    return uuidlist;
}

bool BluetoothDevice::isProfileSupported(QString profile)
{
    QVariantMap props = m_device->GetProperties();

    QStringList uuidlist = props["UUIDs"].toStringList();

    return uuidlist.contains(profile.toLower());
}

bool BluetoothDevice::connected()
{
    QVariantMap props = m_device->GetProperties();
    return props["Connected"].toBool();
}

bool BluetoothDevice::audioConnected()
{
    if(!audio)
    {
        audio = new OrgBluezAudioInterface("org.bluez",m_device->path(), QDBusConnection::systemBus(),this);
        connect(audio,SIGNAL(PropertyChanged(QString,QDBusVariant)),this,SLOT(audioPropertiesChanged(QString,QDBusVariant)));
    }

    QVariantMap props = audio->GetProperties();
    return props["State"].toString() == "connected";
}

bool BluetoothDevice::inputConnected()
{
    if(!input)
    {
        input = new OrgBluezInputInterface("org.bluez",m_device->path(), QDBusConnection::systemBus(),this);
        connect(input,SIGNAL(PropertyChanged(QString,QDBusVariant)),this, SLOT(inputPropertiesChanged(QString,QDBusVariant)));
    }

    QVariantMap props = input->GetProperties();
    return props["Connected"].toBool();
}

QString BluetoothDevice::alias()
{
    QVariantMap props = m_device->GetProperties();
    return props["Alias"].toString();
}

QString BluetoothDevice::name()
{
    QVariantMap props = m_device->GetProperties();
    return props["Name"].toString();
}

QString BluetoothDevice::address()
{
    QVariantMap props = m_device->GetProperties();
    return props["Address"].toString();
}

QString BluetoothDevice::icon()
{
    QVariantMap props = m_device->GetProperties();
    return props["Icon"].toString();
}

QString BluetoothDevice::path()
{
    return m_device->path();
}

bool BluetoothDevice::trusted()
{
    QVariantMap props = m_device->GetProperties();
    return props["Trusted"].toBool();
}

void BluetoothDevice::setTrusted(bool trust)
{
    m_device->SetProperty("Trusted",QDBusVariant(trust));
}

void BluetoothDevice::propertyChanged(QString name,QDBusVariant value)
{
    qDebug()<<"BluetoothDevice::propertyChanged()";

    if(name == "Connected")
    {
        emit connectedChanged(value.variant().toBool());
    }

    if(name == "UUIDs")
    {
        emit profilesChanged(value.variant().toStringList());
    }

    if(name == "Trusted")
    {
        emit trustedChanged(value.variant().toBool());
    }
    emit propertyChanged(name,value.variant());

    ///TODO: create individual signals for each property
}

void BluetoothDevice::audioPropertiesChanged(QString name,QDBusVariant value)
{
    ///don't think i need to be doing this:
    //emit propertyChanged(name, value.variant());

    if(name == "State")
    {
        emit audioConnectedChanged(value.variant().toString() == "connected");
    }
}

void BluetoothDevice::inputPropertiesChanged(QString name, QDBusVariant value)
{
    qDebug()<<"Input properties changed: "<<name<<" "<<value.variant();
    if(name == "Connected")
    {
        emit inputConnectedChanged(value.variant().toBool());
    }
}
