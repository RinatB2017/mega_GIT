#include <QCoreApplication>
#include "buffer.h"
#include "asyncdns.h"
#include <QObject>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QHostAddress google_dns("8.8.8.8");
    AsyncDNS resolver(google_dns);

    resolver.sendDNSRequest("www.demohn.com");
    return a.exec();
}
