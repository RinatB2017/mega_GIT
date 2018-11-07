/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#include <QtWidgets>
//--------------------------------------------------------------------------------
#include "rgb_dislpay_led.hpp"
#include "rgb_dislpay.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
RGB_dislpay::RGB_dislpay(QWidget *parent) :
    QWidget(parent)
{
    double pixelPerMm = QApplication::screens().at(0)->logicalDotsPerInch()/2.54/10;
    double w_led = pixelPerMm * 3.5;    // Ширина 3.5 mm
    double h_led = pixelPerMm * 3.5;    // Высота 3.5 mm

    grid = new QGridLayout();

    for(int row=0; row<SCREEN_HEIGTH; row++)
    {
        for(int col=0; col<SCREEN_WIDTH; col++)
        {
            RGB_dislpay_led *led = new RGB_dislpay_led(w_led, h_led, this);
            connect(led,    SIGNAL(info(QString)),  this,   SIGNAL(info(QString)));
            connect(led,    SIGNAL(debug(QString)), this,   SIGNAL(debug(QString)));
            connect(led,    SIGNAL(error(QString)), this,   SIGNAL(error(QString)));
            connect(led,    SIGNAL(trace(QString)), this,   SIGNAL(trace(QString)));

            led->setProperty("property_col", col);
            led->setProperty("property_row", row);

            grid->addWidget(led, row, col);

            l_buttons.append(led);
        }
    }

    QWidget *w = new QWidget(this);
    w->setLayout(grid);

    layout()->addWidget(w);

    //adjustSize();
    //setFixedSize(width(), height());
}
//--------------------------------------------------------------------------------
RGB_dislpay::~RGB_dislpay()
{

}
//--------------------------------------------------------------------------------
