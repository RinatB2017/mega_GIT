/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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
#ifndef MULTIMETERBOX_HPP
#define MULTIMETERBOX_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
namespace Ui {
    class Multimeter;
}
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class BatteryProgressBar;
class QPushButton;
class QToolButton;
class QToolBar;

class Multimeter_V786_2;

#ifndef NO_GRAPHER
class GrapherBox;
#endif
//--------------------------------------------------------------------------------
class MultimeterBox : public MyWidget
{
    Q_OBJECT

public:
    MultimeterBox(const QString &title,
                  QWidget *parent = nullptr);
    ~MultimeterBox();

signals:
    void start(void);
    void stop(void);

    void radiomodule_send(QByteArray);

private slots:
    void measurement_changed(int index);

    void start_measuring(void);
    void stop_measuring(void);

    void search_devices(void);

    void measuring_finished(void);

    void power(bool state);
    void block_interface(bool state);

#ifdef FAKE
    void fake(void);
#endif
    
#ifdef TESTBAR
    void test(void);
#endif

private:
    Ui::Multimeter *ui;
    QString title;

    BatteryProgressBar *battery_progress_bar;
    QPushButton *btn_find_multimeter;

    Multimeter_V786_2 *multimeter;

#ifndef NO_GRAPHER
    GrapherBox *grapher;
#endif

    bool flag_measuring;

    int curve_UDC = 0;
    int curve_UAC = 0;
    int curve_IDC = 0;
    int curve_IAC = 0;
    int curve_R = 0;
    int curve_C = 0;
    int curve_L = 0;
    int curve_F = 0;
    int curve_P = 0;
    int curve_Q = 0;
    int curve_S = 0;
    int curve_PF = 0;

#ifdef FAKE
    void createTestBar(void);
#endif

#ifdef FAKE
    void createFakeBar(void);
#endif

    void init(void);
#ifndef NO_GRAPHER
    void init_grapherbox(void);
    void add_grapher_value(double value);
#endif

    int get_measurement(bool *ok);
    int get_retention(bool *ok);
    int get_deviation(bool *ok);
    int get_limit(bool *ok);

    void reset_show_values(void);
    void prepare_param(void);

    void msgError(const QString &message);

    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif // MULTIMETERBOX_HPP
