/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#include "testwidget.hpp"
#include "ui_testwidget.h"
//--------------------------------------------------------------------------------
TestWidget::TestWidget(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::TestWidget)
{
    ui->setupUi(this);

    connect(ui->btn_run, &QToolButton::clicked, [this]() {
        QList<QWidget *> widgets = findChildren<QWidget *>();
        emit info(QString("Widgets found %1").arg(widgets.count()));
        foreach (QWidget *widget, widgets)
        {
            emit info(widget->objectName());
        }
    });

    load_widgets();
}
//--------------------------------------------------------------------------------
TestWidget::~TestWidget()
{
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void TestWidget::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool TestWidget::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void TestWidget::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void TestWidget::save_setting(void)
{

}
//--------------------------------------------------------------------------------
