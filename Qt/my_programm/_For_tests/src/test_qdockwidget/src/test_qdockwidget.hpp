/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#ifndef TEST_QDOCKWIDGET_HPP
#define TEST_QDOCKWIDGET_HPP
//--------------------------------------------------------------------------------
#include <QDockWidget>
//--------------------------------------------------------------------------------
class QToolButton;
class QGridLayout;
//--------------------------------------------------------------------------------
class Test_QDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    Test_QDockWidget(const QString &title,
                QWidget *parent = 0);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

private slots:
    void allowedAreasChanged ( Qt::DockWidgetAreas allowedAreas );
    void dockLocationChanged ( Qt::DockWidgetArea area );
    void featuresChanged ( QDockWidget::DockWidgetFeatures features );
    void topLevelChanged ( bool topLevel );
    void visibilityChanged ( bool visible );

private:
    QList<QToolButton *> widgets;
    QGridLayout *grid_map = 0;

    void create_left_dock(void);
    void create_top_dock(void);
};
//--------------------------------------------------------------------------------
#endif // TEST_QDOCKWIDGET_HPP
