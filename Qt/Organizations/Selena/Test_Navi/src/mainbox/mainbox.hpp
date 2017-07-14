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
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class Proto_NMEA_0183;
class MySplashScreen;
class SerialBox;
class QToolButton;
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
    void output_latitude_string(QString);
    void output_longitude_string(QString);
    void output_observation(QString);

    void send(QByteArray);

private slots:
    void test(void);
    void read_data(QByteArray ba);

private:
    MySplashScreen *splash = 0;
    Ui::MainBox *ui = 0;
    SerialBox *serialBox = 0;
    QByteArray data_rs232;
    bool is_ready;
    unsigned char test_byte;
    unsigned int cnt;

    Proto_NMEA_0183 *proto = 0;

    void init(void);
    void init_protocol(void);
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
