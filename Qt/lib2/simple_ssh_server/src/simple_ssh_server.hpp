#pragma once

#include <QObject>
#include <QThread>

class SimpleSshServer : public QThread {
    Q_OBJECT
public:
    explicit SimpleSshServer(quint16 port = 2222, QObject *parent = nullptr);
    ~SimpleSshServer();

    bool startServer();
    void stopServer();

protected:
    void run() override;

private:
    quint16 m_port;
    QString m_keyPath;
    struct ssh_bind_struct* m_sshBind;
    std::atomic<bool> m_running;

    bool ensureHostKeyExists();
    void handleClient(struct ssh_session_struct* session);
    void runConsole(struct ssh_channel_struct* channel);
};
