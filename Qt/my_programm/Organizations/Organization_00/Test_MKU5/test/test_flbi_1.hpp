//--------------------------------------------------------------------------------
#ifndef _TEST_FLBI_1_H
#define _TEST_FLBI_1_H
//--------------------------------------------------------------------------------
#include <QtNetwork>
#include <QObject>
#include <QtDebug>
//--------------------------------------------------------------------------------
#include "test_thread.hpp"
#include "proto.hpp"
//--------------------------------------------------------------------------------
class Test_FLBI_1 : public TestThread
{
    Q_OBJECT

public:
    Test_FLBI_1(QObject*, const QString address, unsigned int port);
    void run();

private:
    QByteArray get_filled_array(int);
};
//--------------------------------------------------------------------------------
#endif
