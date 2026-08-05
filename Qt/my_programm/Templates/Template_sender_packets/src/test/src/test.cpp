/*********************************************************************************
**                                                                              **
**     Copyright (C) 2026                                                       **
**                                                                              **
**     This program is free software: you can redistribute it and/or modify     **
**     it under the terms of the GNU General Public License as published by     **
**     the Free Software Foundation, either version 3 of the License, or        **
**     (at your option) any later version.                                      **
**                                                                              **
**     This program is distributed in the hope that it will be useful,          **
**     but WITHOUT ANY WARRANTY; without even the implied warranty of           **
**     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **
**     GNU General Public License for more details.                             **
**                                                                              **
**     You should have received a copy of the GNU General Public License        **
**     along with this program.  If not, see http://www.gnu.org/licenses/.      **
**                                                                              **
**********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#include "mainwindow.hpp"
#include "template_sender_packets_mainbox.hpp"
#include "test.hpp"
//--------------------------------------------------------------------------------
Test::Test()
{
    QTest::qWait(0);    // нужно обязательно
}
//--------------------------------------------------------------------------------
void Test::setMainWindow(MainWindow *mainWindow)
{
    mw = mainWindow;
}
//--------------------------------------------------------------------------------
void Test::initTestCase()
{
    mb = mw->findChild<MainBox *>("MainBox");
    QVERIFY(mb);
}
//--------------------------------------------------------------------------------
void Test::init()
{
    serialWidget = mb->findChild<SerialWidget *>("serial_widget");
    QVERIFY(serialWidget);

    serialWidget->setProperty("mock_open", true);

    spy = new QSignalSpy(mb, &MainBox::send);
    QVERIFY(spy->isValid());
}
//--------------------------------------------------------------------------------
void Test::cleanup()
{
    serialWidget->setProperty("mock_open", false);

    if (spy)
    {
        delete spy;
        spy = nullptr;
    }
}
//--------------------------------------------------------------------------------
void Test::test_GUI()
{
    QComboBox *cb = mw->findChild<QComboBox *>("cb_test");
    QVERIFY(cb);

    QToolButton *tb = mw->findChild<QToolButton *>("btn_choice_test");
    QVERIFY(tb);
}
//--------------------------------------------------------------------------------
void Test::test_func()
{
    MainBox *mb = mw->findChild<MainBox *>("MainBox");
    QVERIFY(mb);
}
//--------------------------------------------------------------------------------
void Test::test_signals()
{
    QTimer::singleShot(200, this, [this]() {
        QByteArray fakeResponse = ":0100010F96\n";
        QMetaObject::invokeMethod(serialWidget, "output", Q_ARG(QByteArray, fakeResponse));
    });

    mb->get_ID();
    QCOMPARE(spy->count(), 1);
    QList<QVariant> arguments = spy->takeFirst();
    QString debugText = arguments.at(0).toString();
    QCOMPARE(debugText, QString(":0100000D\n"));

    QSpinBox *sbId = mb->findChild<QSpinBox *>("sb_id");
    QVERIFY(sbId);
    QCOMPARE(sbId->value(), 15);
}
//--------------------------------------------------------------------------------
void Test::cleanupTestCase()
{

}
//--------------------------------------------------------------------------------
