/*********************************************************************************
**                                                                              **
**     Copyright (C) 2022                                                       **
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
#include "v786_2_multimeter.hpp"
#include "v786_2_packets.hpp"
//--------------------------------------------------------------------------------
#include "handcontrol.hpp"
#include "ui_handcontrol.h"
#include "defines.hpp"
//--------------------------------------------------------------------------------
HandControl::HandControl(Calibrator *calibrator,
                         Multimeter_V786_2 *multimeter,
                         QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HandControl),
    calibrator(calibrator),
    multimeter(multimeter)
{
    init();
}
//--------------------------------------------------------------------------------
HandControl::~HandControl()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void HandControl::connect_log(void)
{
    connect(this, SIGNAL(info(QString)),  parentWidget(), SIGNAL(info(QString)));
    connect(this, SIGNAL(debug(QString)), parentWidget(), SIGNAL(debug(QString)));
    connect(this, SIGNAL(error(QString)), parentWidget(), SIGNAL(error(QString)));
    connect(this, SIGNAL(trace(QString)), parentWidget(), SIGNAL(trace(QString)));
}
//--------------------------------------------------------------------------------
void HandControl::init(void)
{
    ui->setupUi(this);

    connect_log();

    serno = 0;
    year = 0;
    address = 0;
    channel = 0;

    ui->cb_multimeter_cmd->addItem(V764_2_RF_STATUS_REQUEST_TEXT, QVariant(V764_2_RF_STATUS_REQUEST));
    ui->cb_multimeter_cmd->addItem(V764_2_RF_REMOTE_CONTROL_ENABLE_TEXT, QVariant(V764_2_RF_REMOTE_CONTROL_ENABLE));
    ui->cb_multimeter_cmd->addItem(V764_2_RF_REMOTE_CONTROL_DISABLE_TEXT, QVariant(V764_2_RF_REMOTE_CONTROL_DISABLE));
    ui->cb_multimeter_cmd->addItem(V764_2_RF_SERVICE_ENABLE_TEXT, QVariant(V764_2_RF_SERVICE_ENABLE));
    ui->cb_multimeter_cmd->addItem(V764_2_RF_POWER_OFF_TEXT, QVariant(V764_2_RF_POWER_OFF));
    ui->cb_multimeter_cmd->addItem(V764_2_RF_DC_VOLTAGE_MEASUREMENT_TEXT, QVariant(V764_2_RF_DC_VOLTAGE_MEASUREMENT));
    ui->cb_multimeter_cmd->addItem(V764_2_RF_AC_VOLTAGE_MEASUREMENT_TEXT, QVariant(V764_2_RF_AC_VOLTAGE_MEASUREMENT));
    ui->cb_multimeter_cmd->addItem(V764_2_RF_DC_CURRENT_MEASUREMENT_TEXT, QVariant(V764_2_RF_DC_CURRENT_MEASUREMENT));
    ui->cb_multimeter_cmd->addItem(V764_2_RF_AC_CURRENT_MEASUREMENT_TEXT, QVariant(V764_2_RF_AC_CURRENT_MEASUREMENT));
    ui->cb_multimeter_cmd->addItem(V764_2_RF_RESISTANCE_MEASUREMENT_TEXT, QVariant(V764_2_RF_RESISTANCE_MEASUREMENT));
    ui->cb_multimeter_cmd->addItem(V764_2_RF_CAPACITANCE_MEASUREMENT_TEXT, QVariant(V764_2_RF_CAPACITANCE_MEASUREMENT));
    ui->cb_multimeter_cmd->addItem(V764_2_RF_INDUCTANCE_MEASUREMENT_TEXT, QVariant(V764_2_RF_INDUCTANCE_MEASUREMENT));
    ui->cb_multimeter_cmd->addItem(V764_2_RF_BUZZER_TEST_TEXT, QVariant(V764_2_RF_BUZZER_TEST));
    ui->cb_multimeter_cmd->addItem(V764_2_RF_FREQUENCY_MEASUREMENT_TEXT, QVariant(V764_2_RF_FREQUENCY_MEASUREMENT));
    ui->cb_multimeter_cmd->addItem(V764_2_RF_ACTIVE_POWER_MEASUREMENT_TEXT, QVariant(V764_2_RF_ACTIVE_POWER_MEASUREMENT));
    ui->cb_multimeter_cmd->addItem(V764_2_RF_REACTIVE_POWER_MEASUREMENT_TEXT, QVariant(V764_2_RF_REACTIVE_POWER_MEASUREMENT));
    ui->cb_multimeter_cmd->addItem(V764_2_RF_FULL_POWER_MEASUREMENT_TEXT, QVariant(V764_2_RF_FULL_POWER_MEASUREMENT));
    ui->cb_multimeter_cmd->addItem(V764_2_RF_COS_FI_MEASUREMENT_TEXT, QVariant(V764_2_RF_COS_FI_MEASUREMENT));
    ui->cb_multimeter_cmd->addItem(V764_2_RF_UDC_CAL_TEXT, QVariant(V764_2_RF_UDC_CAL));
    ui->cb_multimeter_cmd->addItem(V764_2_RF_UAC_CAL_TEXT, QVariant(V764_2_RF_UAC_CAL));
    ui->cb_multimeter_cmd->addItem(V764_2_RF_IDC_CAL_TEXT, QVariant(V764_2_RF_IDC_CAL));
    ui->cb_multimeter_cmd->addItem(V764_2_RF_IAC_CAL_TEXT, QVariant(V764_2_RF_IAC_CAL));
    ui->cb_multimeter_cmd->addItem(V764_2_RF_RES_CAL_TEXT, QVariant(V764_2_RF_RES_CAL));
    ui->cb_multimeter_cmd->addItem(V764_2_RF_FLASH_WRITE_TEXT, QVariant(V764_2_RF_FLASH_WRITE));

    ui->cb_multimeter_limit_U->addItem(L_200mV_TEXT, QVariant(L_200mV));
    ui->cb_multimeter_limit_U->addItem(L_2V_TEXT,    QVariant(L_2V));
    ui->cb_multimeter_limit_U->addItem(L_20V_TEXT,   QVariant(L_20V));
    ui->cb_multimeter_limit_U->addItem(L_200V_TEXT,  QVariant(L_200V));
    ui->cb_multimeter_limit_U->addItem(L_1000V_TEXT, QVariant(L_1000V));

    ui->cb_multimeter_limit_I->addItem(L_200mkA_TEXT,   QVariant(L_200mkA));
    ui->cb_multimeter_limit_I->addItem(L_2mA_TEXT,      QVariant(L_2mA));
    ui->cb_multimeter_limit_I->addItem(L_20mA_TEXT,     QVariant(L_20mA));
    ui->cb_multimeter_limit_I->addItem(L_200mA_TEXT,    QVariant(L_200mA));
    ui->cb_multimeter_limit_I->addItem(L_2000mA_TEXT,   QVariant(L_2000mA));
    ui->cb_multimeter_limit_I->addItem(L_20A_TEXT,      QVariant(L_20A));

    ui->cb_multimeter_limit_R->addItem(L_200o_TEXT,     QVariant(L_200o));
    ui->cb_multimeter_limit_R->addItem(L_2kOm_TEXT,     QVariant(L_2kOm));
    ui->cb_multimeter_limit_R->addItem(L_20kOm_TEXT,    QVariant(L_20kOm));
    ui->cb_multimeter_limit_R->addItem(L_200kOm_TEXT,   QVariant(L_200kOm));
    ui->cb_multimeter_limit_R->addItem(L_2Mom_TEXT,     QVariant(L_2Mom));


    connect(ui->btn_multimeter_run_cmd, SIGNAL(clicked()), this, SLOT(send_cmd_to_multimeter()));
    connect(ui->cb_multimeter_cmd, SIGNAL(currentIndexChanged(int)), this, SLOT(change_cmd_multimeter(int)));

    ui->cb_multimeter_cmd->setCurrentIndex(1);
    ui->cb_multimeter_cmd->setCurrentIndex(0);
}
//--------------------------------------------------------------------------------
void HandControl::set_serno(unsigned int value)
{
    serno = value;
}
//--------------------------------------------------------------------------------
void HandControl::set_year(unsigned int value)
{
    year = value;
}
//--------------------------------------------------------------------------------
void HandControl::set_address(unsigned int value)
{
    address = value;
}
//--------------------------------------------------------------------------------
void HandControl::set_channel(unsigned int value)
{
    channel = value;
}
//--------------------------------------------------------------------------------
void HandControl::send_cmd_to_multimeter(void)
{
    if(!multimeter)
    {
        emit error("multimeter is 0!");
        return;
    }

    bool ok = false;
    int cmd = ui->cb_multimeter_cmd->itemData(ui->cb_multimeter_cmd->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok) return;

    multimeter->set_serno(static_cast<uint>(serno));
    multimeter->set_year(static_cast<uint>(year));
    multimeter->set_address(static_cast<uint>(address));
    multimeter->set_channel(static_cast<uint>(channel));
    multimeter->set_timeout(timeout);

    ui->btn_multimeter_run_cmd->setEnabled(false);
    multimeter->print_cmd_text(cmd);
    ok = multimeter->find_cmd(cmd);
    if(!ok)
    {
        multimeter->print_last_error();
    }
    ui->btn_multimeter_run_cmd->setEnabled(true);
}
//--------------------------------------------------------------------------------
void HandControl::controls_visible(bool U,
                                   bool I,
                                   bool R,
                                   bool freq,
                                   bool auto_limit)
{
    ui->cb_multimeter_limit_U->setVisible(U);
    ui->cb_multimeter_limit_I->setVisible(I);
    ui->cb_multimeter_limit_R->setVisible(R);
    ui->sb_multimeter_freq->setVisible(freq);
    ui->cb_auto->setVisible(auto_limit);
}
//--------------------------------------------------------------------------------
void HandControl::change_cmd_multimeter(int index)
{
    switch(index + 1)
    {
    case V764_2_RF_STATUS_REQUEST:              controls_visible(false, false, false, false, false);   break;
    case V764_2_RF_REMOTE_CONTROL_ENABLE:       controls_visible(false, false, false, false, false);   break;
    case V764_2_RF_REMOTE_CONTROL_DISABLE:      controls_visible(false, false, false, false, false);   break;
    case V764_2_RF_SERVICE_ENABLE:              controls_visible(false, false, false, false, false);   break;
    case V764_2_RF_POWER_OFF:                   controls_visible(false, false, false, false, false);   break;
    case V764_2_RF_DC_VOLTAGE_MEASUREMENT:      controls_visible(true, false, false, false, false);    break;
    case V764_2_RF_AC_VOLTAGE_MEASUREMENT:      controls_visible(true, false, false, true, false);     break;
    case V764_2_RF_DC_CURRENT_MEASUREMENT:      controls_visible(false, true, false, false, false);    break;
    case V764_2_RF_AC_CURRENT_MEASUREMENT:      controls_visible(false, true, false, true, false);     break;
    case V764_2_RF_RESISTANCE_MEASUREMENT:      controls_visible(false, false, true, false, false);    break;
    case V764_2_RF_CAPACITANCE_MEASUREMENT:     controls_visible(false, false, false, false, false);   break;
    case V764_2_RF_INDUCTANCE_MEASUREMENT:      controls_visible(false, false, false, false, false);   break;
    case V764_2_RF_BUZZER_TEST:                 controls_visible(false, false, false, false, false);   break;
    case V764_2_RF_FREQUENCY_MEASUREMENT:       controls_visible(false, false, false, false, false);   break;
    case V764_2_RF_ACTIVE_POWER_MEASUREMENT:    controls_visible(false, false, false, false, false);   break;
    case V764_2_RF_REACTIVE_POWER_MEASUREMENT:  controls_visible(false, false, false, false, false);   break;
    case V764_2_RF_FULL_POWER_MEASUREMENT:      controls_visible(false, false, false, false, false);   break;
    case V764_2_RF_COS_FI_MEASUREMENT:          controls_visible(false, false, false, false, false);   break;
    case V764_2_RF_UDC_CAL:                     controls_visible(true, false, false, false, false);    break;
    case V764_2_RF_UAC_CAL:                     controls_visible(true, false, false, false, false);    break;
    case V764_2_RF_IDC_CAL:                     controls_visible(false, true, false, false, false);    break;
    case V764_2_RF_IAC_CAL:                     controls_visible(false, true, false, true, false);     break;
    case V764_2_RF_RES_CAL:                     controls_visible(false, false, true, false, false);    break;
    case V764_2_RF_FLASH_WRITE:                 controls_visible(false, false, false, false, false);   break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
