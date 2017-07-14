//--------------------------------------------------------------------------------
#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
class QByteArray;
class QString;
class QUdpSocket;
//--------------------------------------------------------------------------------
class UDP_Client : public QWidget
{
    Q_OBJECT

public:
    explicit UDP_Client(QWidget *parent = 0);
    ~UDP_Client();

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

public slots:
    void send(const QString &data);

private slots:
    void readPendingDatagrams(void);
    void log(const QString &data);

private:
    QWidget *parent;
    QUdpSocket *udpSocket;
    QString fileName;
    QString address;
    unsigned int port;

    void send_data(QByteArray);
    QString getLocalAddress(void);
    void init(void);
    void connect_log(void);
};
//--------------------------------------------------------------------------------
#endif
