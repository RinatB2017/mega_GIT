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
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mymainwindow.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_config();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

#if 0
    MyMainWindow *mw = dynamic_cast<MyMainWindow *>(parent());
    Q_CHECK_PTR(mw);
    connect(mw, SIGNAL(notifySignal()), this,   SLOT(test_1()));
#endif

    if(sizeHint().height() > 0)
    {
        setMinimumHeight(sizeHint().height());
    }
    load_config();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &MainBox::test_0 });
    commands.append({ ID_TEST_1, "test 1", &MainBox::test_1 });
    commands.append({ ID_TEST_2, "test 2", &MainBox::test_2 });
    commands.append({ ID_TEST_3, "test 3", &MainBox::test_3 });
    commands.append({ ID_TEST_4, "test 4", &MainBox::test_4 });
    commands.append({ ID_TEST_5, "test 5", &MainBox::test_5 });
    commands.append({ ID_TEST_6, "test 6", 0 });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QCheckBox *cb_block = new QCheckBox("block");
    testbar->addWidget(cb_block);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    connect(cb_block, SIGNAL(clicked(bool)), cb_test,           SLOT(setDisabled(bool)));
    connect(cb_block, SIGNAL(clicked(bool)), btn_choice_test,   SLOT(setDisabled(bool)));
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok) return;
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef bool (MainBox::*function)(void);
            function x;
            x = command.func;
            if(x)
            {
                (this->*x)();
            }
            else
            {
                emit error("no func");
            }

            return;
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::inFunc(QPushButton *btn, saveSlot slot)
{
    connect(btn,    &QPushButton::clicked,  this,   slot);
}
//--------------------------------------------------------------------------------
void MainBox::s_inFunc(void)
{
    QMessageBox::information(0,"","info");
}
//--------------------------------------------------------------------------------
bool MainBox::split_address(const QString address, int *a, int *b, int *c, int *d, int *port)
{
    QStringList sl = address.split(":");
    if(sl.count() != 2)
    {
        //emit error(QString("count %1").arg(sl.count()));
        return false;
    }
    QString host = sl.at(0);
    QStringList sl_address = host.split(".");
    if(sl_address.count() != 4)
    {
        //emit error(QString("count %1").arg(sl_address.count()));
        return false;
    }
    bool ok = false;
    int t_a = sl_address.at(0).toInt(&ok);
    if(!ok) return false;
    int t_b = sl_address.at(1).toInt(&ok);
    if(!ok) return false;
    int t_c = sl_address.at(2).toInt(&ok);
    if(!ok) return false;
    int t_d = sl_address.at(3).toInt(&ok);
    if(!ok) return false;
    *a = t_a;
    *b = t_b;
    *c = t_c;
    *d = t_d;

    QString port_str = sl.at(1);
    int t_port = port_str.toInt(&ok);
    if(!ok) return false;
    *port = t_port;

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit info("Test_0()");

#if 1
    My_class addr;
    addr = 0x12345678;
    emit info(QString("0x%1").arg(addr[0], 2, 16, QChar('0')));
    emit info(QString("0x%1").arg(addr[1], 2, 16, QChar('0')));
    emit info(QString("0x%1").arg(addr[2], 2, 16, QChar('0')));
    emit info(QString("0x%1").arg(addr[3], 2, 16, QChar('0')));

    emit info(QString("0x%1").arg(addr.get_value(), 8, 16, QChar('0')));
#endif

#if 0
    QString address = "1.2.3.4:5678";

    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    int port = 0;
    bool ok = split_address(address, &a, &b, &c, &d, &port);
    if(ok)
    {
        emit info(QString("address %1.%2.%3.%4:%5")
                  .arg(a)
                  .arg(b)
                  .arg(c)
                  .arg(d)
                  .arg(port));
    }
    else
    {
        emit error("ERROR");
    }
#endif

#if 0
    const QMetaObject &mo = Programmer::staticMetaObject;
    int index = mo.indexOfEnumerator("Language");
    QMetaEnum me = mo.enumerator(index);
    Programmer::Language p = Programmer::CPP;
    Programmer::Language l= static_cast<Programmer::Language>(me.keyToValue("CPP"));
    emit info(QString("%1:%2")
              .arg(me.valueToKey(p))
              .arg(l));
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_1()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit info("Test_2()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit info("Test_3()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit info("Test_4()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit info("Test_5()");

    return true;
}
//--------------------------------------------------------------------------------
quint32 MainBox::test(const QByteArray ba)
{
    quint32 temp = 0;

    for(int n=0; n<ba.length(); n++)
    {
        temp += (char)ba.at(n);
    }

    return temp;
}
//--------------------------------------------------------------------------------
bool MainBox::test2(int a,
                    int b,
                    int *c,
                    int *d)
{
    *c = a;
    *d = b;
    return true;
}
//--------------------------------------------------------------------------------
