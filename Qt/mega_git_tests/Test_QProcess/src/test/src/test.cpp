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
#define private public
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "test_QProcess_mainbox.hpp"
#include "test.hpp"
//--------------------------------------------------------------------------------
Test::Test()
{
    mw = dynamic_cast<MainWindow *>(qApp->activeWindow());
    QVERIFY(mw);
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

    QCOMPARE(mb->test(), true);

    QString cmd = "python";
    QString filename = "test2.py";

    QCOMPARE(remove_file(filename), true);
    QCOMPARE(create_python_test_file(filename), true);

    QCOMPARE(mb->run_command(cmd, filename), true);
    QTimer::singleShot(100, [this, filename]{
        QCOMPARE(remove_file(filename), true);
    });
}
//--------------------------------------------------------------------------------
bool Test::create_python_test_file(const QString &filename)
{
    QString text;
    text.append("if __name__ == \"__main__\":\n");
    text.append("    print(\"All OK\")\n");

    if(filename.isEmpty())
    {
        qDebug() << "file" << filename << "is empty!";
        return false;
    }

    QFile file(filename);
    if (!file.open(QIODevice::NewOnly | QIODevice::Text))
    {
        qDebug() << "file" << filename << "not open";
        return false;
    }

    qint64 len = file.write(text.toLocal8Bit());
    if(len != text.length())
    {
        qDebug() << len << "!=" << text.length();
        return false;
    }

    file.close();

    return true;
}
//--------------------------------------------------------------------------------
bool Test::remove_file(const QString &filename)
{
    if(!QFile::exists(filename))
    {
        return true;
    }
    return QFile::remove(filename);
}
//--------------------------------------------------------------------------------
