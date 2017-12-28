/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTime>

#include <QAction>
#include <QMenu>

#include <QToolButton>
#include <QToolBar>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "mainbox.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox),
    serialBox(0)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

    serialBox = new SerialBox5(this, "RS232");
    serialBox->add_menu(2);

    ui->serial_layout->addWidget(serialBox);
    ui->serial_layout->addStretch();

    ui->le_rssi->setReadOnly(true);
    ui->le_rsrp->setReadOnly(true);
    ui->le_sinr->setReadOnly(true);
    ui->le_rsrq->setReadOnly(true);

    connect(this, SIGNAL(send(QByteArray)), serialBox, SLOT(input(QByteArray)));
    connect(serialBox, SIGNAL(output(QByteArray)), this, SLOT(read_usb_modem(QByteArray)));

    connect(this, SIGNAL(send_RSSI(QString)), ui->le_rssi, SLOT(setText(QString)));
    connect(this, SIGNAL(send_RSRP(QString)), ui->le_rsrp, SLOT(setText(QString)));
    connect(this, SIGNAL(send_SINR(QString)), ui->le_sinr, SLOT(setText(QString)));
    connect(this, SIGNAL(send_RSRQ(QString)), ui->le_rsrq, SLOT(setText(QString)));
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);
    if(mw == nullptr)
    {
        return;
    }

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");

    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    QByteArray ba;
    ba.clear();
    ba.append("AT+CUSD=1,*111*217#,15");
    //ba.append("AT+CUSD=1,\"*111*217#\",15");
    ba.append(0x0D);

    emit send(ba);
}
//--------------------------------------------------------------------------------
int MainBox::calc_SINR(int value)
{
    if(value == 0)
    {
        return -20;
    }

    if((value >= 1) && (value <= 251))
    {
        return (value - 1) / 5 - 20;
    }

    return 30;
}
//--------------------------------------------------------------------------------
void MainBox::read_usb_modem(QByteArray ba)
{
    emit info(ba);

    QString str = ba;
    QStringList lines = str.split("\n");

    //QRegExp temp = QRegExp("^HCSQ:\"LTE\",[0-9]{1,3},[0-9]{1,3},[0-9]{1,3},[0-9]{1,3}");
    QRegExp temp = QRegExp("([0-9]{1,3}),([0-9]{1,3}),([0-9]{1,3}),([0-9]{1,3})");

    for(int n=0; n<lines.size(); n++)
    {
        QString line = lines.at(n);
        if(temp.indexIn(line) >= 0)
        {
            int RSSI = temp.cap(1).toInt() - 121;
            int RSRP = temp.cap(2).toInt() - 141;
            int SINR = calc_SINR(temp.cap(3).toInt());
            int RSRQ = temp.cap(4).toInt() / 2 - 20;

            emit send_RSSI(QString("%1").arg(RSSI));
            emit send_RSRP(QString("%1").arg(RSRP));
            emit send_SINR(QString("%1").arg(SINR));
            emit send_RSRQ(QString("%1").arg(RSRQ));
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
