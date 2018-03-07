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
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
#pragma pack (push, 1)

//--------------------------------------------------------------------------------
#pragma pack (push, 1)

typedef struct PACKET
{
  uint8_t     cmd_8;  // команда
  uint16_t    data;   // данные
} header;

#pragma pack(pop)

#define CMD_SET_DELAY_MS    1
#define CMD_SET_BRIGHTNESS  2

#define CMD_01 10
#define CMD_02 20
#define CMD_03 30
#define CMD_04 40
#define CMD_05 50
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class QToolButton;
class QSpinBox;
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
    void read_data(QByteArray ba);

    //---    
    void command(uint8_t cmd, uint16_t data);

    void set_delay_ms(void);
    void set_brightness(void);

    void cmd_1(void);
    void cmd_2(void);
    void cmd_3(void);
    void cmd_4(void);
    void cmd_5(void);
    //---

    void choice_test(void);
    void test_0(void);
    void test_1(void);
    void test_2(void);
    void test_3(void);
    void test_4(void);
    void test_5(void);

private:
    MySplashScreen *splash = 0;
    Ui::MainBox *ui = 0;
    QByteArray data_rs232;
    QByteArray clean_data;

    enum {
        ID_TEST_0 = 1000,
        ID_TEST_1,
        ID_TEST_2,
        ID_TEST_3,
        ID_TEST_4,
        ID_TEST_5,
        ID_TEST_6
    };

    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        void (MainBox::*func)(void);
    } CMD_t;
    QList<CMD> commands;

    QComboBox *cb_test = 0;

    bool is_ready = false;
    void wait(int max_time_ms);

    void init(void);
    void init_widgets(void);

    void createTestBar(void);
    void createSerialBox(void);

    QString convert_data_to_ascii(uint8_t data);
    uint8_t convert_ascii_to_value(char hi, char lo);

    void analize(void);

    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
