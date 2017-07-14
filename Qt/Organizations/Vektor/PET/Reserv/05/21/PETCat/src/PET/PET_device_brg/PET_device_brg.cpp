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
#include <QComboBox>
#include <QSpinBox>
#include <QSettings>
#include <QWidget>
#include <QDebug>
#include <QIcon>
//--------------------------------------------------------------------------------
#include "PET_device_brg.hpp"
//--------------------------------------------------------------------------------
#include "myfiledialog.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
PET_device_brg::PET_device_brg(QWidget *parent) :
    PET_device_base(parent)
{

}
//--------------------------------------------------------------------------------
PET_device_brg::~PET_device_brg()
{

}
//--------------------------------------------------------------------------------
int PET_device_brg::get_PID(void)
{
    return PET_PID_BRG;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_brg::get_test(void *i_data, int *i_data_len,
                                    void *o_data, int *o_data_len)
{
    Q_CHECK_PTR(i_data);
    Q_CHECK_PTR(i_data_len);
    Q_CHECK_PTR(o_data);
    Q_CHECK_PTR(o_data_len);
    return _get_test(i_data, i_data_len,
                     o_data, o_data_len);
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_brg::get_device_info(pet_device_info_t *data,
                                           int *data_len)
{
    Q_CHECK_PTR(data);
    Q_CHECK_PTR(data_len);
    return _get_device_info(data, data_len);
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_brg::get_ctrl(pet_brg_ctrl_t *data, int *data_len)
{
    Q_CHECK_PTR(data);
    Q_CHECK_PTR(data_len);
    return _get_ctrl(data, data_len);
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_brg::get_state(pet_brg_state_t *data, int *data_len)
{
    Q_CHECK_PTR(data);
    Q_CHECK_PTR(data_len);
    return _get_state(data, data_len);
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_brg::get_ch_ctrl(uint16_t *i_data, int *i_data_len,
                                       pet_brg_ch_ctrl_t *o_data, int *o_data_len)
{
    Q_CHECK_PTR(i_data);
    Q_CHECK_PTR(i_data_len);
    Q_CHECK_PTR(o_data);
    Q_CHECK_PTR(o_data_len);
    return _get_ch_ctrl(i_data, i_data_len,
                        o_data, o_data_len);
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_brg::get_ch_state(uint16_t *i_data, int *i_data_len,
                                        pet_brg_ch_state_t *o_data, int *o_data_len)
{
    Q_CHECK_PTR(i_data);
    Q_CHECK_PTR(i_data_len);
    Q_CHECK_PTR(o_data);
    Q_CHECK_PTR(o_data_len);
    return _get_ch_state(i_data, i_data_len,
                         o_data, o_data_len);
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_brg::get_calibration(uint16_t *i_data,                int *i_data_len,
                                           pet_brg_calibration_t *o_data,   int *o_data_len)
{
    Q_CHECK_PTR(i_data);
    Q_CHECK_PTR(i_data_len);
    Q_CHECK_PTR(o_data);
    Q_CHECK_PTR(o_data_len);
    return _get_calibration(i_data, i_data_len,
                            o_data, o_data_len);
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_brg::set_calibration(pet_brg_calibration_t *i_data, int *i_data_len,
                                           pet_brg_calibration_t *o_data, int *o_data_len)
{
    Q_CHECK_PTR(i_data);
    Q_CHECK_PTR(i_data_len);
    Q_CHECK_PTR(o_data);
    Q_CHECK_PTR(o_data_len);
    return _set_calibration(i_data, i_data_len,
                            o_data, o_data_len);
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_brg::get_oscillogram(pet_wave_t *i_data,      int *i_data_len,
                                           pet_wave_data_t *o_data, int *o_data_len)
{
    Q_CHECK_PTR(i_data);
    Q_CHECK_PTR(i_data_len);
    Q_CHECK_PTR(o_data);
    Q_CHECK_PTR(o_data_len);
    return _get_wave(i_data, i_data_len,
                     o_data, o_data_len);
}
//--------------------------------------------------------------------------------
void PET_device_brg::f_import(void)
{
    QString filename;
    filename.clear();

    MyFileDialog *dlg = new MyFileDialog("filedialog_states", "brg_dlg");
    dlg->setNameFilter("CFG (*.cfg)");
    dlg->setDefaultSuffix(tr("cfg"));
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        filename = files.at(0);
    }
    delete dlg;

    if(filename.isEmpty())
    {
        emit error("Файл не выбран");
        return;
    }

    if(check_id_pet(filename, get_PID()) == false)
    {
        emit error("Файл не распознан");
        return;
    }

    load_QSpinBox(filename, "setting_name");
    load_QDoubleSpinBox(filename, "setting_name");
    load_QComboBox(filename, "setting_name");

    emit info("Выполнено");
}
//--------------------------------------------------------------------------------
void PET_device_brg::f_export(void)
{
    QString filename;
    filename.clear();

    MyFileDialog *dlg = new MyFileDialog("filedialog_states", "brg_dlg");
    dlg->setAcceptMode(MyFileDialog::AcceptSave);
    dlg->setNameFilter("CFG (*.cfg)");
    dlg->setDefaultSuffix(tr("cfg"));
    dlg->selectFile("без имени");
    dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        filename = files.at(0);
    }
    delete dlg;

    if(filename.isEmpty())
    {
        emit error("Файл не выбран");
        return;
    }

    save_id_pet(filename, get_PID());
    save_QSpinBox(filename, "setting_name");
    save_QDoubleSpinBox(filename, "setting_name");
    save_QComboBox(filename, "setting_name");

    emit info("Выполнено");
}
//--------------------------------------------------------------------------------
void PET_device_brg::save_to_EEPROM(void)
{
    emit info("Сохранение текущей конфигурации устройства в энергонезависимой памяти");

    QString title   = "PETCAT";
    QString text    = "Сохранить текущей конфигурации устройства в энергонезависимую память?";
    int btn = MyWidget::messagebox_question(title, text);

    if(btn != MyWidget::Yes)
    {
        emit error("Отменено");
        return;
    }

    emit debug(QString("addr %1").arg(get_pet_addr()));
    PET_RESULT res = config_save();
    if(res != PET_SUCCESS)
    {
        messagebox_critical("Ошибка", "Сохранение текущей конфигурации устройства в энергонезависимую память не удалась!");

        emit error("Сохранение текущей конфигурации устройства в энергонезависимую память не удалась!");
        emit error(PET_device_base::get_PET_RESULT_string(res));
        return;
    }
    emit info("Выполнено");
}
//--------------------------------------------------------------------------------
void PET_device_brg::set_default(void)
{
    emit info("Установка конфигурации устройства в значения по умолчанию");

    QString title   = "PETCAT";
    QString text    = "Установить конфигурации устройства в значения по умолчанию?";
    int btn = MyWidget::messagebox_question(title, text);

    if(btn != MyWidget::Yes)
    {
        emit error("Отменено");
        return;
    }

    emit debug(QString("addr %1").arg(get_pet_addr()));
    PET_RESULT res = config_default();
    if(res != PET_SUCCESS)
    {
        messagebox_critical("Ошибка", "Установка конфигурации устройства в значения по умолчанию не удалась!");

        emit error("Установка конфигурации устройства в значения по умолчанию не удалась!");
        emit error(PET_device_base::get_PET_RESULT_string(res));
        return;
    }
    emit info("Выполнено");
}
//--------------------------------------------------------------------------------
