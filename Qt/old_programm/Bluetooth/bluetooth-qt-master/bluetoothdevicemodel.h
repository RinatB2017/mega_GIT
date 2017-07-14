#ifndef BLUETOOTHDEVICEMODEL_H
#define BLUETOOTHDEVICEMODEL_H

#include <QAbstractListModel>
#include "bluemanager.h"
#include "blueadapter.h"
#include "bluetoothdevice.h"

class BluetoothDevicesModel : public QAbstractListModel
{
    Q_OBJECT;
	
	Q_PROPERTY(bool powered READ powered WRITE makePowered NOTIFY poweredChanged);
	Q_PROPERTY(bool discoverable READ discoverable WRITE makeDiscoverable NOTIFY discoverableChanged);
	Q_PROPERTY(int discoverableTimeout READ discoverableTimeout WRITE setDiscoverableTimeout NOTIFY discoverableTimeoutChanged);
	Q_PROPERTY(bool adapterPresent READ adapterPresent NOTIFY adapterChanged);
public:
	explicit BluetoothDevicesModel(QObject *parent = 0);

	bool adapterPresent() { return adapter != NULL; }

public slots:

	int columnCount(const QModelIndex &) const { return 1; }
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role) const;

	QString devicePath(QString name);

	QList<BluetoothDevice*> devices(){ return m_devices; }

	BluetoothDevice* device(QString path);
	void makePowered(bool poweredValue);
	bool powered();
	void makeDiscoverable(bool discoverableValue);
	bool discoverable();
	int discoverableTimeout();
	void setDiscoverableTimeout(int timeout);

private slots:
	void adapterAdded(QDBusObjectPath);
	void adapterRemoved(QDBusObjectPath);
	void deviceCreated(QDBusObjectPath);
	void deviceRemoved(QDBusObjectPath);

	void devicePropertyChanged(QString name, QVariant value);
	void adapterPropertyChanged(QString name, QDBusVariant value);

signals:
	void poweredChanged(bool powered);
	void discoverableTimeoutChanged(int timeout);
	void devicePaired(BluetoothDevice* device);
	void discoverableChanged(bool discoverable);
	void adapterChanged(bool adapterPresent);

private:
	OrgBluezManagerInterface *manager;
	OrgBluezAdapterInterface *adapter;
	QList<BluetoothDevice*> m_devices;
};

#endif // BLUETOOTHDEVICEMODEL_H
