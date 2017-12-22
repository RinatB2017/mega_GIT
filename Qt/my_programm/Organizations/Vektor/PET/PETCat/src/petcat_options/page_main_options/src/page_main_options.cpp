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
#include <QMouseEvent>
//--------------------------------------------------------------------------------
#include "page_main_options.hpp"
#include "ui_page_main_options.h"
//--------------------------------------------------------------------------------
#include "petcat_options.hpp"
#include "pet_defines.hpp"
//--------------------------------------------------------------------------------
Page_main_options::Page_main_options(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Page_main_options)
{
    init();
}
//--------------------------------------------------------------------------------
Page_main_options::~Page_main_options()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Page_main_options::init(void)
{
    ui->setupUi(this);

    ui->sb_port_cmd->setRange(0, 0xFFFF);
    ui->sb_port_event->setRange(0, 0xFFFF);

    ui->sb_max_net_timeout->setRange(100, 10000);
    ui->sb_max_pet_timeout->setRange(100, 10000);

    ui->sb_port_cmd->setValue(PETCat_options::get_port_cmd());
    ui->sb_port_event->setValue(PETCat_options::get_port_event());

    ui->sb_max_net_timeout->setValue(max_net_timeout.toInt());
    ui->sb_max_pet_timeout->setValue(max_pet_timeout.toInt());

    PETCat_options::get_max_net_timeout(&max_net_timeout);
    PETCat_options::get_max_pet_timeout(&max_pet_timeout);

    QVariant v_is_shows_info = true;
    QVariant v_is_shows_debug = true;
    QVariant v_is_shows_error = true;
    QVariant v_is_shows_trace = true;
    get_param(PETCAT_OPTIONS_MAIN_GROUP,    PARAM_IS_SHOWS_INFO,    DEFAULT_PARAM_IS_SHOWS_INFO,    &v_is_shows_info);
    get_param(PETCAT_OPTIONS_MAIN_GROUP,    PARAM_IS_SHOWS_DEBUG,   DEFAULT_PARAM_IS_SHOWS_DEBUG,   &v_is_shows_debug);
    get_param(PETCAT_OPTIONS_MAIN_GROUP,    PARAM_IS_SHOWS_ERROR,   DEFAULT_PARAM_IS_SHOWS_ERROR,   &v_is_shows_error);
    get_param(PETCAT_OPTIONS_MAIN_GROUP,    PARAM_IS_SHOWS_TRACE,   DEFAULT_PARAM_IS_SHOWS_TRACE,   &v_is_shows_trace);

    connect(ui->btn_accept, SIGNAL(clicked(bool)),  this,   SLOT(t_accept()));
}
//--------------------------------------------------------------------------------
void Page_main_options::t_accept(void)
{
    block_this_button(true);

    uint16_t p_cmd = ui->sb_port_cmd->value();
    uint16_t p_event = ui->sb_port_event->value();

    uint16_t max_net_timeout = ui->sb_max_net_timeout->value();
    uint16_t max_pet_timeout = ui->sb_max_pet_timeout->value();

    set_param(PETCAT_OPTIONS_MAIN_GROUP,    PARAM_PORT_CMD_NAME,    p_cmd);
    set_param(PETCAT_OPTIONS_MAIN_GROUP,    PARAM_PORT_EVENT_NAME,  p_event);
    set_param(PETCAT_OPTIONS_MAIN_GROUP,    PARAM_MAX_NET_TIMEOUT,  max_net_timeout);
    set_param(PETCAT_OPTIONS_MAIN_GROUP,    PARAM_MAX_PET_TIMEOUT,  max_pet_timeout);

    block_this_button(false);
}
//--------------------------------------------------------------------------------
bool Page_main_options::eventFilter(QObject*, QEvent* event)
{
    QMouseEvent *mouseEvent = (QMouseEvent *) event;
    //---
    if(mouseEvent->type() == QMouseEvent::Wheel)
    {
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
