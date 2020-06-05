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
#include "croppicture.hpp"
#include "ui_croppicture.h"
//--------------------------------------------------------------------------------
CropPicture::CropPicture(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::CropPicture)
{
    ui->setupUi(this);

    init();
}
//--------------------------------------------------------------------------------
CropPicture::~CropPicture()
{
    save_value("inc_value", ui->sb_inc->value());
    delete ui;
}
//--------------------------------------------------------------------------------
void CropPicture::init(void)
{
    ui->sb_inc->setRange(1, 100);
    ui->sb_inc->setValue(load_value("inc_value").toInt());

    connect(ui->btn_up,         &QToolButton::clicked,  this,   &CropPicture::up);
    connect(ui->btn_left,       &QToolButton::clicked,  this,   &CropPicture::left);
    connect(ui->btn_plus,       &QToolButton::clicked,  this,   &CropPicture::plus);
    connect(ui->btn_minus,      &QToolButton::clicked,  this,   &CropPicture::minus);
    connect(ui->btn_right,      &QToolButton::clicked,  this,   &CropPicture::right);
    connect(ui->btn_down,       &QToolButton::clicked,  this,   &CropPicture::down);
    connect(ui->btn_up_left,    &QToolButton::clicked,  this,   &CropPicture::up_left);
    connect(ui->btn_down_right, &QToolButton::clicked,  this,   &CropPicture::down_right);
}
//--------------------------------------------------------------------------------
void CropPicture::up(void)
{

}
//--------------------------------------------------------------------------------
void CropPicture::down(void)
{

}
//--------------------------------------------------------------------------------
void CropPicture::left(void)
{

}
//--------------------------------------------------------------------------------
void CropPicture::right(void)
{

}
//--------------------------------------------------------------------------------
void CropPicture::up_left(void)
{

}
//--------------------------------------------------------------------------------
void CropPicture::down_right(void)
{

}
//--------------------------------------------------------------------------------
void CropPicture::plus(void)
{
    pixmap_width  += ui->sb_inc->value();
    pixmap_height += ui->sb_inc->value();

    ui->picture_label->setPixmap(orig_pixmap.scaled(pixmap_width,
                                                    pixmap_height,
                                                    Qt::KeepAspectRatio));
}
//--------------------------------------------------------------------------------
void CropPicture::minus(void)
{
    pixmap_width  -= ui->sb_inc->value();
    pixmap_height -= ui->sb_inc->value();

    ui->picture_label->setPixmap(orig_pixmap.scaled(pixmap_width,
                                                    pixmap_height,
                                                    Qt::KeepAspectRatio));
}
//--------------------------------------------------------------------------------
bool CropPicture::load_pixmap(QPixmap pixmap)
{
    orig_pixmap = pixmap;
    pixmap_width  = pixmap.width();
    pixmap_height = pixmap.height();

    ui->picture_label->setPixmap(pixmap);
    return true;
}
//--------------------------------------------------------------------------------
void CropPicture::resizeEvent(QResizeEvent *event)
{
    emit info(QString("%1 %2")
              .arg(event->size().width())
              .arg(event->size().height()));
}
//--------------------------------------------------------------------------------
void CropPicture::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool CropPicture::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void CropPicture::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void CropPicture::save_setting(void)
{

}
//--------------------------------------------------------------------------------
