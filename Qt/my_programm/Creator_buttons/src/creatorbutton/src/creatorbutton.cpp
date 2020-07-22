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
#include "myfiledialog.hpp"
#include "ui_creatorbutton.h"
#include "defines.hpp"
//--------------------------------------------------------------------------------
CreatorButton::CreatorButton(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::CreatorButton)
{
    ui->setupUi(this);

    QTimer::singleShot(0, [this]{
        init();
    });
}
//--------------------------------------------------------------------------------
CreatorButton::~CreatorButton()
{
    save_widgets();

    save_value(S_ELLIPSE_BORDER_COLOR,      ui->ellipse_widget->get_border_color());
    save_value(S_RECTANGLE_BORDER_COLOR,      ui->rectangle_widget->get_border_color());
    save_value(S_ARROW_LEFT_BORDER_COLOR,      ui->arrow_left_widget->get_border_color());
    save_value(S_ARROW_RIGHT_BORDER_COLOR,      ui->arrow_right_widget->get_border_color());

    delete ui;
}
//--------------------------------------------------------------------------------
void CreatorButton::init(void)
{
    load_widgets();

    ui->ellipse_widget->set_border_color(load_value(S_ELLIPSE_BORDER_COLOR).value<QColor>());
    ui->rectangle_widget->set_border_color(load_value(S_RECTANGLE_BORDER_COLOR).value<QColor>());
    ui->arrow_left_widget->set_border_color(load_value(S_ARROW_LEFT_BORDER_COLOR).value<QColor>());
    ui->arrow_right_widget->set_border_color(load_value(S_ARROW_RIGHT_BORDER_COLOR).value<QColor>());

    ui->ellipse_widget->set_caption("Ellipse");
    ui->rectangle_widget->set_caption("Rectangle");
    ui->arrow_left_widget->set_caption("Arrow left");
    ui->arrow_right_widget->set_caption("Arrow right");

    connect(ui->btn_background_color,               &QPushButton::clicked,  this,   &CreatorButton::set_background_color);
    connect(ui->cb_background_color_transparent,    &QCheckBox::stateChanged,  this,    &CreatorButton::create_picture);

    connect(ui->ellipse_widget, &PictureParam::picture_update,  this,   &CreatorButton::create_picture);

    create_picture();
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
    emit trace(Q_FUNC_INFO);

    int w = get_width();
    int h = get_height();

    QPixmap *pixmap = new QPixmap(w, h);
    QPen *pen = new QPen();

    int border = 0;

    int offset_t = 0;
    int offset_b = 0;
    int offset_l = 0;
    int offset_r = 0;

    //---
    QPainter painter;
    painter.begin(pixmap);

//    const QPixmap *p = ui->lbl_picture->pixmap();
//    if(p)
//    {
//        QRgb rgb = ui->lbl_picture->pixmap()->toImage().pixel(ui->lbl_picture->width() / 2,
//                                                              ui->lbl_picture->height() / 2);
//        pixmap->fill(QColor(rgb));
//        pixmap->fill(Qt::lightGray);
//    }

//    pixmap->fill(Qt::transparent);
//    pixmap->fill(qRgba(255, 255, 255, 255));
//    pixmap->fill(qRgb(255, 255, 255));

    if(ui->cb_background_color_transparent->isChecked())
    {
        pixmap->fill(QColor(Qt::transparent));
    }
    else
    {
        pixmap->fill(background_color);
    }

    if(ui->ellipse_widget->get_active())
    {
        border = ui->ellipse_widget->get_border();
        if(border < 0) border = 0;

        if(ui->ellipse_widget->get_border_color_transparent() == false)
        {
            pen->setColor(ui->ellipse_widget->get_border_color());
        }

        pen->setWidth(border);
        pen->setStyle(Qt::SolidLine);
        painter.setPen(*pen);

        offset_t = ui->ellipse_widget->get_margen_t();
        offset_b = ui->ellipse_widget->get_margen_b();
        offset_l = ui->ellipse_widget->get_margen_l();
        offset_r = ui->ellipse_widget->get_margen_r();

        painter.drawEllipse(border / 2 + offset_l,
                            border / 2 + offset_t,
                            w - border - (offset_r + offset_r),
                            h - border - (offset_t + offset_b));
    }

    if(ui->rectangle_widget->get_active())
    {
        border = ui->rectangle_widget->get_border();
        if(border < 0) border = 0;

        if(ui->rectangle_widget->get_border_color_transparent() == false)
        {
            pen->setColor(ui->rectangle_widget->get_border_color());
        }

        pen->setWidth(border);
        pen->setStyle(Qt::SolidLine);
        painter.setPen(*pen);

        offset_t = ui->rectangle_widget->get_margen_t();
        offset_b = ui->rectangle_widget->get_margen_b();
        offset_l = ui->rectangle_widget->get_margen_l();
        offset_r = ui->rectangle_widget->get_margen_r();

        painter.drawRect(border / 2 + offset_l,
                         border / 2 + offset_t,
                         w - border - (offset_r + offset_r),
                         h - border - (offset_t + offset_b));
    }

    if(ui->arrow_left_widget->get_active())
    {
        border = ui->arrow_left_widget->get_border();
        if(border < 0) border = 0;

        if(ui->arrow_left_widget->get_border_color_transparent() == false)
        {
            pen->setColor(ui->arrow_left_widget->get_border_color());
        }
        pen->setWidth(border);
        pen->setStyle(Qt::SolidLine);
        painter.setPen(*pen);

        offset_t = ui->arrow_left_widget->get_margen_t();
        offset_b = ui->arrow_left_widget->get_margen_b();
        offset_l = ui->arrow_left_widget->get_margen_l();
        offset_r = ui->arrow_left_widget->get_margen_r();

        painter.drawLine(border / 2 + offset_l,
                         h / 2 + offset_t,
                         w - border - (offset_r + offset_r),
                         border / 2 - (offset_t + offset_b));
        painter.drawLine(border / 2 + offset_l,
                         h / 2 + offset_t,
                         w - border - (offset_r + offset_r),
                         h - border - (offset_t + offset_b));
        painter.drawLine(w - border + offset_l,
                         border / 2 + offset_t,
                         w - border - (offset_r + offset_r),
                         h - border - (offset_t + offset_b));
    }

    if(ui->arrow_right_widget->get_active())
    {
        border = ui->arrow_right_widget->get_border();
        if(border < 0) border = 0;

        if(ui->arrow_right_widget->get_border_color_transparent() == false)
        {
            pen->setColor(ui->arrow_right_widget->get_border_color());
        }
        pen->setWidth(border);
        pen->setStyle(Qt::SolidLine);
        painter.setPen(*pen);

        offset_t = ui->arrow_right_widget->get_margen_t();
        offset_b = ui->arrow_right_widget->get_margen_b();
        offset_l = ui->arrow_right_widget->get_margen_l();
        offset_r = ui->arrow_right_widget->get_margen_r();

        painter.drawLine(border / 2 + offset_l,
                         border / 2 + offset_t,
                         w - border - (offset_r + offset_r),
                         h / 2 - (offset_t + offset_b));
        painter.drawLine(border / 2 + offset_l,
                         h - border + offset_t,
                         w - border - (offset_r + offset_r),
                         h / 2 - (offset_t + offset_b));
        painter.drawLine(border / 2 + offset_l,
                         border / 2 + offset_t,
                         border / 2 - (offset_r + offset_r),
                         h - border - (offset_t + offset_b));
    }
    //---

    ui->lbl_picture->setPixmap(*pixmap);
}
//--------------------------------------------------------------------------------
void CreatorButton::set_color(QToolButton *btn, QColor color)
{
    QString color_str = QString("background:#%1%2%3")
            .arg(color.red(),     2, 16, QChar('0'))
            .arg(color.green(),   2, 16, QChar('0'))
            .arg(color.blue(),    2, 16, QChar('0'));
    btn->setStyleSheet(color_str);
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
        setProperty("border_color", background_color);
        QToolButton *btn = static_cast<QToolButton *>(sender());
        if(btn)
        {
            set_color(btn, background_color);
        }
    }
    dlg_color->deleteLater();
}
//--------------------------------------------------------------------------------
void CreatorButton::save_picture_to(void){
    MyFileDialog *dlg = new MyFileDialog("creator_button", "creator_button", this);
    dlg->setAcceptMode(QFileDialog::AcceptSave);
    dlg->setOption(QFileDialog::DontConfirmOverwrite, false);
    dlg->setNameFilter("PNG files (*.png)");
    dlg->selectFile("noname");
    dlg->setDefaultSuffix("png");
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    int btn = dlg->exec();
    if(btn == MyFileDialog::Accepted)
    {
        QStringList files = dlg->selectedFiles();
        QString filename = files.at(0);

        ui->lbl_picture->pixmap(Qt::ReturnByValue).save(filename);
    }
}
//--------------------------------------------------------------------------------
void CreatorButton::load_setting(void)
{
    emit debug("load_setting !");

    ui->sb_w->setRange(MIN_WIDTH,   MAX_WIDTH);
    ui->sb_h->setRange(MIN_HEIGHT,  MAX_HEIGHT);

    set_width(load_value(S_SIZE_W).toInt());
    set_height(load_value(S_SIZE_H).toInt());

    connect(ui->sb_w,   SIGNAL(valueChanged(int)),    this,   SLOT(create_picture()));
    connect(ui->sb_h,   SIGNAL(valueChanged(int)),    this,   SLOT(create_picture()));

    connect(ui->ellipse_widget,     &PictureParam::picture_update,  this,   &CreatorButton::create_picture);
    connect(ui->rectangle_widget,   &PictureParam::picture_update,  this,   &CreatorButton::create_picture);
    connect(ui->arrow_left_widget,  &PictureParam::picture_update,  this,   &CreatorButton::create_picture);
    connect(ui->arrow_right_widget, &PictureParam::picture_update,  this,   &CreatorButton::create_picture);

    connect(ui->btn_create,     &QPushButton::clicked,  this,   &CreatorButton::create_picture);
    connect(ui->btn_save_to,    &QPushButton::clicked,  this,   &CreatorButton::save_picture_to);
}
//--------------------------------------------------------------------------------
void CreatorButton::save_setting(void)
{
    emit debug("save_setting !");
}
//--------------------------------------------------------------------------------
