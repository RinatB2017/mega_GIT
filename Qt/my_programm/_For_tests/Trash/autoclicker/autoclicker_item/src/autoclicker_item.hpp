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
#ifndef AUTOCLICKER_ITEM_HPP
#define AUTOCLICKER_ITEM_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
namespace Ui {
    class AutoClicker_item;
}
//--------------------------------------------------------------------------------
class AutoClicker_item : public QWidget
{
    Q_OBJECT

signals:
    void remove_item(void);

public:
    explicit AutoClicker_item(QWidget *parent = nullptr);
    ~AutoClicker_item();

    void set_name(const QString &name);
    QString get_name(void);

    void set_id(int new_id);
    int  get_id(void);

    QTime get_time(void);

    void set_x(int x);
    int  get_x(void);

    void set_y(int y);
    int  get_y(void);

private:
    Ui::AutoClicker_item *ui;
    QString name;
    int id = 0;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // AUTOCLICKER_ITEM_HPP
