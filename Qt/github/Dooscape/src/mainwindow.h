#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QtWebKitWidgets>

#ifdef Q_OS_WIN
#include <QtWinExtras>
#endif

#include "aboutsoftware.h"
#include "commandbar.h"
#include "lockscreen.h"
#include "panel.h"
#include "progressview.h"
#include "settingswindow.h"
#include "sidepanel.h"
#include "tabview.h"
#include "webinfo.h"
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

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();

    public slots:
        void loadFile(QString file);
        void loadConfig();
        void newTab();
        void showSettings();
        void showStartupPage(); // Startup for frist use
        void loadStartup(); // Startup while opening

        void setColor(QString colorName);
        void setFullScreen();

    protected:
        void resizeEvent(QResizeEvent *event);
        void closeEvent(QCloseEvent *event);

    private slots:
        void createPanels();
        void createActs();
        void createMenu();
        void createToolBar();
        void createJumpList();
        void createCentral();
        void setup();
        void updateColor();

        void openFile();
        void saveFile();
        void printPage();

        void closeTab();
        void closeTab(int index);

        void goBack();
        void goForward();
        void reload();
        void stop();
        void goHome();

        void goToUrl();
        void research();

        void zoomIn();
        void zoomOut();
        void resetZoom();

        void addBookmark();
        void goToBookmark(QString url);
        void manageBookmarks();

        void hideAllPanels();
        void showPanel(Panel *item, bool view);
        void showWebInfo();
        void showField();
        void showAboutSoft();
        void showTabView();
        void showProgress();
        
        void takeScreenShoot();
        void lockWindow();
        void inspectPage();

        void loadStarted();
        void loadProgress(int value);
        void loadFinished();

        void update();
        void updateTabs();
        void updateBookmarks();

        void tabChanged();
        void iconChanged();
        void urlChanged(QUrl url);
        void titleChanged(QString title);

        WebView *currentView();
        WebWidget *currentWidget();

    private:
        QFrame *central;
        QLineEdit *spotLight;
        QMenu *mainMenu;
        QPushButton *btnQuitArea;
        QSettings *settings;
        QSignalMapper *tabsMapper;
        QSignalMapper *bookmarksMapper;
        QStackedWidget *widgetManager;
        QString color;
        QTabWidget *tabs;
        QToolBar *mainToolBar;

        AboutSoftware *aboutSoft;
        CommandBar *field;
        ProgressView *progress;
        SidePanel *ano;
        SettingsWindow *config;
        TabView *tabBar;
        WebInfo *infoWidget;
        LockScreen *lockWidget;

        QMenu *menuFile;
        QMenu *menuTabs;
       // QMenu *menuClosedTabs;
        QMenu *menuBookmarks;

        QAction *actBack;
        QAction *actForward;
        QAction *actReload;
        QAction *actStop;
        QAction *actHome;

        QAction *actNewTab;
        QAction *actCloseTab;
        QAction *actViewTabs;

        QAction *actOpenFile;
        QAction *actSaveFile;
        QAction *actPrint;

        QAction *actViewFullScreen;
        QAction *actViewWebInfo;
        QAction *actViewField;
        QAction *actViewSettings;

        QAction *actZoomIn;
        QAction *actZoomOut;
        QAction *actResetZoom;

        QAction *actAddBookmark;
        QAction *actViewBookmarks;
        QAction *actManageBookmarks;
        
        QAction *actScreenShoot;
        QAction *actLock;

        QAction *actAboutSoft;

        QAction *actQuit;
};

#endif // MAINWINDOW_H
