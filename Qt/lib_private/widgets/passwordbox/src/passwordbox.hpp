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
#ifndef PASSWORDBOX_HPP
#define PASSWORDBOX_HPP
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#define P_ID "id"
//--------------------------------------------------------------------------------
namespace Ui {
    class PasswordBox;
}
//--------------------------------------------------------------------------------
class PasswordBox : public MyWidget
{
    Q_OBJECT

signals:
    void pass_is_true(void);

public:
    explicit PasswordBox(QWidget *parent = nullptr);
    ~PasswordBox();

private:
    Ui::PasswordBox *ui;
    QList<QToolButton *> l_buttons;
    QString pass;
    QString true_pass = "1235";

    void init(void);
    void click(void);
    void bs(void);
    void run(void);
    void append_char(const QString &key);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};
//--------------------------------------------------------------------------------
#endif // PASSWORDBOX_HPP
