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
#include "testbrowser.hpp"
//--------------------------------------------------------------------------------
TestBrowser::TestBrowser(QWidget *parent) :
    QWebEngineView(parent)
{
    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    //        installEventFilter(this);
    QApplication::instance()->installEventFilter(this);
    setMouseTracking(true);
}
//--------------------------------------------------------------------------------
bool TestBrowser::eventFilter(QObject *object, QEvent *event)
{
    if (object->parent() == this && event->type() == QEvent::MouseButtonPress)
    {
        origin = static_cast<QMouseEvent *>(event)->pos();
        rubberBand->setGeometry(QRect(origin, QSize()));
        rubberBand->show();

        mousePressEvent(static_cast<QMouseEvent *>(event));
    }
    if (object->parent() == this && event->type() == QEvent::MouseButtonRelease)
    {
        QPixmap pixmap = grab(QRect(origin.x(),
                                    origin.y(),
                                    static_cast<QMouseEvent *>(event)->pos().x() - origin.x(),
                                    static_cast<QMouseEvent *>(event)->pos().y() - origin.y()));
        emit add_pixmap(pixmap);

        rubberBand->hide();
        mouseReleaseEvent(static_cast<QMouseEvent *>(event));
    }
    if (object->parent() == this && event->type() == QEvent::MouseMove)
    {
        rubberBand->setGeometry(QRect(origin, static_cast<QMouseEvent *>(event)->pos()).normalized());
        mouseMoveEvent(static_cast<QMouseEvent *>(event));
    }

    return false;
}
//--------------------------------------------------------------------------------
