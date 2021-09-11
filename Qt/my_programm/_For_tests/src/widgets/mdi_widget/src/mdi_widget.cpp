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
#include "mdi_widget.hpp"
#include "ui_mdi_widget.h"
//--------------------------------------------------------------------------------
MDI_widget::MDI_widget(QWidget *parent) :
    Default_widget(parent),
    ui(new Ui::MDI_widget)
{
    init();;
}
//--------------------------------------------------------------------------------
MDI_widget::~MDI_widget()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MDI_widget::init(void)
{
    ui->setupUi(this);

    mdi_widget_0 = new QLineEdit(this);
    mdi_widget_0->setObjectName("widget_0");
    mdi_widget_0->setAttribute(Qt::WA_DeleteOnClose);

    mdi_widget_1 = new QLineEdit(this);
    mdi_widget_1->setObjectName("widget_1");
    mdi_widget_1->setAttribute(Qt::WA_DeleteOnClose);

    mdi_widget_2 = new QLineEdit(this);
    mdi_widget_2->setObjectName("widget_2");
    mdi_widget_2->setAttribute(Qt::WA_DeleteOnClose);

    w0 = ui->mdiArea->addSubWindow(mdi_widget_0);
    w1 = ui->mdiArea->addSubWindow(mdi_widget_1);
    w2 = ui->mdiArea->addSubWindow(mdi_widget_2);

    w0->setObjectName("mdi_widget0");
    w1->setObjectName("mdi_widget1");
    w2->setObjectName("mdi_widget2");
}
//--------------------------------------------------------------------------------
