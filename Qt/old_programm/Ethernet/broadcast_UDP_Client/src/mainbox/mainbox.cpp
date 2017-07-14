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
#include <QWidget>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "udp_client.hpp"
#include "mainbox.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::connect_log(void)
{
        connect(this, SIGNAL(info(QString)),    topLevelWidget(), SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)),   topLevelWidget(), SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)),   topLevelWidget(), SIGNAL(error(QString)));
        connect(this, SIGNAL(message(QString)), topLevelWidget(), SIGNAL(message(QString)));
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);
    connect_log();

    client = new UDP_Client(this);

    connect(ui->btn_Send, SIGNAL(clicked()), this, SLOT(send()));

    setMinimumWidth(800);
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    emit debug("updateText()");
}
//--------------------------------------------------------------------------------
void MainBox::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::send(void)
{
    QByteArray ba;

    ba.clear();
    ba.append("xxx");

    client->send_broadcast_data(ba);
}
//--------------------------------------------------------------------------------
