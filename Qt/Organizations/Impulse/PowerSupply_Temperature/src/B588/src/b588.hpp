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
#ifndef B588_HPP
#define B588_HPP
//--------------------------------------------------------------------------------
#include <QWidget>

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class B588;
}
//--------------------------------------------------------------------------------
class QToolButton;
class QToolBar;
class QComboBox;
class QTimer;

class GrapherBox;
class Powersupply_B588;
//--------------------------------------------------------------------------------
class B588 : public MyWidget
{
    Q_OBJECT

public:
    explicit B588(QWidget *parent);
    ~B588();

private slots:
    void find_device(void);
    void measuring(bool state);
    void update(void);
    void test(void);

    void log(const QString &text);

private:
    Ui::B588 *ui = 0;

    bool is_blocked;
    QComboBox *cb_test = 0;

    void createTestBar(void);

#ifndef NO_GRAPH
    GrapherBox *grapher = 0;
    int curve_1 = 0;
    int curve_2 = 0;
    int curve_3 = 0;
    int curve_temp = 0;
#endif

    Powersupply_B588 *powersupply = 0;

    QTimer *timer = 0;

    void init(void);
    void connect_log(void);

    QToolButton *add_button(QToolBar *tool_bar,
                            QToolButton *tool_button,
                            QIcon icon,
                            const QString &text,
                            const QString &tool_tip);

    void correct_temperature(void);

protected:
    void changeEvent(QEvent *event);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
