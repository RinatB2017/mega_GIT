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
#include "scan_home_network.hpp"
#include "ui_scan_home_network.h"
//--------------------------------------------------------------------------------
#define IPV4    "IPV4"
#define MASK    "MASK"
//--------------------------------------------------------------------------------
Scan_home_network::Scan_home_network(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Scan_home_network)
{
    init();
}
//--------------------------------------------------------------------------------
Scan_home_network::~Scan_home_network()
{
    if(myProcess)
    {
        disconnect(myProcess,  &QProcess::started,                 this,   &Scan_home_network::started);
        disconnect(myProcess,  static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
                   this,       &Scan_home_network::finished);
        disconnect(myProcess,  &QProcess::errorOccurred,           this,   &Scan_home_network::process_error);
        disconnect(myProcess,  &QProcess::readyReadStandardOutput, this,   &Scan_home_network::readData);
        disconnect(myProcess,  &QProcess::readyReadStandardError,  this,   &Scan_home_network::readData);

        delete myProcess;
    }

    delete ui;
}
//--------------------------------------------------------------------------------
void Scan_home_network::init(void)
{
    ui->setupUi(this);

    ui->sb_mask->setRange(0, 0xFF);

    ui->btn_scan->setIcon(QIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload)));
    connect(ui->btn_scan,   &QToolButton::clicked,  this,   &Scan_home_network::scan);

    create_engine();
}
//--------------------------------------------------------------------------------
void Scan_home_network::scan(void)
{
    QStringList params;
    params.append("-sn");
    params.append(QString("%1/%2")
                  .arg(ui->ipv4_widget->get_url().host())
                  .arg(ui->sb_mask->value()));

    //nmap -sn 192.168.1.0/24
    myProcess->start("nmap", params);
}
//--------------------------------------------------------------------------------
void Scan_home_network::create_engine(void)
{
    myProcess = new QProcess(this);
    connect(myProcess,  &QProcess::started,                 this,   &Scan_home_network::started);
    connect(myProcess,  static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this,       &Scan_home_network::finished);
    connect(myProcess,  &QProcess::errorOccurred,           this,   &Scan_home_network::process_error);
    connect(myProcess,  &QProcess::readyReadStandardOutput, this,   &Scan_home_network::readData);
    connect(myProcess,  &QProcess::readyReadStandardError,  this,   &Scan_home_network::readData);
}
//--------------------------------------------------------------------------------
void Scan_home_network::started(void)
{
    block_interface(true);
    emit info("Процесс начат!");
}
//--------------------------------------------------------------------------------
void Scan_home_network::finished(int result)
{
    block_interface(false);

    if(result == 0)
    {
        emit info("Процесс завершен!");
    }
    else
    {
        emit error(QString("Процесс завершён с ошибкой (%1)").arg(result));
    }
}
//--------------------------------------------------------------------------------
void Scan_home_network::process_error(QProcess::ProcessError p_error)
{
    switch(p_error)
    {
    case QProcess::FailedToStart:
        emit error("FailedToStart");
        break;

    case QProcess::Crashed:
        emit error("Crashed");
        break;

    case QProcess::Timedout:
        emit error("Timedout");
        break;

    case QProcess::ReadError:
        emit error("ReadError");
        break;

    case QProcess::WriteError:
        emit error("WriteError");
        break;

    case QProcess::UnknownError:
        emit error("UnknownError");
        break;
    }
}
//--------------------------------------------------------------------------------
void Scan_home_network::readData(void)
{
    QString output = myProcess->readAllStandardOutput();
    ui->log_widget->infoLog(output);
}
//--------------------------------------------------------------------------------
void Scan_home_network::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool Scan_home_network::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Scan_home_network::load_setting(void)
{
    QUrl url;
    url.setHost(load_string(IPV4));

    int value = 24;
    bool ok = load_int(MASK, &value);
    if(ok)
    {
        ui->sb_mask->setValue(value);
    }

    ui->ipv4_widget->set_url(QUrl(url));
}
//--------------------------------------------------------------------------------
void Scan_home_network::save_setting(void)
{
    save_string(IPV4,   ui->ipv4_widget->get_url().host());
    save_int(MASK,      ui->sb_mask->value());
}
//--------------------------------------------------------------------------------
