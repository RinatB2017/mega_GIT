/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#ifndef MAIN_OPTIONS_HPP
#define MAIN_OPTIONS_HPP
//--------------------------------------------------------------------------------
#include <QVariant>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class Main_options;
}
//--------------------------------------------------------------------------------
class Main_options : public MyWidget
{
    Q_OBJECT

public:
    explicit Main_options(QWidget *parent = 0);
    ~Main_options();

signals:
    void signal_is_shows_info(bool);
    void signal_is_shows_debug(bool);
    void signal_is_shows_error(bool);
    void signal_is_shows_trace(bool);

private slots:
    void block_wheel(void);

    void t_accept(void);

    void is_shows_info(bool state);
    void is_shows_debug(bool state);
    void is_shows_error(bool state);
    void is_shows_trace(bool state);

private:
    Ui::Main_options *ui;

    QVariant max_net_timeout = 3000;
    QVariant max_pet_timeout = 1000;

    bool flag_is_shows_info = true;
    bool flag_is_shows_debug = false;
    bool flag_is_shows_error = true;
    bool flag_is_shows_trace = false;

    void init(void);

protected:
    bool eventFilter(QObject*, QEvent* event);

};
//--------------------------------------------------------------------------------
#endif
