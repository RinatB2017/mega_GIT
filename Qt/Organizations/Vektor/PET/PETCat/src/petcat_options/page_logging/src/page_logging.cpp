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
#include "page_logging.hpp"
#include "ui_page_logging.h"
//--------------------------------------------------------------------------------
#include "petcat_options.hpp"
#include "pet_defines.hpp"
//--------------------------------------------------------------------------------
Page_logging::Page_logging(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Page_logging)
{
    init();
}
//--------------------------------------------------------------------------------
Page_logging::~Page_logging()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Page_logging::init(void)
{
    ui->setupUi(this);

    QVariant v_is_shows_info = true;
    QVariant v_is_shows_debug = true;
    QVariant v_is_shows_error = true;
    QVariant v_is_shows_trace = true;
    get_param(PETCAT_OPTIONS_MAIN_GROUP,    PARAM_IS_SHOWS_INFO,    DEFAULT_PARAM_IS_SHOWS_INFO,    &v_is_shows_info);
    get_param(PETCAT_OPTIONS_MAIN_GROUP,    PARAM_IS_SHOWS_DEBUG,   DEFAULT_PARAM_IS_SHOWS_DEBUG,   &v_is_shows_debug);
    get_param(PETCAT_OPTIONS_MAIN_GROUP,    PARAM_IS_SHOWS_ERROR,   DEFAULT_PARAM_IS_SHOWS_ERROR,   &v_is_shows_error);
    get_param(PETCAT_OPTIONS_MAIN_GROUP,    PARAM_IS_SHOWS_TRACE,   DEFAULT_PARAM_IS_SHOWS_TRACE,   &v_is_shows_trace);

    flag_is_shows_info  = v_is_shows_info.toBool();
    flag_is_shows_debug = v_is_shows_debug.toBool();
    flag_is_shows_error = v_is_shows_error.toBool();
    flag_is_shows_trace = v_is_shows_trace.toBool();

    ui->cb_show_info->setChecked(flag_is_shows_info);
    ui->cb_show_debug->setChecked(flag_is_shows_debug);
    ui->cb_show_error->setChecked(flag_is_shows_error);
    ui->cb_show_trace->setChecked(flag_is_shows_trace);

    connect(ui->btn_accept, SIGNAL(clicked(bool)),  this,   SLOT(t_accept()));

    connect(ui->cb_show_info,   SIGNAL(clicked(bool)),  this,   SLOT(is_shows_info(bool)));
    connect(ui->cb_show_debug,  SIGNAL(clicked(bool)),  this,   SLOT(is_shows_debug(bool)));
    connect(ui->cb_show_error,  SIGNAL(clicked(bool)),  this,   SLOT(is_shows_error(bool)));
    connect(ui->cb_show_trace,  SIGNAL(clicked(bool)),  this,   SLOT(is_shows_trace(bool)));

#ifndef UNDER_CONSTRUCTION
    ui->cb_show_debug->setVisible(false);
    ui->cb_show_trace->setVisible(false);

    flag_is_shows_debug = false;
    flag_is_shows_trace = false;
#endif
}
//--------------------------------------------------------------------------------
void Page_logging::t_accept(void)
{
    block_this_button(true);

    set_param(PETCAT_OPTIONS_MAIN_GROUP,    PARAM_IS_SHOWS_INFO,    ui->cb_show_info->isChecked());
    set_param(PETCAT_OPTIONS_MAIN_GROUP,    PARAM_IS_SHOWS_DEBUG,   ui->cb_show_debug->isChecked());
    set_param(PETCAT_OPTIONS_MAIN_GROUP,    PARAM_IS_SHOWS_ERROR,   ui->cb_show_error->isChecked());
    set_param(PETCAT_OPTIONS_MAIN_GROUP,    PARAM_IS_SHOWS_TRACE,   ui->cb_show_trace->isChecked());

    block_this_button(false);
}
//--------------------------------------------------------------------------------
void Page_logging::is_shows_info(bool state)
{
    flag_is_shows_info = state;
    emit signal_is_shows_info(state);
}
//--------------------------------------------------------------------------------
void Page_logging::is_shows_debug(bool state)
{
    flag_is_shows_debug = state;
    emit signal_is_shows_debug(state);
}
//--------------------------------------------------------------------------------
void Page_logging::is_shows_error(bool state)
{
    flag_is_shows_error = state;
    emit signal_is_shows_error(state);
}
//--------------------------------------------------------------------------------
void Page_logging::is_shows_trace(bool state)
{
    flag_is_shows_trace = state;
    emit signal_is_shows_trace(state);
}
//--------------------------------------------------------------------------------
bool Page_logging::eventFilter(QObject*, QEvent* event)
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
