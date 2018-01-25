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
#include <QPushButton>
#include <QToolButton>
#include <QUdpSocket>
#include <QTime>
//--------------------------------------------------------------------------------
#include "testbox_1.hpp"
#include "ui_testbox_1.h"
//--------------------------------------------------------------------------------
#include "PET_device_base.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
#include "pet_bos.h"
#include "pet_brg.h"
#include "pet_errno.h"
//--------------------------------------------------------------------------------
#include "demo_device_base.hpp"
//--------------------------------------------------------------------------------
TestBox_1::TestBox_1(QList<pet_discovery_data> pet_config, QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::TestBox_1),
    p_pet_config(pet_config)
{
    init();
    load_widgets("TestBox_1");    //TODO
}
//--------------------------------------------------------------------------------
TestBox_1::~TestBox_1()
{
    save_widgets("TestBox_1");    //TODO
    device->deleteLater();

    delete ui;
}
//--------------------------------------------------------------------------------
int TestBox_1::get_PID(void)
{
    return PET_PID_SIM;
}
//--------------------------------------------------------------------------------
void TestBox_1::init(void)
{
    ui->setupUi(this);
    //---
    device = new Device_BOS(this);
    connect(device,   SIGNAL(info(QString)),    this,   SIGNAL(info(QString)));
    connect(device,   SIGNAL(debug(QString)),   this,   SIGNAL(debug(QString)));
    connect(device,   SIGNAL(error(QString)),   this,   SIGNAL(error(QString)));
    connect(device,   SIGNAL(trace(QString)),   this,   SIGNAL(trace(QString)));
    //---
    connect(ui->btn_test_1, SIGNAL(clicked(bool)),  this,   SLOT(test()));
    //---
    block_wheel();
}
//--------------------------------------------------------------------------------
PET_RESULT TestBox_1::check_packet(uint8_t addr,
                                   uint8_t cmd,
                                   uint8_t len,
                                   QByteArray input_data,
                                   QByteArray *output_packet)
{
    Q_CHECK_PTR(device);
    if(device == nullptr) return PET_ERR_FAULT;
    if(output_packet == nullptr)    return PET_ERR_FAULT;

    pet_frame_t frame;
    frame.addr = addr;
    frame.cmd = cmd;
    frame.len = len;

    QByteArray temp;
    temp.clear();
    temp.append((char *)&frame, sizeof(frame));
    temp.append(input_data);

    PET_RESULT res = device->check(temp, output_packet);
    emit debug(PET_device_base::get_PET_RESULT_string(res));
    return res;
}
//--------------------------------------------------------------------------------
void TestBox_1::test(void)
{
    block_this_button(true);

    QByteArray input;
    QByteArray output;
    PET_RESULT res = PET_ERR_NONE;

    device->set_pet_address(1);

    //---
    input.clear();
    uint8_t cmd = PET_CMD_TEST;
    emit info(QString("check cmd 0x%1").arg(cmd, 0, 16));

    res = check_packet(1, cmd, input.length(), input, &output);
    if(res == PET_SUCCESS)
        emit info("test: OK");
    else
        emit error(PET_device_base::get_PET_RESULT_string((res)));

    block_this_button(false);
}
//--------------------------------------------------------------------------------
