/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#ifndef MESH_CONTROL_HPP
#define MESH_CONTROL_HPP
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class Mesh_control;
}
//--------------------------------------------------------------------------------
class Mesh_control : public MyWidget
{
    Q_OBJECT

public:
    explicit Mesh_control(QString o_name, QWidget *parent = nullptr);
    ~Mesh_control();

signals:
    void send(QString msg);

private slots:
    void read_data(QByteArray data);

    void led_on(void);
    void led_off(void);

private:
    Ui::Mesh_control *ui;

    void init(void);
    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif // MESH_CONTROL_HPP
