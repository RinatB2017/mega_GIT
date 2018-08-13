/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "mesh_control.hpp"
#include "ui_mesh_control.h"
//--------------------------------------------------------------------------------
Mesh_control::Mesh_control(QString o_name, QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Mesh_control)
{
    if(o_name.isEmpty())    o_name = "noname";
    setObjectName(o_name);
    init();
}
//--------------------------------------------------------------------------------
Mesh_control::~Mesh_control()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Mesh_control::init(void)
{
    ui->setupUi(this);

    ui->widget_serial->set_fix_baudrate(115200);
    ui->widget_log->set_o_name(QString("%1_log").arg(objectName()));
    ui->widget_log->load_settings();

    //qDebug() << "ui->widget_log->objectName()" << ui->widget_log->objectName();

    connect(this,   SIGNAL(send(QString)),  ui->widget_serial,  SLOT(input(QString)));

    connect(ui->btn_ON,     SIGNAL(clicked(bool)),  this,   SLOT(led_on()));
    connect(ui->btn_OFF,    SIGNAL(clicked(bool)),  this,   SLOT(led_off()));

    connect(this,   SIGNAL(info(QString)),  ui->widget_log, SLOT(infoLog(QString)));
    connect(this,   SIGNAL(debug(QString)), ui->widget_log, SLOT(debugLog(QString)));
    connect(this,   SIGNAL(error(QString)), ui->widget_log, SLOT(errorLog(QString)));
    connect(this,   SIGNAL(trace(QString)), ui->widget_log, SLOT(traceLog(QString)));

    connect(ui->widget_serial,  SIGNAL(output(QByteArray)), this,   SLOT(read_data(QByteArray)));
}
//--------------------------------------------------------------------------------
void Mesh_control::read_data(QByteArray data)
{
    if(data.isEmpty())
    {
        return;
    }
    emit info(data.data());

    if(data.contains("led_ON"))
    {
        ui->widget_led->setState(true);
    }
    if(data.contains("led_OFF"))
    {
        ui->widget_led->setState(false);
    }
}
//--------------------------------------------------------------------------------
void Mesh_control::led_on(void)
{
    emit debug("send 1");
    emit send("1");
}
//--------------------------------------------------------------------------------
void Mesh_control::led_off(void)
{
    emit debug("send 0");
    emit send("0");
}
//--------------------------------------------------------------------------------
void Mesh_control::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
