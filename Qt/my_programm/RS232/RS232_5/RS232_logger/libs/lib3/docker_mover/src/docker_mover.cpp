/*********************************************************************************
**                                                                              **
**     Copyright (C) 2023                                                       **
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
#include "docker_mover.hpp"
#include "ui_docker_mover.h"
//--------------------------------------------------------------------------------
Docker_mover::Docker_mover(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Docker_mover)
{
    init();
}
//--------------------------------------------------------------------------------
Docker_mover::~Docker_mover()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Docker_mover::init(void)
{
    ui->setupUi(this);

    connect(ui->btn_up,     &QToolButton::clicked,
            this,           &Docker_mover::up);
    connect(ui->btn_down,   &QToolButton::clicked,
            this,           &Docker_mover::down);
    connect(ui->btn_left,   &QToolButton::clicked,
            this,           &Docker_mover::left);
    connect(ui->btn_right,  &QToolButton::clicked,
            this,           &Docker_mover::right);
}
//--------------------------------------------------------------------------------
DOCKER_STATES Docker_mover::get_state(void)
{
    return state;
}
//--------------------------------------------------------------------------------
void Docker_mover::up(void)
{
    state = UP;
    accept();
}
//--------------------------------------------------------------------------------
void Docker_mover::down(void)
{
    state = DOWN;
    accept();
}
//--------------------------------------------------------------------------------
void Docker_mover::left(void)
{
    state = LEFT;
    accept();
}
//--------------------------------------------------------------------------------
void Docker_mover::right(void)
{
    state = RIGHT;
    accept();
}
//--------------------------------------------------------------------------------
