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
#include "udp_client.hpp"
#include "mainbox.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox)
{
    ui->setupUi(this);

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
    ui->lne_Data->setText("$GLZDA,123456.12,01,02,2012,01,02*38");

    client = new UDP_Client(this);

    connect(ui->btn_Send, SIGNAL(clicked()), this, SLOT(send()));

    setMinimumWidth(800);
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
    QString data;
    QByteArray temp;

    data = ui->lne_Data->text();
    emit debug(QString("send(%1)").arg(data));
    data.append("\r\n");

    temp.clear();
    temp.append(data);
    emit debug(temp.toHex());

    client->send(data);
}
//--------------------------------------------------------------------------------
