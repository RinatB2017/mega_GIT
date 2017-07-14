/*********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#include <QApplication>
#include <QObject>
#include <QWidget>
#include <QList>
//--------------------------------------------------------------------------------
#include <QTest>
#include "test.hpp"
//--------------------------------------------------------------------------------
#define private public

#include "mainwindow.hpp"
#include "mainbox.hpp"
//--------------------------------------------------------------------------------
Test::Test(MainWindow *value)
{
    mw = value;
}
//--------------------------------------------------------------------------------
void Test::test_GUI(void)
{
    QVERIFY(mw);

    QComboBox *cb = mw->findChild<QComboBox *>("cb_test");
    QVERIFY(cb);
    if(cb)
    {
        QTest::keyClick(cb, Qt::Key_Down);
        QTest::keyClick(cb, Qt::Key_Down);
    }

    QToolButton *tb = mw->findChild<QToolButton *>("btn_choice_test");
    QVERIFY(tb);
    if(tb)
    {
        QTest::mouseClick(tb, Qt::LeftButton);
    }
}
//--------------------------------------------------------------------------------
void Test::test_func(void)
{
    MainBox *mb = new MainBox(0, 0);
    QCOMPARE(mb->test_0(), true);
    QCOMPARE(mb->test_1(), true);
    QCOMPARE(mb->test_2(), true);
    QCOMPARE(mb->test_3(), true);
    QCOMPARE(mb->test_4(), true);
    QCOMPARE(mb->test_5(), true);
}
//--------------------------------------------------------------------------------
