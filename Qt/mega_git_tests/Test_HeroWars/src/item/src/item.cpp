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
#include "item_options.hpp"
#include "item.hpp"
#include "ui_item.h"
//--------------------------------------------------------------------------------
Item::Item(QListWidget *lw, QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Item)
{
    parent_lw = lw;
    init();
}
//--------------------------------------------------------------------------------
Item::~Item()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Item::init(void)
{
    ui->setupUi(this);

    ui->btn_close->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogCloseButton));
    ui->btn_click->setIcon(qApp->style()->standardIcon(QStyle::SP_CommandLink));
    ui->btn_load->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogOpenButton));
    ui->btn_save->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogSaveButton));
    ui->btn_info->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogHelpButton));

    connect(ui->btn_close,  &QToolButton::clicked,  this,   &Item::item_close);
    connect(ui->btn_click,  &QToolButton::clicked,  this,   &Item::item_click);
    connect(ui->btn_load,   &QToolButton::clicked,  this,   &Item::item_load);
    connect(ui->btn_save,   &QToolButton::clicked,  this,   &Item::item_save);
    connect(ui->btn_info,   &QToolButton::clicked,  this,   &Item::item_info);
}
//--------------------------------------------------------------------------------
void Item::item_close(void)
{
    emit remove_item(property(P_INDEX).toInt());
}
//--------------------------------------------------------------------------------
void Item::item_click(void)
{
    fail();
}
//--------------------------------------------------------------------------------
void Item::item_load(void)
{
    MyFileDialog *dlg = new MyFileDialog("item_load", "item_load", this);
    dlg->setNameFilter("PNG files (*.png)");
    dlg->selectFile("noname");
    dlg->setDefaultSuffix("png");
    dlg->setOption(MyFileDialog::DontUseNativeDialog, true);
    int btn = dlg->exec();
    if(btn == MyFileDialog::Accepted)
    {
        QStringList files = dlg->selectedFiles();
        QString filename = files.at(0);
        if(filename.isEmpty() == false)
        {
            pixmap.load(filename);
            ui->lbl_picture->setPixmap(pixmap);
            adjustSize();
        }
    }
}
//--------------------------------------------------------------------------------
void Item::item_save(void)
{
    if(pixmap.isNull())
    {
        emit error("Nothing to keep");
        return;
    }

    MyFileDialog *dlg = new MyFileDialog("item_save", "log_box");
    dlg->setAcceptMode(MyFileDialog::AcceptSave);
    dlg->setNameFilter("PNG files (*.png)");
    dlg->setDefaultSuffix("png");
    dlg->setOption(MyFileDialog::DontUseNativeDialog, true);
    dlg->setDirectory(".");
    dlg->selectFile("noname");
    dlg->setOption(MyFileDialog::DontConfirmOverwrite, false);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        QString filename = files.at(0);
        if(filename.isEmpty() == false)
        {
            pixmap.save(filename);
        }
    }
    dlg->deleteLater();
}
//--------------------------------------------------------------------------------
void Item::item_info(void)
{
    fail();
}
//--------------------------------------------------------------------------------
void Item::set_pixmap(const QPixmap &pixmap)
{
    this->pixmap = pixmap;
    ui->lbl_picture->setPixmap(pixmap);
}
//--------------------------------------------------------------------------------
void Item::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool Item::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Item::load_setting(void)
{
//    emit info(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void Item::save_setting(void)
{
//    emit info(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
