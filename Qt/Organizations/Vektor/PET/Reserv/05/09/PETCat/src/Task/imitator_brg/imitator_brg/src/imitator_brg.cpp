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
#include "imitator_brg.hpp"
//--------------------------------------------------------------------------------
#include "ad9106.hpp"
//--------------------------------------------------------------------------------
#include "qxmlputget.h"
//--------------------------------------------------------------------------------
Imitator_BRG::Imitator_BRG(QObject *parent) :
    QObject(parent)
{
    init();
}
//--------------------------------------------------------------------------------
Imitator_BRG::~Imitator_BRG()
{

}
//--------------------------------------------------------------------------------
void Imitator_BRG::init(void)
{
    init_sl_registers();
}
//--------------------------------------------------------------------------------
void Imitator_BRG::init_sl_registers(void)
{
    sl_registers.clear();
    sl_registers << "SPICONFIG";
    sl_registers << "POWERCONFIG";
    sl_registers << "CLOCKCONFIG";
    sl_registers << "REFADJ";
    sl_registers << "DAC4AGAIN";
    sl_registers << "DAC3AGAIN";
    sl_registers << "DAC2AGAIN";
    sl_registers << "DAC1AGAIN";
    sl_registers << "DACxRANGE";
    sl_registers << "DAC4RSET";
    sl_registers << "DAC3RSET";
    sl_registers << "DAC2RSET";
    sl_registers << "DAC1RSET";
    sl_registers << "CALCONFIG";
    sl_registers << "COMPOFFSET";
    sl_registers << "RAMUPDATE";
    sl_registers << "PAT_STATUS";
    sl_registers << "PAT_TYPE";
    sl_registers << "PATTERN_DLY";
    sl_registers << "DAC4DOF";
    sl_registers << "DAC3DOF";
    sl_registers << "DAC2DOF";
    sl_registers << "DAC1DOF";
    sl_registers << "WAV4_3CONFIG";
    sl_registers << "WAV2_1CONFIG";
    sl_registers << "PAT_TIMEBASE";
    sl_registers << "PAT_PERIOD";
    sl_registers << "DAC4_3PATx";
    sl_registers << "DAC2_1PATx";
    sl_registers << "DOUT_START_DLY";
    sl_registers << "DOUT_CONFIG";
    sl_registers << "DAC4_CST";
    sl_registers << "DAC3_CST";
    sl_registers << "DAC2_CST";
    sl_registers << "DAC1_CST";
    sl_registers << "DAC4_DGAIN";
    sl_registers << "DAC3_DGAIN";
    sl_registers << "DAC2_DGAIN";
    sl_registers << "DAC1_DGAIN";
    sl_registers << "SAW4_3CONFIG";
    sl_registers << "SAW2_1CONFIG";
    sl_registers << "DDS_TW32";
    sl_registers << "DDS_TW1";
    sl_registers << "DDS4_PW";
    sl_registers << "DDS3_PW";
    sl_registers << "DDS2_PW";
    sl_registers << "DDS1_PW";
    sl_registers << "TRIG_TW_SEL";
    sl_registers << "DDSx_CONFIG";
    sl_registers << "TWRAM_CONFIG";
    sl_registers << "START_DLY4";
    sl_registers << "START_ADDR4";
    sl_registers << "STOP_ADDR4";
    sl_registers << "DDS_CYC4";
    sl_registers << "START_DLY3";
    sl_registers << "START_ADDR3";
    sl_registers << "STOP_ADDR3";
    sl_registers << "DDS_CYC3";
    sl_registers << "START_DLY2";
    sl_registers << "START_ADDR2";
    sl_registers << "STOP_ADDR2";
    sl_registers << "DDS_CYC2";
    sl_registers << "START_DLY1";
    sl_registers << "START_ADDR1";
    sl_registers << "STOP_ADDR1";
    sl_registers << "DDS_CYC1";
    sl_registers << "CFG_ERROR";
}
//--------------------------------------------------------------------------------
void Imitator_BRG::convert_xml(void)
{
    //---
    emit debug(QString("sl len = %1").arg(sl_registers.count()));
    //---

    bool ok = false;
    QString error_message;
    int error_line = 0;
    int error_column = 0;
    QString filename = "RegistersProfiles.xml";
    QString filename_new = "RegistersProfiles_new.xml";

    QXmlPut *put = new QXmlPut("ArrayOfProfile");

    QXmlGet *get = new QXmlGet();
    ok = get->load(filename, &error_message, &error_line, &error_column);
    if(!ok)
    {
        emit error(QString(tr("Файл %1 не занружен Ошибка: %2 line %3 column %4"))
                   .arg(filename)
                   .arg(error_message)
                   .arg(error_line)
                   .arg(error_column));
        return;
    }
    do
    {
        ok = get->findNextAndDescend("Profile");
        if(ok)
        {
            put->descend("Profile");
            ok = get->find("Name");
            if(ok)
            {
                QString name = get->getString();
                put->putString("Name", name);
                emit info(name);

                ok = get->findNextAndDescend("RegistersData");
                if(ok)
                {
                    int cnt = 0;
                    put->descend("RegistersData");
                    do
                    {
                        if(cnt <= sl_registers.count())
                        {
                            ok = get->findNext("unsignedShort");
                            if(ok)
                            {
                                put->putString(sl_registers.at(cnt), get->getString());
                                cnt++;
                            }
                        }
                        else
                        {
                            emit error(QString("sl count %1 != cnt %2")
                                       .arg(sl_registers.count())
                                       .arg(cnt));
                        }
                    } while(ok);
                    ok = get->rise();
                    ok = get->rise();

                    put->rise();
                    put->rise();
                }
            }
        }
    } while(ok);

    put->save(filename_new);
    emit info("Конвертация XML файла: OK");
}
//--------------------------------------------------------------------------------
bool Imitator_BRG::write_registers(void)
{
    QString filename = ":/doc/Simulator.xml";
    QString error_message;
    int error_line = 0;
    int error_column = 0;
    bool ok = false;

    emit info(QString("open file %1").arg(filename));

    QXmlGet *get = new QXmlGet();
    ok = get->load(filename, &error_message, &error_line, &error_column);
    if(!ok)
    {
        emit error(QString(tr("Файл %1 не загружен Ошибка: %2 line %3 column %4"))
                   .arg(filename)
                   .arg(error_message)
                   .arg(error_line)
                   .arg(error_column));
        return false;
    }
    do
    {
        ok = get->findNextAndDescend("Profile");
        if(ok)
        {
            ok = get->find("Name");
            if(ok)
            {
                QString name = get->getString();
                emit info(name);
                ok = get->findNextAndDescend("RegistersData");
                if(ok)
                {
                    foreach (QString reg, sl_registers)
                    {
                        ok = get->find(reg);
                        if(ok)
                        {
#if 0
                            emit info(QString("%1   %2")
                                      .arg(reg)
                                      .arg(get->getString()));
#endif
                        }
                        ok = AD9106_write(reg, get->getString().toInt());
                        if(!ok)
                        {
                            emit error(QString("error write %1").arg(reg));
                            return false;
                        }
                    }
                    ok = get->rise();
                    ok = get->rise();
                }
            }
        }
    } while(ok);
    return true;
}
//--------------------------------------------------------------------------------
bool Imitator_BRG::dev_open(void)
{
    #define MAX_STR 255

    int res;
    wchar_t wstr[MAX_STR];

    // Enumerate and print the HID devices on the system
    struct hid_device_info *devs, *cur_dev;

    devs = hid_enumerate(VID, PID);
    cur_dev = devs;
    if(cur_dev == NULL)
    {
        emit error("Имитатор БРГ не найден!");
        return false;
    }
    while (cur_dev)
    {
        emit info("Имитатор найден");
        emit info(QString("  Type: %1:%2 ").arg(cur_dev->vendor_id).arg(cur_dev->product_id));
        emit info(QString("  Path: %1").arg(cur_dev->path));
        emit info(QString("  Serial_number: %1").arg(QString::fromWCharArray(cur_dev->serial_number)));
        emit info("");
        emit info(QString("  Manufacturer: %1").arg(QString::fromWCharArray(cur_dev->manufacturer_string)));
        emit info(QString("  Product:      %1").arg(QString::fromWCharArray(cur_dev->product_string)));
        emit info("");
        cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);

    // Open the device using the VID, PID,
    // and optionally the Serial number.
    int cnt_err = 0;
    while(dev == 0)
    {
        dev = hid_open(VID, PID, NULL);
        if(dev == 0)
        {
            cnt_err++;
        }
        if(cnt_err > 9)
        {
            emit error("gid not open!");
            return false;
        }
    }

    // Read the Manufacturer String
    res = hid_get_manufacturer_string(dev, wstr, MAX_STR);
    if(res == 0)
    {
        emit info(QString("Manufacturer String: %1").arg(QString::fromWCharArray(wstr)));
    }
    else
    {
        emit error("Ошибка: hid_get_manufacturer_string");
        return false;
    }

    // Read the Product String
    res = hid_get_product_string(dev, wstr, MAX_STR);
    if(res == 0)
    {
        emit info(QString("Product String: %1").arg(QString::fromWCharArray(wstr)));
    }
    else
    {
        emit error("Ошибка: hid_get_product_string");
        return false;
    }

    // Read the Serial Number String
    res = hid_get_serial_number_string(dev, wstr, MAX_STR);
    if(res == 0)
    {
        emit info(QString("Serial Number String: %1").arg(QString::fromWCharArray(wstr)));
    }
    else
    {
        emit error("Ошибка: hid_get_serial_number_string");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
void Imitator_BRG::dev_close(void)
{
    if(dev != 0)
    {
        hid_close(dev);
        dev = 0;
    }
}
//--------------------------------------------------------------------------------
bool Imitator_BRG::refresh(void)
{
    bool ok = dev_open();
    if(ok)
    {
        dev_close();
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
bool Imitator_BRG::ApplySettings(void)
{
    //start
    if(dev == NULL)
    {
        emit error("handle not created");
        return false;
    }

    int res = 0;
    memset(output_buf, 0, sizeof(output_buf));

    output_buf[0] = 0x00;
    output_buf[1] = 0x82;
    output_buf[2] = 0x90;

    res = hid_send_feature_report(dev, output_buf, sizeof(output_buf));
    if(res < 0)
    {
        emit error(QString("hid_send_feature_report return %1").arg(res));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
void Imitator_BRG::ManualReset(void)
{
    emit info("ManualReset");

    //reset
    if(dev == NULL)
    {
        emit error("handle not created");
        return;
    }

    int res = 0;
    memset(output_buf, 0, sizeof(output_buf));

    output_buf[0] = 0x00;
    output_buf[1] = 0x82;
    output_buf[2] = 0x80;

    res = hid_send_feature_report(dev, output_buf, sizeof(output_buf));
    if(res < 0)
    {
        emit error(QString("hid_send_feature_report return %1").arg(res));
        return;
    }
    emit info("Сброс: OK");
}
//--------------------------------------------------------------------------------
bool Imitator_BRG::StopGeneration(void)
{
    emit info("StopGeneration");

    //stop
    if(dev == NULL)
    {
        emit error("handle not created");
        return false;
    }

    int res = 0;
    memset(output_buf, 0, sizeof(output_buf));

    output_buf[0] = 0x00;
    output_buf[1] = 0x82;
    output_buf[2] = 0x92;

    res = hid_send_feature_report(dev, output_buf, sizeof(output_buf));
    if(res < 0)
    {
        emit error(QString("hid_send_feature_report return %1").arg(res));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool Imitator_BRG::AD9106_read(QString name_reg, uint16_t *data)
{
    if(dev == NULL)
    {
        emit error("handle not created");
        return false;
    }
    if(name_reg.isEmpty())
    {
        emit error("name register is empty");
        return false;
    }

    bool ok = false;
    reg_t r;
    AD9106 *ad9106 = new AD9106();
    ok = ad9106->get_reg(name_reg, &r);
    ad9106->deleteLater();
    if(!ok)
    {
        emit error("Register not found!");
        return false;
    }
    int res = 0;
    memset(output_buf, 0, sizeof(output_buf));

    answer_t *i_packet = (answer_t *)output_buf;
    i_packet->cmd = 0x8f;
    i_packet->addr = r.address;
    i_packet->num = 1;

    res = hid_send_feature_report(dev, output_buf, sizeof(output_buf));
    if(res < 0)
    {
        emit error(QString("res = %1").arg(res));
        return false;
    }

    memset(output_buf, 0, sizeof(output_buf));
    res = hid_get_feature_report(dev, output_buf, sizeof(output_buf));
    if(res < 0)
    {
        emit error(QString("hid_get_feature_report return %1").arg(res));
        return false;
    }

    answer_t *o_packet = (answer_t *)output_buf;
    *data = o_packet->data;
    return true;
}
//--------------------------------------------------------------------------------
bool Imitator_BRG::AD9106_write(QString name_reg, uint16_t data)
{
    if(dev == NULL)
    {
        emit error("handle not created");
        return false;
    }
    if(name_reg.isEmpty())
    {
        emit error("name register is empty");
        return false;
    }

    emit debug(QString("%1:%2")
               .arg(name_reg)
               .arg(data));

    bool ok = false;
    reg_t r;
    AD9106 *ad9106 = new AD9106();
    ok = ad9106->get_reg(name_reg, &r);
    ad9106->deleteLater();
    if(!ok)
    {
        emit error("Register not found!");
        return false;
    }
    int res = 0;
    memset(output_buf, 0, sizeof(output_buf));

    answer_t *i_packet = (answer_t *)output_buf;
    i_packet->cmd = 0x85;
    i_packet->addr = r.address;
    i_packet->data = data;
    i_packet->num = 1;

    res = hid_send_feature_report(dev, output_buf, sizeof(output_buf));
    if(res < 0)
    {
        emit error(QString("hid_send_feature_report return %1").arg(res));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool Imitator_BRG::ReadADC(uint8_t channel, uint16_t *data)
{
    if(dev == NULL)
    {
        emit error("handle not created");
        return false;
    }

    int res = 0;
    memset(output_buf, 0, sizeof(output_buf));

    output_buf[0] = 0x00;
    output_buf[1] = 0x9A;
    output_buf[2] = 0x20;
    output_buf[3] = channel;

    res = hid_send_feature_report(dev, output_buf, sizeof(output_buf));
    if(res < 0)
    {
        emit error(QString("hid_send_feature_report return %1").arg(res));
        return false;
    }

    *data = (uint16_t)(output_buf[4] + (output_buf[5] << 8));
    return true;
}
//--------------------------------------------------------------------------------
bool Imitator_BRG::ReadVoltage(int channel, double *voltage)
{
    bool ok = false;
    uint16_t ADCData = 0;

    ok = ReadADC(channel, &ADCData);
    if(!ok) return ok;

    switch(channel)
    {
    case ADC_GetVoltage::AVCC:
        *voltage = (float)(0.00226f * 2.0F * ADCData);
        break;

    case ADC_GetVoltage::AVSS:
        *voltage = (float)(0.00226f * 2.0f * ADCData);
        break;

    case ADC_GetVoltage::BIAS:
        *voltage = (float)(0.00226f * 16.0f * ADCData);
        break;

    case ADC_GetVoltage::VCCIN:
        *voltage = (float)(0.00226f * 8.0f * ADCData);
        break;

    default:
        ok = false;
        break;
    }

    return ok;
}
//--------------------------------------------------------------------------------
void Imitator_BRG::start(void)
{
    bool ok = false;

    ok = dev_open();
    if(!ok)
    {
        return;
    }

    ok = write_registers();
    if(!ok)
    {
        return;
    }

    //FIXME
#if 0
    ok = set_values();
    if(!ok)
    {
        return;
    }
#endif

    ok = ApplySettings();
    if(!ok)
    {
        return;
    }

    dev_close();

    emit info("Старт: OK");
}
//--------------------------------------------------------------------------------
void Imitator_BRG::stop(void)
{
    bool ok = false;

    ok = dev_open();
    if(!ok)
    {
        return;
    }

    ok = StopGeneration();
    if(!ok)
    {
        return;
    }
    dev_close();

    emit info("Стоп: OK");
}
//--------------------------------------------------------------------------------
bool Imitator_BRG::set_param(PARAM param)
{
    bool ok = false;
    QString reg;

    uint16_t DDS_TW32 = (uint16_t)(param.DDS_TW >> 8);            //41
    uint16_t DDS_TW1  = (uint16_t)((param.DDS_TW & 0xff) << 8);   //42

    reg = "DDS_TW32"; ok = AD9106_write(reg, DDS_TW32); if(!ok) { emit error(QString("error write %1").arg(reg));  return false; }
    reg = "DDS_TW1"; ok = AD9106_write(reg, DDS_TW1); if(!ok) { emit error(QString("error write %1").arg(reg));  return false; }

    reg = "DAC4_DGAIN"; ok = AD9106_write(reg, param.DAC_DGAIN_XP); if(!ok) { emit error(QString("error write %1").arg(reg));  return false; }
    reg = "DAC3_DGAIN"; ok = AD9106_write(reg, param.DAC_DGAIN_XN); if(!ok) { emit error(QString("error write %1").arg(reg));  return false; }
    reg = "DAC1_DGAIN"; ok = AD9106_write(reg, param.DAC_DGAIN_YP); if(!ok) { emit error(QString("error write %1").arg(reg));  return false; }
    reg = "DAC2_DGAIN"; ok = AD9106_write(reg, param.DAC_DGAIN_YN); if(!ok) { emit error(QString("error write %1").arg(reg));  return false; }

    reg = "DAC4DOF"; ok = AD9106_write(reg, param.DAC_DOF_XP); if(!ok) { emit error(QString("error write %1").arg(reg));  return false; }
    reg = "DAC3DOF"; ok = AD9106_write(reg, param.DAC_DOF_XN); if(!ok) { emit error(QString("error write %1").arg(reg));  return false; }
    reg = "DAC1DOF"; ok = AD9106_write(reg, param.DAC_DOF_YP); if(!ok) { emit error(QString("error write %1").arg(reg));  return false; }
    reg = "DAC2DOF"; ok = AD9106_write(reg, param.DAC_DOF_YN); if(!ok) { emit error(QString("error write %1").arg(reg));  return false; }

    reg = "DDS4_PW"; ok = AD9106_write(reg, param.DDS_PW_XP); if(!ok) { emit error(QString("error write %1").arg(reg));  return false; }
    reg = "DDS3_PW"; ok = AD9106_write(reg, param.DDS_PW_XN); if(!ok) { emit error(QString("error write %1").arg(reg));  return false; }
    reg = "DDS1_PW"; ok = AD9106_write(reg, param.DDS_PW_YP); if(!ok) { emit error(QString("error write %1").arg(reg));  return false; }
    reg = "DDS2_PW"; ok = AD9106_write(reg, param.DDS_PW_YN); if(!ok) { emit error(QString("error write %1").arg(reg));  return false; }

    return true;
}
//--------------------------------------------------------------------------------
