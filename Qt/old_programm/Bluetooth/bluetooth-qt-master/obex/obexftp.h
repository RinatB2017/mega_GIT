#ifndef OBEXFTP_H
#define OBEXFTP_H

#include <QObject>
#include "obextransfer.h"
#include "obexsession.h"
#include "obexclient.h"
#include "obexagent.h"

class ObexFtp : public QObject
{
Q_OBJECT
public:

	enum State
	{
		Disconnected = 0,
		Connecting,
		Connected,
		Busy
	};

	explicit ObexFtp(QString device, QObject *parent = 0);

	bool isConnected();

public slots:

	void connect();
	void disconnect();
	void cancel();

	void transferFile(QString localfile, QString remotefile);

private slots:
	void finishedTransfer(QDBusPendingCallWatcher*);
	void finishedConnecting(QDBusPendingCallWatcher*);

	///for the agent adaptor:
	void Complete(const QDBusObjectPath &path);
	void Error(const QDBusObjectPath &path, const QString &error);
	void Progress(const QDBusObjectPath &path, qulonglong transferred);
	void Release();


signals:
	void transferProgress(quint64 done, quint64 total);
	void transferComplete();
	void error(QString);
	void connected();

private:
	QString m_device;
	OrgOpenobexSessionInterface* m_session;
	OrgOpenobexFileTransferInterface* m_ftp;
	OrgOpenobexClientInterface* m_client;
	ObexAgentAdaptor* agent;

};

#endif // OBEXFTP_H
