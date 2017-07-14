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
#ifndef B588_HPP
#define B588_HPP
//--------------------------------------------------------------------------------
#include <QFrame>
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class B588;
}
//--------------------------------------------------------------------------------
class Powersupply_B588;

class QToolButton;
class QToolBar;
class GrapherBox;

#ifdef USE_V764
    class Multimeter_V764;
#endif
#ifdef USE_V7642
    class Multimeter_V764_2;
#endif
//--------------------------------------------------------------------------------
class B588 : public QFrame
{
    Q_OBJECT

public:
    explicit B588(QWidget *parent = 0);
    ~B588();

    bool is_blocked_exit(void);

    int xxx(int x);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

private slots:
    void init_multimeter(void);
    void find_devices(void);

private:
    Ui::B588 *ui = 0;

    Powersupply_B588  *powersupply = 0;

#ifdef USE_V764
    Multimeter_V764 *multimeter = 0;
#endif
#ifdef USE_V7642
    Multimeter_V764_2 *multimeter = 0;
#endif

    GrapherBox *grapher = 0;
    int index_progressbar = 0;

    int grapher_index = 0;

    int curve_powersuply = 0;
    int curve_B7642 = 0;

    int curve_U_parrot = 0;
    int curve_I_parrot = 0;
    int curve_U_input_parrot = 0;

    void connect_log(void);
    void init(void);

    QToolButton *add_button(QToolBar *tool_bar,
                            QToolButton *tool_button,
                            QIcon icon,
                            const QString &text,
                            const QString &tool_tip);
    void createTestBar(void);

    void measuring_B588(void);
    void measuring_V7642(void);

    void wait_msec(int timeout_msec);

    void block_interface(bool state);
};
//--------------------------------------------------------------------------------
#endif // B588_HPP
