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
#include "resizewidget.hpp"
#include "ui_resizewidget.h"
//--------------------------------------------------------------------------------
ResizeWidget::ResizeWidget(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::ResizeWidget)
{
    ui->setupUi(this);

    ui->label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->label->setScaledContents(true);
}
//--------------------------------------------------------------------------------
ResizeWidget::~ResizeWidget()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void ResizeWidget::resizeEvent(QResizeEvent *event)
{
    QPixmap pixmap(":/logo/logo.png");
    //    QPixmap new_pixmap = pixmap.scaled(event->size());
    //    ui->label->setPixmap(new_pixmap);
    //    ui->label->resize(event->size());

    emit info(QString("%1 : %2")
              .arg(event->size().width())
              .arg(event->size().height()));

#if 0
    QWidget::resizeEvent(event);
    ui->label->setPixmap(pixmap.scaled(event->size().width(),
                                       event->size().height(),
                                       Qt::KeepAspectRatio));
#endif

#if 0
    ui->label->resize(event->size());
#endif

#if 0
    QPixmap temp_pixmap = pixmap.copy(0,
                                      0,
                                      event->size().width(),
                                      event->size().height());
    ui->label->setPixmap(temp_pixmap);
#endif
}
//--------------------------------------------------------------------------------
void ResizeWidget::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool ResizeWidget::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void ResizeWidget::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void ResizeWidget::save_setting(void)
{

}
//--------------------------------------------------------------------------------
