/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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

    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x01_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x01));
    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x02_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x02));
    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x03_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x03));
    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x04_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x04));
    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x05_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x05));
    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x06_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x06));
    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x07_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x07));
    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x08_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x08));
    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x09_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x09));
    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x0A_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x0A));
    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x0B_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x0B));
    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x0C_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x0C));
    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x0D_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x0D));
    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x0E_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x0E));
    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x0F_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x0F));
    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x10_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x10));
    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x11_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x11));
    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x12_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x12));
    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x13_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x13));
    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x14_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x14));
    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x15_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x15));
    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x16_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x16));
    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x17_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x17));
    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x18_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x18));
    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x19_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x19));
    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x1A_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x1A));
    ui->cb_multimeter_cmd->addItem(V764_2_CMD_0x1B_TEXT, QVariant(Qt::UserRole + V764_2_CMD_0x1B));

    ui->cb_multimeter_limit_U->addItem(L_200mV_TEXT, QVariant(Qt::UserRole + L_200mV));
    ui->cb_multimeter_limit_U->addItem(L_2V_TEXT,    QVariant(Qt::UserRole + L_2V));
    ui->cb_multimeter_limit_U->addItem(L_20V_TEXT,   QVariant(Qt::UserRole + L_20V));
    ui->cb_multimeter_limit_U->addItem(L_200V_TEXT,  QVariant(Qt::UserRole + L_200V));
    ui->cb_multimeter_limit_U->addItem(L_1000V_TEXT, QVariant(Qt::UserRole + L_1000V));

    ui->cb_multimeter_limit_I->addItem(L_200mkA_TEXT,   QVariant(Qt::UserRole + L_200mkA));
    ui->cb_multimeter_limit_I->addItem(L_2mA_TEXT,      QVariant(Qt::UserRole + L_2mA));
    ui->cb_multimeter_limit_I->addItem(L_20mA_TEXT,     QVariant(Qt::UserRole + L_20mA));
    ui->cb_multimeter_limit_I->addItem(L_200mA_TEXT,    QVariant(Qt::UserRole + L_200mA));
    ui->cb_multimeter_limit_I->addItem(L_2000mA_TEXT,   QVariant(Qt::UserRole + L_2000mA));
    ui->cb_multimeter_limit_I->addItem(L_20A_TEXT,      QVariant(Qt::UserRole + L_20A));

    ui->cb_multimeter_limit_R->addItem(L_200o_TEXT,     QVariant(Qt::UserRole + L_200o));
    ui->cb_multimeter_limit_R->addItem(L_2kOm_TEXT,     QVariant(Qt::UserRole + L_2kOm));
    ui->cb_multimeter_limit_R->addItem(L_20kOm_TEXT,    QVariant(Qt::UserRole + L_20kOm));
    ui->cb_multimeter_limit_R->addItem(L_200kOm_TEXT,   QVariant(Qt::UserRole + L_200kOm));
    ui->cb_multimeter_limit_R->addItem(L_2Mom_TEXT,     QVariant(Qt::UserRole + L_2Mom));


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
    int cmd = ui->cb_multimeter_cmd->itemData(ui->cb_multimeter_cmd->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;

    multimeter->set_serno(serno);
    multimeter->set_year(year);
    multimeter->set_address(address);
    multimeter->set_channel(channel);
    multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);

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
    case V764_2_CMD_0x01:   controls_visible(false, false, false, false, false);   break;
    case V764_2_CMD_0x02:   controls_visible(false, false, false, false, false);   break;
    case V764_2_CMD_0x03:   controls_visible(false, false, false, false, false);   break;
    case V764_2_CMD_0x04:   controls_visible(false, false, false, false, false);   break;
    case V764_2_CMD_0x05:   controls_visible(false, false, false, false, false);   break;
    case V764_2_CMD_0x06:   controls_visible(true, false, false, false, false);    break;
    case V764_2_CMD_0x07:   controls_visible(true, false, false, true, false);     break;
    case V764_2_CMD_0x08:   controls_visible(false, true, false, false, false);    break;
    case V764_2_CMD_0x09:   controls_visible(false, true, false, true, false);     break;
    case V764_2_CMD_0x0A:   controls_visible(false, false, true, false, false);    break;
    case V764_2_CMD_0x0B:   controls_visible(false, false, false, false, false);   break;
    case V764_2_CMD_0x0C:   controls_visible(false, false, false, false, false);   break;
    case V764_2_CMD_0x0D:   controls_visible(false, false, false, false, false);   break;
    case V764_2_CMD_0x0E:   controls_visible(false, false, false, false, false);   break;
    case V764_2_CMD_0x0F:   controls_visible(false, false, false, false, false);   break;
    case V764_2_CMD_0x10:   controls_visible(false, false, false, false, false);   break;
    case V764_2_CMD_0x11:   controls_visible(false, false, false, false, false);   break;
    case V764_2_CMD_0x12:   controls_visible(false, false, false, false, false);   break;
    case V764_2_CMD_0x13:   controls_visible(true, false, false, false, false);    break;
    case V764_2_CMD_0x14:   controls_visible(true, false, false, false, false);    break;
    case V764_2_CMD_0x15:   controls_visible(false, true, false, false, false);    break;
    case V764_2_CMD_0x16:   controls_visible(false, true, false, true, false);     break;
    case V764_2_CMD_0x17:   controls_visible(false, false, true, false, false);    break;
    case V764_2_CMD_0x18:   controls_visible(false, false, false, false, false);   break;
    case V764_2_CMD_0x19:   controls_visible(false, false, false, false, false);   break;
    case V764_2_CMD_0x1A:   controls_visible(false, false, false, false, false);   break;
    case V764_2_CMD_0x1B:   controls_visible(false, false, false, false, false);   break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
