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
#ifndef MYSPLASHSCREEN_HPP
#define MYSPLASHSCREEN_HPP
//--------------------------------------------------------------------------------
#include <QSplashScreen>
#include <QPixmap>
//--------------------------------------------------------------------------------
class QProgressBar;
//--------------------------------------------------------------------------------
class MySplashScreen : public QSplashScreen
{
    Q_OBJECT
public:
    MySplashScreen(const QPixmap &pixmap,
                   int max_progress = 100,
                   QWidget *parent = 0);

    void set_progress(int value);

public slots:
    void showMessage(const QString &message,
                     int alignment = Qt::AlignLeft,
                     const QColor &color = Qt::black);

private:
    QProgressBar *progress = 0;
    int current_progress = 0;
    int max_progress = 0;
    
protected:
    bool eventFilter(QObject *target, QEvent *event);
};
//--------------------------------------------------------------------------------
#endif // MYSPLASHSCREEN_HPP
