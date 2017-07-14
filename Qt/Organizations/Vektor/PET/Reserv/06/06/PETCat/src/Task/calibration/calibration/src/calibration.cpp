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
    pet_brg = new PET_device_brg(this);
    //---
    QHostAddress addr = p_pet_config.at(0).host;
    PET_RESULT err = pet_brg->set_comm_param(addr,
                                             PETCat_options::get_port_cmd());
    if(err != PET_SUCCESS)
    {
        emit error(PET_device_base::get_PET_RESULT_string(err));
    }

    bool ok = pet_brg->bind(QHostAddress::AnyIPv4);
    if(!ok)
    {
        emit error(pet_brg->get_last_error());
    }
    //---
    imitator_brg = new Imitator_BRG(this);
    connect(imitator_brg,   SIGNAL(info(QString)),      this,   SIGNAL(info(QString)));
    connect(imitator_brg,   SIGNAL(debug(QString)),     this,   SIGNAL(debug(QString)));
    connect(imitator_brg,   SIGNAL(error(QString)),     this,   SIGNAL(error(QString)));
    connect(imitator_brg,   SIGNAL(message(QString)),   this,   SIGNAL(message(QString)));
    //---
    ui->le_device->setReadOnly(true);
    ui->le_device->setText(QString("%1:%2 (Адрес %3)")
                           .arg(p_pet_config.at(0).host.toString())
                           .arg(p_pet_config.at(0).port)
                           .arg(p_pet_config.at(0).addr));
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
    connect(ui->btn_select,     SIGNAL(clicked(bool)),  this,   SLOT(btn_select_click()));
    connect(ui->btn_apply,      SIGNAL(clicked(bool)),  this,   SLOT(btn_apply_click()));
    connect(ui->btn_refresh,    SIGNAL(clicked(bool)),  this,   SLOT(btn_refresh_click()));
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

#ifndef QT_DEBUG
    ui->graph_widget->setVisible(false);
#else
    ui->graph_widget->set_silense(true);
#endif

    curve_data = ui->graph_widget->add_curve("data");
    curve_ideal = ui->graph_widget->add_curve("ideal");
