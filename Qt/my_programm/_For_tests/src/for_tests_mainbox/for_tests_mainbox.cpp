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
#include "mysplashscreen.hpp"
#include "mymainwindow.hpp"
#include "for_tests_mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include <QNetworkReply>
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
    save_widgets("for_test");
    save_config();

    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

    ui->spinBox->setRange(0, 1e6);

    //---
    connect(this,           SIGNAL(dpi_set(int)),       ui->DPI_widget, SLOT(set_value(int)));
    connect(ui->DPI_widget, SIGNAL(value(int)),         ui->spinBox,    SLOT(setValue(int)));
    connect(ui->spinBox,    SIGNAL(valueChanged(int)),  ui->DPI_widget, SLOT(set_value(int)));
    //---

    connect(ui->sb_1,   SIGNAL(valueChanged(int)),  this,   SLOT(check_in()));
    connect(ui->sb_2,   SIGNAL(valueChanged(int)),  this,   SLOT(check_in()));
    connect(ui->sb_res, SIGNAL(valueChanged(int)),  this,   SLOT(check_in()));
    connect(ui->btn_ok, SIGNAL(clicked(bool)),      this,   SLOT(victory()));
    check_in();

#if 1
    //setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
#else
    if(sizeHint().height() > 0)
    {
        setMinimumHeight(sizeHint().height());
    }
#endif
    load_widgets("for_test");
    load_config();
}
//--------------------------------------------------------------------------------
void MainBox::check_in(void)
{
    int a = ui->sb_1->value();
    int b = ui->sb_2->value();
    int c = ui->sb_res->value();

#if 0
    emit debug("check_in");
    emit debug(QString("a %1").arg(a));
    emit debug(QString("b %1").arg(b));
    emit debug(QString("c %1").arg(c));
#endif

    bool res = ((a + b) == c);
    ui->btn_ok->setEnabled(res);
}
//--------------------------------------------------------------------------------
void MainBox::victory(void)
{
    messagebox_info("Info", "Victory!");
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
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

    //---
    IPV4 *ipv4 = new IPV4(this);
    testbar->addWidget(ipv4);
    //---

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
int MainBox::get_cnt(void)
{
    emit trace(Q_FUNC_INFO);
    return qrand() % 10;
}
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_0()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_1()");

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
    emit trace(Q_FUNC_INFO);
    emit info("Test_2()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_3()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_4()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_5()");

    return true;
}
//--------------------------------------------------------------------------------
quint32 MainBox::test(const QByteArray ba)
{
    emit trace(Q_FUNC_INFO);
    quint32 temp = 0;

    for(int n=0; n<ba.length(); n++)
    {
        temp += (char)ba.at(n);
    }

    return temp;
}
//--------------------------------------------------------------------------------
