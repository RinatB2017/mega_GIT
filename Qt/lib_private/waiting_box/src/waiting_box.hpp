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
#ifndef WAITING_BOX_HPP
#define WAITING_BOX_HPP
//--------------------------------------------------------------------------------
#include <QCloseEvent>
#include <QDialog>
//--------------------------------------------------------------------------------
namespace Ui {
    class Waiting_box;
}
//--------------------------------------------------------------------------------
class Waiting_box : public QDialog
{
    Q_OBJECT

public:
    explicit Waiting_box(QWidget *parent);
    ~Waiting_box();

    void set_close_form(bool state);

private:
    Ui::Waiting_box *ui;
    bool flag_close_form = false;

protected:
    void closeEvent(QCloseEvent *event) override;
};
//--------------------------------------------------------------------------------
#endif // WAITING_BOX_HPP
