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
#ifdef V764
    #include "v764_multimeter.hpp"
#endif
#ifdef V7642
    #include "v764_2_multimeter.hpp"
#endif
#ifdef V780
    #include "v780_multimeter.hpp"
#endif
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class Multimeter_V780;
class MySplashScreen;
class QToolButton;
class QToolBar;
class GrapherBox;

class QDoubleSpinBox;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    ~MainBox();

signals:
    void start();
    void stop();

private slots:
    void test(void);

    void power(bool state);
    void find_device(void);

    void get_limit(int limit);
    void get_measurement_mode(MODE mode);

    void start_measuring(void);
    void stop_measuring(void);

#ifdef MEASURING_CURRENT_IN_SHUNT
    void calc(void);
#endif

    void send_cmd(void);

private:
    MySplashScreen *splash = 0;
    Ui::MainBox *ui = 0;

#ifdef V764
    Multimeter_V764 *multimeter = 0;
#endif
#ifdef V7642
    Multimeter_V764_2 *multimeter = 0;
#endif
#ifdef V780
    Multimeter_V780 *multimeter = 0;
#endif

    bool flag_measuring;

    int multimeter_state;
    int multimeter_limit;

    GrapherBox *grapher = 0;
    int curve_UDC = 0;
    int curve_UAC = 0;
    int curve_IDC = 0;
    int curve_IAC = 0;
    int curve_R = 0;
#ifdef MEASURING_CURRENT_IN_SHUNT
    int curve_I = 0;
#endif

#ifdef MEASURING_CURRENT_IN_SHUNT
    QDoubleSpinBox *db_voltage_value = 0;
    QDoubleSpinBox *db_shunt_value = 0;
    QDoubleSpinBox *db_current_value = 0;
#endif

    void init(void);

    QToolButton *add_button(QToolBar *tool_bar,
                            QToolButton *tool_button,
                            QIcon icon,
                            const QString &text,
                            const QString &tool_tip);

    void createTestBar(void);
    void createMeasureCurrentBar(void);

    void block_interface(bool state);
    void display_measuring_value(int value_uV);

protected:
    void changeEvent(QEvent *event);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
