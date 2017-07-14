#ifndef SIDEPANEL_H
#define SIDEPANEL_H

#include <QtWidgets>

#include "progressindicator.h"

/*
    This file is part of Dooscape.

    Dooscape is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Dooscape is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Dooscape.  If not, see <http://www.gnu.org/licenses/>.
*/

class SidePanel : public QWidget
{
    Q_OBJECT
    public:
        explicit SidePanel(QWidget *centralWidget, QWidget *parent = 0);

        ProgressIndicator *progress();
    signals:

    public slots:
        void setColor(QString color);
        void addAction(QAction *action);
        void addMenu(QMenu *menu, QIcon icon);
        void setMainMenu(QMenu *menu, QIcon icon);
        void showMainMenu();
        void setToolBarVisible(bool visible);
        void clearSideBar();

    private slots:
        void applyStyle();

    private:
        ProgressIndicator *progressWidget;
        QToolBar *actionsBar;
        QToolBar *blackBar;
        QToolButton *btnMainMenu;
        QString colorName;
        QSettings *settings;
};

#endif // SIDEPANEL_H
