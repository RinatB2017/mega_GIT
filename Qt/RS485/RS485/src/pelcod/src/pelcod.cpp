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
#include "pelcod.hpp"
#include "ui_pelcod.h"
//--------------------------------------------------------------------------------
#define Set_Preset  3
#define Clr_Preset  5
#define Go_Preset   7
#define Run_Pattern 0x23
//--------------------------------------------------------------------------------
#include "crc.h"
//--------------------------------------------------------------------------------
union PELCO_PACKET
{
    struct BODY
    {
        uint8_t sync;
        uint8_t address;
        uint8_t cmd1;
        uint8_t cmd2;
        uint8_t data1;
        uint8_t data2;
        uint8_t crc;
    } body;
    uint8_t buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
PelcoD::PelcoD(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PelcoD)
{
    init();
}
//--------------------------------------------------------------------------------
PelcoD::~PelcoD()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void PelcoD::init(void)
{
    ui->setupUi(this);

    ui->sb_sync->setRange(0, 0xFF);
    ui->sb_address->setRange(0, 0xFF);
    ui->sb_cmd1->setRange(0, 0xFF);
    ui->sb_cmd2->setRange(0, 0xFF);
    ui->sb_data1->setRange(0, 0xFF);
    ui->sb_data2->setRange(0, 0xFF);
    ui->sb_chk_sum->setRange(0, 0xFF);

    ui->sb_save_position->setRange(0, 0xFF);
    ui->sb_move_position->setRange(0, 0xFF);

    ui->sb_addr_cam_presets->setRange(0, 0xFF);

    ui->sb_sync->setValue(0xFF);

    ui->sb_sync->setProperty("no_save", 0);

    ui->sb_sync->setReadOnly(true);
    ui->sb_chk_sum->setReadOnly(true);

    connect(ui->btn_send,   SIGNAL(clicked(bool)),  this,   SLOT(f_send()));

    connect(ui->sb_sync,    SIGNAL(editingFinished()),  this,   SLOT(refresh()));
    connect(ui->sb_address, SIGNAL(editingFinished()),  this,   SLOT(refresh()));
    connect(ui->sb_cmd1,    SIGNAL(editingFinished()),  this,   SLOT(refresh()));
    connect(ui->sb_cmd2,    SIGNAL(editingFinished()),  this,   SLOT(refresh()));
    connect(ui->sb_data1,   SIGNAL(editingFinished()),  this,   SLOT(refresh()));
    connect(ui->sb_data2,   SIGNAL(editingFinished()),  this,   SLOT(refresh()));

    connect(ui->btn_move_position,  SIGNAL(clicked(bool)),  this,   SLOT(f_Move_position()));
    connect(ui->btn_save_position,  SIGNAL(clicked(bool)),  this,   SLOT(f_Save_position()));
    connect(ui->btn_wiper,          SIGNAL(clicked(bool)),  this,   SLOT(f_Wiper()));
    connect(ui->btn_run_tur_1,      SIGNAL(clicked(bool)),  this,   SLOT(f_Run_Tur_1()));
}
//--------------------------------------------------------------------------------
void PelcoD::refresh(void)
{
    PELCO_PACKET packet;
    packet.body.sync     = ui->sb_sync->value();
    packet.body.address  = ui->sb_address->value();
    packet.body.cmd1     = ui->sb_cmd1->value();
    packet.body.cmd2     = ui->sb_cmd2->value();
    packet.body.data1    = ui->sb_data1->value();
    packet.body.data2    = ui->sb_data2->value();

    ui->sb_chk_sum->setValue(CRC::pelco_crc8((uint8_t *)&packet.buf[1], 5));
}
//--------------------------------------------------------------------------------
void PelcoD::f_Save_position(void)
{
    Camera_Save_position(ui->sb_save_position->value());
}
//--------------------------------------------------------------------------------
void PelcoD::f_Move_position(void)
{
    Camera_Move_position(ui->sb_move_position->value());
}
//--------------------------------------------------------------------------------
void PelcoD::f_Wiper(void)
{
    Camera_Wiper();
}
//--------------------------------------------------------------------------------
void PelcoD::f_Run_Tur_1(void)
{
    Camera_Run_Tur_1();
}
//--------------------------------------------------------------------------------
void PelcoD::f_send(void)
{
    PELCO_PACKET packet;
    packet.body.sync     = ui->sb_sync->value();
    packet.body.address  = ui->sb_address->value();
    packet.body.cmd1     = ui->sb_cmd1->value();
    packet.body.cmd2     = ui->sb_cmd2->value();
    packet.body.data1    = ui->sb_data1->value();
    packet.body.data2    = ui->sb_data2->value();
    packet.body.crc      = CRC::pelco_crc8((uint8_t *)&packet.buf[1], 5);

    QByteArray ba;
    ba.append((char *)&packet.buf, sizeof(packet));

    emit send(ba);
    emit info(ba.toHex());
}
//--------------------------------------------------------------------------------
void PelcoD::Camera_Save_position(char preset)
{
    PELCO_PACKET packet;
    packet.body.sync     = 0xFF;
    packet.body.address  = ui->sb_addr_cam_presets->value();
    packet.body.cmd1     = 0;
    packet.body.cmd2     = Set_Preset;
    packet.body.data1    = 0;
    packet.body.data2    = preset;
    packet.body.crc      = CRC::pelco_crc8((uint8_t *)&packet.buf[1], 5);

    QByteArray ba;
    ba.append((char *)&packet.buf, sizeof(packet));

    emit send(ba);
    emit info(ba.toHex());
}
//--------------------------------------------------------------------------------
void PelcoD::Camera_Move_position(char preset)
{
    PELCO_PACKET packet;
    packet.body.sync     = 0xFF;
    packet.body.address  = ui->sb_addr_cam_presets->value();
    packet.body.cmd1     = 0;
    packet.body.cmd2     = Go_Preset;
    packet.body.data1    = 0;
    packet.body.data2    = preset;
    packet.body.crc      = CRC::pelco_crc8((uint8_t *)&packet.buf[1], 5);

    QByteArray ba;
    ba.append((char *)&packet.buf, sizeof(packet));

    emit send(ba);
    emit info(ba.toHex());
}
//--------------------------------------------------------------------------------
void PelcoD::Camera_Wiper(void)
{
    PELCO_PACKET packet;
    packet.body.sync     = 0xFF;
    packet.body.address  = ui->sb_addr_cam_presets->value();
    packet.body.cmd1     = 0;
    packet.body.cmd2     = Go_Preset;
    packet.body.data1    = 0;
    packet.body.data2    = 63;
    packet.body.crc      = CRC::pelco_crc8((uint8_t *)&packet.buf[1], 5);

    QByteArray ba;
    ba.append((char *)&packet.buf, sizeof(packet));

    emit send(ba);
    emit info(ba.toHex());
}
//--------------------------------------------------------------------------------
void PelcoD::Camera_Run_Tur_1(void)
{
    PELCO_PACKET packet;
    packet.body.sync     = 0xFF;
    packet.body.address  = ui->sb_addr_cam_presets->value();
    packet.body.cmd1     = 0;
    packet.body.cmd2     = Run_Pattern;
    packet.body.data1    = 0;
    packet.body.data2    = 0;
    packet.body.crc      = CRC::pelco_crc8((uint8_t *)&packet.buf[1], 5);

    QByteArray ba;
    ba.append((char *)&packet.buf, sizeof(packet));

    emit send(ba);
    emit info(ba.toHex());
}
//--------------------------------------------------------------------------------
void PelcoD::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
