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
#include "file_widget.hpp"
#include "ui_file_widget.h"
//--------------------------------------------------------------------------------
File_widget::File_widget(QWidget *parent) :
    Default_widget(parent),
    ui(new Ui::File_widget)
{
    init();;
}
//--------------------------------------------------------------------------------
File_widget::~File_widget()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void File_widget::init(void)
{
    ui->setupUi(this);

    file_model = new QFileSystemModel(this);
    file_model->setNameFilters(QStringList() << "*");
    file_model->setNameFilterDisables(false);

#if 0
    file_model->setHeaderData(0, Qt::Horizontal, "a0");
    file_model->setHeaderData(1, Qt::Horizontal, "a1");
    file_model->setHeaderData(2, Qt::Horizontal, "a2");
    file_model->setHeaderData(3, Qt::Horizontal, "a3");
#endif

    ui->tree->setModel(file_model);

#if 1
//    int w = ui->tree->width();
//    emit info(QString("w: %1").arg(w));
    ui->tree->setColumnWidth(0, 300);
    ui->tree->setColumnWidth(1, 100);
    ui->tree->setColumnWidth(2, 100);
    ui->tree->setColumnWidth(3, 100);
#endif

#if 0
    ui->tree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tree->header()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->tree->header()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->tree->header()->setSectionResizeMode(3, QHeaderView::Fixed);
#endif
}
//--------------------------------------------------------------------------------
