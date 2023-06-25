/*********************************************************************************
**                                                                              **
**     Copyright (C) 2023                                                       **
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
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QDebug>
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#include "b590_powersupply.hpp"
//--------------------------------------------------------------------------------
#include "find_powersupply.hpp"
//--------------------------------------------------------------------------------
Find_powersupply::Find_powersupply(Powersupply_B590 *powersupply,
                                   QFrame *parent) :
    QFrame(parent),
    powersupply(powersupply)
{
    init();
}
//--------------------------------------------------------------------------------
Find_powersupply::~Find_powersupply()
{
    cb_port_list->deleteLater();
    pb_find->deleteLater();
    pb_update_port_list->deleteLater();
}
//--------------------------------------------------------------------------------
void Find_powersupply::connect_log(void)
{
    if(parentWidget())
    {
        connect(this, SIGNAL(info(QString)),  parentWidget(), SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parentWidget(), SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parentWidget(), SIGNAL(error(QString)));
        connect(this, SIGNAL(trace(QString)), parentWidget(), SIGNAL(trace(QString)));
    }
    else
    {
        connect(this, SIGNAL(info(QString)),  this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(trace(QString)), this, SLOT(log(QString)));
    }
}
//--------------------------------------------------------------------------------
void Find_powersupply::log(const QString &text)
{
    qDebug() << text;
}
//--------------------------------------------------------------------------------
void Find_powersupply::init(void)
{
    connect_log();

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->setMargin(0);
    hbox->setSpacing(0);

    cb_port_list = new QComboBox(this);

    pb_find = new QPushButton(this);
    pb_find->setText("find");
    pb_find->setObjectName("pb_find");

    pb_update_port_list = new QPushButton(this);
    pb_update_port_list->setText("update");
    pb_update_port_list->setObjectName("pb_update_port_list");

    hbox->addWidget(pb_update_port_list);
    hbox->addWidget(cb_port_list);
    hbox->addWidget(pb_find);

    connect(pb_find, SIGNAL(clicked(bool)), this, SLOT(find()));
    connect(pb_update_port_list, SIGNAL(clicked(bool)), this, SLOT(update()));

    update();

    setLayout(hbox);
    setFixedHeight(sizeHint().height());
    setMinimumWidth(sizeHint().width() + 50);
}
//--------------------------------------------------------------------------------
void Find_powersupply::find(void)
{
    //powersupply->print_last_error();
    QStandardItemModel *model = (QStandardItemModel *)cb_port_list->model();
    for (int i=0; i<cb_port_list->count(); i++)
    {
        if(model->item(i)->checkState() == Qt::Checked)
        {
            qDebug() << model->item(i)->text();
        }
    }
}
//--------------------------------------------------------------------------------
void Find_powersupply::update(void)
{
    cb_port_list->clear();
    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
        cb_port_list->addItem(port.portName());
    }

    QStandardItemModel *model = new QStandardItemModel();
    for (int i=0; i<cb_port_list->count(); i++)
    {
        QStandardItem *item = new QStandardItem();
        item->setText(cb_port_list->itemText(i));
        item->setCheckable(true);
        item->setCheckState(Qt::Checked);
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        model->setItem(i, item);
    }

    cb_port_list->setModel(model);
}
//--------------------------------------------------------------------------------

