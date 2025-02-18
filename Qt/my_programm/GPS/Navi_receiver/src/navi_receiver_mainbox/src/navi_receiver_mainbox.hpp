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
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
#include <QPointer>
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class Proto_NMEA_0183;
class MySplashScreen;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent,
            MySplashScreen *splash);
    virtual ~MainBox();

signals:
    void output_latitude_string(QString);
    void output_longitude_string(QString);
    void output_observation(QString);

    void message(QString);

    void send(QByteArray);

private slots:
    void test(void);
    void read_data(QByteArray ba);

    void skip_backward(void);
    void seek_backward(void);
    void seek_forward(void);
    void skip_forward(void);

private:
    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;
    QByteArray data_rs232;

    Proto_NMEA_0183 *proto;

    QList<QByteArray> fake_data;
    int index_fake_data = 0;
    int max_index_fake_data = 0;
    QPointer<QLineEdit> le_index;

    void init(void);
    void init_protocol(void);
    void createTestBar(void);
    void read_fake_data(void);
    void analize(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
