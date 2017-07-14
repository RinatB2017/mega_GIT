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
#include "printbox.hpp"
#include "ui_printbox.h"
#include "defines.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
PrintBox::PrintBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrintBox)
{
    init();

#ifdef QT_DEBUG
    QList<QWidget *> all_obj = findChildren<QWidget *>();
    foreach(QWidget *obj, all_obj)
    {
        if(obj->objectName().isEmpty() == false)
        {
            obj->setToolTip(obj->objectName());
        }
    }
#endif
}
//--------------------------------------------------------------------------------
PrintBox::~PrintBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void PrintBox::init(void)
{
    ui->setupUi(this);
    if(parentWidget())
    {
        connect(this, SIGNAL(info(QString)),    parentWidget(), SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)),   parentWidget(), SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)),   parentWidget(), SIGNAL(error(QString)));
        connect(this, SIGNAL(trace(QString)),   parentWidget(), SIGNAL(trace(QString)));
    }

    connect(ui->btn_choose_printer, SIGNAL(clicked(bool)), this, SLOT(btn_choose_printer_click()));
}
//--------------------------------------------------------------------------------
void PrintBox::btn_choose_printer_click(void)
{
    choose_printer();
}
//--------------------------------------------------------------------------------
void PrintBox::choose_printer(void)
{
    emit error(TODODO);
    MyWidget::messagebox_critical("Ошибка", TODODO);
}
//--------------------------------------------------------------------------------
