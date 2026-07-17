#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QProcess>
#include <QMap>

class SimpleRemoteConsole : public QObject {
    Q_OBJECT
public:
    explicit SimpleRemoteConsole(quint16 port = 2222, QObject *parent = nullptr);
    ~SimpleRemoteConsole();

    bool startServer();
    void stopServer();

private slots:
    void onNewConnection();
    void onClientReadyRead();
    void onClientDisconnected();
    void onProcessReadyRead();

private:
    quint16 m_port;
    QTcpServer *m_tcpServer;

    // Храним связку: какой сокет клиента управляет каким процессом ОС
    QMap<QTcpSocket*, QProcess*> m_clients;
};
