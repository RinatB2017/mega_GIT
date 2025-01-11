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
#include "hsv_widget.hpp"
#include "ui_hsv_widget.h"
//--------------------------------------------------------------------------------
HSV_widget::HSV_widget(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::HSV_widget)
{
    init();
}
//--------------------------------------------------------------------------------
HSV_widget::~HSV_widget()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void HSV_widget::init(void)
{
    ui->setupUi(this);

    ui->picture_label->setMinimumSize(M_WIDTH, M_HEIGHT);

    ui->sb_saturation->setRange(-255, 255);
    ui->sb_value->setRange(-255, 255);

    ui->btn_load->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogOpenButton));
    ui->btn_saturation->setIcon(qApp->style()->standardIcon(QStyle::SP_CommandLink));
    ui->btn_value->setIcon(qApp->style()->standardIcon(QStyle::SP_CommandLink));

    connect(ui->btn_load,       &QToolButton::clicked,  this,   &HSV_widget::load_picture);
    connect(ui->btn_saturation, &QToolButton::clicked,  this,   &HSV_widget::set_saturation);
    connect(ui->btn_value,      &QToolButton::clicked,  this,   &HSV_widget::set_value);
}
//--------------------------------------------------------------------------------
void HSV_widget::load_picture(void)
{
    MyFileDialog *dlg = new MyFileDialog("hsv_widget");
    dlg->setNameFilter("PNG files (*.png)");
    dlg->setDefaultSuffix("png");
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        QString filename = files.at(0);
        if(filename.isEmpty() == false)
        {
            QPixmap pixmap;
            bool ok = pixmap.load(filename);
            if(ok)
            {
                ui->picture_label->setPixmap(pixmap);
            }
        }
    }
    delete dlg;
}
//--------------------------------------------------------------------------------
void HSV_widget::set_saturation(void)
{
    lock_interface();

    QImage image = ui->picture_label->pixmap(Qt::ReturnByValue).toImage();

    int width   = image.width();
    int height  = image.height();
    int inc_s   = ui->sb_saturation->value();

    QColor color;
    int h = 0;
    int s = 0;
    int v = 0;

    //---
#if 0
    QByteArray ba;
    QBuffer    buffer( &ba );
    buffer.open( QIODevice::WriteOnly );
    image.save( &buffer, "BMP" );
    emit info(QString("Ba len:      %1").arg(ba.length()));
    emit info(QString("Buffer size: %1").arg(buffer.size()));
    emit info(QString("width:  %1").arg(width));
    emit info(QString("height: %1").arg(height));
    emit info(QString("size 1: %1").arg(width * height));
    emit info(QString("size 2: %1").arg(width * height * 2));
    emit info(QString("size 3: %1").arg(width * height * 3));
    emit info(QString("size 4: %1").arg(width * height * 4));
#endif
    //---

    QElapsedTimer timer;
    timer.start();
    for(int y=0; y<height; y++)
    {
        for(int x=0; x<width; x++)
        {
            color = image.pixelColor(x, y);

            color.getHsv(&h, &s, &v);

            s += inc_s;
            if(s > 255) s = 255;
            if(s < 0)   s = 0;

            color = QColor::fromHsv(h, s, v);

            image.setPixelColor(x, y, color);
        }
        //QCoreApplication::processEvents();
    }
    emit info(QString("Elapsed %1 msec").arg(timer.elapsed()));
    ui->picture_label->setPixmap(QPixmap::fromImage(image));

    unlock_interface();
}
//--------------------------------------------------------------------------------
void HSV_widget::set_value(void)
{
    lock_interface();

    QImage image = ui->picture_label->pixmap(Qt::ReturnByValue).toImage();

    int width = image.width();
    int height = image.height();
    int inc_v = ui->sb_value->value();

    QColor color;
    int h = 0;
    int s = 0;
    int v = 0;

    QElapsedTimer timer;
    timer.start();
    for(int y=0; y<height; y++)
    {
        for(int x=0; x<width; x++)
        {
            color = image.pixelColor(x, y);

            color.getHsv(&h, &s, &v);

            v += inc_v;
            if(v > 255) v = 255;
            if(v < 0)   v = 0;

            color = QColor::fromHsv(h, s, v);

            image.setPixelColor(x, y, color);
        }
        //QCoreApplication::processEvents();
    }
    emit info(QString("Elapsed %1 msec").arg(timer.elapsed()));
    ui->picture_label->setPixmap(QPixmap::fromImage(image));

    unlock_interface();
}
//--------------------------------------------------------------------------------
void HSV_widget::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool HSV_widget::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void HSV_widget::load_setting(void)
{
    bool ok;
    int s = 0;
    int v = 0;

    ok = load_int("saturation", &s);
    if(ok)
    {
        ui->sb_saturation->setValue(s);
    }
    ok = load_int("value", &v);
    if(ok)
    {
        ui->sb_value->setValue(s);
    }
}
//--------------------------------------------------------------------------------
void HSV_widget::save_setting(void)
{
    save_int("saturation", ui->sb_saturation->value());
    save_int("value", ui->sb_value->value());
}
//--------------------------------------------------------------------------------
