//--------------------------------------------------------------------------------
#ifndef _TEST_RS232_2_H
#define _TEST_RS232_2_H
//--------------------------------------------------------------------------------
#include <QtNetwork>
#include <QObject>
#include <QtDebug>
//--------------------------------------------------------------------------------
#include "test_thread.hpp"
#include "proto.hpp"
//--------------------------------------------------------------------------------
class Test_RS232_2 : public TestThread
{
    Q_OBJECT

public:
    Test_RS232_2(QObject*parent, const QString address, unsigned int port);
    void run();

private:
    QByteArray get_filled_array(int);
};
//--------------------------------------------------------------------------------
#endif
