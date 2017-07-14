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
#include <QWidget>
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
class SerialBox;
class QToolButton;
class QComboBox;
class QToolBar;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    ~MainBox();

signals:
    void send(QByteArray);

private slots:
    void run(void);
    void read_data(QByteArray ba);

    QByteArray get_command_string(const QString &cmd);

    void command_AT(void);
    void command_RESET(void);
    void command_VERSION(void);
    void command_ORGL(void);
    void command_ADDR(void);
    void command_NAME(void);
    void command_RNAME(void);
    void command_ROLE(void);
    void command_CLASS(void);
    void command_IAC(void);
    void command_INQM(void);
    void command_PSWD(void);
    void command_UART(void);
    void command_CMODE(void);
    void command_BIND(void);
    void command_POLAR(void);
    void command_PIO(void);
    void command_MPIO(void);
    void command_IPSCAN(void);
    void command_SNIFF(void);
    void command_SENM(void);
    void command_PMSAD(void);
    void command_RMAAD(void);
    void command_FSAD(void);
    void command_ADCN(void);
    void command_MRAD(void);
    void command_STATE(void);
    void command_INIT(void);
    void command_INQ(void);
    void command_INQC(void);
    void command_PAIR(void);
    void command_LINK(void);
    void command_DISC(void);
    void command_ENSNIFF(void);
    void command_EXSNIFF(void);

private:
    MySplashScreen *splash = 0;
    Ui::MainBox *ui = 0;
    SerialBox *serialBox = 0;
    QByteArray data_rs232;
    bool is_ready = false;
    unsigned char test_byte = 0;
    unsigned int cnt = 0;

    QComboBox *cb_command = 0;

    void init(void);
    void connect_log(void);
    void wait(int max_time_ms);

    QToolButton *add_button(QToolBar *tool_bar,
                            QToolButton *tool_button,
                            QIcon icon,
                            const QString &text,
                            const QString &tool_tip);

    void createTestBar(void);

protected:
    void changeEvent(QEvent *event);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
