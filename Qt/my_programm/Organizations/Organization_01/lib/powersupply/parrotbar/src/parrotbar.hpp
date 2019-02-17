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
#ifndef PARROTBAR_H
#define PARROTBAR_H
//--------------------------------------------------------------------------------
#include <QToolBar>
//--------------------------------------------------------------------------------
class QToolButton;
class QSpinBox;
//--------------------------------------------------------------------------------
class ParrotBar : public QToolBar
{
    Q_OBJECT
public:
    explicit ParrotBar(QWidget *parent = nullptr);

signals:
    void set_UI(int U, int I);

private slots:
    void set(void);

private:
    QSpinBox *parrot_U;
    QSpinBox *parrot_I;
    QToolButton *btn;
};
//--------------------------------------------------------------------------------
#endif
