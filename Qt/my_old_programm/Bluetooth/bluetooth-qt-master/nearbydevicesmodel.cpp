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

#include "nearbydevicesmodel.h"
#include "bluetoothbaseagent.h"

NearbyDevicesModel::NearbyDevicesModel(QObject *parent) :
	QAbstractListModel(parent), adapter(NULL), agent(NULL)
{
	manager = new OrgBluezManagerInterface(
			"org.bluez",
			"/", QDBusConnection::systemBus(), this);

	connect(manager,SIGNAL(AdapterAdded(QDBusObjectPath)),this,SLOT(adapterAdded(QDBusObjectPath)));
	connect(manager,SIGNAL(AdapterRemoved(QDBusObjectPath)),this,SLOT(adapterRemoved(QDBusObjectPath)));
	adapterAdded(QDBusObjectPath());

	QHash<int,QByteArray> roles;
	QMetaObject properties = NearbyItem::staticMetaObject;
	for(int i=0; i<properties.propertyCount();i++)
	{
		roles[i]=properties.property(i).name();
	}
    //??? setRoleNames(roles);
}


int NearbyDevicesModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return devices.size();
}

QVariant NearbyDevicesModel::data(const QModelIndex &index, int role) const
{

	if(!index.isValid() || index.row() < 0)
	{
		qDebug()<<"invalid index"<<index.row();
		return QVariant();
	}

	QString roleName = roleNames()[role];
	QMetaObject object = NearbyItem::staticMetaObject;

	for(int i=0; i<object.propertyCount(); i++)
	{
		if(object.property(i).name() == roleName)
		{

			return object.property(i).read(devices[index.row()]);
		}
	}

	return QVariant();
}

void NearbyDevicesModel::pair(QString hwaddy)
{

	qDebug()<<"attempting to pair with "<<hwaddy;
	if(!adapter) return;
	if(!agent) agent = new AsyncAgent("/temp/agent", this);

	adapter->CreatePairedDevice(hwaddy,
				   QDBusObjectPath("/temp/agent"),"");

	//qDebug()<<"new object created: "<<object.path();
}

void NearbyDevicesModel::discover(bool start)
{
	if(!adapter) return;

	if(start)
		adapter->StartDiscovery();
	else adapter->StopDiscovery();
}

void NearbyDevicesModel::removeAll(bool)
{
	for(int i=0;i<devices.size();i++)
	{
		devices.removeAt(i);
	}
}

void NearbyDevicesModel::replyRequestConfirmation(bool confirmed)
{
	qDebug()<<"reply to RequestConfirmation:"<<confirmed;
	if(agent) agent->replyRequestConfirmation(confirmed);
}

void NearbyDevicesModel::replyPasskey(uint passkey)
{
	if(agent) agent->replyPasskey(passkey);
}

void NearbyDevicesModel::replyRequestPidCode(QString pidCode)
{
	if(agent) agent->replyRequestPidCode(pidCode);
}

void NearbyDevicesModel::setAdapterProperty(QString name, QVariant value)
{
    if(adapter) adapter->setProperty(name.toLatin1().data(),value);
}

void NearbyDevicesModel::deviceCreated(QString hwaddy, QVariantMap properties)
{
	bool found = false;
	foreach(NearbyItem* path, devices)
	{
		if(path->address() == hwaddy)
		{
			found=true;
			break;
		}
	}

	if(!found)
	{
		beginInsertRows(QModelIndex(), devices.size(), devices.size());

		NearbyItem* item = new NearbyItem(properties["Alias"].toString(),
			       hwaddy,properties["Icon"].toString(),properties["LegacyPairing"].toBool(),this);

		devices.append(item);
		emit nearbyDeviceFound(devices.indexOf(item));
		endInsertRows();
	}
}

void NearbyDevicesModel::deviceRemoved(QString hwaddy)
{
	foreach(NearbyItem* device, devices)
	{
		if(device->address() == hwaddy)
		{
			qDebug()<<"device "<<device->name()<<" has disappeared";
			int i=devices.indexOf(device);
			beginRemoveRows(QModelIndex(),i,i);
			devices.removeAt(i);
			emit nearbyDeviceRemoved(i);
			endRemoveRows();
		}
	}
}

void NearbyDevicesModel::adapterAdded(QDBusObjectPath path)
{
	if(adapter && adapter->path() == path.path()) return;

	QDBusObjectPath adapterpath = manager->DefaultAdapter();

	if(adapterpath.path() == "")
		return;

	adapter = new OrgBluezAdapterInterface(
			"org.bluez",
			adapterpath.path(),
			QDBusConnection::systemBus(), this);

	connect(adapter,
		SIGNAL(DeviceFound(QString, QVariantMap)),
		this,
		SLOT(deviceCreated(QString, QVariantMap)));
	connect(adapter,
		SIGNAL(DeviceDisappeared(QString)),
		this,
		SLOT(deviceRemoved(QString)));
	connect(adapter,
			SIGNAL(PropertyChanged(QString,QDBusVariant)),
			this,
			SLOT(adapterPropertiesChangedSlot(QString,QDBusVariant)));
}

void NearbyDevicesModel::adapterRemoved(QDBusObjectPath)
{
	QDBusObjectPath adapterpath = manager->DefaultAdapter();

	if(adapterpath.path() == "")
	{
		removeAll(true);
		if(adapter){ delete adapter; adapter = NULL; }
		return;
	}
}

void NearbyDevicesModel::adapterPropertiesChangedSlot(QString n,QDBusVariant v)
{
	adapterPropertiesChanged(n,v.variant());
}
