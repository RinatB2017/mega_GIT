/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#include <QTest>
//--------------------------------------------------------------------------------
#include "test_function.hpp"
//--------------------------------------------------------------------------------
#include "mymainwindow.hpp"
//--------------------------------------------------------------------------------
Test_function::Test_function(void)
{
    mw = dynamic_cast<MyMainWindow *>(qApp->activeWindow());
    QVERIFY(mw);
}
//--------------------------------------------------------------------------------
void Test_function::lineedit_set(QString name, QString value)
{
    QLineEdit *le = mw->findChild<QLineEdit *>(name);
    QVERIFY(le);

    le->setText(value);
}
//--------------------------------------------------------------------------------
void Test_function::lineedit_set(QWidget *widget, QString name, QString value)
{
    QVERIFY(widget);

    QLineEdit *le = widget->findChild<QLineEdit *>(name);
    QVERIFY(le);

    le->setText(value);
}
//--------------------------------------------------------------------------------
QString Test_function::lineedit_get(QString name)
{
    QLineEdit *le = mw->findChild<QLineEdit *>(name);
    Q_CHECK_PTR(le);

    return le->text();
}
//--------------------------------------------------------------------------------
QString Test_function::lineedit_get(QWidget *widget, QString name)
{
    Q_CHECK_PTR(widget);

    QLineEdit *le = widget->findChild<QLineEdit *>(name);
    Q_CHECK_PTR(le);

    return le->text();
}
//--------------------------------------------------------------------------------
void Test_function::button_click(QString name)
{
    QAbstractButton *btn = mw->findChild<QAbstractButton *>(name);
    QVERIFY(btn);

    QTest::mouseClick(btn, Qt::LeftButton);
}
//--------------------------------------------------------------------------------
void Test_function::button_click(QWidget *widget, QString name)
{
    QVERIFY(widget);

    QAbstractButton *btn = widget->findChild<QAbstractButton *>(name);
    QVERIFY(btn);

    QTest::mouseClick(btn, Qt::LeftButton);
}
//--------------------------------------------------------------------------------
void Test_function::combobox_key_down_and_check_value(QString name, QString value)
{
    QComboBox *cb = mw->findChild<QComboBox *>(name);
    QVERIFY(cb);

    QTest::keyClick(cb, Qt::Key_Down);

    QCOMPARE(cb->currentText(), value);
}
//--------------------------------------------------------------------------------
void Test_function::combobox_key_down_and_check_value(QWidget *widget, QString name, QString value)
{
    QVERIFY(widget);

    QComboBox *cb = widget->findChild<QComboBox *>(name);
    QVERIFY(cb);

    QTest::keyClick(cb, Qt::Key_Down);

    QCOMPARE(cb->currentText(), value);
}
//--------------------------------------------------------------------------------
