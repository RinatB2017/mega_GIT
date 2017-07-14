//--------------------------------------------------------------------------------
#ifndef _TEST2_H
#define _TEST2_H
//--------------------------------------------------------------------------------
#include <QtNetwork>
#include <QObject>
#include <QtDebug>
//--------------------------------------------------------------------------------
#include "test_thread.hpp"
#include "proto.hpp"
//--------------------------------------------------------------------------------
class Test_RS232_1 : public TestThread
{
    Q_OBJECT

public:
    Test_RS232_1(QObject* ,const QString address, unsigned int port);
    void run();

private:
    QByteArray get_filled_array(int);
    QString address;
    unsigned int port;
};
//--------------------------------------------------------------------------------
#endif
