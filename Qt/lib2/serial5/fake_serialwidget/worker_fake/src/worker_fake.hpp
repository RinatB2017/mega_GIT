/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                      **
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
#ifndef WORKER_FAKE_HPP
#define WORKER_FAKE_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
namespace Ui {
    class Worker_fake;
}
//--------------------------------------------------------------------------------
class Worker_fake : public QWidget
{
    Q_OBJECT

signals:
    void output(QByteArray text);

public:
    explicit Worker_fake(QWidget *parent = nullptr);
    ~Worker_fake();

    void input(QByteArray data);

private:
    Ui::Worker_fake *ui;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // Worker_fake_HPP
