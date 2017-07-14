#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QtWidgets>

#include "bookmarksmanager.h"
#include "scapeui.h"

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

class SettingsPanel : public QFrame
{
    Q_OBJECT
    public:
        explicit SettingsPanel(QWidget *parent = 0);

    signals:

    public slots:
        QString title();
        void setTitle(QString title);
        void setMainLayout(QLayout *layout);

    private slots:

    private:
        QGridLayout *mainLayout;
        QLabel *lblTitle;
        QScrollArea *scrollWidget;

};

class TabList : public QFrame
{
    Q_OBJECT
    public:
        explicit TabList(QWidget *parent = 0);

    signals:
        void showTab(int index);

    public slots:
        void addTab(QString title,int index);
        void setCurrent(int index);
        QString nameAt(int index);

    private:
        QVBoxLayout *layout;
        QSignalMapper *map;
};

class SettingsWindow : public QFrame
{
    Q_OBJECT
    public:
        explicit SettingsWindow(QWidget *parent = 0);

    signals:
        void colorChanged();
        void canClose();
        void closeRequested();

    protected:
        void resizeEvent(QResizeEvent *event);

    public slots:
        void loadSettings();
        void setCurrentColor(QString backgroundColor);
        void setColor(QString color);
        void askSave();
        void goToTab(int tab);

        QColor getNewColor();
        QString getNewBgImage();  // Ne pas oublier : c'est pour que quand on change de fond, bein a partir de mainwindow on change le fond pour tester

    private slots:
        void genCustomise();
        void genNavigation();
        void genInterface();
        void genBookmarks();
        void genSecurity();
        void genOptions();
        void genReset();

        void chooseColor();
        void chooseBgImage();
        void selectAll();

        void indexChanged(int index);
        void saveSettings();
        void reset();

    private:
        QSettings *settings;
        QStackedWidget *mainLayout;
        QPushButton *btnClose;

        TabList *tabBar;
        BookmarksManager *bkmManager;

        QColor newColor;
        QColor oldColor;
        QString newBgImage;
        QString oldBgImage;

        SettingsPanel *tabCustomise;
        SettingsPanel *tabNavigation;
        SettingsPanel *tabInterface;
        SettingsPanel *tabSecurity;
        SettingsPanel *tabOptions;
        SettingsPanel *tabBookmarks;
        SettingsPanel *tabReset;

        // Customise
        QCheckBox *checkAnimation;
        QLineEdit *editBgColor;
        QToolButton *btnBgImageMain;
        QToolButton *btnBgImageLock;
        QToolButton *previewBgImageMain;
        QToolButton *previewBgImageLock;
        QLineEdit *editBgImageMain;
        QLineEdit *editBgImageLock;

        // Navigation
        QRadioButton *radioEmptyStartup;
        QRadioButton *radioHomeStartup;
        QRadioButton *radioSpecificStartup;
        QRadioButton *radioEmptyNewTab;
        QRadioButton *radioHomeNewTab;
        QRadioButton *radioSpecificNewTab;
        QLineEdit *editSpecificStartup;
        QLineEdit *editSpecificNewTab;
        QLineEdit *editHomePage;
        QLineEdit *editPrefixSearchUrl;
        QLineEdit *editSuffixSearchUrl;

        // Interface
        QCheckBox *checkSideBar;
        QCheckBox *checkToolBar;
        QCheckBox *checkTabBar;

        QCheckBox *checkSideBarAddTab;
        QCheckBox *checkSideBarTabs;
        QCheckBox *checkSideBarCmd;
        QCheckBox *checkSideBarBack;
        QCheckBox *checkSideBarForward;
        QCheckBox *checkSideBarReload;
        QCheckBox *checkSideBarHome;
        QCheckBox *checkSideBarInformations;
        QCheckBox *checkSideBarAddBookmark;
        QCheckBox *checkSideBarBookmarksMenu;
        QCheckBox *checkSideBarOpenFile;
        QCheckBox *checkSideBarSaveFile;
        QCheckBox *checkSideBarLock;
        QCheckBox *checkSideBarScreen;
        QCheckBox *checkSideBarFull;
        QCheckBox *checkSideBarConfig;

        QCheckBox *checkToolBarAddTab;
        QCheckBox *checkToolBarBack;
        QCheckBox *checkToolBarForward;
        QCheckBox *checkToolBarReload;
        QCheckBox *checkToolBarHome;
        QCheckBox *checkToolBarAddBookmark;
        QCheckBox *checkToolBarBookmarksMenu;
        QCheckBox *checkToolBarTabs;
        QCheckBox *checkToolBarCmd;
        QCheckBox *checkToolBarInformations;
        QCheckBox *checkToolBarOpenFile;
        QCheckBox *checkToolBarSaveFile;
        QCheckBox *checkToolBarLock;
        QCheckBox *checkToolBarScreen;
        QCheckBox *checkToolBarFull;
        QCheckBox *checkToolBarConfig;

        QToolButton *btnCheckAllToolBar;
        QToolButton *btnCheckAllSideBar;

        // Security
        QLineEdit *editUserName;
        QLineEdit *editPass1;
        QLineEdit *editPass2;
        QLineEdit *editIndice;
        QCheckBox *checkLockAtStartup;
        QCheckBox *checkUnlockToClose;
        QCheckBox *checkLockConfig;
        QCheckBox *checkLockLocker;

};

#endif // SETTINGSWINDOW_H
