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

#if 1
    connect(ui->btn_0,  SIGNAL(clicked(bool)),  this,   SLOT(new_test()));
#endif

#if 1
    for(int y=0; y<10; y++)
    {
        for(int x=0; x<5; x++)
        {
            QLabel *lbl = new QLabel(this);
            lbl->setPixmap(QPixmap(":/red/0.png"));

            ui->gridLayout->addWidget(lbl, y, x);
        }
    }
#endif

#if 1
    ui->btn_test->setCheckable(true);
    connect(ui->btn_test,   SIGNAL(toggled(bool)),  ui->safe, SLOT(setHidden(bool)));
#endif

#if 0
    MyMainWindow *mw = dynamic_cast<MyMainWindow *>(parent());
    Q_CHECK_PTR(mw);
    connect(mw, SIGNAL(notifySignal()), this,   SLOT(test_1()));
#endif

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
bool MainBox::test_0(void)
{
    emit info("Test_0()");
    emit trace(Q_FUNC_INFO);

#if 1
    int x = rand() % ui->gridLayout->columnCount();
    int y = rand() % ui->gridLayout->rowCount();

    QLayoutItem *item = ui->gridLayout->itemAtPosition(y, x);
    Q_CHECK_PTR(item);

    QLabel *label = dynamic_cast<QLabel*>(item->widget());
    Q_CHECK_PTR(label);

    QPixmap pixmap;
    pixmap.load(":/red/1.png");

    label->setPixmap(pixmap);
#endif

#if 0
    int x = 5;
    Q_ASSERT(x == 0);
#endif

#if 0
    uint16_t x = 0x55AA;
    emit info(QString("%1").arg(x, 16, 2, QChar('0')));

    uint8_t y = 20;
    emit info(QString("%1").arg(y, 2, 5, QChar('0')));

    //Q_ASSERT(y > 0);
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_1()");
    emit trace(Q_FUNC_INFO);

#if 0
    auto *w = new QLabel;
    w->setText("XXX");
    w->show();
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
