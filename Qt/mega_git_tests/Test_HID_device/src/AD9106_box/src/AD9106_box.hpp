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
#ifndef AD9106_BOX_HPP
#define AD9106_BOX_HPP
//--------------------------------------------------------------------------------
#include <QTableWidget>
#include <QHeaderView>
#ifdef Q_OS_LINUX
#   include <stdint.h>
#endif
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
    #include <hidapi/hidapi.h>
#endif
#ifdef Q_OS_WIN
    #include "hidapi_win/hidapi.h"
#endif
//--------------------------------------------------------------------------------
#pragma pack(push, 1)

typedef struct question
{
    uint8_t     zero;
    uint8_t     cmd;
    uint8_t     num;
    uint16_t    addr;
} question_t;

typedef struct answer
{
    uint8_t     zero;
    uint8_t     cmd;
    uint8_t     num;
    uint16_t    addr;
    uint16_t    data;
} answer_t;

enum ADC_GetVoltage
{
    AVCC = 0,
    AVSS = 1,
    BIAS = 2,
    VCCIN = 3
};

enum DAC_SetVoltage
{
    REFIO = 0,
    TEMPRETURE = 1,
    NU0 = 2,
    NU1 = 3
};

#pragma pack(pop)
//--------------------------------------------------------------------------------
namespace Ui {
    class AD9106_Box;
}
//--------------------------------------------------------------------------------
class CurveBox;
class QHexEdit;
//--------------------------------------------------------------------------------
class AD9106_Box : public MyWidget
{
    Q_OBJECT

public:
    explicit AD9106_Box(QWidget *parent = nullptr);
    virtual ~AD9106_Box();

signals:
    void block_widget(bool);

private slots:
    void choice_test();
    void test();

    //---
    void dev_open();
    void dev_close();
    void dev_read_all_registers();
    void dev_write_all_registers();

    void ApplySettings();
    void StopGeneration();
    void ManualReset();

    void read_xml();
    void convert_xml();

    void set_values();
    //---
    void btn_open_click();
    void btn_read_all_click();
    void btn_close_click();
    void btn_ApplySettings_click();
    void btn_StopGeneration_click();
    void btn_ManualReset_click();
    void btn_read_xml_click();
    void btn_convert_xml_click();
    void btn_set_values_click();
    //---

    void click(bool state);

private:
    struct CMD
    {
        int cmd;
        QString cmd_text;
        void (AD9106_Box::*func)();
    };
    Ui::AD9106_Box *ui;

    //---
    hid_device *dev = nullptr;
    bool AD9106_read(QString name_reg, uint16_t *data);
    bool AD9106_write(QString name_reg, uint16_t data);
    //---
    bool ReadADC(uint8_t channel, uint16_t *data);
    bool ReadTemperature(float *temperature);
    bool ReadVoltage(int channel, double *voltage);
    // DDS.ADC_GetVoltage.AVCC
    // DDS.ADC_GetVoltage.AVSS
    // DDS.ADC_GetVoltage.BIAS
    // DDS.ADC_GetVoltage.VCCIN
    //---
#ifdef Q_OS_LINUX
    uint8_t output_buf[128];
#endif
#ifdef Q_OS_WIN
    uint8_t output_buf[256];
#endif

    QPointer<QComboBox> cb_test;
    QList<CMD> commands;

    QStringList sl_registers;

#ifdef GRAPHER
    CurveBox *curve;
#endif

    void init();
    void init_sl_registers();

    void createTestBar();

    void updateText();
    bool programm_is_exit();
    void load_setting();
    void save_setting();

protected:
    void changeEvent(QEvent *event);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
