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
#include <QToolButton>
#include <QToolBar>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "generator_curve.hpp"
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "mainbox.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    if(generator)
    {
        generator->deleteLater();
    }
    if(serial)
    {
        serial->deleteLater();
    }

    delete ui;
#ifdef QT_DEBUG
    qDebug() << "~MainBox()";
#endif
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    generator = new Generator_Curve(this);
    serial = new SerialBox5(this, "RS-232");

    Q_CHECK_PTR(generator);
    Q_CHECK_PTR(serial);

    ui->layout_generator->addWidget(generator);
    ui->layout_serial->addWidget(serial);

    //TODO
    connect(generator,  SIGNAL(send(QString)),  serial, SLOT(input(QString)));

    connect(serial, SIGNAL(output(QByteArray)), this, SLOT(temp(QByteArray)));
}
//--------------------------------------------------------------------------------
void MainBox::temp(QByteArray data)
{
    emit debug(data.data());
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
