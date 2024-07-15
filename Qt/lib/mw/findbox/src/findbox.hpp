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
#ifndef FINDBOX_HPP
#define FINDBOX_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QStyle>
//--------------------------------------------------------------------------------
namespace Ui {
    class FindBox;
}
//--------------------------------------------------------------------------------
class FindBox : public QWidget
{
    Q_OBJECT

signals:
    void find_prev(const QString &);
    void find_next(const QString &);

public:
    explicit FindBox(QWidget *parent = nullptr);
    virtual ~FindBox();

private:
    Ui::FindBox *ui;

    void init(void);

    void f_hide(void);
    void f_prev(void);
    void f_next(void);
};

#endif // FINDBOX_HPP
//--------------------------------------------------------------------------------
