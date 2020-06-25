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
#include "myfiledialog.hpp"
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

    ui->btn_load->setIcon(QIcon(qApp->style()->standardIcon(QStyle::SP_DialogOpenButton)));
    ui->btn_save->setIcon(QIcon(qApp->style()->standardIcon(QStyle::SP_DialogSaveButton)));

    connect(ui->btn_up,         &QToolButton::clicked,  this,   &CropPicture::up);
    connect(ui->btn_left,       &QToolButton::clicked,  this,   &CropPicture::left);
    connect(ui->btn_plus,       &QToolButton::clicked,  this,   &CropPicture::plus);
    connect(ui->btn_minus,      &QToolButton::clicked,  this,   &CropPicture::minus);
    connect(ui->btn_right,      &QToolButton::clicked,  this,   &CropPicture::right);
    connect(ui->btn_down,       &QToolButton::clicked,  this,   &CropPicture::down);
    connect(ui->btn_up_left,    &QToolButton::clicked,  this,   &CropPicture::up_left);
    connect(ui->btn_down_right, &QToolButton::clicked,  this,   &CropPicture::down_right);

    connect(ui->btn_load,       &QToolButton::clicked,  this,   &CropPicture::load);
    connect(ui->btn_save,       &QToolButton::clicked,  this,   &CropPicture::save);
}
//--------------------------------------------------------------------------------
void CropPicture::load(void)
{
    MyFileDialog *dlg = new MyFileDialog("croppicture", "croppicture", this);
    dlg->setNameFilter("PNG files (*.png)");
    dlg->selectFile("noname");
    dlg->setDefaultSuffix("png");
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    int btn = dlg->exec();
    if(btn == MyFileDialog::Accepted)
    {
        QStringList files = dlg->selectedFiles();
        QString filename = files.at(0);

        if(!filename.isEmpty())
        {
            QPixmap pixmap;
            bool ok = pixmap.load(filename);
            if(ok)
            {
                orig_pixmap = pixmap;
                pixmap_width  = ui->picture_label->width();
                pixmap_height = ui->picture_label->height();

                QPixmap temp_pixmap = pixmap.copy(pos_x,
                                                  pos_y,
                                                  pixmap_width,
                                                  pixmap_height);
                ui->picture_label->setPixmap(temp_pixmap);
            }
        }
    }
}
//--------------------------------------------------------------------------------
void CropPicture::save(void)
{
    MyFileDialog *dlg = new MyFileDialog("croppicture", "croppicture", this);
    dlg->setNameFilter("PNG files (*.png)");
    dlg->selectFile("noname");
    dlg->setDefaultSuffix("png");
    dlg->setOption(QFileDialog::DontUseNativeDialog,    true);
    dlg->setOption(QFileDialog::DontConfirmOverwrite,   false);
    int btn = dlg->exec();
    if(btn == MyFileDialog::Accepted)
    {
        QStringList files = dlg->selectedFiles();
        QString filename = files.at(0);

        if(!filename.isEmpty())
        {
            MyWidget::messagebox_info("Info",   filename);
        }
    }
}
//--------------------------------------------------------------------------------
void CropPicture::update_picture(void)
{
#if 1
    QPixmap temp_pixmap = orig_pixmap.copy(pos_x,
                                           pos_y,
                                           pixmap_width,
                                           pixmap_height);
#else
    QPixmap temp_pixmap = orig_pixmap.copy(pos_x,
                                           pos_y,
                                           ui->picture_label->width(),
                                           ui->picture_label->height());
#endif
    ui->picture_label->setPixmap(temp_pixmap);
}
//--------------------------------------------------------------------------------
void CropPicture::up(void)
{
    if(pos_y > 0)
    {
        pos_y--;
        update_picture();
    }
}
//--------------------------------------------------------------------------------
void CropPicture::down(void)
{
    if(pos_y < pixmap_height)
    {
        pos_y++;
        update_picture();
    }
}
//--------------------------------------------------------------------------------
void CropPicture::left(void)
{
    if(pos_x > 0)
    {
        pos_x--;
        update_picture();
    }
}
//--------------------------------------------------------------------------------
void CropPicture::right(void)
{
    if(pos_x < pixmap_width)
    {
        pos_x++;
        update_picture();
    }
}
//--------------------------------------------------------------------------------
void CropPicture::up_left(void)
{
    up();
    left();
}
//--------------------------------------------------------------------------------
void CropPicture::down_right(void)
{
    down();
    right();
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

    QPixmap temp_pixmap = pixmap.copy(pos_x,
                                      pos_y,
                                      ui->picture_label->width(),
                                      ui->picture_label->height());
    ui->picture_label->setPixmap(temp_pixmap);

    return true;
}
//--------------------------------------------------------------------------------
void CropPicture::resizeEvent(QResizeEvent *event)
{
//    int w = event->size().width();
//    int h = event->size().height();

    emit info(QString("1: %1 %2")
              .arg(ui->picture_label->width())
              .arg(ui->picture_label->height()));

    pixmap_width  = ui->picture_label->width();
    pixmap_height = ui->picture_label->height();

//    ui->picture_label->clear();
    QWidget::resizeEvent(event);

//    ui->picture_label->update();
//    emit info(QString("2: %1 %2")
//              .arg(ui->picture_label->width())
//              .arg(ui->picture_label->height()));

    update_picture();
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
