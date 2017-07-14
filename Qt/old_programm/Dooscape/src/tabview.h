#ifndef TABVIEW_H
#define TABVIEW_H

#include "panel.h"
#include "webview.h"

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

class TabView : public Panel
{
    Q_OBJECT
    public:
        explicit TabView(QWidget *parent = 0);

    signals:
        void addTab();
        void closeCurrentTab();
        void setCurrentTab(int index);
        void tabCloseRequested(int index);
        void viewModeChanged();

    public slots:
        void setTabWidget(QTabWidget *tabs);
        void changeView();

    private:
        QVBoxLayout *layout;
        QSignalMapper *viewSignal;
        QSignalMapper *closeSignal;

        QAction *actViewNormal;
        QAction *actViewCompact;

        bool compact;

};

class TabItem : public QFrame
{
    Q_OBJECT
    public:
        explicit TabItem(QWidget *parent = 0);

    signals:
        void closeRequested();
        void clicked();

    protected:
        void resizeEvent(QResizeEvent *event);

    public slots:
        void setActive(bool active);
        void setTitle(QString title);
        void setIcon(QIcon icon);
        void setCompactMode(bool compact);

    private:
        QPushButton *btnIcon;
        QToolButton *btnTitle;
        QPushButton *btnClose;

};

#endif // TABVIEW_H
