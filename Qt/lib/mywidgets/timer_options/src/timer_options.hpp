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
#ifndef TIMER_OPTIONS_HPP
#define TIMER_OPTIONS_HPP
//--------------------------------------------------------------------------------
#include <QDialog>
#include <QTime>
//--------------------------------------------------------------------------------
namespace Ui {
    class Timer_options;
}
//--------------------------------------------------------------------------------
class Timer_options : public QDialog
{
    Q_OBJECT

public:
    explicit Timer_options(QWidget *parent = nullptr);
    virtual ~Timer_options();

    void  set_time(const QTime &time);
    QTime get_time(void);

    void set_message(const QString &message);
    QString get_message(void);

private:
    Ui::Timer_options *ui;
};
//--------------------------------------------------------------------------------
#endif // TIMER_OPTIONS_HPP
