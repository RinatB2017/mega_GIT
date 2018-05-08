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
#include <QStyleOptionProgressBarV2>
#include <QDesktopWidget>
#include <QSplashScreen>
#include <QApplication>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QLabel>
#include <QRect>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
//--------------------------------------------------------------------------------
MySplashScreen::MySplashScreen(const QPixmap &pixmap,
                               int max_progress,
                               QWidget *parent) :
    QSplashScreen(parent),
    current_progress(0),
    max_progress(max_progress)
{
    progress = new QProgressBar(this);
    progress->setValue(0);
    progress->setMaximum(max_progress);
    progress->setEnabled(true);

    QVBoxLayout *vbox = new QVBoxLayout;

    QLabel *label = new QLabel(this);
    label->setPixmap(pixmap);

    vbox->addWidget(label);
    vbox->addWidget(progress);

    setLayout(vbox);
    setCursor(Qt::BusyCursor);

    adjustSize();
    QRect available_geom = QDesktopWidget().availableGeometry();

    int app_left = available_geom.width() / 2 - width() / 2;
    int app_top  = available_geom.height() / 2 - height() / 2;

    setGeometry(app_left, app_top, width(), height());
    move(app_left, app_top);

    //label->installEventFilter(this);
}
//--------------------------------------------------------------------------------
void MySplashScreen::showMessage(const QString &message,
                                 int alignment,
                                 const QColor &color)
{
    QSplashScreen::showMessage(message, alignment, color);
    progress->setValue(current_progress);
    current_progress++;
    if(current_progress > max_progress)
    {
        current_progress=0;
    }
    repaint();
}
//--------------------------------------------------------------------------------
void MySplashScreen::set_progress(int value)
{
    current_progress = value;
    progress->setValue(current_progress);
    //repaint();
}
//--------------------------------------------------------------------------------
bool MySplashScreen::eventFilter(QObject *target, QEvent *event)
{
#ifdef QT_DEBUG
    qDebug() << "event!!!!";
#endif
    Q_UNUSED(target)
    Q_UNUSED(event)
    return true;
}
//--------------------------------------------------------------------------------
