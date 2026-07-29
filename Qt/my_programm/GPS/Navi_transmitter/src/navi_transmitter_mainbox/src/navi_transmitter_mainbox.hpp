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
class MySplashScreen;
class SerialBox5;
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
    void test();

    void test_GGA();
    void test_GSA();
    void test_GSV();
    void test_RMC();
    void test_VTG();
    void test_GLL();
    void test_ZDA();

    void test_PIREA();
    void test_PIRFV();
    void test_PIRGK();
    void test_PIRRA();

    void read_data(QByteArray);
    void start();
    void stop();
    void update();

private:
    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;

    QList<QByteArray> fake_data;
    int index_fake_data = 0;
    int max_index_fake_data = 0;

    QTimer *timer = nullptr;

    void init();
    void createTestBar();
    QString get_checksum(const QString &data);
    void read_fake_data();

    QString get_utc_string();
    QString get_latitude_string();
    QString get_longitude_string();

    void updateText();
    bool programm_is_exit();
    void load_setting();
    void save_setting();
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
