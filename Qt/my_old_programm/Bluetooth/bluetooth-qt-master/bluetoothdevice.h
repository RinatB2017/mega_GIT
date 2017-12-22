#ifndef BLUETOOTHDEVICE_H
#define BLUETOOTHDEVICE_H

#include <QObject>
#include <QStringList>
#include <bluedevice.h>

class OrgBluezAudioInterface;
class OrgBluezInputInterface;

class BluetoothDevice : public QObject
{
    Q_OBJECT	
	Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged);
	Q_PROPERTY(bool audioConnected READ audioConnected NOTIFY audioConnectedChanged)
	Q_PROPERTY(bool inputConnected READ inputConnected NOTIFY inputConnectedChanged)
	Q_PROPERTY(bool trusted READ trusted WRITE setTrusted NOTIFY trustedChanged)
	Q_PROPERTY(QStringList profiles READ profiles NOTIFY profilesChanged)
	Q_PROPERTY(QString alias READ alias)
	Q_PROPERTY(QString name READ name)
	Q_PROPERTY(QString address READ address)
	Q_PROPERTY(QString icon READ icon)
	Q_PROPERTY(QString path READ path)

public:
	explicit BluetoothDevice(QDBusObjectPath path = QDBusObjectPath(), QObject *parent = 0);

signals:
	void connectedChanged(bool isConnected);
	void audioConnectedChanged(bool isConnected);
	void inputConnectedChanged(bool isConnected);
	void propertyChanged(QString name, QVariant value);
	void profilesChanged(QStringList uuids);
	void trustedChanged(bool trust);

public slots:
	void unpair();
	void connectAudio();
	void connectAudioSrc();
	QString connectSerial();
	void connectInput();
	void disconnect();
	void disconnectAudio();
	bool trusted();
	void setTrusted(bool trust);

	QStringList profiles();
	bool isProfileSupported(QString profile);

	///properties:
	bool connected();
	bool audioConnected();
	bool inputConnected();
	QString alias();
	QString name();
	QString address();
	QString icon();
	QString path();

private slots:
	void propertyChanged(QString name,QDBusVariant value);
	void audioPropertiesChanged(QString name, QDBusVariant value);
	void inputPropertiesChanged(QString name, QDBusVariant value);

private:
	OrgBluezDeviceInterface *m_device;
	OrgBluezAudioInterface *audio;
	OrgBluezInputInterface *input;
};

#endif // BLUETOOTHDEVICE_H
