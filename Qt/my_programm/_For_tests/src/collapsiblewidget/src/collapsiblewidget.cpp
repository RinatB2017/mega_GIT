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
#include "collapsiblewidget.hpp"
#include "ui_collapsiblewidget.h"
//--------------------------------------------------------------------------------
CollapsibleWidget::CollapsibleWidget(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::CollapsibleWidget)
{
    init();
}
//--------------------------------------------------------------------------------
CollapsibleWidget::~CollapsibleWidget()
{
    if(timer)
    {
        timer->disconnect();
        timer->deleteLater();
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void CollapsibleWidget::init(void)
{
    ui->setupUi(this);

    init_timer();
    adjustSize();

    max_time_msec = 1000;
    max_size = ui->main_frame->height();
    //inc_height = static_cast<qreal>(max_size) / static_cast<qreal>(max_time_msec);
    inc_height = 100;  //TODO для теста

    emit info(QString("max_size %1").arg(max_size));
    emit info(QString("inc_height %1").arg(inc_height));

    connect(ui->btn_collapse,   &QToolButton::clicked,  this, &CollapsibleWidget::collapse);
    ui->btn_collapse->setProperty(NO_SAVE, true);
    ui->main_frame->setFixedSize(ui->main_frame->width(), 0);
}
//--------------------------------------------------------------------------------
void CollapsibleWidget::init_timer(void)
{
    timer = new QTimer(this);
    connect(timer,  &QTimer::timeout,   this,   &CollapsibleWidget::update);
}
//--------------------------------------------------------------------------------
void CollapsibleWidget::collapse(bool new_state)
{
    //ui->main_frame->setVisible(state);
    emit info(QString("inc_height %1").arg(inc_height));
    if(new_state)
    {
        state = COLLAPSE_ON;
    }
    else
    {
        state = COLLAPSE_OFF;
    }
    timer->start(1000);
}
//--------------------------------------------------------------------------------
void CollapsibleWidget::update(void)
{
    int new_height = 0;
    switch(state)
    {
    case COLLAPSE_NONE:
        //emit info("COLLAPSE_NONE");
        break;

    case COLLAPSE_ON:
        emit info("COLLAPSE_ON");
        new_height = ui->main_frame->height() + inc_height;
        emit info(QString("new_height %1").arg(new_height));
        ui->main_frame->setFixedSize(ui->main_frame->width(),
                                     new_height);
        if(ui->main_frame->height() >= max_size)
        {
            state = COLLAPSE_NONE;
        }
        break;

    case COLLAPSE_OFF:
        emit info("COLLAPSE_OFF");
        new_height = ui->main_frame->height() - inc_height;
        emit info(QString("new_height %1").arg(new_height));
        if(new_height < 0)
        {
            state = COLLAPSE_NONE;
            break;
        }
        ui->main_frame->setFixedSize(ui->main_frame->width(),
                                     new_height);
        break;

    default:
        state = COLLAPSE_NONE;
        break;
    }
}
//--------------------------------------------------------------------------------
void CollapsibleWidget::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool CollapsibleWidget::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void CollapsibleWidget::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void CollapsibleWidget::save_setting(void)
{

}
//--------------------------------------------------------------------------------
