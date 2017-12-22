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
#ifndef BLINKER_H
#define BLINKER_H
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QList>
//--------------------------------------------------------------------------------
class QStackedWidget;
class QVBoxLayout;
//--------------------------------------------------------------------------------
class Tester : public QWidget
{
    Q_OBJECT
public:
    explicit Tester(QWidget *parent = 0);
    
protected:
    void keyPressEvent(QKeyEvent *event);

private:
    QVBoxLayout *vbox;
    int width, height;
    int state;

    QStackedWidget *tab;

    void draw_test(void);
    void add_grid(int step);
    void add_rect(QColor color);
    void add_circle(void);
    void add_text(void);
};
//--------------------------------------------------------------------------------
#endif
