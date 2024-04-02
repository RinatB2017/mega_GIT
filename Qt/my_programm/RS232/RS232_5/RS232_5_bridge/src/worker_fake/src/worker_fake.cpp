/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                      **
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
#include "mainwindow.hpp"
#include "worker_fake.hpp"
#include "ui_worker_fake.h"
//--------------------------------------------------------------------------------
#if QT_VERSION > QT_VERSION_CHECK(6,0,0)
#   include <QRegularExpression>
#   include <QRegularExpressionValidator>
#endif
//--------------------------------------------------------------------------------
Worker_fake::Worker_fake(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Worker_fake)
{
    init();
}
//--------------------------------------------------------------------------------
Worker_fake::~Worker_fake()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Worker_fake::init(void)
{
    ui->setupUi(this);

    ui->btn_send_hex->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_send_text->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));

    connect(ui->btn_send_hex,   &QToolButton::clicked,  this,   &Worker_fake::send_hex);
    connect(ui->btn_send_text,  &QToolButton::clicked,  this,   &Worker_fake::send_text);

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QRegExp regex("^[0-9A-F]{100}$", Qt::CaseInsensitive);  // не более 100 символов
    QRegExpValidator *validator = new QRegExpValidator(regex, this);
#else
    QRegularExpression regex("^[0-9A-F]{100}$");  // не более 100 символов
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(regex, this);
#endif
    ui->le_hex->setValidator(validator);

#ifdef USE_DOCK_WIDGETS
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw)
    {
        mw->add_dock_widget("FAKE log", "fake_log_dock",  Qt::BottomDockWidgetArea, this);
    }
    else
    {
        emit error("mw not found!");
    }
#endif
}
//--------------------------------------------------------------------------------
void Worker_fake::input(QByteArray data)
{
    QString temp;
    temp.append(data);

    QRegularExpression regex("(^[\\40-\\176]{1,47}$)"); // Printable ASCII characters only
    if(regex.match(temp).hasMatch())
    {
        ui->log_widget->infoLog(data);
    }
    else
    {
        ui->log_widget->errorLog(data.toHex().toUpper());
    }
}
//--------------------------------------------------------------------------------
void Worker_fake::send_hex(void)
{
    QByteArray ba;
    ba.append(QByteArray::fromHex(ui->le_hex->text().toLocal8Bit()));
    emit output(ba);
}
//--------------------------------------------------------------------------------
void Worker_fake::send_text(void)
{
//    QByteArray ba;
//    ba.append(ui->le_text->text());
//    emit output(ba);

    emit output(ui->le_text->text().toLatin1());
}
//--------------------------------------------------------------------------------
void Worker_fake::port_open(void)
{

}
//--------------------------------------------------------------------------------
void Worker_fake::port_close(void)
{

}
//--------------------------------------------------------------------------------
