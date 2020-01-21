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
//--------------------------------------------------------------------------------
#include <QSignalSpy>
#include <QTest>
//--------------------------------------------------------------------------------
#include "test.hpp"
//--------------------------------------------------------------------------------
#define private public
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "hlk_rm04_widget.hpp"
//--------------------------------------------------------------------------------
Test::Test()
{
    mw = dynamic_cast<MainWindow *>(qApp->activeWindow());
    QVERIFY(mw);
}
//--------------------------------------------------------------------------------
Test::~Test()
{

}
//--------------------------------------------------------------------------------
void Test::test_GUI_network(void)
{
    QLineEdit *le_ssid = mw->findChild<QLineEdit *>("le_ssid");
    QLineEdit *le_password = mw->findChild<QLineEdit *>("le_password");
    QComboBox *cb_encrypt_type = mw->findChild<QComboBox *>("cb_encrypt_type");

    QVERIFY(le_ssid);
    QVERIFY(le_password);
    QVERIFY(cb_encrypt_type);
}
//--------------------------------------------------------------------------------
void Test::test_HLK_RM04_widget(void)
{
    QByteArray ba;
    ba.append("test");
    ba.append('\n');
    ba.append("test");

    HLK_RM04_widget *w = mw->findChild<HLK_RM04_widget *>("hlk_rm04_widget");
    QVERIFY(w);

    QCOMPARE(w->split_data(ba), 2);
}
//--------------------------------------------------------------------------------

