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
#include <ftd2xx.h>
#include <ftdi.h>
//--------------------------------------------------------------------------------
#include "at93c56.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class QToolButton;
class QToolBar;

class GrapherBox;
class SerialBox;
//--------------------------------------------------------------------------------
class MainBox : public QWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    virtual ~MainBox();

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void send(QByteArray);

    void toolButtonStyleChanged(Qt::ToolButtonStyle);
    void iconSizeChanged(QSize);

public slots:

private slots:
    void log(const QString &data);
    void test_i2c_read(void);
    void test_i2c_write(void);

    void erase(void);
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

#ifdef SERIAL
    void input(const QByteArray &data);
#endif

    void read(void);
    void save(void);

private:
    MySplashScreen *splash;
    Ui::MainBox *ui;
    bool test_flag;

    GrapherBox *grapher;
#ifdef SERIAL
    SerialBox *serial;
    QByteArray *serial_data;
#endif

    ftdi_context ftdi;

    void init(void);
    void connect_log(void);

    QToolButton *add_button(QToolBar *tool_bar,
                            QToolButton *tool_button,
                            QIcon icon,
                            const QString &text,
                            const QString &tool_tip);

    void createTestBar(void);
    void createMenu(void);

    void load_setting(void);
    void save_setting(void);

    I2C_Freq get_i2c_freq(void);

    bool erase_eerpom(int VID, int PID);

protected:
    void changeEvent(QEvent *event);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
