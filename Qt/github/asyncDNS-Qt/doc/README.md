# asyncDNS-qt

This project implements a small part of DNS protocol ([rfc1035](https://www.ietf.org/rfc/rfc1035.txt)) in C++ and Qt framework.

Thus, it can send a DNS request, receive a DNS response, and get a IP.

Each query is cached to save requests.

And in order to simplify the parsing work, I have additionally wrote Buffer::unpack and Buffer::pack method. The usage is listed below, and you can just individually use this class.

__NOTE__: In fact, Qt itself has a [QDnsLookup](http://doc.qt.io/qt-5/qdnslookup.html) class to accomplish this work.

## Usage

```
// main.cpp

#include <QCoreApplication>
#include "buffer.h"
#include "asyncdns.h"
#include <QObject>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QHostAddress google_dns("8.8.8.8");
    AsyncDNS resolver(google_dns);

    resolver.sendDNSRequest("www.zhihu.com");

    return a.exec();
}
```

## TODO

- DNS cache

## Buffer Class

(just dig a hole, updating...)
