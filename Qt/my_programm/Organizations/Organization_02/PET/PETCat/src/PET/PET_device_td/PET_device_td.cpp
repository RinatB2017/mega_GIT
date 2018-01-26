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
#include <QSettings>
#include <QComboBox>
#include <QSpinBox>
#include <QWidget>
#include <QIcon>
//--------------------------------------------------------------------------------
#include "PET_device_td.hpp"
//--------------------------------------------------------------------------------
#include "myfiledialog.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
PET_device_td::PET_device_td(QWidget *parent) :
    PET_device_base(parent)
{

}
//--------------------------------------------------------------------------------
PET_device_td::~PET_device_td()
{

}
//--------------------------------------------------------------------------------
int PET_device_td::get_PID(void)
{
    return PET_PID_BOS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_td::get_test(void *i_data, int *i_data_len,
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
PET_RESULT PET_device_td::get_device_info(pet_device_info_t *data,
                                          int *data_len)
{
    Q_CHECK_PTR(data);
    Q_CHECK_PTR(data_len);
    return _get_device_info(data, data_len);
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_td::get_ctrl(pet_bos_ctrl_t *data,
                                   int *data_len)
{
    Q_CHECK_PTR(data);
    Q_CHECK_PTR(data_len);
    return _get_ctrl(data, data_len);
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_td::get_state(pet_bos_state_t *data,
                                    int *data_len)
{
    Q_CHECK_PTR(data);
    Q_CHECK_PTR(data_len);
    return _get_state(data, data_len);
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_td::get_ch_ctrl(uint16_t *i_data, int *i_data_len,
                                      pet_bos_ch_ctrl_t *o_data, int *o_data_len)
{
    Q_CHECK_PTR(i_data);
    Q_CHECK_PTR(i_data_len);
    Q_CHECK_PTR(o_data);
    Q_CHECK_PTR(o_data_len);
    return _get_ch_ctrl(i_data, i_data_len,
                        o_data, o_data_len);
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_td::get_ch_state(uint16_t *i_data, int *i_data_len,
                                       pet_bos_ch_state_t *o_data, int *o_data_len)
{
    Q_CHECK_PTR(i_data);
    Q_CHECK_PTR(i_data_len);
    Q_CHECK_PTR(o_data);
    Q_CHECK_PTR(o_data_len);
    return _get_ch_state(i_data, i_data_len,
                         o_data, o_data_len);
}
//--------------------------------------------------------------------------------
void PET_device_td::f_import(void)
{
    QString filename;
    filename.clear();

    MyFileDialog *dlg = new MyFileDialog("filedialog_states", "td_dlg");
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
    }

    load_QSpinBox(filename, "setting_name");
    load_QDoubleSpinBox(filename, "setting_name");
    load_QComboBox(filename, "setting_name");

    emit info("Выполнено");
}
//--------------------------------------------------------------------------------
void PET_device_td::f_export(void)
{
    QString filename;
    filename.clear();

    MyFileDialog *dlg = new MyFileDialog("filedialog_states", "td_dlg");
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
void PET_device_td::save_to_EEPROM(void)
{
    emit info("Сохранение текущей конфигурации устройства в энергонезависимой памяти");

    QString title   = "PETCAT";
    QString text    = "Сохранить текущей конфигурации устройства в энергонезависимую память?";
    int btn = messagebox_question(title, text);

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
        return;
    }
    emit info("Выполнено");
}
//--------------------------------------------------------------------------------
void PET_device_td::set_default(void)
{
    emit info("Установка конфигурации устройства в значения по умолчанию");

    QString title   = "PETCAT";
    QString text    = "Установить конфигурации устройства в значения по умолчанию?";
    int btn = messagebox_question(title, text);

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
        return;
    }
    emit info("Выполнено");
}
//--------------------------------------------------------------------------------
