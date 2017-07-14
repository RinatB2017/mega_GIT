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
#include <QVBoxLayout>
#include <QScrollArea>
#include <QRadioButton>
#include <QToolButton>
#include <QCheckBox>
#include <QComboBox>
#include <QDateTime>
#include <QLineEdit>
#include <QSpinBox>
#include <QDebug>
#include <QTime>
//--------------------------------------------------------------------------------
#include "pet_td_main_page.hpp"
#include "ui_pet_td_main_page.h"
#include "defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
PET_td_main_page::PET_td_main_page(QWidget *parent, bool is_demo) :
    PET_device_td(parent),
    ui(new Ui::PET_td_main_page),
    flag_is_demo(is_demo)
{
    init();
}
//--------------------------------------------------------------------------------
PET_td_main_page::~PET_td_main_page()
{
    if(p_device_info)   p_device_info->deleteLater();
    if(p_net_config)    p_net_config->deleteLater();

    delete ui;
}
//--------------------------------------------------------------------------------
void PET_td_main_page::update_widgets(void)
{
    main_refresh();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void PET_td_main_page::main_refresh(void)
{
    block_this_button(true);
    f_PET_CMD_GET_DEVICE_INFO();
    f_PET_CMD_GET_CONTROL();
    block_this_button(false);
}
//--------------------------------------------------------------------------------
void PET_td_main_page::main_apply(void)
{
    block_this_button(true);
    // нельзя это устанавливать
    block_this_button(false);
}
//--------------------------------------------------------------------------------
void PET_td_main_page::main_close(void)
{
    emit error("Close");
    parentWidget()->close();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void PET_td_main_page::add_page_p_device_info(void)
{
    p_device_info = new PET_device_info_page(this);
    //---
    QWidget *w = new QWidget(this);
    QVBoxLayout *l_device_info = new QVBoxLayout;
    l_device_info->addWidget(p_device_info);
    l_device_info->addStretch(1);
    w->setLayout(l_device_info);
    //---
    QScrollArea *scroll = new QScrollArea;
    scroll->setWidgetResizable( true );
    scroll->setWidget(w);
    scroll->setObjectName("p_device_info");
    //---
    ui->tabWidget->addTab(scroll, tr("Общие"));
}
//--------------------------------------------------------------------------------
void PET_td_main_page::add_page_p_net_config(void)
{
    p_net_config = new PET_net_config_page(this);
    //---
    QWidget *w = new QWidget(this);
    QVBoxLayout *l_net_config = new QVBoxLayout;
    l_net_config->addWidget(p_net_config);
    l_net_config->addStretch(1);
    w->setLayout(l_net_config);
    //---
    QScrollArea *scroll = new QScrollArea;
    scroll->setWidgetResizable( true );
    scroll->setWidget(w);
    scroll->setObjectName("p_net_config");
    //---
    ui->tabWidget->addTab(scroll, tr("Сеть"));
}
//--------------------------------------------------------------------------------
void PET_td_main_page::init(void)
{
    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(0);
    //---
    add_page_p_device_info();
    add_page_p_net_config();
    //---
    ui->btn_close->setDisabled(true);
    //---
    if(flag_is_demo)
    {
        ui->btn_refresh->setDisabled(true);
        ui->btn_apply->setDisabled(true);
        ui->btn_close->setDisabled(true);
    }
    else
    {
        connect(ui->btn_refresh,    SIGNAL(clicked(bool)),     this,   SLOT(main_refresh()));
        connect(ui->btn_apply,      SIGNAL(clicked(bool)),     this,   SLOT(main_apply()));
        connect(ui->btn_close,      SIGNAL(clicked(bool)),     this,   SLOT(main_close()));
    }
    //---
    connect(ui->btn_import, SIGNAL(clicked(bool)),  this,   SLOT(f_import()));
    connect(ui->btn_export, SIGNAL(clicked(bool)),  this,   SLOT(f_export()));
}
//--------------------------------------------------------------------------------
void PET_td_main_page::all_refresh(void)
{
    f_PET_CMD_GET_DEVICE_INFO();
    //f_PET_CMD_GET_STATE();
    f_PET_CMD_GET_CONTROL();
}
//--------------------------------------------------------------------------------
void PET_td_main_page::get_packet(QByteArray packet)
{
    if(packet.length() < (int)sizeof(pet_frame_t))
    {
        return;
    }
    pet_frame_t *frame = (pet_frame_t *)packet.data();
    Q_CHECK_PTR(frame);
    uint8_t cmd = frame->cmd & 0x7F;
    switch(cmd)
    {
    case PET_CMD_GET_DEVICE_INFO:
    case PET_CMD_GET_CONTROL:
    case PET_CMD_GET_STATE:
        is_ok = true;
        break;

    default:
        break;
    }

    if(is_ok)
    {
        clean_packet_buf.clear();
        clean_packet_buf.append(packet);
    }
}
//--------------------------------------------------------------------------------
PET_RESULT PET_td_main_page::f_PET_CMD_GET_DEVICE_INFO(void)
{
    emit info("Запрос информации об устройстве");
    block_interface(p_device_info, true);
    //---
    pet_device_info_t d_info;
    int data_len = sizeof(d_info);
    //---
    PET_RESULT err = PET_ERR_FAULT;
    do
    {
        err = get_device_info(&d_info, &data_len);
        if(err != PET_SUCCESS)
        {
            QString title = "Ошибка";
            QString text = QString("%1\n").arg(get_PET_RESULT_string(err));
            text += "Повторить запрос?";

            int btn = messagebox_question(title, text);

            if(btn != MyWidget::Yes)
            {
                emit error(get_PET_RESULT_string(err));
                return err;
            }
        }
    } while (err != PET_SUCCESS);
    //---
    p_device_info->set(d_info);
    block_interface(p_device_info, false);
    emit info("Выполнено");
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_td_main_page::f_PET_CMD_GET_CONTROL(void)
{
    emit info("Запрос параметров режима работы устройства");
    //block_interface(p_bos_ctrl, true);
    //---
    pet_bos_ctrl_t ctrl;
    int ctrl_len = sizeof(ctrl);
    //---
    PET_RESULT err = PET_ERR_FAULT;
    do
    {
        err = get_ctrl(&ctrl, &ctrl_len);
        if(err != PET_SUCCESS)
        {
            QString title = "Ошибка";
            QString text = QString("%1\n").arg(get_PET_RESULT_string(err));
            text += "Повторить запрос?";

            int btn = messagebox_question(title, text);

            if(btn != MyWidget::Yes)
            {
                emit error(get_PET_RESULT_string(err));
                return err;
            }
        }
    } while (err != PET_SUCCESS);
    //---
    p_net_config->set(ctrl);
    //block_interface(p_bos_ctrl, false);
    emit info("Выполнено");
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_td_main_page::f_PET_CMD_SET_CONTROL(void)
{
    emit info("Установка параметров режима работы устройства");
    //---
    pet_bos_ctrl_t i_data;
    int i_data_len = sizeof(i_data);
    //---
    pet_bos_ctrl_t o_data;
    int o_data_len = sizeof(o_data);
    //'---
    PET_RESULT err = PET_ERR_FAULT;
    do
    {
        err = _set_ctrl(&i_data, &i_data_len,
                        &o_data, &o_data_len);
        if(err != PET_SUCCESS)
        {
            QString title = "Ошибка";
            QString text = QString("%1\n").arg(get_PET_RESULT_string(err));
            text += "Повторить запрос?";

            int btn = messagebox_question(title, text);

            if(btn != MyWidget::Yes)
            {
                emit error(get_PET_RESULT_string(err));
                return err;
            }
        }
    } while (err != PET_SUCCESS);
    //---
    emit info("Выполнено");
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
void PET_td_main_page::block_this_button(bool state)
{
    QObject *btn = (QObject *)sender();
    if(!btn) return;

    if (QAbstractButton *pb=qobject_cast<QAbstractButton *>(btn))
    {
        pb->setDisabled(state);
    }
}
//--------------------------------------------------------------------------------
void PET_td_main_page::block_interface(QWidget *widget, bool state)
{
    if(!widget)
    {
        return;
    }
    QList<QCheckBox *> all_chb = widget->findChildren<QCheckBox *>();
    foreach(QCheckBox *obj, all_chb)
    {
        if(obj->property(FLAG_NO_BLOCK).toBool() == false)
        {
            obj->setDisabled(state);
        }
    }
    QList<QComboBox *> all_cb = widget->findChildren<QComboBox *>();
    foreach(QComboBox *obj, all_cb)
    {
        if(obj->property(FLAG_NO_BLOCK).toBool() == false)
        {
            obj->setDisabled(state);
        }
    }
    QList<QSpinBox *> all_sb = widget->findChildren<QSpinBox *>();
    foreach(QSpinBox *obj, all_sb)
    {
        if(obj->property(FLAG_NO_BLOCK).toBool() == false)
        {
            obj->setDisabled(state);
        }
    }
    QList<QRadioButton *> all_rb = widget->findChildren<QRadioButton *>();
    foreach(QRadioButton *obj, all_rb)
    {
        if(obj->property(FLAG_NO_BLOCK).toBool() == false)
        {
            obj->setDisabled(state);
        }
    }
    QList<QLineEdit *> all_le = widget->findChildren<QLineEdit *>();
    foreach(QLineEdit *obj, all_le)
    {
        if(obj->property(FLAG_NO_BLOCK).toBool() == false)
        {
            obj->setDisabled(state);
        }
    }
}
//--------------------------------------------------------------------------------
bool PET_td_main_page::check_packet(uint8_t cmd, QByteArray packet)
{
    //emit debug(QString("check_size_packet: [%1]").arg(packet.toHex().data()));
    if(packet.length() < (int)sizeof(pet_frame_t))
    {
        emit error("PET_td_main_page::check_size_packet: packet too small");
        return false;
    }

    pet_frame_t *frame = (pet_frame_t *)packet.data();
    Q_CHECK_PTR(frame);
    int len_packet = frame->len + sizeof(pet_frame_t);
    if(packet.length() != len_packet)
    {
        emit error("PET_td_main_page::check_size_packet: bad len");
        return false;
    }

    uint8_t p_cmd = frame->cmd & 0x7F;
    if(p_cmd != cmd) return false;
    if(frame->cmd & PET_FRAME_CMD_FAULT_MSK)
    {
        pet_frame_error_t *err_frame = (pet_frame_error_t *)packet.data();
        emit error(get_PET_RESULT_string(err_frame->result_cmd));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
//FIXME костыль
void PET_td_main_page::f_import(void)
{
    PET_device_td::f_import();
}
void PET_td_main_page::f_export(void)
{
    PET_device_td::f_export();
}
//--------------------------------------------------------------------------------