#endif
    //---
    refresh();
}
//--------------------------------------------------------------------------------
void Calibration::btn_select_click(void)
{
    select();
}
//--------------------------------------------------------------------------------
void Calibration::btn_apply_click(void)
{
    apply();
}
//--------------------------------------------------------------------------------
void Calibration::btn_refresh_click(void)
{
    refresh();
}
//--------------------------------------------------------------------------------
bool Calibration::get_voltage(double *value)
{
    bool ok = false;
    double i_voltage = 0;
    double temp_voltage = 0;
    for(int n=0; n<max_cnt_read; n++)
    {
        ok = get_raw_voltage(&temp_voltage);
        if(ok == false)
        {
            return false;
        }
        i_voltage += temp_voltage;
        if(timeout > 0)
        {
            Sleeper::msleep(timeout);
        }
    }
    i_voltage /= (double)max_cnt_read;
    *value = i_voltage;
    return true;
}
//--------------------------------------------------------------------------------
bool Calibration::brg_correct(double value_K,
                              double value_B)
{
    pet_brg_ch_calibration data_1;
    pet_brg_ch_calibration data_2;
    pet_brg_ch_calibration data_3;
    pet_brg_ch_calibration data_4;

    pet_brg_calibration all_data;
    int i_all_data = sizeof(all_data);

    pet_brg_calibration o_data;
    int o_all_data = sizeof(o_data);

    channel_1->get(&data_1);
    channel_2->get(&data_2);
    channel_3->get(&data_3);
    channel_4->get(&data_4);

    if(channel == 0)
    {
        data_1.xp_gain += value_K;
        data_1.xp_offset += value_B;
    }
    if(channel == 1)
    {
        data_1.xn_gain += value_K;
        data_1.xn_offset += value_B;
    }
    if(channel == 2)
    {
        data_1.yp_gain += value_K;
        data_1.yp_offset += value_B;
    }
    if(channel == 3)
    {
        data_1.yn_gain += value_K;
        data_1.yn_offset += value_B;
    }

    memcpy((char *)&all_data.ch_calibration[0], (char *)&data_1, sizeof(data_1));
    memcpy((char *)&all_data.ch_calibration[1], (char *)&data_2, sizeof(data_2));
    memcpy((char *)&all_data.ch_calibration[2], (char *)&data_3, sizeof(data_3));
    memcpy((char *)&all_data.ch_calibration[3], (char *)&data_4, sizeof(data_4));

    channel_1->set(data_1);
    channel_2->set(data_2);
    channel_3->set(data_3);
    channel_4->set(data_4);

    return pet_brg->set_calibration(&all_data,  &i_all_data,
                                    &o_data,    &o_all_data);
}
//--------------------------------------------------------------------------------
bool Calibration::get_k_b(double *value_K,
                          double *value_B)
{
    bool ok = false;
    int i = 0;
    double step = (max_A - min_A) / max_point;
    double voltage = min_A;

    double **f;
    f = new double*[2];
    f[0] = new double[max_point];
    f[1] = new double[max_point];

    while(voltage < max_A)
    {
        emit info(QString("Устанавливаем напряжение %1 мВ").arg(voltage * 1000));
        ok = set_voltage(voltage);
        if(ok == false)
        {
            emit error("Не удалось установить напряжение");
            return false;
        }

        if(timeout > 0)
        {
            Sleeper::msleep(timeout);
        }

        double i_voltage = 0;
        ok = get_voltage(&i_voltage);
        if(ok == false)
        {
            emit error("Не удалось получить напряжение");
            return false;
        }
        emit info(QString("Измеренное напряжение %1 мВ").arg(i_voltage));

        f[0][i] = (double) i;
        f[1][i] = i_voltage;

        ui->graph_widget->add_curve_data(curve_data,    i_voltage);
        ui->graph_widget->add_curve_data(curve_ideal,   voltage);

        voltage += step;
        i++;
    }

    double k = 0;
    double b = 0;
    getApprox(f, &k, &b, max_point);

    *value_K = k;
    *value_B = b;
    return true;
}
//--------------------------------------------------------------------------------
void Calibration::run_calib_coords(int num_channel)
{
    QString title = "Калибровка";
    QString text = "Все подключено правильно?";
    QString tmp_str;
    int btn = messagebox_question(title, text);
    if(btn != QMessageBox::Yes)
    {
        text = "Калибровка отменена";
        emit error(text);
        messagebox_critical(title, text);
        return;
    }

    bool ok = false;
    channel = num_channel;
    max_point = c_param->get_N();
    max_cnt_read = c_param->get_max_cnt_read();
    max_cnt_calib = c_param->get_max_cnt_calib();
    timeout = c_param->get_timeout();
    min_A = c_param->get_A_min();
    max_A = c_param->get_A_max();
    max_delta_B = c_param->get_B();
    max_delta_K = c_param->get_K();
    freq = c_param->get_F();

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
            messagebox_critical("Ошибка", "Имитатор БРГ не найден");
            return;
        }
    }
    //---
    ui->graph_widget->clear();
    //---
    double k = 0;
    double b = 0;
    for(int n=0; n<max_cnt_calib; n++)
    {
        ok = get_k_b(&k, &b);
        if(ok)
        {
            emit info(QString("k = %1").arg(k));
            emit info(QString("b = %1").arg(b));
        }
        //TODO пробую, потом исправлю
        double delta_B = b;
        double delta_K = k - 1.0;
        if(delta_B < 0) delta_B = -delta_B;
        if(delta_K < 0) delta_K = -delta_K;
        if((delta_K > max_delta_K) || (delta_B > max_delta_B))
        {
            ok = brg_correct(k, b);
            if(ok == false)
            {
                tmp_str = "Калибровка канала завершена с ошибкой";
                emit error(tmp_str);
                messagebox_critical("Ошибка", tmp_str);
                imitator_brg->dev_close();
                return;
            }
        }
        else
        {
            emit info("Калибровка канала прошла успешно");
            imitator_brg->dev_close();
            return;
        }
    }

    //---
    imitator_brg->dev_close();
}
//--------------------------------------------------------------------------------
// Вычисление коэффициентов аппроксимирующей прямой
void Calibration::getApprox(double **x,
                            double *k,
                            double *b,
                            int n)
{
    double sumx = 0;
    double sumy = 0;
    double sumx2 = 0;
    double sumxy = 0;
    for (int i=0; i<n; i++)
    {
        sumx += x[0][i];
        sumy += x[1][i];
        sumx2 += x[0][i] * x[0][i];
        sumxy += x[0][i] * x[1][i];
    }
    *k = (n*sumxy - (sumx*sumy)) / (n*sumx2 - sumx*sumx);
    *b = (sumy - *k*sumx) / n;
    return;
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
    QList<int> permitted_dev;
    permitted_dev.clear();
    permitted_dev.append(PET_PID_BOS);
    permitted_dev.append(PET_PID_BRG);
    permitted_dev.append(PET_PID_SIM);
    permitted_dev.append(PET_PID_TD);
    permitted_dev.append(PET_PID_BRG_SIM);

    FindPetBox *f = new FindPetBox(permitted_dev, this);

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
    pet_brg->set_pet_addr(pet_addr);
    res = pet_brg->set_calibration(&data,   &data_len,
                                   &data,   &data_len);
    if(res != PET_SUCCESS)
    {
        emit error(QString("%1").arg(PET_device_base::get_PET_RESULT_string(res)));
    }
}
//--------------------------------------------------------------------------------
bool Calibration::set_voltage(double value)
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

    uint64_t DDS_TW = (uint64_t)(((float)c_param->get_F() * 1000.0) / ((float)Dev_FCLK / (float)0x1000000));
    qDebug() << "freq" << c_param->get_F() * 1000;
    qDebug() << "DDS_TW" << DDS_TW;

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

    bool ok = false;
    ok = imitator_brg->write_registers();
    if(ok == false)
    {
        emit error("Ошибка записи регистров");
        return false;
    }

    ok = imitator_brg->set_param(param);
    if(ok == false)
    {
        emit error("Ошибка установки параметров");
        return false;
    }

    ok = imitator_brg->ApplySettings();
    if(ok == false)
    {
        emit error("Ошибка применения настроек");
        return false;
    }

    return true;
}
//--------------------------------------------------------------------------------
bool Calibration::get_raw_voltage(double *value)
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
    if(o_data.count == 0)
    {
        return false;
    }
    //---
    double raw_value = 0;
    pet_wave_t *pet_wave = (pet_wave_t *)o_data.buffer;

    for(unsigned int n=0; n<o_data.count; n++)
    {
        if(channel == 0) raw_value += qPow(pet_wave->point[n].xp, 2);
        if(channel == 1) raw_value += qPow(pet_wave->point[n].xn, 2);
        if(channel == 2) raw_value += qPow(pet_wave->point[n].yp, 2);
        if(channel == 3) raw_value += qPow(pet_wave->point[n].yn, 2);
    }
    raw_value /= (qreal)o_data.count;
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
