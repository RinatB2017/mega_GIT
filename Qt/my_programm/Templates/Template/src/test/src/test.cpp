/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#include <QApplication>
#include <QObject>
#include <QWidget>
#include <QList>
#include <QTest>
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "template_mainbox.hpp"
#include "test.hpp"
//--------------------------------------------------------------------------------
Test::Test()
{
    QTest::qWait(0);    // нужно обязательно
}
//--------------------------------------------------------------------------------
void Test::initTestCase()
{
    QVERIFY(mw != nullptr);
}
//--------------------------------------------------------------------------------
void Test::setMainWindow(MainWindow *mainWindow)
{
    mw = mainWindow;
}
//--------------------------------------------------------------------------------
void Test::test_GUI(void)
{
    QComboBox *cb = mw->findChild<QComboBox *>("cb_test");
    QVERIFY(cb);
    QTest::keyClick(cb, Qt::Key_Down);
    QTest::keyClick(cb, Qt::Key_Down);

    QToolButton *tb = mw->findChild<QToolButton *>("btn_choice_test");
    QVERIFY(tb);
    QTest::mouseClick(tb, Qt::LeftButton);
}
//--------------------------------------------------------------------------------
void Test::test_func(void)
{
    MainBox *mb = mw->findChild<MainBox *>("MainBox");
    QVERIFY(mb);
}
//--------------------------------------------------------------------------------
void Test::test_signals(void)
{

}
//--------------------------------------------------------------------------------
