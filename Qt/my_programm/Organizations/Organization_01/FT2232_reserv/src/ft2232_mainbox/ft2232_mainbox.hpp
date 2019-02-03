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

#include <ftd2xx.h>
#include <ftdi.h>
#include "i2c.hpp"

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class QToolButton;
class QToolBar;

class GrapherBox;
class SerialBox5;
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

    void toolButtonStyleChanged(Qt::ToolButtonStyle);
    void iconSizeChanged(QSize);

private slots:
    void test_i2c_read(void);
    void test_i2c_write(void);

    void test(void);
    void test_at93c56(void);
    void test_MCP4922(void);
    void test_AD8400(void);
    void test_i2c(void);

    int read_eeprom_value(enum ftdi_eeprom_value value_name);
    void test_eeprom(void);

    void read_all(void);
    void write_data(void);
    void clear_all(void);

    void bitbang_on(void);
    void bitbang_off(void);

    void input(const QByteArray &data);

    void read(void);
    void save(void);

private:
    MySplashScreen *splash;
    Ui::MainBox *ui;
    bool test_flag = false;

    GrapherBox *grapher;
    SerialBox5 *serial;
    QByteArray *serial_data;

    ftdi_context ftdi;

    void init(void);

    void createTestBar(void);

    void load_setting(void);
    void save_setting(void);

    I2C_Freq get_i2c_freq(void);

    bool erase_eerpom(int VID, int PID);

    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
