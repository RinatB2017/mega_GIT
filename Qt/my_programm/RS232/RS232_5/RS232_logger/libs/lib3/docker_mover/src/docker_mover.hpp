/*********************************************************************************
**                                                                              **
**     Copyright (C) 2023                                                       **
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
#ifndef DOCKER_MOVER_HPP
#define DOCKER_MOVER_HPP
//--------------------------------------------------------------------------------
#include <QDialog>
//--------------------------------------------------------------------------------
enum DOCKER_STATES {
    NO_STATE =  0,
    UP,
    DOWN,
    LEFT,
    RIGHT
};
//--------------------------------------------------------------------------------
namespace Ui {
    class Docker_mover;
}
//--------------------------------------------------------------------------------
class Docker_mover : public QDialog
{
    Q_OBJECT

public:
    explicit Docker_mover(QWidget *parent = nullptr);
    ~Docker_mover();

    DOCKER_STATES get_state(void);

private:
    Ui::Docker_mover *ui;
    DOCKER_STATES state = NO_STATE;

    void init(void);

    void up(void);
    void down(void);
    void left(void);
    void right(void);
};
//--------------------------------------------------------------------------------
#endif // DOCKER_MOVER_HPP
