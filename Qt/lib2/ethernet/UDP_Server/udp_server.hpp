//---------------------------------------------------------------------------
#ifndef UDP_SERVER_H
#define UDP_SERVER_H
//---------------------------------------------------------------------------
#include <QHostAddress>
#include <QWidget>
//---------------------------------------------------------------------------
class QUdpSocket;
class QByteArray;
//---------------------------------------------------------------------------
class UDP_Server : public QWidget
{
    Q_OBJECT

public:
    explicit UDP_Server(QWidget *parent = 0);
    ~UDP_Server();

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

    void send(const QByteArray &data);

public slots:
    int createServerOnPort(const QHostAddress address, unsigned int port);

private slots:
    void readPendingDatagrams(void);

private:
    QUdpSocket *udpSocket;

    void init(void);
    QString getLocalAddress(void);
};
//--------------------------------------------------------------------------------
#endif
