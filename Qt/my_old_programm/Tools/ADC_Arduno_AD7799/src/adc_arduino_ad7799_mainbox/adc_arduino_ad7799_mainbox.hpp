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
#ifndef MAINBOX_H
#define MAINBOX_H
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QQueue>
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class SerialBox5;
class GrapherBox;
class Plot;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent = 0);
    ~MainBox();

private slots:
    void raw_data(QByteArray);
    void analize_packet(void);

private:
    Ui::MainBox *ui = 0;
    QWidget *parent = 0;

    GrapherBox *grapher = 0;
    int curve_ch0 = 0;
    int curve_ch1 = 0;
    int curve_ch2 = 0;

    SerialBox5 *serial = 0;
    QByteArray dirty_data;
    QByteArray clean_data;

    void init(void);

    unsigned char convert_ansi_to_dec(char data);
    QString convert_dec_to_ansi(unsigned char data);

    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_H
