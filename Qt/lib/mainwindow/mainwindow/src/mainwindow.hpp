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
#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
//--------------------------------------------------------------------------------
#include <QSystemTrayIcon>
#include <QDesktopWidget>
#include <QStyleFactory>
#include <QResizeEvent>
#include <QApplication>
#include <QFontDialog>
#include <QTranslator>
#include <QToolButton>
#include <QPushButton>
#include <QMessageBox>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStatusBar>
#include <QSettings>
#include <QDateTime>
#include <QTextEdit>
#include <QToolBar>
#include <QMenuBar>
#include <QAction>
#include <QStyle>
#include <QTimer>
#include <QLabel>
#include <QMenu>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
    #include <syslog.h>
#endif
#if defined Q_OS_WIN || defined Q_OS_MAC
    #define LOG_EMERG       0       /* system is unusable */
    #define LOG_ALERT       1       /* action must be taken immediately */
    #define LOG_CRIT        2       /* critical conditions */
    #define LOG_ERR         3       /* error conditions */
    #define LOG_WARNING     4       /* warning conditions */
    #define LOG_NOTICE      5       /* normal but significant condition */
    #define LOG_INFO        6       /* informational */
    #define LOG_DEBUG       7       /* debug-level messages */
#endif
//--------------------------------------------------------------------------------
class LogBox;
class LogDock;
//--------------------------------------------------------------------------------
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QMainWindow* parent = 0);
    ~MainWindow();

    void setCentralWidget(QWidget *widget);
    MainWindow *getThis(void);

#ifndef NO_MENU
    QMenuBar *get_menubar(void);
    QMenu *get_file_menu(void);
    QMenu *get_help_menu(void);
    QMenu *get_options_menu(void);
    QMenu *get_windows_menu(void);
#endif

    bool add_menu(int pos_x, QMenu *menu);
    bool add_action(QMenu *menu,
                    int pos_y,
                    QAction *action);
    bool add_separator(QMenu *menu,
                       int pos_y);

signals:
    void updateLanguage(void);

    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void signal_is_shows_info(bool);
    void signal_is_shows_debug(bool);
    void signal_is_shows_error(bool);
    void signal_is_shows_trace(bool);

    void syslog(int level,
                QString src,
                QString message);

public slots:
#ifndef NO_STATUSBAR
    void set_status1_text(const QString &data);
    void set_status2_text(const QString &data);
#endif

    void set_focus(const QString &);
#ifndef NO_LOG
    void set_log_font(void);
#endif

private slots:
    void log(const QString &data);
    void set_app_font(void);
    void setStyles(void);
    void closeOnExit(void);
    void about(void);
    void setMenuLanguage(void);
    void setToolBarLanguage(void);
    void help(void);
    void updateText(void);

    void load_setting(void);
    void save_setting(void);

    void shot(void);

#ifndef NO_LOG
    void slot_is_shows_info(bool state);
    void slot_is_shows_debug(bool state);
    void slot_is_shows_error(bool state);
    void slot_is_shows_trace(bool state);
#endif

#ifndef NO_TRAYICON
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
#endif
#ifndef NO_STYLETOOLBAR
    void setToolBarStyles(void);
#endif

protected:
#ifndef ONLY_ENGLISH
    QTranslator *appTranslator = 0;
    QTranslator *sysTranslator = 0;
#endif
    QString orgName;
    QString appName;
    QString appVersion;
    QString style_name;

#ifndef NO_MENU
    QMenuBar *mainBar = 0;

    QMenu *m_fileMenu = 0;
    QMenu *m_optionsMenu = 0;
#ifndef NO_LOG
    QMenu *m_windowsMenu = 0;
#endif
    QMenu *m_helpMenu = 0;

    QAction *a_Exit = 0;
    QAction *a_Help = 0;
    QAction *a_About = 0;
    QAction *a_Font = 0;
#ifndef NO_LOG
    QAction *a_Log_Font = 0;
#endif
    QAction *a_AskExit = 0;

    QMenu *m_styleMenu = 0;

#ifndef ONLY_ENGLISH
    QMenu *m_langMenu = 0;
#endif
#ifndef ONLY_ENGLISH
    QAction *a_Rus = 0;
    QAction *a_Eng = 0;
#endif
#endif

#ifndef NO_STATUSBAR
    QLabel *statusLabel1 = 0;
    QLabel *statusLabel2 = 0;
#endif

    QWidget *mainWidget = 0;

#ifndef NO_TRAYICON
    QSystemTrayIcon *trayIcon = 0;
    QMenu *trayIconMenu = 0;
#endif

    QAction *a_minimizeAction = 0;
    QAction *a_maximizeAction = 0;
    QAction *a_restoreAction = 0;
    QAction *a_quitAction = 0;

    bool flag_close = false;

    void load_main(void);
    void save_main(void);

    void init(void);

    void load_translations(void);
#ifndef NO_MENU
    void createMenus(void);
#endif
#ifndef NO_STATUSBAR
    void createStatusBar(void);
#endif
#ifndef NO_TRAYICON
    void createTrayIcon(void);
#endif

#ifndef NO_TOOLBAR
    QToolButton *btnExit = 0;
    QToolButton *btnFont = 0;
#ifndef ONLY_ENGLISH
    QToolButton *btnRus = 0;
    QToolButton *btnEng = 0;
#endif
    QToolButton *btnStyle = 0;
    QToolButton *btnHelp = 0;
    QToolButton *btnAbout = 0;
    void createToolBar(void);
#endif

#ifndef NO_STYLETOOLBAR
    QToolBar *toolBar = 0;
    void createStyleToolBar(void);
#endif

#ifndef NO_LOG
    LogDock *ld = 0;
    void createLog(void);
#endif

#ifndef NO_LOG
    QAction *a_is_shows_info = 0;
    QAction *a_is_shows_debug = 0;
    QAction *a_is_shows_error = 0;
    QAction *a_is_shows_trace = 0;
#endif

#ifdef  DEMO
    void check_date(void);
#endif

    void closeEvent(QCloseEvent *event);
#ifndef NO_RESIZE
    void resizeEvent (QResizeEvent * event);
#endif
};
//--------------------------------------------------------------------------------
#endif //MAINWINDOW_HPP
