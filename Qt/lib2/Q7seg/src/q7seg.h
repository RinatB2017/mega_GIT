/***************************************************************************
 *   Copyright (C) 2008 - Giuseppe Cigala                                  *
 *   g_cigala@virgilio.it                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef Q7SEG_H
#define Q7SEG_H

#include <QtGui>
#include <QtDesigner/QDesignerExportWidget>


class QDESIGNER_WIDGET_EXPORT Q7seg : public QWidget
{
    Q_OBJECT
    Q_ENUMS(NumColor)
    Q_PROPERTY(int num READ num WRITE setNumber)
    Q_PROPERTY(NumColor color READ color WRITE setColor)

public:

    Q7seg(QWidget *parent = 0);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    QColor pen() const;
    QColor brush() const;
    enum NumColor
    {
        Red=1, Green=2, Yellow=3
    };
    int num() const
    {
        return value;
    }
    NumColor color() const
    {
        return ncolor;
    }

public slots:

    void setColor(NumColor);
    void setNumber(int);

protected:

    void paintEvent(QPaintEvent *);
    void paintBorder();
    void paintSegment();

private:

    QColor brushColorArr [8];
    QColor penColorArr [8];
    QColor brushColor;
    QColor penColor;
    QColor brushColorOff;
    QColor penColorOff;
    int penWidth;
    int value;
    NumColor ncolor;
};

#endif
