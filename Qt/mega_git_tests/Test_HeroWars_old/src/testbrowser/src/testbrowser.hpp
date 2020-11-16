/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#ifndef TESTBROWSER
#define TESTBROWSER
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include <QWebEngineView>
//--------------------------------------------------------------------------------
// https://forum.qt.io/topic/54233/how-to-capture-mouse-events-in-webengineview

class TestBrowser : public QWebEngineView
{
    Q_OBJECT

signals:
    void add_pixmap(const QPixmap &);

public:
    TestBrowser(QWidget *parent = nullptr);

private:
    QPoint origin;
    QPointer<QRubberBand> rubberBand;

protected:
    bool eventFilter(QObject *object, QEvent *event);
};
//--------------------------------------------------------------------------------
#endif
