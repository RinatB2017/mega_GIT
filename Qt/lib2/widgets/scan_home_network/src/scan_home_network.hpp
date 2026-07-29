/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#ifndef SCAN_HOME_NETWORK_HPP
#define SCAN_HOME_NETWORK_HPP
//--------------------------------------------------------------------------------
#include <QProcess>
#include <QPointer>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class Scan_home_network;
}
//--------------------------------------------------------------------------------
class Scan_home_network : public MyWidget
{
    Q_OBJECT

public:
    explicit Scan_home_network(QWidget *parent = nullptr);
    virtual ~Scan_home_network();

private:
    Ui::Scan_home_network *ui;

    QPointer<QProcess> myProcess;

    void init();
    void create_engine();

    void scan();

    void started();
    void finished(int result);
    void process_error(QProcess::ProcessError p_error);
    void readData();

    void updateText();
    bool programm_is_exit();
    void load_setting();
    void save_setting();
};
//--------------------------------------------------------------------------------
#endif // SCAN_HOME_NETWORK_HPP
