/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#include <QCoreApplication>
//--------------------------------------------------------------------------------
#include "serialbox5.hpp"
#include "grapherbox.hpp"
#include "ui_adc_oscilloscope_new_mainbox.h"
#include "adc_oscilloscope_new_mainbox.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox),
    parent(parent)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init()
{
    //y[n] = double(308.0)*qSin(double(n)*double(M_PI)/double(180.0));
    ui->setupUi(this);

    serial = new SerialBox5(this, tr("RS-232"));
    Q_ASSERT(serial);
    connect(serial, SIGNAL(output(QByteArray)), this, SLOT(raw_data(QByteArray)));

    grapher = new GrapherBox(this);
    Q_ASSERT(grapher);
    grapher->set_title("ADC");
    grapher->set_axis_scale_x(0, SIZE);
    grapher->set_axis_scale_y(0, 1024);

    grapher->add_curve("signal");

    ui->hbox->addWidget(serial);
    ui->hbox->addWidget(grapher);

    updateText();

    setMinimumWidth( 1024 );
}
//--------------------------------------------------------------------------------
void MainBox::raw_data(QByteArray data)
{
    int len = data.length();
    if(len < 0) return;

    for(int n=0; n<len; n++)
    {
        switch(data.at(n))
        {
        case ':':
            dirty_data.clear();
            break;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            dirty_data.append(data.at(n));
            break;

        case 0x0A:
            if((dirty_data.length() % 4) == 0)
            {
                drawing();
            }
            dirty_data.clear();
            break;

        default:
            break;
        }
    }
}
//--------------------------------------------------------------------------------
unsigned char MainBox::convert_ansi_to_dec(char data)
{
    unsigned char temp = 0;
    switch(data)
    {
    case '0': temp = 0x00; break;
    case '1': temp = 0x01; break;
    case '2': temp = 0x02; break;
    case '3': temp = 0x03; break;
    case '4': temp = 0x04; break;
    case '5': temp = 0x05; break;
    case '6': temp = 0x06; break;
    case '7': temp = 0x07; break;
    case '8': temp = 0x08; break;
    case '9': temp = 0x09; break;
    case 'A': temp = 0x0A; break;
    case 'B': temp = 0x0B; break;
    case 'C': temp = 0x0C; break;
    case 'D': temp = 0x0D; break;
    case 'E': temp = 0x0E; break;
    case 'F': temp = 0x0F; break;
    default: break;
    }

    return temp;
}
//--------------------------------------------------------------------------------
void MainBox::drawing(void)
{
    for(int n=0; n<dirty_data.length(); n+=4)
    {
        union DECADES temp;

        temp.decades.d = convert_ansi_to_dec(dirty_data.at(n));
        temp.decades.c = convert_ansi_to_dec(dirty_data.at(n+1));
        temp.decades.b = convert_ansi_to_dec(dirty_data.at(n+2));
        temp.decades.a = convert_ansi_to_dec(dirty_data.at(n+3));

        QCoreApplication::processEvents();
        grapher->add_curve_data(0, temp.value);
    }
}
//--------------------------------------------------------------------------------
void MainBox::append(double value)
{
    QCoreApplication::processEvents();
    grapher->add_curve_data(0, value);
}
//--------------------------------------------------------------------------------
void MainBox::updateText()
{

}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
