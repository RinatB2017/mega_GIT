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
//--------------------------------------------------------------------------------
#ifndef MAINBOX_H
#define MAINBOX_H
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QQueue>
//--------------------------------------------------------------------------------
#define SIZE 100
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
union DECADES
{
  short value;
  struct
  {
    unsigned a: 4;
    unsigned b: 4;
    unsigned c: 4;
    unsigned d: 4;
  }
  decades;
};
//--------------------------------------------------------------------------------
class SerialBox5;
class GrapherBox;
class Plot;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent = nullptr);
    virtual ~MainBox();

private slots:
    void raw_data( QByteArray );

private:
    Ui::MainBox *ui;
    QWidget *parent;

    GrapherBox *grapher;
    SerialBox5 *serial;
    QByteArray dirty_data;

    void init();
    int getPositionStep();
    int getSizeStep();

    unsigned char convert_ansi_to_dec(char data);
    void drawing();
    void append(double value);

    void updateText();
    bool programm_is_exit();
    void load_setting();
    void save_setting();
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_H
