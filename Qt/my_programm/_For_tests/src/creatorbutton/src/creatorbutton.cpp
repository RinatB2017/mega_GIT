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
#include "creatorbutton.hpp"
#include "ui_creatorbutton.h"
//--------------------------------------------------------------------------------
CreatorButton::CreatorButton(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::CreatorButton)
{
    init();
}
//--------------------------------------------------------------------------------
CreatorButton::~CreatorButton()
{
    save_setting();
    delete ui;
}
//--------------------------------------------------------------------------------
void CreatorButton::init(void)
{
    ui->setupUi(this);

    load_setting();
}
//--------------------------------------------------------------------------------
void CreatorButton::set_width(int width)
{
    if(width < MIN_WIDTH)  width = MIN_WIDTH;
    if(width > MAX_WIDTH)  width = MAX_WIDTH;
    ui->sb_w->setValue(width);
}
//--------------------------------------------------------------------------------
int CreatorButton::get_width(void)
{
    return ui->sb_w->value();
}
//--------------------------------------------------------------------------------
void CreatorButton::set_height(int height)
{
    if(height < MIN_HEIGHT)  height = MIN_HEIGHT;
    if(height > MAX_HEIGHT)  height = MAX_HEIGHT;
    ui->sb_h->setValue(height);
}
//--------------------------------------------------------------------------------
int CreatorButton::get_height(void)
{
    return ui->sb_h->value();
}
//--------------------------------------------------------------------------------
void CreatorButton::set_border(int size)
{
    ui->sb_border_w->setValue(size);
}
//--------------------------------------------------------------------------------
int CreatorButton::get_border(void)
{
    return ui->sb_border_w->value();
}
//--------------------------------------------------------------------------------
void CreatorButton::set_border_color(void)
{
    QColorDialog *dlg_color = new QColorDialog();
    dlg_color->setCurrentColor(border_color);
    int btn = dlg_color->exec();
    if(btn == QColorDialog::Accepted)
    {
        border_color = dlg_color->currentColor();
        QToolButton *btn = static_cast<QToolButton *>(sender());
        if(btn)
        {
            set_color(btn, border_color);
        }
    }
    dlg_color->deleteLater();
}
//--------------------------------------------------------------------------------
void CreatorButton::set_background_color(void)
{
    QColorDialog *dlg_color = new QColorDialog();
    dlg_color->setCurrentColor(background_color);
    int btn = dlg_color->exec();
    if(btn == QColorDialog::Accepted)
    {
        background_color = dlg_color->currentColor();
        QToolButton *btn = static_cast<QToolButton *>(sender());
        if(btn)
        {
            set_color(btn, background_color);
        }
    }
    dlg_color->deleteLater();
}
//--------------------------------------------------------------------------------
void CreatorButton::set_color(QToolButton *btn, QColor color)
{
    QString color_str = QString("background:#%1%2%3")
            .arg(color.red(),     2, 16, QChar('0'))
            .arg(color.green(),   2, 16, QChar('0'))
            .arg(color.blue(),    2, 16, QChar('0'));
    emit debug(color_str);
    btn->setStyleSheet(color_str);
}
//--------------------------------------------------------------------------------
void CreatorButton::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool CreatorButton::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void CreatorButton::create_picture(void)
{
    int w = get_width();
    int h = get_height();

    QPixmap *pixmap = new QPixmap(w, h);

    int border = get_border();
    if(border <= 0) border = 1;
    QPen *pen = new QPen(Qt::red, border, Qt::SolidLine);

    pixmap->fill(Qt::white);
//    pixmap->fill(Qt::transparent);

    //---
    QPainter painter;
    painter.begin(pixmap);
    painter.drawLine(0, 0, w, h);
    painter.setPen(*pen);
    painter.drawEllipse(border / 2,
                        border / 2,
                        w - border,
                        h - border);
    //---

    ui->lbl_picture->setPixmap(*pixmap);
}
//--------------------------------------------------------------------------------
void CreatorButton::save_picture_to(void)
{
    emit error("Пока не сделано!");
}
//--------------------------------------------------------------------------------
void CreatorButton::load_setting(void)
{
    emit debug("load_setting !");

    ui->sb_w->setRange(MIN_WIDTH,   MAX_WIDTH);
    ui->sb_h->setRange(MIN_HEIGHT,  MAX_HEIGHT);

    ui->sb_border_w->setRange(0, MAX_WIDTH / 2);

    ui->sb_margin_t->setRange(0, MAX_HEIGHT / 2);
    ui->sb_margin_b->setRange(0, MAX_HEIGHT / 2);
    ui->sb_margin_l->setRange(0, MAX_WIDTH / 2);
    ui->sb_margin_r->setRange(0, MAX_WIDTH / 2);

    set_width(load_value("size_w").toInt());
    set_height(load_value("size_h").toInt());
    set_border(load_value("size_border").toInt());

    border_color = load_value("border_color").value<QColor>();  //TODO интересный способ, надо запомнить
    background_color = load_value("background_color").value<QColor>();

    set_color(ui->btn_border_color,     border_color);
    set_color(ui->btn_background_color, background_color);

    connect(ui->sb_w,   SIGNAL(valueChanged(int)),    this,   SLOT(create_picture()));
    connect(ui->sb_h,   SIGNAL(valueChanged(int)),    this,   SLOT(create_picture()));
    connect(ui->sb_border_w,    SIGNAL(valueChanged(int)),    this,   SLOT(create_picture()));
    connect(ui->sb_margin_t,    SIGNAL(valueChanged(int)),    this,   SLOT(create_picture()));
    connect(ui->sb_margin_b,    SIGNAL(valueChanged(int)),    this,   SLOT(create_picture()));
    connect(ui->sb_margin_l,    SIGNAL(valueChanged(int)),    this,   SLOT(create_picture()));
    connect(ui->sb_margin_r,    SIGNAL(valueChanged(int)),    this,   SLOT(create_picture()));

    connect(ui->btn_border_color,       &QPushButton::clicked,  this,   &CreatorButton::set_border_color);
    connect(ui->btn_background_color,   &QPushButton::clicked,  this,   &CreatorButton::set_background_color);

    connect(ui->cb_border_color_transparent,        &QCheckBox::stateChanged,   ui->btn_border_color,       &QToolButton::setDisabled);
    connect(ui->cb_background_color_transparent,    &QCheckBox::stateChanged,   ui->btn_background_color,   &QToolButton::setDisabled);

    connect(ui->btn_create,     &QPushButton::clicked,  this,   &CreatorButton::create_picture);
    connect(ui->btn_save_to,    &QPushButton::clicked,  this,   &CreatorButton::save_picture_to);
}
//--------------------------------------------------------------------------------
void CreatorButton::save_setting(void)
{
    emit debug("save_setting !");

    save_value("size_w", ui->sb_w->value());
    save_value("size_h", ui->sb_h->value());
    save_value("size_border",   ui->sb_border_w->value());
    save_value("border_color",      border_color);
    save_value("background_color",  background_color);
}
//--------------------------------------------------------------------------------
