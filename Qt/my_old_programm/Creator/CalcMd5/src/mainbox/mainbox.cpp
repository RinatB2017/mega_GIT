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
#include <QFileDialog>
#include <QObject>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "mainbox.hpp"
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
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    connect(ui->BtnOpen, SIGNAL(clicked()), this, SLOT(calc_md5()));
}
//--------------------------------------------------------------------------------
void MainBox::calc_md5(void)
{
    QFileDialog *dlg;
    QString temp;

    dlg = new QFileDialog;
    dlg->setNameFilter(tr("All files (*.*)"));
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    dlg->setDirectory(".");
    dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        ui->leMd5->clear();
        QStringList files = dlg->selectedFiles();
        ui->leMd5->setText(get_md5(files.at(0)));
    }
}
//--------------------------------------------------------------------------------
QString MainBox::get_md5(const QString &filename)
{
    QString md5_string = "";
    QFile file(filename);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
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
