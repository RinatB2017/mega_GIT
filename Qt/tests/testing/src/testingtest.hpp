#ifndef TESTINGTEST
#define TESTINGTEST

#include <QObject>

class TestingTest : public QObject
{
    Q_OBJECT

public:
    TestingTest();

private slots:
    void testCase1();
};
#endif
