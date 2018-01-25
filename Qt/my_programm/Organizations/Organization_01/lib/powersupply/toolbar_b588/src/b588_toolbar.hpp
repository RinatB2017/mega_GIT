/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#ifndef B588_TOOLBAR_HPP
#define B588_TOOLBAR_HPP
//--------------------------------------------------------------------------------
#include <QPushButton>
#include <QComboBox>
#include <QToolBar>
//--------------------------------------------------------------------------------
class B588_toolbar : public QToolBar
{
    Q_OBJECT

public:
    B588_toolbar(const QString &title, QWidget *parent = 0);
    B588_toolbar(QWidget *parent = 0);
    ~B588_toolbar();

signals:
    void press(int index);

private slots:
    void currentIndexChanged(int index);
    void run(void);

private:
    QComboBox *c1 = 0;
    QComboBox *c2 = 0;
    QPushButton *btn_run = 0;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // B588_TOOLBAR_HPP
