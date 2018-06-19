#include <QString>
#include <QtTest>

#include "testingtest.hpp"

TestingTest::TestingTest()
{
}

void TestingTest::testCase1()
{
    QVERIFY2(true, "Failure");
}
