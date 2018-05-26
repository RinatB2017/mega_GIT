#include "asyncdns.h"

AsyncDNS::AsyncDNS(QObject *parent) : QObject(parent)
{
    QHostAddress dns_server("114.114.114.114");
    quint16      port = 53;
    DNS_server = dns_server;
    DNS_port   = port;

    AsyncDNS(dns_server, port, parent);
}

AsyncDNS::AsyncDNS(QHostAddress host, quint16 port, QObject *parent) : QObject(parent)
{
    socket = new QUdpSocket(this);
    DNS_server = host;
    DNS_port   = port;

    socket->bind(DNS_port);

    connect(socket, SIGNAL(readyRead()) , this, SLOT(handleReadData()) );
    connect(this, SIGNAL(resolve(QString)), this, SLOT(onResolve(QString)) );
}

AsyncDNS::~AsyncDNS()
{

}

QString AsyncDNS::build_address(QString address, int direction)
{
    QString fmt_address("");
    if(direction == BDADDR::TO_QS){
        QStringList label_list = address.split(".");

        /* fmt domain data*/
        for(int j = 0; j < label_list.length(); j++)
        {
            char len = (label_list.at(j)).size();
            fmt_address.append(len);
            fmt_address.append(label_list.at(j));
        }

        fmt_address.append('\0');
        return fmt_address;
    }else if(direction == BDADDR::FROM_QS){
        int index = 0;
        while(index < address.size()){
            if(address[index] > 0 && address[index] <= 63){
                address.replace(index,1,QChar('.'));
            }
            index += 1;
        }
        // throw the last '\0' character.
        address.chop(1);
        fmt_address = address.mid(1);
        return fmt_address;
    }else{
        return fmt_address; // empty string
    }
}

Buffer AsyncDNS::build_request(QString address, ushort qtype, ushort request_id)
{
    /*
    * DNS header
    *
    *
    * 1 line = 2 octets = 16 bits
    *
    *                                 1  1  1  1  1  1
    *   0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    * +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    * |                      ID                       |
    * +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    * |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
    * +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    * |                    QDCOUNT                    |
    * +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    * |                    ANCOUNT                    |
    * +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    * |                    NSCOUNT                    |
    * +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    * |                    ARCOUNT                    |
    * +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    *
    * DNS Message Format
    * +---------------------+
    * |        Header       |
    * +---------------------+
    * |       Question      | the question for the name server
    * +---------------------+
    * |        Answer       | RRs answering the question
    * +---------------------+
    * |      Authority      | RRs pointing toward an authority
    * +---------------------+
    * |      Additional     | RRs holding additional information
    * +---------------------+
    *
    * DNS Question Format
    *                               1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                     QNAME                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QTYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QCLASS                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    */

    /* domain name DATA format:
     * [LEN-1]<label-1>[LEN-2]<label-2>...[LEN-N]
     *
     * e.g. :
     * 03 www 06 google 03 com 00
     */

    QString fmt_address = build_address(address);
    Buffer header    = Buffer::pack("HBBHHHH",request_id, 1, 0, 1, 0, 0, 0);
    Buffer question  = Buffer::pack("XBHH", fmt_address.toStdString().c_str(), '\0', qtype, QCLASS::IN);

    Buffer send_data = Buffer::pack("xx",header,question);
    return send_data;
}

QUdpSocket * AsyncDNS::getSocket()
{
    return socket;
}

unsigned int AsyncDNS::parse_RR(char * response_data, DNS_RR *rr)
{
    return Buffer::unpack("HHHIH[&5]",response_data,
                          &(rr->name),
                          &(rr->type),
                          &(rr->rclass),
                          &(rr->ttl),
                          &(rr->rdlength),
                          &(rr->rdata));
}

unsigned int AsyncDNS::parse_RR(Buffer &response_data, DNS_RR *rr)
{
    char * dat = response_data.data();
    return parse_RR(dat, rr);
}

