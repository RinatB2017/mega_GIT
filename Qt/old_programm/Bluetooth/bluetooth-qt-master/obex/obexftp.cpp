#include "obexftp.h"

#include <QDebug>

ObexFtp::ObexFtp(QString device, QObject *parent) :
    QObject(parent), m_device(device), m_session(0), m_ftp(0), agent(0)
{
	m_client = new OrgOpenobexClientInterface("org.openobex.client", "/", QDBusConnection::sessionBus(), this);

}

bool ObexFtp::isConnected()
{
	if(!m_session) return false;

	QVariantMap props = m_session->GetProperties();
	foreach(QString key, props.keys())
	{
		qDebug()<<"session property key: "<<key<<" value: "<<props[key];
	}

	return !props["AgentPath"].toString().isEmpty();
}

void ObexFtp::connect()
{
	if(m_session) delete m_session;

	QVariantMap session;
	session["Destination"] = m_device;
	session["Target"] = "FTP";

	QDBusPendingCall async = m_client->CreateSession(session);

	QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(async, this);

	QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
			 this, SLOT(finishedConnecting(QDBusPendingCallWatcher*)));

}

void ObexFtp::disconnect()
{
	if(isConnected())
		m_session->Close();
}

void ObexFtp::cancel()
{

}

void ObexFtp::transferFile(QString localfile, QString remotefile)
{
	if(!isConnected())
	{
		qDebug()<<"must be connected first.";
		return;
	}

	QDBusPendingCall async = m_ftp->PutFile(localfile,remotefile);
	QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(async, this);

	QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
			 this, SLOT(finishedTransfer(QDBusPendingCallWatcher*)));
}

void ObexFtp::finishedTransfer(QDBusPendingCallWatcher* call)
{
	QDBusPendingReply<void> reply = *call;

	if(!reply.isValid())
	{
		QDBusError errors = reply.error();
		emit error(errors.name()+" : "+errors.message());
	}
}

void ObexFtp::finishedConnecting(QDBusPendingCallWatcher* call)
{
	QDBusPendingReply<QDBusObjectPath> reply = *call;

	if(!reply.isValid())
	{
		QDBusError errors = reply.error();
		emit error(errors.name()+" : "+errors.message());
	}

	QDBusObjectPath sessionPath = reply.value();

	qDebug()<< sessionPath.path();

	m_session = new OrgOpenobexSessionInterface("org.openobex.client", sessionPath.path(), QDBusConnection::sessionBus());

	if(agent) delete agent;
	agent = new ObexAgentAdaptor(this);

	QDBusConnection::sessionBus().registerObject(sessionPath.path(), agent);
	m_session->AssignAgent(sessionPath);

	if(m_ftp) m_ftp->deleteLater();

	m_ftp = new OrgOpenobexFileTransferInterface("org.openobex.client",
						     sessionPath.path(),QDBusConnection::sessionBus(),this);

	emit connected();
}

void ObexFtp::Complete(const QDBusObjectPath &path)
{
	emit transferComplete();
    Q_UNUSED(path)
}

void ObexFtp::Error(const QDBusObjectPath &path, const QString &errors)
{
	emit error(path.path()+" : "+errors);
}

void ObexFtp::Progress(const QDBusObjectPath &path, qulonglong transferred)
{
	OrgOpenobexTransferInterface transfer("org.openobex.client",path.path(),QDBusConnection::sessionBus());

	QVariantMap props = transfer.GetProperties();

	emit transferProgress(transferred, props["Size"].toULongLong());
}

void ObexFtp::Release()
{
	///FIXME: not sure what to do here
}
