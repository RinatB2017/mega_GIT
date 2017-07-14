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
#include <QMessageBox>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QScrollArea>
#include <QUdpSocket>
#include <QTime>
//--------------------------------------------------------------------------------
#include "sleeper.h"
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
#include "pet_brg.h"
#include "pet_errno.h"
//--------------------------------------------------------------------------------
#include "calibration_channel.hpp"
#include "calibration_param.hpp"
//--------------------------------------------------------------------------------
#include "imitator_brg.hpp"
//--------------------------------------------------------------------------------
Calibration::Calibration(QList<pet_discovery_data> pet_config, QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Calibration),
    p_pet_config(pet_config)
{
    init();

    //load_QSpinBox("Calibration");
    //load_QDoubleSpinBox("Calibration");
}
//--------------------------------------------------------------------------------
Calibration::~Calibration()
{
    //save_QSpinBox("Calibration");
    //save_QDoubleSpinBox("Calibration");

    delete ui;
}
//--------------------------------------------------------------------------------
void Calibration::init(void)
{
    ui->setupUi(this);

    //---
    comm = new PET_comm();
    connect(comm,   SIGNAL(info(QString)),      this,   SIGNAL(info(QString)));
    connect(comm,   SIGNAL(debug(QString)),     this,   SIGNAL(debug(QString)));
    connect(comm,   SIGNAL(error(QString)),     this,   SIGNAL(error(QString)));
    connect(comm,   SIGNAL(message(QString)),   this,   SIGNAL(message(QString)));

    QHostAddress addr = p_pet_config.at(0).host;
    comm->set_comm_param(addr, PETCat_options::get_port_cmd());
    bool ok = comm->bind(QHostAddress::AnyIPv4);
    if(!ok)
    {
        emit error(comm->get_last_error());
    }
    //---
    imitator_brg = new Imitator_BRG(this);
    connect(imitator_brg,   SIGNAL(info(QString)),      this,   SIGNAL(info(QString)));
    connect(imitator_brg,   SIGNAL(debug(QString)),     this,   SIGNAL(debug(QString)));
    connect(imitator_brg,   SIGNAL(error(QString)),     this,   SIGNAL(error(QString)));
    connect(imitator_brg,   SIGNAL(message(QString)),   this,   SIGNAL(message(QString)));
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
    channel_1 = new Calibration_channel(0, this);
    channel_2 = new Calibration_channel(1, this);
    channel_3 = new Calibration_channel(2, this);
    channel_4 = new Calibration_channel(3, this);

    connect(channel_1,  SIGNAL(run_calib_coord(int)),   this,   SLOT(run_calib_coords(int)));
    connect(channel_2,  SIGNAL(run_calib_coord(int)),   this,   SLOT(run_calib_coords(int)));
    connect(channel_3,  SIGNAL(run_calib_coord(int)),   this,   SLOT(run_calib_coords(int)));
    connect(channel_4,  SIGNAL(run_calib_coord(int)),   this,   SLOT(run_calib_coords(int)));

    c_param = new Calibration_param(this);

    ui->sa_1->setWidget(channel_1);
    ui->sa_2->setWidget(channel_2);
    ui->sa_3->setWidget(channel_3);
    ui->sa_4->setWidget(channel_4);

    ui->sa_param->setWidget(c_param);
    //---
    connect(ui->btn_select,     SIGNAL(clicked(bool)),  this,   SLOT(select()));
    connect(ui->btn_apply,      SIGNAL(clicked(bool)),  this,   SLOT(apply()));
    connect(ui->btn_refresh,    SIGNAL(clicked(bool)),  this,   SLOT(refresh()));
    //---
#if 0
    ui->graph_widget->setVisible(false);
#else
    ui->graph_widget->set_title("тестовый график");
    ui->graph_widget->set_visible_btn_all_OFF(false);
    ui->graph_widget->set_visible_btn_all_ON(false);
    ui->graph_widget->set_visible_btn_Load(false);
    ui->graph_widget->set_visible_btn_Save(false);
    ui->graph_widget->set_visible_btn_Statistic(false);
    ui->graph_widget->set_visible_btn_Options(false);

    ui->graph_widget->set_silense(true);

    curve_data = ui->graph_widget->add_curve("data");
    curve_ideal = ui->graph_widget->add_curve("ideal");
#endif
    //---
}
//--------------------------------------------------------------------------------
void Calibration::run_calib_coords(int num_channel)
{
    QString title = "Калибровка";
    QString text = "Все подключено правильно?";
    int btn = messagebox_question(title, text);
    if(btn != QMessageBox::Yes)
    {
        text = "Калибровка отменена";
        emit error(text);
        //messagebox_critical(title, text);
        return;
    }

    bool ok = false;
    int channel = num_channel;
    int max_point = c_param->get_N();
    int max_cnt_read = c_param->get_max_cnt_read();
    int max_cnt_calib = c_param->get_max_cnt_calib();
    int timeout = c_param->get_timeout();
    double min_A = c_param->get_A_min();
    double max_A = c_param->get_A_max();
    double max_delta_B = c_param->get_B();
    double max_delta_K = c_param->get_K();
    double freq = c_param->get_F();

    emit info(QString("Производится калибровка канала %1").arg(channel + 1));
    emit info(QString("Количество точек калибровки %1").arg(max_point));
    emit info(QString("Кол-во циклов чтения %1").arg(max_cnt_read));
    emit info(QString("Кол-во попыток калибровки %1").arg(max_cnt_calib));
    emit info(QString("Таймаут %1 msec").arg(timeout));
    emit info(QString("A мин. %1").arg(min_A));
    emit info(QString("A макс. %1").arg(max_A));
    emit info(QString("Точность B %1%").arg(max_delta_B));
    emit info(QString("Точность K %1%").arg(max_delta_K));
    emit info(QString("Частота %1 кГц").arg(freq));

    emit info("Инициализируется имитатор БРГ");
    if(imitator_brg)
    {
        ok = imitator_brg->dev_open();
        if(ok == false)
        {
            return;
        }
    }
    //---
    double step = (max_A - min_A) / max_point;
    double voltage = min_A;
    while(voltage < max_A)
    {
        emit info(QString("Устанавливаем напряжение %1 мВ").arg(voltage));
        ok = set_voltage(channel, voltage);
        if(ok == false)
        {
            emit error("Не удалось установить напряжение");
            imitator_brg->dev_close();
            return;
        }

        if(timeout > 0)
        {
            Sleeper::msleep(timeout);
        }

        double i_voltage = 0;
        ok = get_voltage(channel, &i_voltage);
        if(ok == false)
        {
            emit error("Не удалось получить напряжение");
            imitator_brg->dev_close();
            return;
        }

        ui->graph_widget->add_curve_data(curve_data,    i_voltage);
        ui->graph_widget->add_curve_data(curve_ideal,   voltage);

        voltage += step;
    }

    //---
    imitator_brg->dev_close();
    //---
    emit error("Калибровка завершена с ошибкой");
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
    f->deleteLater();
}
//--------------------------------------------------------------------------------
void Calibration::refresh(void)
{
    uint16_t pet_addr = p_pet_config.at(0).addr;
    int pet_addr_len = sizeof(pet_addr);

    pet_brg_calibration_t data;
    int data_len = sizeof(data);

    pet_brg->set_pet_addr(pet_addr);

    QList<Calibration_channel *> l_channels;
    l_channels.clear();
    l_channels.append(channel_1);
    l_channels.append(channel_2);
    l_channels.append(channel_3);
    l_channels.append(channel_4);

    PET_RESULT res = PET_SUCCESS;
    emit info(QString("Чтение калибровочной информации устройства ПЭТ (адрес %1)").arg(pet_addr));
    res = pet_brg->get_calibration(&pet_addr,   &pet_addr_len,
                                   &data,       &data_len);
    if(res != PET_SUCCESS)
    {
        emit error(QString("%1").arg(PET_device_base::get_PET_RESULT_string(res)));
        return;
    }

    for(int ch=0; ch<4; ch++)
    {
        l_channels[ch]->set(data.ch_calibration[0]);
    }
}
//--------------------------------------------------------------------------------
void Calibration::apply(void)
{
    uint16_t pet_addr = p_pet_config.at(0).addr;

    pet_brg_calibration_t data;
    int data_len = sizeof(data);

    pet_brg->set_pet_addr(pet_addr);

    QList<Calibration_channel *> l_channels;
    l_channels.clear();
    l_channels.append(channel_1);
    l_channels.append(channel_2);
    l_channels.append(channel_3);
    l_channels.append(channel_4);

    for(int ch=0; ch<4; ch++)
    {
        pet_brg_ch_calibration temp;
        l_channels[ch]->get(&temp);
        memcpy((void *)&data.ch_calibration[ch],
               (void *)&temp,
               sizeof(data.ch_calibration[ch]));
    }

    PET_RESULT res = PET_SUCCESS;
    emit info(QString("Запись калибровочной информации устройства ПЭТ (адрес %1)").arg(pet_addr));
    res = pet_brg->set_calibration(&data,   &data_len,
                                   &data,   &data_len);
    if(res != PET_SUCCESS)
    {
        emit error(QString("%1").arg(PET_device_base::get_PET_RESULT_string(res)));
    }
}
//--------------------------------------------------------------------------------
bool Calibration::set_voltage(uint8_t channel, double value)
{
    if(channel > 3)
    {
        emit error(QString("Некорректный номер канала %1").arg(channel));
        return false;
    }
    if(imitator_brg->is_open() == false)
    {
        emit error("Имитатор не открыт!");
        return false;
    }

    //---
    const float _Amplitude1mV = 7.142857142857143f;
    //const float _Offset1mV = 28.0f;
    //const float _Phase1deg = 11.0f;

    const uint64_t Dev_FCLK = 125000000; // тактовая частота DDS в Гц

    uint64_t DDS_TW = (uint64_t)((c_param->get_F() * 1000.0) / (float)Dev_FCLK / (float)0x1000000);

    uint16_t reg_value = (uint16_t)((value * 1000.0f) * _Amplitude1mV);

    uint16_t DAC_DGAIN_XP = (channel == 0) ? reg_value : 0;
    uint16_t DAC_DGAIN_XN = (channel == 1) ? reg_value : 0;
    uint16_t DAC_DGAIN_YP = (channel == 2) ? reg_value : 0;
    uint16_t DAC_DGAIN_YN = (channel == 3) ? reg_value : 0;

    uint16_t DAC_DOF_XP = 0;
    uint16_t DAC_DOF_XN = 0;
    uint16_t DAC_DOF_YP = 0;
    uint16_t DAC_DOF_YN = 0;

    uint16_t DDS_PW_XP = 0;
    uint16_t DDS_PW_XN = 0;
    uint16_t DDS_PW_YP = 0;
    uint16_t DDS_PW_YN = 0;

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

    imitator_brg->set_param(param);
    imitator_brg->start();
    //---

    return true;
}
//--------------------------------------------------------------------------------
bool Calibration::get_voltage(uint8_t channel, double *value)
{
    if(channel > 3)
    {
        emit error(QString("Некорректный номер канала %1").arg(channel));
        return false;
    }

    //---
    pet_wave_t i_wave;
    int i_wave_len = sizeof(i_wave);
    //---
    pet_wave_data_t o_data;
    int o_wave_len;
    //---

    i_wave.channel = channel;
    i_wave.mode = (PET_WAVE_MODE_SYNC_NONE | PET_WAVE_MODE_SYNC_NONE | PET_WAVE_MODE_EN_MSK);
    i_wave.count = c_param->get_N();

    pet_brg->set_pet_addr(p_pet_config.at(0).addr);
    PET_RESULT  res = pet_brg->get_oscillogram(&i_wave,  &i_wave_len,
                                               &o_data,  &o_wave_len);
    if(res != PET_SUCCESS)
    {
        emit error(PET_device_base::get_PET_RESULT_string(res));
        return false;
    }
    //---
    double raw_value = 0;
    pet_wave_t *pet_wave = (pet_wave_t *)o_data.buffer;
    if(pet_wave->count == 0)
    {
        return false;
    }

    for(unsigned int n=0; n<pet_wave->count; n++)
    {
        if(channel == 0) raw_value += qPow(pet_wave->point[n].xp, 2);
        if(channel == 1) raw_value += qPow(pet_wave->point[n].xn, 2);
        if(channel == 2) raw_value += qPow(pet_wave->point[n].yp, 2);
        if(channel == 3) raw_value += qPow(pet_wave->point[n].yn, 2);
    }
    raw_value /= (qreal)pet_wave->count;
    double value_RMS = qSqrt(raw_value);
    *value = value_RMS;
    //---

    return true;
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
