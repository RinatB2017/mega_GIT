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
#include "myprogressdialog.hpp"
//--------------------------------------------------------------------------------
MyProgressDialog::MyProgressDialog(const QString title,
                                   const QString text,
                                   QWidget *parent) :
    QProgressDialog(parent)
{
    setWindowFlags(Qt::Dialog |
                   Qt::WindowTitleHint |
                   Qt::WindowSystemMenuHint |
                   Qt::WindowCloseButtonHint);

    QProgressBar *p_bar = new QProgressBar();
    p_bar->setMinimum(0);
    p_bar->setMaximum(0);

    QStyle *style = QStyleFactory::create("Fusion");
    if(style)
    {
        p_bar->setStyle(style);
    }

    setWindowTitle(title);
    setLabelText(text);
    setCancelButton(0);
    setCancelButtonText(0);
    setFixedSize(320, 60);
    setMinimum(0);
    setMaximum(0);
    setBar(p_bar);
}
//--------------------------------------------------------------------------------
