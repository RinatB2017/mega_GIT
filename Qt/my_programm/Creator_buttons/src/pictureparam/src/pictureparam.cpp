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
#include "pictureparam.hpp"
#include "ui_pictureparam.h"
#include "defines.hpp"
//--------------------------------------------------------------------------------
PictureParam::PictureParam(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::PictureParam)
{
    init();
}
//--------------------------------------------------------------------------------
PictureParam::~PictureParam()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void PictureParam::init(void)
{
    ui->setupUi(this);

    ui->sb_margin_t->setRange(0, MAX_HEIGHT / 2);
    ui->sb_margin_b->setRange(0, MAX_HEIGHT / 2);
    ui->sb_margin_l->setRange(0, MAX_WIDTH / 2);
    ui->sb_margin_r->setRange(0, MAX_WIDTH / 2);

    ui->sb_border->setRange(0, MAX_WIDTH / 2);

    connect(ui->sb_margin_t,    static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  this,   &PictureParam::picture_update);
    connect(ui->sb_margin_b,    static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  this,   &PictureParam::picture_update);
    connect(ui->sb_margin_l,    static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  this,   &PictureParam::picture_update);
    connect(ui->sb_margin_r,    static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  this,   &PictureParam::picture_update);

    connect(ui->cb_active,                          static_cast<void (QCheckBox::*)(int)>(&QCheckBox::stateChanged),  this,   &PictureParam::picture_update);
    connect(ui->cb_border_color_transparent,        static_cast<void (QCheckBox::*)(int)>(&QCheckBox::stateChanged),  this,   &PictureParam::picture_update);

    connect(ui->cb_border_color_transparent,        &QCheckBox::stateChanged,  ui->btn_border_color,        &QCheckBox::setDisabled);

    connect(ui->sb_border,      static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  this,   &PictureParam::picture_update);

    connect(ui->btn_border_color,       &QPushButton::clicked,  this,   static_cast<void (PictureParam::*)(void)>(&PictureParam::set_border_color));
}
//--------------------------------------------------------------------------------
void PictureParam::set_caption(const QString &caption)
{
    emit info(caption);
    ui->gb_picture->setTitle(caption);
}
//--------------------------------------------------------------------------------
int PictureParam::get_margen_t(void)
{
    return ui->sb_margin_t->value();
}
//--------------------------------------------------------------------------------
int PictureParam::get_margen_b(void)
{
    return ui->sb_margin_b->value();
}
//--------------------------------------------------------------------------------
int PictureParam::get_margen_l(void)
{
    return ui->sb_margin_l->value();
}
//--------------------------------------------------------------------------------
int PictureParam::get_margen_r(void)
{
    return ui->sb_margin_r->value();
}
//--------------------------------------------------------------------------------
int PictureParam::get_border(void)
{
    return ui->sb_border->value();
}
//--------------------------------------------------------------------------------
bool PictureParam::get_active(void)
{
    return ui->cb_active->isChecked();
}
//--------------------------------------------------------------------------------
bool PictureParam::get_border_color_transparent(void)
{
    return ui->cb_border_color_transparent->isChecked();
}
//--------------------------------------------------------------------------------
QColor PictureParam::get_border_color(void)
{
    return border_color;
}
//--------------------------------------------------------------------------------
void PictureParam::set_color(QToolButton *btn, QColor color)
{
    QString color_str = QString("background:#%1%2%3")
            .arg(color.red(),     2, 16, QChar('0'))
            .arg(color.green(),   2, 16, QChar('0'))
            .arg(color.blue(),    2, 16, QChar('0'));
    btn->setStyleSheet(color_str);
}
//--------------------------------------------------------------------------------
void PictureParam::set_border_color(void)
{
    QColorDialog *dlg_color = new QColorDialog();
    dlg_color->setCurrentColor(border_color);
    int btn = dlg_color->exec();
    if(btn == QColorDialog::Accepted)
    {
        border_color = dlg_color->currentColor();
        setProperty("border_color", border_color);
        QToolButton *btn = static_cast<QToolButton *>(sender());
        if(btn)
        {
            set_color(btn, border_color);
            emit picture_update();
        }
    }
    dlg_color->deleteLater();
}
//--------------------------------------------------------------------------------
void PictureParam::set_border_color(QColor color)
{
    border_color = color;
    setProperty("border_color", border_color);

    set_color(ui->btn_border_color, border_color);
    emit picture_update();
}
//--------------------------------------------------------------------------------
void PictureParam::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool PictureParam::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void PictureParam::load_setting(void)
{
    emit debug("load_setting !");
}
//--------------------------------------------------------------------------------
void PictureParam::save_setting(void)
{
    emit debug("save_setting !");
}
//--------------------------------------------------------------------------------