unsigned int AsyncDNS::parse_header(char * response_data, DNSHeader *header)
{
    return Buffer::unpack("HBBHHHH",response_data,
                          &(header->request_id),
                          &(header->flag),
                          &(header->rcode),
                          &(header->qd_count),
                          &(header->an_count),
                          &(header->ns_count),
                          &(header->ar_count));
}

unsigned int AsyncDNS::parse_header(Buffer &response_data, DNSHeader *header)
{
    char * dat = response_data.data();
    return parse_header(dat, header);
}

unsigned int AsyncDNS::parse_question(char *response_data, DNS_QD *question)
{
    int offset = 0;
    unsigned int len = 0;
    while( *(response_data + offset) != '\0' )
    {
        offset += 1;
    }

    Buffer hostdata(0);
    // why +1 ?
    // because actual host data has an additional '\0' represents for the end of string.
    QString unpack_fmt = QString("[%1]HH").arg(offset + 1);

    len = Buffer::unpack(unpack_fmt.toStdString().c_str(), response_data,
                         &hostdata,
                         &(question->qtype),
                         &(question->qclass));

    question->host = build_address(hostdata.toQString(), FROM_QS);

    return len;
}

QString AsyncDNS::parse_ip(Buffer &ip_buf)
{
    if(ip_buf.size() != 4) {
        return QString("0.0.0.0");
    }

    QString str = QString("%1.%2.%3.%4").arg(static_cast<uchar>(ip_buf.at(0)), 0, 10)
            .arg(static_cast<uchar>(ip_buf.at(1)), 0, 10)
            .arg(static_cast<uchar>(ip_buf.at(2)), 0, 10)
            .arg(static_cast<uchar>(ip_buf.at(3)), 0, 10);
    return str;
}

// public method
void AsyncDNS::handleReadData()
{
    Buffer datagram;
    QString host;
    QList<QHostAddress> host_set;

    while(socket->hasPendingDatagrams()) {
        datagram.resize(socket->pendingDatagramSize());

        socket->readDatagram(datagram.data(), datagram.size());
    }
    unsigned int offset = 0;

    //parse datagram
    char * dat = datagram.data();

    DNSHeader header;
    DNS_QD    question;
    DNS_RR    rr;
    offset = parse_header(dat, &header);

    dat += offset;
    //parse question
    offset = parse_question(dat, &question);

    host   = question.host;
    dat += offset;

    //cache
    if( !cache.contains(host) ) {
        cache.insert(host, host_set);
    }

    //parse RRs
    int an_rr = header.an_count;
    for(int j = 0; j < an_rr ; j++ ) {
        dat += parse_RR(dat, &rr);

        if( rr.type == QTYPE::A && rr.rclass == QCLASS::IN ) {
            if(rr.rdlength == 4){ // IPV4 data
                bool lock = false;
                QHostAddress addr = QHostAddress(parse_ip(rr.rdata));

                if(cache.contains(host)){
                    for(QHostAddress item : cache[host])
                    {
                        if(item == addr){
                            lock = true;
                        }
                    }
                    if(lock == false)
                    {
                        (cache[host]).append(addr);
                    }
                }
            }else{
                qDebug() << "[ERROR] parse RR error!";
                return ;
            }
        }
    }

    emit resolve(host);
}

void AsyncDNS::sendDNSRequest(QString address)
{
    Buffer req = build_request(address, QTYPE::A, 3);
    socket->writeDatagram(req, DNS_server, DNS_port);
}

void AsyncDNS::onResolve(QString host)
{
    qDebug() << getFirstIP(host);
}

QHostAddress AsyncDNS::getFirstIP(const QString &host)
{
    //retrieve host address from cache
    if(cache.contains(host))
    {
        if(host.length() > 0) {
            return (cache[host])[0];
        } else {
            return QHostAddress("");
        }
    } else {
        return QHostAddress("");
    }
}

QHostAddress AsyncDNS::getRandomIP(const QString &host)
{

    //retrieve host address from cache
    if(cache.contains(host))
    {
        if(host.length() > 0) {
            int total = host.length();
            return (cache[host])[ qrand() % total ];
        } else {
            return QHostAddress("");
        }
    } else {
        return QHostAddress("");
    }
}
