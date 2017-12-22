/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#include "metadata_widget.hpp"
#include "ui_metadata_widget.h"
//--------------------------------------------------------------------------------
#include "metadata.hpp"
//--------------------------------------------------------------------------------
#include "myfiledialog.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
MetaData_widget::MetaData_widget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MetaData_widget),
    parent(parent)
{
    ui->setupUi(this);

    ui->btn_add_metadata->setIcon(qApp->style()->standardIcon(QStyle::SP_DirOpenIcon));
    connect(ui->btn_add_metadata,   SIGNAL(clicked(bool)),  parent,   SIGNAL(signal_add_metadata_file()));
    //connect(ui->btn_add_metadata,   SIGNAL(clicked(bool)),  this,   SLOT(add_new_file()));

    ui->btn_add_metadata->setProperty(FLAG_NO_BLOCK, true);

    refresh_metadata();
}
//--------------------------------------------------------------------------------
MetaData_widget::~MetaData_widget()
{
    disconnect();
    delete ui;
}
//--------------------------------------------------------------------------------
#if 0
void MetaData_widget::add_new_file(void)
{
    QString filename;
    MyFileDialog *dlg = new MyFileDialog("filedialog_states", "md_dlg");
    QStringList filters;
    filters << "Файлы данных (*.dat3)"
            << "Все файлы (*.*)";
    dlg->setNameFilters(filters);
    dlg->setDefaultSuffix(tr("dat3"));
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        filename = files.at(0);

        QFileInfo file;
        file.setFile(filename);

        MetaData::METADATA data;
        data.fullname = filename;
        data.name = file.baseName();

        MyWidget::messagebox_info("file", filename);
    }
    dlg->deleteLater();
}
#endif
//--------------------------------------------------------------------------------
void MetaData_widget::refresh_metadata(void)
{
    MetaData *md = new MetaData();
    connect(md, SIGNAL(info(QString)),      parent,   SIGNAL(info(QString)));
    connect(md, SIGNAL(debug(QString)),     parent,   SIGNAL(debug(QString)));
    connect(md, SIGNAL(error(QString)),     parent,   SIGNAL(error(QString)));
    connect(md, SIGNAL(trace(QString)),     parent,   SIGNAL(trace(QString)));

    md->load_data(ui->cb_metadata);
    md->deleteLater();
}
//--------------------------------------------------------------------------------
int MetaData_widget::count(void)
{
    return ui->cb_metadata->count();
}
//--------------------------------------------------------------------------------
void MetaData_widget::setCurrentText(QString filename)
{
    ui->cb_metadata->setCurrentText(filename);
}
//--------------------------------------------------------------------------------
QString MetaData_widget::MetaData_widget::currentText(void)
{
    return ui->cb_metadata->currentText();
}
//--------------------------------------------------------------------------------
