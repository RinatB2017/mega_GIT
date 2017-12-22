/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#include <QMouseEvent>
#include <QSpinBox>
#include <QLineEdit>
#include <QRegExp>
//--------------------------------------------------------------------------------
#include "PET_device_base.hpp"
//--------------------------------------------------------------------------------
#include "pet_net_config_page.hpp"
#include "ui_pet_net_config_page.h"
//--------------------------------------------------------------------------------
#include "pet.h"
//--------------------------------------------------------------------------------
PET_net_config_page::PET_net_config_page(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::PET_net_config_page)
{
    init();
}
//--------------------------------------------------------------------------------
PET_net_config_page::~PET_net_config_page()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void PET_net_config_page::init(void)
{
    ui->setupUi(this);

    //---
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex (QString("^%1\\.%1\\.%1\\.%1$").arg(ipRange));
    QRegExp ipRegexMAC (QString("^%1\\.%1\\.%1\\.%1\\.%1\\.%1$").arg(ipRange));

    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    QRegExpValidator *ipValidatorMAC = new QRegExpValidator(ipRegexMAC, this);
    //---
    ui->le_event_addr->setValidator(ipValidator);
    ui->le_gateway->setValidator(ipValidator);
    ui->le_ip_addr->setValidator(ipValidator);

    ui->sb_cmd_port->setRange(MIN_UDP_PORT, MAX_UDP_PORT);
    ui->sb_event_port->setRange(MIN_UDP_PORT, MAX_UDP_PORT);

    ui->le_mac_addr->setValidator(ipValidatorMAC);

    ui->le_netmask->setValidator(ipValidator);
    //---
    connect(ui->rb_dhcp,    SIGNAL(toggled(bool)),  ui->le_ip_addr,     SLOT(setDisabled(bool)));
    connect(ui->rb_dhcp,    SIGNAL(toggled(bool)),  ui->le_netmask,     SLOT(setDisabled(bool)));
    connect(ui->rb_dhcp,    SIGNAL(toggled(bool)),  ui->le_gateway,     SLOT(setDisabled(bool)));

    connect(ui->rb_handmade,    SIGNAL(toggled(bool)),  ui->le_ip_addr, SLOT(setEnabled(bool)));
    connect(ui->rb_handmade,    SIGNAL(toggled(bool)),  ui->le_netmask, SLOT(setEnabled(bool)));
    connect(ui->rb_handmade,    SIGNAL(toggled(bool)),  ui->le_gateway, SLOT(setEnabled(bool)));
}
//--------------------------------------------------------------------------------
void PET_net_config_page::block_ctrl_interface(bool state)
{
    ui->le_ip_addr->setDisabled(state);
    ui->le_netmask->setDisabled(state);
    ui->le_gateway->setDisabled(state);
    ui->le_mac_addr->setDisabled(state);

    ui->sb_cmd_port->setDisabled(state);
    ui->sb_event_port->setDisabled(state);

    ui->le_event_addr->setDisabled(state);
}
//--------------------------------------------------------------------------------
void PET_net_config_page::block_state_interface(bool state)
{
    Q_UNUSED(state)
}
//--------------------------------------------------------------------------------
PET_RESULT PET_net_config_page::set(pet_bos_ctrl_t ctrl)
{
    ui->le_ip_addr->setText(PET_device_base::get_ip(ctrl.ip_address));
    ui->le_netmask->setText(PET_device_base::get_ip(ctrl.netmask));
    ui->le_gateway->setText(PET_device_base::get_ip(ctrl.gateway));
    ui->le_mac_addr->setText(PET_device_base::get_mac(ctrl.mac_addr));

    ui->sb_cmd_port->setValue(ctrl.cmd_port);
    ui->sb_event_port->setValue(ctrl.event_port);

    ui->le_event_addr->setText(PET_device_base::get_ip(ctrl.event_addr));

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_net_config_page::get(pet_bos_ctrl_t *ctrl)
{
    Q_CHECK_PTR(ctrl);
    //---
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
