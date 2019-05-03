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
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QKeyEvent>
#include <QPicture>
#include <QPainter>
#include <QLabel>
#include <QList>
#include <QPen>
//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "tester.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
Tester::Tester(QWidget *parent) :
    QWidget(parent),
    state(0)
{
    vbox = new QVBoxLayout();
    tab = new QStackedWidget(this);

    vbox->setMargin(0);
    vbox->addWidget(tab);
    setLayout(vbox);

    QString str;
    str.append("Author: Bikbao Rinat Zinorovich\n"
               "Copyright (C) 2012\n\n"
               "This program is distributed in the hope that it will be useful,\n"
               "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
               "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
               "GNU General Public License for more details.\n\n"
               "   0 - text\n"
               "   1 - circle\n"
               "   2 - grid(8)\n"
               "   3 - grid(16)\n"
               "   4 - grid(32)\n"
               "   5 - grid(64)\n"
               "   6 - rect(red)\n"
               "   7 - rect(green)\n"
               "   8 - rect(blue)\n"
               "   9 - rect(white)\n"
               "   a - rect(black)\n"
               "   ESC - exit");

    QMessageBox::information(this, tr("info"), str, QMessageBox::Ok);

    add_text();             // 0
    add_circle();           // 1
    add_grid(8);            // 2
    add_grid(16);           // 3
    add_grid(32);           // 4
    add_grid(64);           // 5
    add_rect(Qt::red);      // 6
    add_rect(Qt::green);    // 7
    add_rect(Qt::blue);     // 8
    add_rect(Qt::white);    // 9
    add_rect(Qt::black);    // a
}
//--------------------------------------------------------------------------------
void Tester::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_0: state = 0;  draw_test(); break;
    case Qt::Key_1: state = 1;  draw_test(); break;
    case Qt::Key_2: state = 2;  draw_test(); break;
    case Qt::Key_3: state = 3;  draw_test(); break;
    case Qt::Key_4: state = 4;  draw_test(); break;
    case Qt::Key_5: state = 5;  draw_test(); break;
    case Qt::Key_6: state = 6;  draw_test(); break;
    case Qt::Key_7: state = 7;  draw_test(); break;
    case Qt::Key_8: state = 8;  draw_test(); break;
    case Qt::Key_9: state = 9;  draw_test(); break;
    case Qt::Key_A: state = 10; draw_test(); break;

    case Qt::Key_Space:
        draw_test();
        break;

    case Qt::Key_Escape:
        close();
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void Tester::add_text(void)
{
    QPainter painter;
    QPicture picture;
    QLabel *label;
    int y;

    QPen white_pen(Qt::white);

    painter.begin(&picture);
    painter.fillRect(0, 0, MAX_X, MAX_Y, Qt::black);
    painter.setPen(white_pen);
    y=24;
    QList<int> sizes;
    sizes << 6
          << 7
          << 8
          << 9
          << 10
          << 11
          << 12
          << 14
          << 16
          << 18
          << 20
          << 22
          << 24;
    for(int i=0; i<sizes.count(); i++)
    {
        painter.setFont(QFont("Times", sizes.at(i)));
        painter.drawText(10, y, painter.font().toString());
        y+=36;
    }
    painter.end();
    label = new QLabel;
    label->setPicture(picture);

    tab->addWidget(label);
}
//--------------------------------------------------------------------------------
void Tester::add_rect(QColor color)
{
    QPainter painter;
    QPicture picture;
    QLabel *label;

    painter.begin(&picture);
    painter.fillRect(0, 0, MAX_X, MAX_Y, color);
    painter.end();
    label = new QLabel;
    label->setPicture(picture);

    tab->addWidget(label);
}
//--------------------------------------------------------------------------------
void Tester::add_grid(int step)
{
    int x, y;
    QBrush black_brush(Qt::black);

    QPen red_pen(Qt::red);
    QPen white_pen(Qt::white);

    QPainter painter;
    QPicture picture;
    QLabel *label;

    painter.begin(&picture);
    painter.setPen(red_pen);
    painter.setBrush(black_brush);
    painter.drawRect(0, 0, MAX_X-1, MAX_Y-1);
    painter.setPen(white_pen);
    for(x=step; x<MAX_X; x+=step)
        painter.drawLine(x, 0, x, MAX_Y);
    for(y=step; y<MAX_Y; y+=step)
        painter.drawLine(0, y, MAX_X, y);
    painter.end();
    label = new QLabel;
    label->setPicture(picture);

    tab->addWidget(label);
}
//--------------------------------------------------------------------------------
void Tester::add_circle(void)
{
    int radius;
    QBrush black_brush(Qt::black);

    QPen white_pen(Qt::white);

    QPainter painter;
    QPicture picture;
    QLabel *label;

    painter.begin(&picture);
    painter.setPen(white_pen);
    painter.setBrush(black_brush);
    painter.fillRect(0, 0, MAX_X, MAX_Y, Qt::black);
    for(radius=MAX_Y/2; radius>0; radius-=16)
        painter.drawEllipse(QPoint(MAX_X/2, MAX_Y/2), radius, radius);
    painter.end();
    label = new QLabel;
    label->setPicture(picture);

    tab->addWidget(label);
}
//--------------------------------------------------------------------------------
void Tester::draw_test(void)
{
    tab->setCurrentIndex(state);
    if(state < (tab->count() - 1)) state++;
    else state = 0;
}
//--------------------------------------------------------------------------------
