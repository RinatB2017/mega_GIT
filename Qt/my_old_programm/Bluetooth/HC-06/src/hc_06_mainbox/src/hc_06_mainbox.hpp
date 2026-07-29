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
#include <QElapsedTimer>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
class MainBox;
}
//--------------------------------------------------------------------------------
enum {
    AT = 0,
    RESET,
    VERSION,
    ORGL,
    ADDR,
    NAME,
    RNAME,
    ROLE,
    CLASS,
    IAC,
    INQM,
    PSWD,
    UART,
    CMODE,
    BIND,
    POLAR,
    PIO,
    MPIO,
    IPSCAN,
    SNIFF,
    SENM,
    PMSAD,
    RMAAD,
    FSAD,
    ADCN,
    MRAD,
    STATE,
    INIT,
    INQ,
    INQC,
    PAIR,
    LINK,
    DISC,
    ENSNIFF,
    EXSNIFF
};
//--------------------------------------------------------------------------------
class MySplashScreen;
class SerialBox5;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    virtual ~MainBox();

signals:
    void send(QByteArray);

private slots:
    void run();
    void read_data(QByteArray ba);

    QByteArray get_command_string(const QString &cmd);

    void command_AT();
    void command_RESET();
    void command_VERSION();
    void command_ORGL();
    void command_ADDR();
    void command_NAME();
    void command_RNAME();
    void command_ROLE();
    void command_CLASS();
    void command_IAC();
    void command_INQM();
    void command_PSWD();
    void command_UART();
    void command_CMODE();
    void command_BIND();
    void command_POLAR();
    void command_PIO();
    void command_MPIO();
    void command_IPSCAN();
    void command_SNIFF();
    void command_SENM();
    void command_PMSAD();
    void command_RMAAD();
    void command_FSAD();
    void command_ADCN();
    void command_MRAD();
    void command_STATE();
    void command_INIT();
    void command_INQ();
    void command_INQC();
    void command_PAIR();
    void command_LINK();
    void command_DISC();
    void command_ENSNIFF();
    void command_EXSNIFF();

private:
    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;

    SerialBox5 *serialBox;
    QLineEdit *le_name;

    QByteArray data_rs232;
    bool is_ready = false;
    unsigned char test_byte = 0;
    unsigned int cnt = 0;

    QComboBox *cb_command;

    void init();
    void wait(int max_time_ms);
    void show_data();

    void createTestBar();

    void updateText();
    bool programm_is_exit();
    void load_setting();
    void save_setting();
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
