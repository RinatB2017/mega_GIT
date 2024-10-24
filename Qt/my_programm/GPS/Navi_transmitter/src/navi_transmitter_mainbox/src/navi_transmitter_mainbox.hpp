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
    void test(void);

    void test_GGA(void);
    void test_GSA(void);
    void test_GSV(void);
    void test_RMC(void);
    void test_VTG(void);
    void test_GLL(void);
    void test_ZDA(void);

    void test_PIREA(void);
    void test_PIRFV(void);
    void test_PIRGK(void);
    void test_PIRRA(void);

    void read_data(QByteArray);
    void start(void);
    void stop(void);
    void update(void);

private:
    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;

    QList<QByteArray> fake_data;
    int index_fake_data = 0;
    int max_index_fake_data = 0;

    QTimer *timer = nullptr;

    void init(void);
    void createTestBar(void);
    QString get_checksum(const QString &data);
    void read_fake_data(void);

    QString get_utc_string(void);
    QString get_latitude_string(void);
    QString get_longitude_string(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
