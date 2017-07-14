/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#ifndef MYCOMBOBOX_HPP
#define MYCOMBOBOX_HPP
//--------------------------------------------------------------------------------
#include <QDebug>
//--------------------------------------------------------------------------------
#include <QAbstractItemView>
#include <QComboBox>
//--------------------------------------------------------------------------------
class MyComboBox : public QComboBox
{
public:
    explicit MyComboBox(QWidget *parent = 0) : QComboBox(parent)
    {
        setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
    }

    void showPopup(void)
    {
        this->view()->setMinimumWidth(this->view()->sizeHintForColumn(0));
        QComboBox::showPopup();
    }
};
//--------------------------------------------------------------------------------
#endif
