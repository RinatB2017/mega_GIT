#ifndef ASYNCDNS_H
#define ASYNCDNS_H

#include <QObject>
#include <QHostAddress>
#include "buffer.h"
#include <QUdpSocket>
#include <QDebug>
#include <cstring>
#include <QList>
#include <QMap>
class AsyncDNS : public QObject
{
    Q_OBJECT
public:
    enum QTYPE{
        A = 1,
        NS = 2,
        CNAME = 5,
        AAAA = 28 //IPV6
    };

    enum QCLASS{
        IN = 1
    };

    enum BDADDR{
        TO_QS = 1,
        FROM_QS = 2
    };

    struct DNSHeader{
        quint16 request_id;
        quint8  flag;
        quint8  rcode;
        quint16 qd_count;
        quint16 an_count;
        quint16 ns_count;
        quint16 ar_count;
    };

    struct DNS_QD{
        QString host;
        quint16 qtype;
        quint16 qclass;
    };

    struct DNS_RR{
        quint16 name;
        quint16 type;
        quint16 rclass;
        quint32 ttl;
        quint16 rdlength;
        Buffer  rdata;
    };

    explicit AsyncDNS(QObject *parent = 0);
    AsyncDNS(QHostAddress host, quint16 port = 53, QObject *parent = 0);
    ~AsyncDNS();

    void sendDNSRequest(QString address);

    QHostAddress getFirstIP(const QString &host);
    QHostAddress getRandomIP(const QString &host);
    QUdpSocket * getSocket();
private:
    Buffer         build_request(QString address, ushort qtype, ushort request_id);
    QString        build_address(QString address, int direction = TO_QS);

    unsigned int   parse_RR(Buffer &response_data, DNS_RR *rr);
    unsigned int   parse_header(Buffer &response_data, DNSHeader *header);
    unsigned int   parse_RR(char * response_data, DNS_RR *rr);
    unsigned int   parse_header(char * response_data, DNSHeader *header);
    unsigned int   parse_question(char * response_data, DNS_QD *question);

    QString        parse_ip(Buffer &ip_buf);
    QUdpSocket *   socket;
    QHostAddress   DNS_server;
    quint16        DNS_port;

    QMap< QString, QList<QHostAddress> > cache;

signals:
    void resolve(QString host);
public slots:
    void handleReadData();
    void onResolve(QString host);
};

#endif // ASYNCDNS_H
