/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#ifndef MENUDOCK_HPP
#define MENUDOCK_HPP
//--------------------------------------------------------------------------------
#include <QDockWidget>
//--------------------------------------------------------------------------------
class QTreeWidgetItem;
class QTreeWidget;
//--------------------------------------------------------------------------------
class MenuDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit MenuDock(QWidget *parent = 0);
    ~MenuDock();

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void syslog(int level,
                QString src,
                QString message);

private slots:
    void itemClicked(QTreeWidgetItem *item, int index);

private:
    QTreeWidget *tree;
};
//--------------------------------------------------------------------------------
#endif // MENUDOCK_HPP
