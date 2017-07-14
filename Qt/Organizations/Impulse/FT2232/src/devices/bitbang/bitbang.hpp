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
#ifndef BITBANG_HPP
#define BITBANG_HPP
#include <ftd2xx.h>
#include <ftdi.h>
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "at93c56.hpp"
//--------------------------------------------------------------------------------
class FT2232H;
//--------------------------------------------------------------------------------
class BitBang : public QWidget
{
    Q_OBJECT
public:
    explicit BitBang(I2C_Freq freq, QWidget *parent = 0);
    ~BitBang();

    bool open(void);
    void close(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

private slots:
    void log(const QString &data);

public slots:
    void power_on(void);
    void power_off(void);

private:
    FT2232H *ft2232h;

    void connect_log(void);
};
//--------------------------------------------------------------------------------
#endif // BITBANG_HPP
