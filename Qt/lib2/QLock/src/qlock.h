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

#ifndef QLOCK_H
#define QLOCK_H

#include <QtGui>
#include <QCryptographicHash>
#include <QtDesigner/QDesignerExportWidget>


class QDESIGNER_WIDGET_EXPORT QLock : public QWidget
{
    Q_OBJECT

public:
    QLock(QWidget *parent = nullptr);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

signals:
	void isLocked(bool);

public slots:

protected:
    void paintEvent(QPaintEvent *);
    void wheelEvent(QWheelEvent *);
    void mousePressEvent(QMouseEvent *);
    void paintBorder();
    void paintWheels();
    void paintNumbers();
    void paintState();

private:
	int val1;
	int val2;
	int val3;
	int val4;
	int val5;
	int delta;
	QByteArray combination;
	QColor colLock;
	QColor colUnlock;
};

#endif
