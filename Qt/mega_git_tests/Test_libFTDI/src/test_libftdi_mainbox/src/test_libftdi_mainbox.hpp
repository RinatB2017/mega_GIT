/*********************************************************************************
**                                                                              **
**     Copyright (C) 2023                                                       **
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
#include <QElapsedTimer>
#include <QByteArray>
#include <QtGlobal>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
#include "ftdi.h"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    virtual ~MainBox();

private slots:
    void choice_test(void);
    bool test(void);
    bool test2(void);
    bool test3(void);

private:
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (MainBox::*func)(void);
    } CMD_t;

    union DATA
    {
        struct {
            uint16_t data:12;
            uint16_t SHDN:1;
            uint16_t GA:1;
            uint16_t BUF:1;
            uint16_t AB:1;
        } bites;
        uint16_t u16;
    };

    union U16
    {
        uint16_t u16;
        struct {
            uint8_t byte_0;
            uint8_t byte_1;
        } u8_2;
    };

    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;

    QPointer<QComboBox> cb_test;
    QList<CMD> commands;

    void init(void);
    void createTestBar(void);

    struct ftdi_context ftdi;

    void f_open(void);
    void f_close(void);

    void f_read_eeprom(void);
    void f_get_eeprom_buf(void);

    void f_set_eeprom_buf(void);
    void f_write_eeprom(void);

    void f_eeprom_decode(void);
    void f_eeprom_initdefaults(void);

    void f_test(void);

    bool set_bitmode(unsigned char bitmask, unsigned char mode);
    bool write_data(unsigned char *buf, int size);

    uint16_t get_VID(void);
    uint16_t get_PID(void);
    void set_VID(uint16_t value);
    void set_PID(uint16_t value);

    void wait_msec(int timeout_msec);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
