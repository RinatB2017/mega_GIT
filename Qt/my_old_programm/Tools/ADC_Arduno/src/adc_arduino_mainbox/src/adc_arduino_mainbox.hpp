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
#include <QList>
//--------------------------------------------------------------------------------
#include <inttypes.h>
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
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
enum CAPACITY {
    CAPACITY_8  = 8,
    CAPACITY_16 = 16,
    CAPACITY_24 = 24,
    CAPACITY_32 = 32
};
typedef struct {
    uint16_t signature;
    uint8_t  capacity;      // разрядность (8, 16, 24, 32)
    uint8_t  count_channel;  // кол-во каналов
} HEADER;
//--------------------------------------------------------------------------------
class SerialBox5;
class GrapherBox;
class Plot;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent = nullptr);
    ~MainBox();

private slots:
    void raw_data(QByteArray);
    void analize_packet(void);

private:
    Ui::MainBox *ui;
    QWidget *parent;

    GrapherBox *grapher;
    QList<int> curves;

    SerialBox5 *serial;
    QByteArray dirty_data;
    QByteArray clean_data;

    void init(void);

    unsigned char convert_ansi_to_dec(char data);
    QString convert_dec_to_ansi(unsigned char data);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_H
