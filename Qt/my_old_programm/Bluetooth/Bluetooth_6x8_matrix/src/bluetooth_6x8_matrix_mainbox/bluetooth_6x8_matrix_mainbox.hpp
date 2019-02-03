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
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include <stdint.h>
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//---------------------------------------------------------------
#pragma pack (push, 1)
//---------------------------------------------------------------
#define NUM_LEDS_PER_STRIP 6
#define NUM_STRIPS         8
//---------------------------------------------------------------
typedef struct
{
    uint8_t address;
    uint8_t cmd;
    uint8_t count_data;
}
RGB_HEADER;
//---------------------------------------------------------------
union CMD_0x01_QUESTION
{
    struct {
        RGB_HEADER header;
        struct {
            uint8_t led[NUM_LEDS_PER_STRIP][NUM_STRIPS];
        } data;
        uint8_t crc16;
    }
    body;
    uint8_t buf[sizeof(body)];
};

union CMD_0x01_ANSWER
{
    struct {
        RGB_HEADER header;
        struct {
            uint8_t result;
        } data;
        uint8_t crc8;
    }
    body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
class MySplashScreen;
class SerialBox5;
class QToolButton;
class QToolBar;
class Led;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent,
            MySplashScreen *splash);
    ~MainBox();

signals:
    void send(QByteArray);

private slots:
    void test(void);
    void read_data(QByteArray ba);

private:
    MySplashScreen *splash;
    Ui::MainBox *ui;
    SerialBox5 *serialBox;

    QByteArray clean_data;

    Led *buf_led[NUM_STRIPS][NUM_LEDS_PER_STRIP];

    void init(void);

    void createTestBar(void);
    void read_modbus(void);
    void send_answer_data(void);

    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
