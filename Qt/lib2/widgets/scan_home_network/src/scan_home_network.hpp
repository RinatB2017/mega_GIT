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
    ~Scan_home_network();

private:
    Ui::Scan_home_network *ui;

    QPointer<QProcess> myProcess;

    void init(void);
    void create_engine(void);

    void scan(void);

    void started(void);
    void finished(int result);
    void process_error(QProcess::ProcessError p_error);
    void readData(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // SCAN_HOME_NETWORK_HPP
