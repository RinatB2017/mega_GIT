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
#ifndef PELCOD_HPP
#define PELCOD_HPP
//--------------------------------------------------------------------------------
#include <QFrame>
//--------------------------------------------------------------------------------
namespace Ui {
    class PelcoD;
}
//--------------------------------------------------------------------------------
class PelcoD : public QFrame
{
    Q_OBJECT

public:
    explicit PelcoD(QWidget *parent = 0);
    ~PelcoD();

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void send(QByteArray);

private slots:
    void f_send(void);
    void refresh(void);

    void f_Save_position(void);
    void f_Move_position(void);
    void f_Wiper(void);
    void f_Run_Tur_1(void);

private:
    Ui::PelcoD *ui = 0;

    void init(void);

    void Camera_Save_position(char preset);
    void Camera_Move_position(char preset);
    void Camera_Wiper(void);
    void Camera_Run_Tur_1(void);

protected:
    void changeEvent(QEvent *event);

};
//--------------------------------------------------------------------------------
#endif
