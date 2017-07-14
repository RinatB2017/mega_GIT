/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#include <QTreeWidgetItem>
#include <QToolButton>
#include <QGridLayout>
#include <QSpinBox>
#include <QTimer>
//--------------------------------------------------------------------------------
#include "filterbox.hpp"
#include "ui_filterbox.h"
//--------------------------------------------------------------------------------
#include "pet.h"
//--------------------------------------------------------------------------------
FilterBox::FilterBox(Client *client, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterBox),
    parent(parent),
    client(client)
{
    init();
}
//--------------------------------------------------------------------------------
FilterBox::~FilterBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void FilterBox::init(void)
{
    ui->setupUi(this);
    if(parent)
    {
        connect(this, SIGNAL(info(QString)),    parent, SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)),   parent, SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)),   parent, SIGNAL(error(QString)));
        connect(this, SIGNAL(message(QString)), parent, SIGNAL(message(QString)));
    }

    ui->tw_filter_params->setRowCount(50);
    ui->tw_filter_params->setColumnCount(2);
    ui->tw_filter_params->setHorizontalHeaderLabels(QStringList() << "xxx" << "yyy");
    for(int y=0; y<50; y++)
    {
        for(int x=0; x<2; x++)
        {
            QTableWidgetItem *newItem = new QTableWidgetItem(tr("item %1").arg((y)*(x)));
            ui->tw_filter_params->setItem(y, x, newItem);
        }
    }

    setWindowTitle(tr("Фильтр"));
}
//--------------------------------------------------------------------------------
void FilterBox::block_this_button(bool state)
{
    QObject *btn = (QObject *)sender();
    if(!btn) return;

    if (QPushButton *pb=qobject_cast<QPushButton *>(btn))
    {
        pb->setDisabled(state);
    }
    if (QToolButton *tb=qobject_cast<QToolButton *>(btn))
    {
        tb->setDisabled(state);
    }
}
//--------------------------------------------------------------------------------
void FilterBox::block_interface(bool state)
{
    QList<QPushButton *> all_pushbutton = topLevelWidget()->findChildren<QPushButton *>();
    foreach(QPushButton *obj, all_pushbutton)
    {
        obj->setDisabled(state);
    }
}
//--------------------------------------------------------------------------------
