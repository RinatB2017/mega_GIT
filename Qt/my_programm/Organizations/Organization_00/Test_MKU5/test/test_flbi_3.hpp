//--------------------------------------------------------------------------------
#ifndef _TEST_FLBI_3_H
#define _TEST_FLBI_3_H
//--------------------------------------------------------------------------------
#include <QtNetwork>
#include <QObject>
#include <QtDebug>
//--------------------------------------------------------------------------------
#include "test_thread.hpp"
#include "proto.hpp"
//--------------------------------------------------------------------------------
class Test_FLBI_3 : public TestThread
{
    Q_OBJECT

public:
    Test_FLBI_3(QObject*parent, const QString address, unsigned int port);
    void run();

private:
    QByteArray get_filled_array(int);
};
//--------------------------------------------------------------------------------
#endif
