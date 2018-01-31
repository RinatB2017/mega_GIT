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

    //---
    QByteArray ba;
    ba.resize(0xFFFF);

    ba[0] = 0xAB;
    ba[0x10] = 0xBA;
    ba[0xFFFF] = 0x00;
    //---
    QPalette palette;

    palette.setBrush(QPalette::WindowText,  QBrush(QColor((QRgb)0x000000), Qt::SolidPattern));
    palette.setBrush(QPalette::Button,      QBrush(QColor((QRgb)0xd4d4d4), Qt::SolidPattern));
    palette.setBrush(QPalette::Light,       QBrush(QColor((QRgb)0xffffff), Qt::SolidPattern));
    palette.setBrush(QPalette::Text,        QBrush(QColor((QRgb)0x000000), Qt::SolidPattern));
    palette.setBrush(QPalette::BrightText,  QBrush(QColor((QRgb)0xffffff), Qt::SolidPattern));
    palette.setBrush(QPalette::ButtonText,  QBrush(QColor((QRgb)0x000000), Qt::SolidPattern));
    palette.setBrush(QPalette::Base,        QBrush(QColor((QRgb)0xffffff), Qt::SolidPattern));
    palette.setBrush(QPalette::Window,      QBrush(QColor((QRgb)0xd4d4d4), Qt::SolidPattern));
    palette.setBrush(QPalette::Shadow,      QBrush(QColor((QRgb)0x404040), Qt::SolidPattern));

    ui->hex_widget->setPalette(palette);
    ui->hex_widget->setReadOnly(true);
    //---
    ui->hex_widget->setData(QHexEditData::fromMemory(ba));
    //---

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
#include "hexspinbox.hpp"
bool MainBox::test_0(void)
{
    emit info("Test_0()");

#if 0
    QTableWidget
#endif

#if 0
    int i = 0;

    QGridLayout *grid = new QGridLayout;
    grid->setSpacing(0);
    grid->setMargin(0);
    int cnt  = 0;
    for(int y=0; y<32; y++)
    {
        grid->addWidget(new QLabel(QString("%1").arg(cnt, 4, 16, QChar('0')).toUpper()));
        cnt += 0x10;
        for(int x=0; x<16; x++)
        {
            EditableLabel *le = new EditableLabel;
            le->setText(QString("%1").arg(i, 4, 16, QChar('0')).toUpper());
            le->setReadOnly(true);
            i++;

            grid->addWidget(le, y, x+1);
        }
    }
    QWidget *w = new QWidget;
    w->setLayout(grid);

    QScrollArea *sa = new QScrollArea;
    sa->setWidget(w);

    sa->show();
#endif

#if 0
    HexLineEdit *hle = new HexLineEdit;
    hle->show();
#endif

#if 0
    QGridLayout *grid = new QGridLayout;
    grid->setSpacing(0);
    grid->setMargin(0);
    int cnt  = 0;
    for(int y=0; y<16; y++)
    {
        grid->addWidget(new QLabel(QString("%1").arg(cnt, 4, 16, QChar('0')).toUpper()));
        cnt += 0x10;
        for(int x=0; x<16; x++)
        {
            HexSpinBox *hsb = new HexSpinBox;
            grid->addWidget(hsb, y, x+1);
        }
    }

    QWidget *w = new QWidget;
    w->setLayout(grid);

    w->show();
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
