#include "simpletcpserver.h"
#include <QDebug>

SimpleRemoteConsole::SimpleRemoteConsole(quint16 port, QObject *parent)
    : QObject(parent), m_port(port), m_tcpServer(nullptr) {}

SimpleRemoteConsole::~SimpleRemoteConsole() {
    stopServer();
}

bool SimpleRemoteConsole::startServer() {
    if (m_tcpServer) return true;

    m_tcpServer = new QTcpServer(this);
    connect(m_tcpServer, &QTcpServer::newConnection, this, &SimpleRemoteConsole::onNewConnection);

    if (!m_tcpServer->listen(QHostAddress::Any, m_port)) {
        qCritical() << "Не удалось открыть TCP порт:" << m_tcpServer->errorString();
        delete m_tcpServer;
        m_tcpServer = nullptr;
        return false;
    }

    qInfo() << "Удаленная консоль запущена на порту" << m_port;
    return true;
}

void SimpleRemoteConsole::stopServer() {
    if (!m_tcpServer) return;

    m_tcpServer->close();
    delete m_tcpServer;
    m_tcpServer = nullptr;

    for (auto it = m_clients.begin(); it != m_clients.end(); ++it) {
        QTcpSocket* socket = it.key();
        QProcess* process = it.value();

        if (process && process->state() == QProcess::Running) {
            process->terminate();
            process->waitForFinished(500);
            delete process;
        }
        socket->disconnectFromHost();
        delete socket;
    }
    m_clients.clear();
    qInfo() << "Сервер удаленной консоли остановлен.";
}

void SimpleRemoteConsole::onNewConnection() {
    while (m_tcpServer->hasPendingConnections()) {
        QTcpSocket *clientSocket = m_tcpServer->nextPendingConnection();
        if (!clientSocket) continue;

        QProcess *process = new QProcess(this);
        process->setProcessChannelMode(QProcess::MergedChannels);

#ifdef Q_OS_WIN
        process->setProgram("cmd.exe");
        process->setArguments(QStringList() << "/Q");
#else
        process->setProgram("/bin/sh");
        // Запускаем в интерактивном режиме, чтобы убрать внутреннюю буферизацию Linux-шелла
        process->setArguments(QStringList() << "-i");
#endif

        connect(process, &QProcess::readyReadStandardOutput, this, &SimpleRemoteConsole::onProcessReadyRead);

        connect(clientSocket, &QTcpSocket::readyRead, this, &SimpleRemoteConsole::onClientReadyRead);
        connect(clientSocket, &QTcpSocket::disconnected, this, &SimpleRemoteConsole::onClientDisconnected);

        process->start();
        m_clients.insert(clientSocket, process);

        clientSocket->write("=== Connected to Qt Remote Console ===\n");
    }
}

void SimpleRemoteConsole::onClientReadyRead() {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket || !m_clients.contains(socket)) return;

    QProcess *process = m_clients.value(socket);
    QByteArray inputData = socket->readAll();

#ifdef Q_OS_WIN
    inputData.replace("\n", "\r\n");
#endif

    if (process && process->state() == QProcess::Running) {
        process->write(inputData);

        // НАДЁЖНОЕ ВЫТАЛКИВАНИЕ БУФЕРА: Заставляет ОС мгновенно передать данные в пайп процесса
        process->waitForBytesWritten(50);
    }
}

void SimpleRemoteConsole::onProcessReadyRead() {
    QProcess *process = qobject_cast<QProcess*>(sender());
    if (!process) return;

    QTcpSocket *socket = m_clients.key(process, nullptr);
    if (socket && socket->state() == QTcpSocket::ConnectedState) {
        QByteArray output = process->readAllStandardOutput();
        socket->write(output);
    }
}

void SimpleRemoteConsole::onClientDisconnected() {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    if (m_clients.contains(socket)) {
        QProcess *process = m_clients.value(socket);
        if (process) {
            process->terminate();
            process->waitForFinished(500);
            delete process;
        }
        m_clients.remove(socket);
    }
    socket->deleteLater();
}
