#ifndef NEARBYDEVICESMODEL_H
#define NEARBYDEVICESMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QDBusObjectPath>

#include "bluemanager.h"
#include "blueadapter.h"
#include "asyncagent.h"
#include "bluetoothdevice.h"

namespace NearbyDevicesModelRoles
{
	enum
	{
		name = Qt::UserRole + 1,
		address,
		icon
	};
}

class NearbyItem: public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString name READ name)
	Q_PROPERTY(QString address READ address)
	Q_PROPERTY(QString icon READ icon)
	Q_PROPERTY(bool legacyPairing READ legacyPairing)
public:
	NearbyItem(QString name="", QString address="", QString icon="", bool legacy=false, QObject* parent = 0)
	:QObject(parent)
	{
	    m_name=name;
	    m_addy=address;
	    m_icon=icon;
	    m_legacy = legacy;
	}

	QString name() { return m_name; }
	QString address() { return m_addy; }
	QString icon() { return m_icon; }
	bool legacyPairing() { return m_legacy; }
private:
	QString m_name;
	QString m_addy;
	QString m_icon;
	bool m_legacy;
};

class NearbyDevicesModel : public QAbstractListModel
{
	Q_OBJECT
	Q_PROPERTY(BluetoothDevice* pairingDevice READ pairingDevice)
public:
	NearbyDevicesModel(QObject *parent = 0);

	BluetoothDevice* pairingDevice() { if(agent) return agent->device();  else return NULL; }

public slots:

	int columnCount(const QModelIndex &) const { return 1;}
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role) const;

	QString hwAddress(int index) { return devices[index]->address(); }
	QString alias(int index){ return devices[index]->name(); }
	void pair(QString hwaddy);
	void discover(bool start);
	void removeAll(bool);

	void replyRequestConfirmation(bool confirmed);
	void replyPasskey(uint passkey);
	void replyRequestPidCode(QString pidCode);

	void setAdapterProperty(QString name, QVariant value);

private slots:
	void adapterAdded(QDBusObjectPath);
	void adapterRemoved(QDBusObjectPath);
	void deviceCreated(QString hwaddy, QVariantMap properties);
	void deviceRemoved(QString hwaddy);

	void adapterPropertiesChangedSlot(QString n,QDBusVariant v);

signals:
	void requestConfirmation(QString device, uint code);
	void requestPasskey(QString device);
	void requestPidCode(QString device);
	void release();

	void nearbyDeviceFound(int index);
	void nearbyDeviceRemoved(int index);

	void adapterPropertiesChanged(QString name, QVariant value);

private:
	QList<NearbyItem*> devices;
	OrgBluezManagerInterface *manager;
	OrgBluezAdapterInterface *adapter;
	AsyncAgent* agent;	
};

#endif // NEARBYDEVICESMODEL_H
