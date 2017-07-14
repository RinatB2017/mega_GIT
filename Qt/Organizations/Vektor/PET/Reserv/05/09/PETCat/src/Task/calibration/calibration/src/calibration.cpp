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
#include <QTableWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QScrollArea>
#include <QUdpSocket>
#include <QTime>
//--------------------------------------------------------------------------------
#include "calibration.hpp"
#include "ui_calibration.h"
//--------------------------------------------------------------------------------
#include "PET_device_base.hpp"
#include "ad9106.hpp"
//--------------------------------------------------------------------------------
#include "petcat_options.hpp"
#include "findpetbox.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "qxmlputget.h"
//--------------------------------------------------------------------------------
#include "pet.h"
#include "pet_bos.h"
#include "pet_brg.h"
#include "pet_errno.h"
//--------------------------------------------------------------------------------
#include "calibration_channel.hpp"
#include "calibration_param.hpp"
//--------------------------------------------------------------------------------
Calibration::Calibration(QList<pet_discovery_data> pet_config, QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Calibration),
    p_pet_config(pet_config)
{
    init();

    load_QSpinBox("Calibration");
    load_QDoubleSpinBox("Calibration");
}
//--------------------------------------------------------------------------------
Calibration::~Calibration()
{
    save_QSpinBox("Calibration");
    save_QDoubleSpinBox("Calibration");

    delete ui;
}
//--------------------------------------------------------------------------------
void Calibration::init(void)
{
    ui->setupUi(this);

    //---
    pet_brg = new PET_device_brg(this);
    //---
    ui->le_device->setReadOnly(true);
    ui->le_device->setText(QString("%1:%2 (Адрес %3)")
                           .arg(p_pet_config.at(0).host.toString())
                           .arg(p_pet_config.at(0).port)
                           .arg(p_pet_config.at(0).addr));
    //---
    pet_brg->set_comm_param(p_pet_config.at(0).host,
                            p_pet_config.at(0).port);
    //---
    channel_1 = new Calibration_channel(this);
    channel_2 = new Calibration_channel(this);
    channel_3 = new Calibration_channel(this);
    channel_4 = new Calibration_channel(this);

    param = new Calibration_param(this);

    ui->sa_1->setWidget(channel_1);
    ui->sa_2->setWidget(channel_2);
    ui->sa_3->setWidget(channel_3);
    ui->sa_4->setWidget(channel_4);

    ui->sa_param->setWidget(param);
    //---
    connect(ui->btn_select,     SIGNAL(clicked(bool)),  this,   SLOT(select()));
    connect(ui->btn_apply,      SIGNAL(clicked(bool)),  this,   SLOT(apply()));
    connect(ui->btn_refresh,    SIGNAL(clicked(bool)),  this,   SLOT(refresh()));
    //---
    ui->btn_apply->setDisabled(true);
    ui->btn_refresh->setDisabled(true);
}
//--------------------------------------------------------------------------------
void Calibration::start(void)
{
    block_this_button(true);

    //TODO
    emit error(TODODO);
    MyWidget::messagebox_critical("Ошибка", TODODO);

    block_this_button(false);
}
//--------------------------------------------------------------------------------
void Calibration::stop(void)
{
    block_this_button(true);

    //TODO
    emit error(TODODO);
    MyWidget::messagebox_critical("Ошибка", TODODO);

    block_this_button(false);
}
//--------------------------------------------------------------------------------
void Calibration::select(void)
{
    FindPetBox *f = new FindPetBox(PET_ADDR_BROADCAST, this);

    int btn = f->exec();
    if(btn != QDialog::Accepted)
    {
        return;
    }
    p_pet_config.clear();
    p_pet_config = f->get_current_pet_config();

    ui->le_device->setReadOnly(true);
    ui->le_device->setText(QString("%1:%2 (Адрес %3)")
                           .arg(p_pet_config.at(0).host.toString())
                           .arg(p_pet_config.at(0).port)
                           .arg(p_pet_config.at(0).addr));
    //---
    QHostAddress addr = p_pet_config.at(0).host;
    quint32 t_addr = QHostAddress(addr).toIPv4Address();
    t_addr |= 0x000000FF;
    addr = QHostAddress(t_addr);
    //---
    comm->set_comm_param(addr, PETCat_options::get_port_cmd());
    bool ok = comm->bind(QHostAddress::AnyIPv4);
    if(!ok)
    {
        emit error(comm->get_last_error());
    }
    else
    {
        //TODO
    }

    f->deleteLater();
}
//--------------------------------------------------------------------------------
void Calibration::apply(void)
{
    emit error(TODODO);
}
//--------------------------------------------------------------------------------
void Calibration::refresh(void)
{
    emit error(TODODO);
}
//--------------------------------------------------------------------------------
void Calibration::test(void)
{
    block_this_button(true);

    //TODO
    emit error(TODODO);
    MyWidget::messagebox_critical("Ошибка", TODODO);

    block_this_button(false);
}
//--------------------------------------------------------------------------------
