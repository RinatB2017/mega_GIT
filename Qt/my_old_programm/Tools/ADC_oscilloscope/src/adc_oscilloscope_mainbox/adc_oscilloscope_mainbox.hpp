/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#ifndef MAINBOX_H
#define MAINBOX_H
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QQueue>
#include <stdint.h>
//--------------------------------------------------------------------------------
#include "modbusasciiwidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class OscilloscopeBox;
class Serial_thread;
class QToolButton;
class QToolBar;
class Plot;
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#pragma pack (push, 1)
//--------------------------------------------------------------------------------
union DATA_INT16 {
    short value;
    struct {
        unsigned char a;
        unsigned char b;
    } bytes;
};

union DATA_INT32 {
    int value;
    struct {
        unsigned char a;
        unsigned char b;
        unsigned char c;
        unsigned char d;
    } bytes;
};

union DATA_FLOAT {
    float value;
    struct {
        unsigned char a;
        unsigned char b;
        unsigned char c;
        unsigned char d;
    } bytes;
};
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent = nullptr);
    ~MainBox();

private slots:
    void test(void);

    void get_data(const DATA &data);
    void get_data(const QByteArray &data);

private:
    Ui::MainBox *ui;
    QWidget *parent;

    OscilloscopeBox *oscilloscope;
    ModbusAsciiWidget *widget;
    Serial_thread *thread;

    void init(void);

    void createTestBar(void);
    void createSerialBar(void);

    void add_data_1byte(unsigned int index_curve, int8_t  data);
    void add_data_2byte(unsigned int index_curve, int16_t data);
    void add_data_4byte(unsigned int index_curve, int32_t data);
    void add_data_float(unsigned int index_curve, float data);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_H
