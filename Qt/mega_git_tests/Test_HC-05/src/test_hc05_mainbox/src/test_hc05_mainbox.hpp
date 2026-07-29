/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
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
    void choice_test();
    bool f_test();

    void find_device();
    bool get_version();
    bool get_address();
    bool reset();

    void port_read();
    void port_error(QSerialPort::SerialPortError serial_error);

    void test();

private:
    MySplashScreen *splash = 0;
    Ui::MainBox *ui = 0;

    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (MainBox::*func)();
    } CMD_t;

    QSerialPort serial;

    QByteArray rs232_data;
    bool is_ready = false;

    bool flag_closed = false;

    void init_serial();
    bool send_AT();
    void wait(int time_msec);

    QComboBox *cb_test = 0;
    QList<CMD> commands;

    void init();
    void createTestBar();
    bool send_command(QString cmd_string);

    void updateText();
    bool programm_is_exit();
    void load_setting();
    void save_setting();
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
