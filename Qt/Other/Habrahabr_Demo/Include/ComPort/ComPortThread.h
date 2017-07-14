#ifndef COMPORTTHREAD_H
#define COMPORTTHREAD_H

#include <QThread>
#include <QString>
#include <QMutex>
#include <QWaitCondition>

class ComPortThread : public QThread
{
    Q_OBJECT

public:
    static ComPortThread* getInstance()
    {
        static QMutex mutex;

        if (!m_instance)
        {
            mutex.lock();

            if (!m_instance)
            {
                m_instance = new ComPortThread;
            }

            m_refCount++;
            mutex.unlock();
        }

        return m_instance;
    }

    void run();
    void transaction(const QByteArray& request, int waitTimeout);
    void connectCom(QString namePort, int baudRate, int m_dataBits, int m_parity, int m_stopBits, int m_flowControl);
    void disconnectCom();
    bool isConnect();
    void free();

signals:
    void responseMsg(const QByteArray &s);
    void error(const QString &s);
    void timeout(const QString &s);

private:
    ComPortThread(QObject *parent = 0);
    ~ComPortThread();
    ComPortThread(const ComPortThread&);
    ComPortThread& operator=(const ComPortThread&);

private:
    int m_waitTimeout;
    QMutex mutex;
    QWaitCondition cond;

    QString m_portName;
    int m_baudrate;
    int m_dataBits;
    int m_parity;
    int m_stopBits;
    int m_flowControl;

    QByteArray m_request;

    bool m_isConnect;
    bool m_isDisconnecting;
    bool m_isConnecting;
    bool m_isQuit;

    static ComPortThread* m_instance;
    static int m_refCount;
};

#endif // COMPORTTHREAD_H
