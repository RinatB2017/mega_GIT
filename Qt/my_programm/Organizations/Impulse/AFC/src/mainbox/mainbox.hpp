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
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "v786_2_multimeter.hpp"
#include "v786_2_packets.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
enum
{
    CABLE_IN = 0,
    CABLE_A,
    CABLE_mA
};
enum
{
    H4_6 = 0,
    H4_7 = 1
};

//--------------------------------------------------------------------------------
struct questions_data
{
    QString calibrator_question_before;
    QString calibrator_question;

    int cmd;

    int cable_in;
    int calibrator_type;
    bool calibrator_step_by_step; // поклавишно на калибратор
    bool need_amplifier_U;
    bool need_amplifier_I_AC;
    bool need_amplifier_I_DC;
    int step_a;
    int step_b;
    int step_c;
};
//--------------------------------------------------------------------------------
class Multimeter_V786_2;
class Calibrator;

class MySplashScreen;
class QDoubleSpinBox;
class QPushButton;
class QToolButton;
class GrapherBox;
class QComboBox;
class SerialBox5;
class QSpinBox;
class QToolBar;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent,
            MySplashScreen *splash);
    ~MainBox();

private slots:
    void run(void);
    void test(void);

    void change_limit(void);

    void search_devices(void);

private:
    MySplashScreen *splash = 0;
    Ui::MainBox *ui = 0;

    GrapherBox *grapher = 0;
    Calibrator *calibrator = 0;
    Multimeter_V786_2 *multimeter = 0;
    SerialBox5 *serialPort_calibrator = 0;

    QSerialPort serial_radiomodule;
    QPushButton *btn_find_multimeter = 0;

    QVector <questions_data> answers;

    void init(void);

    void createTestBar(void);

    QString prepare_string(const QString &prefix,
                           float value,
                           float limit,
                           int step);
    void prepare_step_1_0(int calibrator_type);
    void msgInfo(const QString &message);
    void msgError(const QString &message);

    void load_setting(void);
    void save_setting(void);

    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
