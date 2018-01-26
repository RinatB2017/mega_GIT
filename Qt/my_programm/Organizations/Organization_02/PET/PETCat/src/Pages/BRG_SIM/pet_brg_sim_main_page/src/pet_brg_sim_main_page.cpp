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
#include <QStackedWidget>
#include <QRadioButton>
#include <QToolButton>
#include <QScrollArea>
#include <QComboBox>
#include <QCheckBox>
#include <QDateTime>
#include <QLineEdit>
#include <QSpinBox>
//--------------------------------------------------------------------------------
#include "pet_brg_sim_main_page.hpp"
#include "ui_pet_brg_sim_main_page.h"
//--------------------------------------------------------------------------------
#include "PET_device_brg.hpp"
//--------------------------------------------------------------------------------
#include "sleeper.h"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
PET_brg_sim_main_page::PET_brg_sim_main_page(QWidget *parent, bool is_demo) :
    PET_device_brg_sim(parent),
    ui(new Ui::PET_brg_sim_main_page),
    flag_is_demo(is_demo)
{
    init();
}
//--------------------------------------------------------------------------------
PET_brg_sim_main_page::~PET_brg_sim_main_page()
{
    if(p_brg_device_info) p_brg_device_info->deleteLater();
    if(p_brg_debug) p_brg_debug->deleteLater();

    if(p_brg_ch_0)  p_brg_ch_0->deleteLater();
    if(p_brg_ch_1)  p_brg_ch_1->deleteLater();
    if(p_brg_ch_2)  p_brg_ch_2->deleteLater();
    if(p_brg_ch_3)  p_brg_ch_3->deleteLater();

    delete ui;
}
//--------------------------------------------------------------------------------
void PET_brg_sim_main_page::add_page_device_info(void)
{
    p_brg_device_info = new PET_device_info_page(this);
    //---
    QWidget *w = new QWidget(this);
    QVBoxLayout *l_device_info = new QVBoxLayout;
    l_device_info->addWidget(p_brg_device_info);
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
void PET_brg_sim_main_page::add_page_debug(void)
{
    p_brg_debug     = new PET_brg_debug_page(0);
    //---
    QWidget *w = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(p_brg_debug);
    layout->addStretch(1);
    w->setLayout(layout);
    //---
    QScrollArea *scroll = new QScrollArea;
    scroll->setWidgetResizable( true );
    scroll->setWidget(w);
    scroll->setObjectName("p_brg_ctrl_state");
    //---
    ui->tabWidget->addTab(scroll, tr("Отладка"));
}
//--------------------------------------------------------------------------------
void PET_brg_sim_main_page::add_page_ch0(void)
{
    p_brg_ch_0  = new PET_brg_ch_page(0, this);
    //---
    QWidget *w = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(p_brg_ch_0);
    layout->addStretch(1);
    w->setLayout(layout);
    //---
    QScrollArea *scroll = new QScrollArea;
    scroll->setWidgetResizable( true );
    scroll->setWidget(w);
    scroll->setObjectName("p_brg_ctrl_state_ch0");
    //---
    ui->tabWidget->addTab(scroll, tr("Канал 1"));
}
//--------------------------------------------------------------------------------
void PET_brg_sim_main_page::add_page_ch1(void)
{
    p_brg_ch_1  = new PET_brg_ch_page(1, this);
    //---
    QWidget *w = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(p_brg_ch_1);
    layout->addStretch(1);
    w->setLayout(layout);
    //---
    QScrollArea *scroll = new QScrollArea;
    scroll->setWidgetResizable( true );
    scroll->setWidget(w);
    scroll->setObjectName("p_brg_ctrl_state_ch1");
    //---
    ui->tabWidget->addTab(scroll, tr("Канал 2"));
}
//--------------------------------------------------------------------------------
void PET_brg_sim_main_page::add_page_ch2(void)
{
    p_brg_ch_2  = new PET_brg_ch_page(2, this);
    //---
    QWidget *w = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(p_brg_ch_2);
    layout->addStretch(1);
    w->setLayout(layout);
    //---
    QScrollArea *scroll = new QScrollArea;
    scroll->setWidgetResizable( true );
    scroll->setWidget(w);
    scroll->setObjectName("p_brg_ctrl_state_ch2");
    //---
    ui->tabWidget->addTab(scroll, tr("Канал 3"));
}
//--------------------------------------------------------------------------------
void PET_brg_sim_main_page::add_page_ch3(void)
{
    p_brg_ch_3  = new PET_brg_ch_page(3, this);
    //---
    QWidget *w = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(p_brg_ch_3);
    layout->addStretch(1);
    w->setLayout(layout);
    //---
    QScrollArea *scroll = new QScrollArea;
    scroll->setWidgetResizable( true );
    scroll->setWidget(w);
    scroll->setObjectName("p_brg_ctrl_state_ch3");
    //---
    ui->tabWidget->addTab(scroll, tr("Канал 4"));
}
//--------------------------------------------------------------------------------
void PET_brg_sim_main_page::init(void)
{
    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(0);

    channel = 0;

    ui->tabWidget->setCurrentIndex(0);
    //---
    add_page_device_info();
    add_page_ch0();
    add_page_ch1();
    add_page_ch2();
    add_page_ch3();
    add_page_debug();
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
        connect(ui->btn_refresh,    SIGNAL(clicked(bool)),  this,   SLOT(btn_refresh_click()));
        connect(ui->btn_apply,      SIGNAL(clicked(bool)),  this,   SLOT(btn_apply_click()));
        connect(ui->btn_close,      SIGNAL(clicked(bool)),  this,   SLOT(btn_close_click()));
    }
    //---
    connect(ui->btn_import, SIGNAL(clicked(bool)),  this,   SLOT(btn_import_click()));
    connect(ui->btn_export, SIGNAL(clicked(bool)),  this,   SLOT(btn_export_click()));
    //---
}
//--------------------------------------------------------------------------------
void PET_brg_sim_main_page::btn_refresh_click(void)
{
    main_refresh();
}
//--------------------------------------------------------------------------------
void PET_brg_sim_main_page::btn_apply_click(void)
{
    main_apply();
}
//--------------------------------------------------------------------------------
void PET_brg_sim_main_page::btn_close_click(void)
{
    main_close();
}
//--------------------------------------------------------------------------------
void PET_brg_sim_main_page::btn_import_click(void)
{
    f_import();
}
//--------------------------------------------------------------------------------
void PET_brg_sim_main_page::btn_export_click(void)
{
    f_export();
}
//--------------------------------------------------------------------------------
void PET_brg_sim_main_page::all_refresh(void)
{
    f_PET_CMD_GET_DEVICE_INFO();
    channel = 0; f_PET_CMD_GET_CH_CONTROL();    f_PET_CMD_GET_CH_STATE();
    channel = 1; f_PET_CMD_GET_CH_CONTROL();    f_PET_CMD_GET_CH_STATE();
    channel = 2; f_PET_CMD_GET_CH_CONTROL();    f_PET_CMD_GET_CH_STATE();
    channel = 3; f_PET_CMD_GET_CH_CONTROL();    f_PET_CMD_GET_CH_STATE();
}
//--------------------------------------------------------------------------------
void PET_brg_sim_main_page::update_widgets(void)
{
    main_refresh();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void PET_brg_sim_main_page::main_refresh(void)
{
    block_this_button(true);
    f_PET_CMD_GET_DEVICE_INFO();
    f_PET_CMD_GET_CONTROL();
    channel = 0;
    f_PET_CMD_GET_CH_CONTROL();
    f_PET_CMD_GET_CH_STATE();
    channel = 1;
    f_PET_CMD_GET_CH_CONTROL();
    f_PET_CMD_GET_CH_STATE();
    channel = 2;
    f_PET_CMD_GET_CH_CONTROL();
    f_PET_CMD_GET_CH_STATE();
    channel = 3;
    f_PET_CMD_GET_CH_CONTROL();
    f_PET_CMD_GET_CH_STATE();
    block_this_button(false);
}
//--------------------------------------------------------------------------------
void PET_brg_sim_main_page::main_apply(void)
{
    block_this_button(true);
    channel = 0;
    f_PET_CMD_SET_CH_CONTROL();
    channel = 1;
    f_PET_CMD_SET_CH_CONTROL();
    channel = 2;
    f_PET_CMD_SET_CH_CONTROL();
    channel = 3;
    f_PET_CMD_SET_CH_CONTROL();
    block_this_button(false);
}
//--------------------------------------------------------------------------------
void PET_brg_sim_main_page::main_close(void)
{
    emit error("Close");
    parentWidget()->close();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void PET_brg_sim_main_page::get_packet(QByteArray packet)
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
    case PET_CMD_GET_CH_CONTROL:
    case PET_CMD_GET_CH_STATE:
    case PET_CMD_SET_CONTROL:
    case PET_CMD_SET_CH_CONTROL:
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
PET_RESULT PET_brg_sim_main_page::f_PET_CMD_GET_DEVICE_INFO(void)
{
    emit info("Запрос информации об устройстве");
    p_brg_device_info->block_ctrl_interface(true);
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
    p_brg_device_info->set(d_info);
    p_brg_device_info->block_ctrl_interface(false);
    emit info("Выполнено");
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_brg_sim_main_page::f_PET_CMD_GET_CONTROL(void)
{
    emit info("Запрос параметров режима работы устройства");
    p_brg_debug->block_ctrl_interface(true);
    //---
    pet_brg_ctrl_t ctrl;
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
    p_brg_debug->set(ctrl);
    p_brg_debug->block_ctrl_interface(false);
    emit info("Выполнено");
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_brg_sim_main_page::f_PET_CMD_GET_CH_CONTROL(void)
{
    emit info(QString("Запрос параметров режима работы канала обработки (%1)").arg(channel));
    //---
    switch (channel)
    {
    case 0: p_brg_ch_0->block_ctrl_interface(true); break;
    case 1: p_brg_ch_1->block_ctrl_interface(true); break;
    case 2: p_brg_ch_2->block_ctrl_interface(true); break;
    case 3: p_brg_ch_3->block_ctrl_interface(true); break;
    }
    //---
    uint16_t i_data = channel;
    int i_data_len = sizeof(i_data);
    //---
    pet_brg_ch_ctrl_t o_data;
    int o_data_len = sizeof(o_data);
    //---
    PET_RESULT err = PET_ERR_FAULT;
    do
    {
        err = get_ch_ctrl(&i_data, &i_data_len,
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
    switch (channel)
    {
    case 0:
        p_brg_ch_0->set_ctrl(o_data);
        p_brg_ch_0->block_ctrl_interface(false);
        break;
    case 1:
        p_brg_ch_1->set_ctrl(o_data);
        p_brg_ch_1->block_ctrl_interface(false);
        break;
    case 2:
        p_brg_ch_2->set_ctrl(o_data);
        p_brg_ch_2->block_ctrl_interface(false);
        break;
    case 3:
        p_brg_ch_3->set_ctrl(o_data);
        p_brg_ch_3->block_ctrl_interface(false);
        break;
    default:    break;
    }

    emit info("Выполнено");
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_brg_sim_main_page::f_PET_CMD_GET_CH_STATE(void)
{
    emit info(QString("Запрос о состоянии канала обработки (%1)").arg(channel));
    //---
    switch (channel)
    {
    case 0: p_brg_ch_0->block_state_interface(true); break;
    case 1: p_brg_ch_1->block_state_interface(true); break;
    case 2: p_brg_ch_2->block_state_interface(true); break;
    case 3: p_brg_ch_3->block_state_interface(true); break;
    }
    //----
    uint16_t i_data = channel;
    int i_data_len = sizeof(i_data);
    //----
    pet_brg_ch_state_t o_data;
    int o_data_len = sizeof(o_data);
    //---
    PET_RESULT err = PET_ERR_FAULT;
    do
    {
        err = get_ch_state(&i_data, &i_data_len,
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
    switch (channel)
    {
    case 0:
        p_brg_ch_0->set_state(o_data);
        p_brg_ch_0->block_state_interface(false);
        break;
    case 1:
        p_brg_ch_1->set_state(o_data);
        p_brg_ch_1->block_state_interface(false);
        break;
    case 2:
        p_brg_ch_2->set_state(o_data);
        p_brg_ch_2->block_state_interface(false);
        break;
    case 3:
        p_brg_ch_3->set_state(o_data);
        p_brg_ch_3->block_state_interface(false);
        break;
    default:    break;
    }

    emit info("Выполнено");
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_brg_sim_main_page::f_PET_CMD_SET_CONTROL(void)
{
    emit info("Установка параметров режима работы устройства");
    //---
    uint16_t i_data = channel;
    int i_data_len = sizeof(i_data);
    //---
    pet_brg_ctrl_t o_data;
    int o_data_len = sizeof(o_data);
    //---
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
    p_brg_debug->set(o_data);
    emit info("Выполнено");
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_brg_sim_main_page::f_PET_CMD_SET_CH_CONTROL(void)
{
    emit info(QString("Установка параметров режима работы канала обработки (%1)").arg(channel));
    //---
    pet_brg_ch_ctrl_t i_data;
    switch (channel)
    {
    case 0: p_brg_ch_0->get_ctrl(&i_data);   break;
    case 1: p_brg_ch_1->get_ctrl(&i_data);   break;
    case 2: p_brg_ch_2->get_ctrl(&i_data);   break;
    case 3: p_brg_ch_3->get_ctrl(&i_data);   break;
    default:    break;
    }
    int i_data_len = sizeof(i_data);
    //---
    pet_brg_ch_ctrl_t o_data;
    int o_data_len = sizeof(o_data);
    //---
    PET_RESULT err = PET_ERR_FAULT;
    do
    {
        err = _set_ch_ctrl(&i_data, &i_data_len,
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
    switch (channel)
    {
    case 0: p_brg_ch_0->set_ctrl(o_data);   break;
    case 1: p_brg_ch_1->set_ctrl(o_data);   break;
    case 2: p_brg_ch_2->set_ctrl(o_data);   break;
    case 3: p_brg_ch_3->set_ctrl(o_data);   break;
    default:    break;
    }
    //---
    emit info("Выполнено");
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
void PET_brg_sim_main_page::block_this_button(bool state)
{
    QObject *btn = (QObject *)sender();
    if(!btn)
    {
        return;
    }

    if (QAbstractButton *pb=qobject_cast<QAbstractButton *>(btn))
    {
        pb->setDisabled(state);
    }
}
//--------------------------------------------------------------------------------
bool PET_brg_sim_main_page::check_packet(uint8_t cmd, QByteArray packet)
{
    //emit debug(QString("check_size_packet: [%1]").arg(packet.toHex().data()));
    if(packet.length() < (int)sizeof(pet_frame_t))
    {
        emit error("PET_brg_sim_main_page::check_size_packet: packet too small");
        return false;
    }

    pet_frame_t *frame = (pet_frame_t *)packet.data();
    Q_CHECK_PTR(frame);
    int len_packet = frame->len + sizeof(pet_frame_t);
    if(packet.length() != len_packet)
    {
        emit error("PET_brg_sim_main_page::check_size_packet: bad len");
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
void PET_brg_sim_main_page::f_import(void)
{
    PET_device_brg_sim::f_import();
}
void PET_brg_sim_main_page::f_export(void)
{
    PET_device_brg_sim::f_export();
}
//--------------------------------------------------------------------------------
