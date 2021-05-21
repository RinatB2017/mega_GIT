//--------------------------------------------------------------------------------
#ifndef UDP_CLIENT_HPP
#define UDP_CLIENT_HPP
//--------------------------------------------------------------------------------
#include <QHostAddress>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class QByteArray;
class QString;
class QUdpSocket;
//--------------------------------------------------------------------------------
class UDP_Client : public MyWidget
{
    Q_OBJECT

public:
    explicit UDP_Client(QWidget *parent = nullptr);
    virtual ~UDP_Client();

    void setAddress(const QHostAddress &address);
    void setPort(quint16 port);
    QByteArray input(const QByteArray &data);

public slots:
    QByteArray send_data(const QByteArray &data);

private slots:
    void readPendingDatagrams(void);

private:
    QWidget *parent;
    QUdpSocket *udpSocket;
    QString fileName;
    QString address;
    quint16 port;

    QString getLocalAddress(void);
    void init(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif
