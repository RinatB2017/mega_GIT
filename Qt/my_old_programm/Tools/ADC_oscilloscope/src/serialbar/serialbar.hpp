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
#ifndef SERIALBAR_HPP
#define SERIALBAR_HPP
//--------------------------------------------------------------------------------
#include <QToolBar>
#include <QWidget>
//--------------------------------------------------------------------------------
class QRadioButton;
class QToolButton;
class QComboBox;
//--------------------------------------------------------------------------------
class SerialBar : public QToolBar
{
    Q_OBJECT
public:
    explicit SerialBar(const QString &title, QWidget *parent = 0);

private slots:

private:
    QComboBox *port_name;
    QComboBox *port_baud;

    QToolButton *port_refresh;
    QToolButton *port_start;
    QToolButton *port_stop;

    QRadioButton *data_1_byte;
    QRadioButton *data_2_byte;
    QRadioButton *data_4_byte;
    QRadioButton *data_float;
};
//--------------------------------------------------------------------------------
#endif // SERIALBAR_HPP
