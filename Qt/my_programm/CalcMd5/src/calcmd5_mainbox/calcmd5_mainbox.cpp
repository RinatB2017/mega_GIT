/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#include <QCryptographicHash>
//--------------------------------------------------------------------------------
#include "ui_calcmd5_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "calcmd5_mainbox.hpp"
#include "myfiledialog.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox),
    parent(parent)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_widgets(APPNAME);
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    connect(ui->BtnOpen, SIGNAL(clicked()), this, SLOT(calc_md5()));

    load_widgets(APPNAME);
}
//--------------------------------------------------------------------------------
void MainBox::calc_md5(void)
{
    QFileDialog *dlg;
    QString temp;

    dlg = new MyFileDialog("md5_dlg", "md5_dlg");
    dlg->setNameFilter(tr("All files (*.*)"));
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    //dlg->setDirectory(".");
    dlg->setOption(QFileDialog::DontConfirmOverwrite, false);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        QString filename = files.at(0);
        QString md5_str = get_md5(filename);
        emit info(QString("%1 %2")
                  .arg(filename)
                  .arg(md5_str));
        messagebox_info(filename, md5_str);
    }
}
//--------------------------------------------------------------------------------
QString MainBox::get_md5(const QString &filename)
{
    QString md5_string = "";
    QFile file(filename);

    if (file.open(QIODevice::ReadOnly))
    {
        QCryptographicHash *md5 = new QCryptographicHash(QCryptographicHash::Md5);
        md5->addData(file.readAll());
        md5_string = QString("%1").arg(md5->result().toHex().toUpper().data());
        delete md5;
    }
    return md5_string;
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
    ui->BtnOpen->setText(QObject::tr("Open"));
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
