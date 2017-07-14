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

#ifndef QPOWER_H
#define QPOWER_H

#include <QtGui>
#include <QtDesigner/QDesignerExportWidget>


class QDESIGNER_WIDGET_EXPORT QPower : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(bool state READ state WRITE setState)

public:

    QPower(QWidget *parent = 0);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    QColor color() const
    {
        return pow_color;
    }

    bool state() const
    {
        return pow_state;
    }

signals:
    void toggled(bool);

public slots:

    void setColor(QColor);
    void setState(bool);


protected:

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void paintBorder();
    void paintPowerOn();
    void paintPowerOff();


private:

    QColor pow_color;
    bool pow_state;
};

#endif
