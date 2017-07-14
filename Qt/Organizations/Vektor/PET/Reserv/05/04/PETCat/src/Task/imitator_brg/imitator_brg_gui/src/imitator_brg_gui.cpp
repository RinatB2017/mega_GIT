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
#include <QUdpSocket>
#include <QTime>
//--------------------------------------------------------------------------------
#include "imitator_brg_gui.hpp"
#include "ui_imitator_brg_gui.h"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "petcat_options.hpp"
//--------------------------------------------------------------------------------
Imitator_BRG_GUI::Imitator_BRG_GUI(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Imitator_BRG_GUI)
{
    init();

    QVariant imitator_brg_vid = IMITATOR_BRG_DEFAULT_VID;
    QVariant imitator_brg_pid = IMITATOR_BRG_DEFAULT_PID;

    PETCat_options *o = new PETCat_options();
    o->get_imitator_brg_option(PARAM_imitator_brg_vid,    &imitator_brg_vid);
    o->get_imitator_brg_option(PARAM_imitator_brg_pid,    &imitator_brg_pid);
    o->deleteLater();

    //FIXME
#if 0
    PID = imitator_brg_pid.toInt();
    VID = imitator_brg_vid.toInt();
#endif

    load_QSpinBox("Calibration");
    load_QDoubleSpinBox("Calibration");
}
//--------------------------------------------------------------------------------
Imitator_BRG_GUI::~Imitator_BRG_GUI()
{
    save_QSpinBox("Calibration");
    save_QDoubleSpinBox("Calibration");

    if(brg)
    {
        brg->disconnect();
        brg->deleteLater();
    }

    delete ui;
}
//--------------------------------------------------------------------------------
void Imitator_BRG_GUI::init(void)
{
    ui->setupUi(this);

    //---
    brg = new Imitator_BRG();
    //---

    //---
    ui->sb_DDS_TW->setRange(100, 20000000);
    ui->sb_DDS_TW->setValue(100000);    //TODO
    //---
    ui->dsb_DAC_DGAIN_xp->setRange(0, 2.5);
    ui->dsb_DAC_DGAIN_xn->setRange(0, 2.5);
    ui->dsb_DAC_DGAIN_yp->setRange(0, 2.5);
    ui->dsb_DAC_DGAIN_yn->setRange(0, 2.5);
    //---
    ui->dsb_DAC_DOF_xp->setRange(-2.5, 2.5);
    ui->dsb_DAC_DOF_xn->setRange(-2.5, 2.5);
    ui->dsb_DAC_DOF_yp->setRange(-2.5, 2.5);
    ui->dsb_DAC_DOF_yn->setRange(-2.5, 2.5);
    //---
    ui->dsb_DDS_PW_xp->setRange(0, 360);
    ui->dsb_DDS_PW_xn->setRange(0, 360);
    ui->dsb_DDS_PW_yp->setRange(0, 360);
    ui->dsb_DDS_PW_yn->setRange(0, 360);
    //---

    //---
    ui->dsb_DAC_DGAIN_xp->setDecimals(2);
    ui->dsb_DAC_DGAIN_xn->setDecimals(2);
    ui->dsb_DAC_DGAIN_yp->setDecimals(2);
    ui->dsb_DAC_DGAIN_yn->setDecimals(2);
    //---
    ui->dsb_DAC_DOF_xp->setDecimals(2);
    ui->dsb_DAC_DOF_xn->setDecimals(2);
    ui->dsb_DAC_DOF_yp->setDecimals(2);
    ui->dsb_DAC_DOF_yn->setDecimals(2);
    //---
    ui->dsb_DDS_PW_xp->setDecimals(2);
    ui->dsb_DDS_PW_xn->setDecimals(2);
    ui->dsb_DDS_PW_yp->setDecimals(2);
    ui->dsb_DDS_PW_yn->setDecimals(2);
    //---

    //---
    ui->dsb_DAC_DGAIN_xp->setSingleStep(0.01);
    ui->dsb_DAC_DGAIN_xn->setSingleStep(0.01);
    ui->dsb_DAC_DGAIN_yp->setSingleStep(0.01);
    ui->dsb_DAC_DGAIN_yn->setSingleStep(0.01);
    //---
    ui->dsb_DAC_DOF_xp->setSingleStep(0.01);
    ui->dsb_DAC_DOF_xn->setSingleStep(0.01);
    ui->dsb_DAC_DOF_yp->setSingleStep(0.01);
    ui->dsb_DAC_DOF_yn->setSingleStep(0.01);
    //---
    ui->dsb_DDS_PW_xp->setSingleStep(0.01);
    ui->dsb_DDS_PW_xn->setSingleStep(0.01);
    ui->dsb_DDS_PW_yp->setSingleStep(0.01);
    ui->dsb_DDS_PW_yn->setSingleStep(0.01);
    //---

    //---
    connect(ui->btn_run,    SIGNAL(clicked(bool)),  this,   SLOT(start()));
    connect(ui->btn_stop,   SIGNAL(clicked(bool)),  this,   SLOT(stop()));
    //---
    connect(ui->btn_refresh,    SIGNAL(clicked(bool)),  this,   SLOT(refresh()));
    brg->refresh();
    //---
}
//--------------------------------------------------------------------------------
void Imitator_BRG_GUI::block_interface(bool state)
{
    ui->sb_DDS_TW->setDisabled(state);
    ui->dsb_DAC_DGAIN_xp->setDisabled(state);
    ui->dsb_DAC_DGAIN_xn->setDisabled(state);
    ui->dsb_DAC_DGAIN_yp->setDisabled(state);
    ui->dsb_DAC_DGAIN_yn->setDisabled(state);
    ui->dsb_DAC_DOF_xp->setDisabled(state);
    ui->dsb_DAC_DOF_xn->setDisabled(state);
    ui->dsb_DAC_DOF_yp->setDisabled(state);
    ui->dsb_DAC_DOF_yn->setDisabled(state);
    ui->dsb_DDS_PW_xp->setDisabled(state);
    ui->dsb_DDS_PW_xn->setDisabled(state);
    ui->dsb_DDS_PW_yp->setDisabled(state);
    ui->dsb_DDS_PW_yn->setDisabled(state);
    ui->btn_run->setDisabled(state);
    ui->btn_stop->setDisabled(state);
}
//--------------------------------------------------------------------------------
bool Imitator_BRG_GUI::set_values(void)
{
    const float _Amplitude1mV = 7.142857142857143f;
    const float _Offset1mV = 28.0f;
    const float _Phase1deg = 11.0f;

    const uint64_t Dev_FCLK = 125000000; // тактовая частота DDS в Гц

    uint64_t DDS_TW = (uint64_t)(((float)ui->sb_DDS_TW->value()) / ((float)Dev_FCLK / (float)0x1000000));

    uint16_t DAC_DGAIN_XP = (uint16_t)(((float)ui->dsb_DAC_DGAIN_xp->value() * 1000.0f) * _Amplitude1mV);
    uint16_t DAC_DGAIN_XN = (uint16_t)(((float)ui->dsb_DAC_DGAIN_xn->value() * 1000.0f) * _Amplitude1mV);
    uint16_t DAC_DGAIN_YP = (uint16_t)(((float)ui->dsb_DAC_DGAIN_yp->value() * 1000.0f) * _Amplitude1mV);
    uint16_t DAC_DGAIN_YN = (uint16_t)(((float)ui->dsb_DAC_DGAIN_yn->value() * 1000.0f) * _Amplitude1mV);

    uint16_t DAC_DOF_XP = (uint16_t)(((float)ui->dsb_DAC_DOF_xp->value() * 1000.0f) * _Offset1mV);
    uint16_t DAC_DOF_XN = (uint16_t)(((float)ui->dsb_DAC_DOF_xn->value() * 1000.0f) * _Offset1mV);
    uint16_t DAC_DOF_YP = (uint16_t)(((float)ui->dsb_DAC_DOF_yp->value() * 1000.0f) * _Offset1mV);
    uint16_t DAC_DOF_YN = (uint16_t)(((float)ui->dsb_DAC_DOF_yn->value() * 1000.0f) * _Offset1mV);

    uint16_t DDS_PW_XP = (uint16_t)((float)ui->dsb_DDS_PW_xp->value() * _Phase1deg) << 4;
    uint16_t DDS_PW_XN = (uint16_t)((float)ui->dsb_DDS_PW_xn->value() * _Phase1deg) << 4;
    uint16_t DDS_PW_YP = (uint16_t)((float)ui->dsb_DDS_PW_yp->value() * _Phase1deg) << 4;
    uint16_t DDS_PW_YN = (uint16_t)((float)ui->dsb_DDS_PW_yn->value() * _Phase1deg) << 4;

    Imitator_BRG::PARAM param;
    param.DDS_TW = DDS_TW;
    param.DAC_DGAIN_XP = DAC_DGAIN_XP;
    param.DAC_DGAIN_XN = DAC_DGAIN_XN;
    param.DAC_DGAIN_YP = DAC_DGAIN_YP;
    param.DAC_DGAIN_YN = DAC_DGAIN_YN;
    param.DAC_DOF_XP = DAC_DOF_XP;
    param.DAC_DOF_XN = DAC_DOF_XN;
    param.DAC_DOF_YP = DAC_DOF_YP;
    param.DAC_DOF_YN = DAC_DOF_YN;
    param.DDS_PW_XP = DDS_PW_XP;
    param.DDS_PW_XN = DDS_PW_XN;
    param.DDS_PW_YP = DDS_PW_YP;
    param.DDS_PW_YN = DDS_PW_YN;

    return brg->set_param(param);
}
//--------------------------------------------------------------------------------
void Imitator_BRG_GUI::test(void)
{
    block_this_button(true);

    block_this_button(false);
}
//--------------------------------------------------------------------------------
