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
#include "ui_for_tests_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mymainwindow.hpp"
#include "mainwindow.hpp"
#include "for_tests_mainbox.hpp"
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

    ui->spinBox->setRange(0, 1e6);

#if 1
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
#else
    if(sizeHint().height() > 0)
    {
        setMinimumHeight(sizeHint().height());
    }
#endif
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
    emit trace(Q_FUNC_INFO);
    QMessageBox::information(0,"","info");
}
//--------------------------------------------------------------------------------
void MainBox::new_test(void)
{
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
bool MainBox::split_address(const QString address,
                            int *a,
                            int *b,
                            int *c,
                            int *d,
                            int *port)
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
int MainBox::get_cnt(void)
{
    emit trace(Q_FUNC_INFO);
    return qrand() % 10;
}
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit info("Test_0()");
    emit trace(Q_FUNC_INFO);

#if 0
    setProperty("xxx", 666);
#endif

#if 0
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);
    mw->add_dock_widget("тест", "test", Qt::RightDockWidgetArea, new QSpinBox);
#endif

#if 0
    QSysInfo i;
    emit info(i.buildAbi());
    emit info(i.buildCpuArchitecture());
    emit info(i.currentCpuArchitecture());
    emit info(i.kernelType());
    emit info(i.kernelVersion());
    emit info(i.machineHostName());
    emit info(i.prettyProductName());
    emit info(i.productType());
    emit info(i.productVersion());
#endif

#if 0
    emit info(QString("PATH = %1").arg(QProcessEnvironment::systemEnvironment().value("PATH")));
#endif

#if 0
    for(int n=0; n<get_cnt(); n++)
    {
        emit info(QString("n=%1").arg(n));
    }
#endif

#if 0
    QList<int> test;
    for(int n=0; n<5; n++)
    {
        test.append(n);
    }
    foreach (auto x, test)
    {
        if(x == 0)
        {
            emit info("append data");
            test.append(10);
            test.append(11);
            test.append(12);
        }
        emit info(QString("x=%1").arg(x));
    }
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_1()");
    emit trace(Q_FUNC_INFO);

#if 0
    emit info("info");
    emit debug("debug");
    emit error("error");
    emit trace("trace");

    emit colorLog("Cyan", QColor(Qt::cyan), QColor(Qt::black));
    emit colorLog("Yellow", QColor(Qt::yellow), QColor(Qt::white));
#endif

#if 0
    emit info(__FILE__);
    emit info(QString("line %1").arg(__LINE__));
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit info("Test_2()");
    emit trace(Q_FUNC_INFO);

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit info("Test_3()");
    emit trace(Q_FUNC_INFO);

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit info("Test_4()");
    emit trace(Q_FUNC_INFO);

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit info("Test_5()");
    emit trace(Q_FUNC_INFO);

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
