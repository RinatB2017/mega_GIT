/*********************************************************************************
**                                                                              **
**     Copyright (C) 2021                                                       **
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
#include "table_widget.hpp"
#include "ui_table_widget.h"
//--------------------------------------------------------------------------------
Table_widget::Table_widget(QWidget *parent) :
    Default_widget(parent),
    ui(new Ui::Table_widget)
{
    init();;
}
//--------------------------------------------------------------------------------
Table_widget::~Table_widget()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Table_widget::init(void)
{
    ui->setupUi(this);

    QStandardItemModel *text_model = new QStandardItemModel(0, 2, this);
    text_model->setHeaderData(0, Qt::Horizontal, "Test");
    text_model->setHorizontalHeaderLabels(QStringList() << "Text" << "N");

    for(int n=0; n<10; n++)
    {
        text_model->insertRow(n);
        text_model->setData(text_model->index(n, 0, QModelIndex()), "text");
        text_model->setData(text_model->index(n, 1, QModelIndex()), n);
    }

    ui->tableView->setModel(text_model);
}
//--------------------------------------------------------------------------------
