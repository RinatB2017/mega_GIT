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
#ifndef PALETTE_HPP
#define PALETTE_HPP
//--------------------------------------------------------------------------------
#include <QGridLayout>
#include <QGroupBox>
//--------------------------------------------------------------------------------
class Diod;
//--------------------------------------------------------------------------------
class MyPalette : public QGroupBox
{
    Q_OBJECT

public:
    MyPalette(int max_x,
              int max_y,
              QWidget *parent = 0);
    ~MyPalette();

    void set_active(bool value);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

private:
    QGridLayout *grid = 0;
    QList<Diod *> buttons;

    bool flag_active = false;

    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif
