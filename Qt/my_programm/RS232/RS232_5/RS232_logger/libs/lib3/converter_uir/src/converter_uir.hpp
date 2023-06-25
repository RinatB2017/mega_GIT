/*********************************************************************************
**                                                                              **
**     Copyright (C) 2022                                                       **
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
#ifndef CONVERTER_UIR_HPP
#define CONVERTER_UIR_HPP
//--------------------------------------------------------------------------------
#include <QStringList>
#include <QObject>
#include "connection.hpp"
//--------------------------------------------------------------------------------
class Converter_UIR : public QObject
{
    Q_OBJECT

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

public:
    explicit Converter_UIR(QObject *parent = nullptr);
    ~Converter_UIR();

    void input(const QString &data);
    void print_value(void);

private:
    enum {
        STATE_NONE = 0,
        STATE_U,
        STATE_I,
        STATE_R,
        STATE_F,
        STATE_VALUE,    // подсчёт UIR
        STATE_FREQ      // подсчёт F
    };
    int state = STATE_NONE;

    int state_result = STATE_NONE;
    QByteArray ba_value;
    QByteArray ba_freq;
    float k_value = 1.0f;
    float k_freq = 1.0f;

    QString mem_data;
    float value = 0.0f;
    float freq = 0.0f;

    void analize(const QString &key);
    void ba_sprit(const QString &ba);
};
//--------------------------------------------------------------------------------
#endif // CONVERTER_UIR_HPP
