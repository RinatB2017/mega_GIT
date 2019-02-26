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
#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   include <syslog.h>
#endif
#if defined Q_OS_WIN || defined Q_OS_MAC
#   define LOG_EMERG       0       /* system is unusable */
#   define LOG_ALERT       1       /* action must be taken immediately */
#   define LOG_CRIT        2       /* critical conditions */
#   define LOG_ERR         3       /* error conditions */
#   define LOG_WARNING     4       /* warning conditions */
#   define LOG_NOTICE      5       /* normal but significant condition */
#   define LOG_INFO        6       /* informational */
#   define LOG_DEBUG       7       /* debug-level messages */
#endif
//--------------------------------------------------------------------------------
#define ICON_HELP       ":/mainwindow/system-help.png"
#define ICON_RU         ":/mainwindow/ru.png"
#define ICON_US         ":/mainwindow/us.png"
#define ICON_STYLE      ":/mainwindow/style.png"
#define ICON_EXIT       ":/mainwindow/system-shutdown.png"
#define ICON_FONT       ":/mainwindow/kfontview.png"
#define ICON_LANG       ":/mainwindow/applications-education-language.png"
//--------------------------------------------------------------------------------
#define APP_PROPERTY_ENG_TEXT   "app_property_eng_text"
//--------------------------------------------------------------------------------
class LogBox;
class LogDock;
class SysLog_dock;
//--------------------------------------------------------------------------------
class ToolButtonAction : public QWidgetAction {
public:
    ToolButtonAction (QAction *action) :
      QWidgetAction (nullptr) {
        QWidget* pWidget = new QWidget (nullptr);

        QHBoxLayout* pLayout = new QHBoxLayout();
        //pLayout->setMargin(0);
        pLayout->setSpacing(0);

        QLabel* pLabel = new QLabel(action->text());  //bug fixed here, pointer was missing
        pLayout->addWidget(pLabel);

        pToolButton = new QToolButton(nullptr);
        pToolButton->setText("...");

        pLayout->addWidget(pToolButton);

        pWidget->setLayout(pLayout);

        setDefaultWidget(pWidget);
    }

    QToolButton *toolButton () {
        return pToolButton;
    }

private:
    QToolButton *pToolButton;
};
//--------------------------------------------------------------------------------
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QMainWindow* parent = nullptr);
    ~MainWindow();

    void setCentralWidget(QWidget *widget);
    MainWindow *getThis(void);

    void setWindowTitle(const QString &title);

    bool add_menu(int pos_x, QMenu *menu);

    bool add_filemenu_menu(int pos_y,
                           QMenu *menu);
    bool add_optionsmenu_menu(int pos_y,
                           QMenu *menu);
    bool add_helpmenu_menu(int pos_y,
                           QMenu *menu);

    bool add_action(QMenu *menu,
                    int pos_y,
                    QAction *action);
    bool add_filemenu_action(int pos_y,
                             QAction *action);
    bool add_optionsmenu_action(int pos_y,
                                QAction *action);
    bool add_windowsmenu_action(QWidget *widget, QAction *action);
    bool add_helpmenu_action(int pos_y,
                             QAction *action);

    bool add_separator(QMenu *menu,
                       int pos_y);

    bool add_filemenu_separator(int pos_y);
    bool add_optionsmenu_separator(int pos_y);
    bool add_helpmenu_separator(int pos_y);

    bool add_dock_widget(QString title,
                         QString objectname,
                         Qt::DockWidgetArea area,
                         QWidget *widget);

    void load_setting(void);
    void save_setting(void);

signals:
    void updateLanguage(void);

    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void colorLog(const QString &, const QColor, const QColor);

    void signal_is_shows_info(bool);
    void signal_is_shows_debug(bool);
    void signal_is_shows_error(bool);
    void signal_is_shows_trace(bool);

    void syslog(QDateTime dt,
                int level,
                int src,
                QString message);
    void syslog(int level,
                int src,
                QString message);

public slots:
    void set_status1_text(const QString &data);
    void set_status2_text(const QString &data);

    void set_focus(const QString &);
#ifndef NO_LOG
    void set_log_font(void);
#endif

    void showMinimized(void);
    void showMaximized(void);
    void showNormal(void);
    void quit(void);

private slots:
    void log(const QString &data);
    void set_app_font(void);
    void setStyles(void);
    void closeOnExit(bool state);
    void alwaysOnTop(bool state);
    void about(void);
    void setMenuLanguage(void);
    void setToolBarLanguage(void);
    void help(void);

#ifndef NO_LOG_INFO
    void slot_is_shows_info(bool state);
#endif
#ifndef NO_LOG_DEBUG
    void slot_is_shows_debug(bool state);
#endif
#ifndef NO_LOG_ERROR
    void slot_is_shows_error(bool state);
#endif
#ifndef NO_LOG_TRACE
    void slot_is_shows_trace(bool state);
#endif

    void set_system_palette(void);
    void set_blue_palette(void);
    void set_dark_palette(void);
    void set_light_palette(void);

    void iconActivated(QSystemTrayIcon::ActivationReason reason);

    void setToolBarStyles(void);

    void change_value(void);

#ifdef  DEMO
    void kill(void);
    void kill2(void);
#endif

private:
    enum {
        SYSTEM_THEME = 0,
        LIGHT_THEME,
        DARK_THEME,
        BLUE_THEME
    };

    QMenu* add_new_menu(QMenu   *parent,
                        QString text,
                        QIcon   *icon);
    typedef void (MainWindow::*v_saveSlot)(void);
    typedef bool (MainWindow::*b_saveSlot)(void);
    bool add_new_action(QMenu   *parent,
                        QString text,
                        QIcon   *icon,
                        v_saveSlot slot);
    bool add_new_action(QMenu   *parent,
                        QString text,
                        QIcon   *icon,
                        b_saveSlot slot);

    QTranslator *translator_system;
    QTranslator *translator_common;
    QTranslator *translator_programm;

    QString orgName;
    QString appName;
    QString appVersion;
    QString style_name;

    QLabel *statusLabel1;
    QLabel *statusLabel2;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QSettings *settings;

    void load_main(void);
    void save_main(void);

    void init(void);

    void load_translations(void);

    void createMenus(void);

    void createStatusBar(void);
    void createTrayIcon(void);

#ifndef NO_TOOLBAR
    QToolBar *toolbar;
    void createToolBar(void);
#endif

#ifndef NO_STYLETOOLBAR
    QToolBar *styletoolbar;
    void createStyleToolBar(void);
#endif

    QDockWidget *sd;

#ifndef NO_LOG
    LogDock *ld;
#endif
    SysLog_dock *syslog_dock;

#ifndef NO_LOG
    void createLog(void);
#endif
    void createSysLog_dock(void);

    int state_theme = DARK_THEME;

    void check_date(void);

    QMenuBar *app_mainBar;

    QList<QMenu   *> app_menus;
    QList<QAction *> app_actions;
    QList<QAbstractButton *> app_buttons;

    QMenu *m_app_filemenu;
    QMenu *m_app_optionsmenu;
    QMenu *m_app_windowsmenu;
    QMenu *m_app_helpmenu;

    QList<QWidget *> l_docs;

    void app_menu_add_separator(QMenu *menu);
    void app_menu_add_exit(QMenu *menu);
    void app_menu_add_font_programm(QMenu *menu);
    void app_menu_add_font_log(QMenu *menu);
    void app_menu_add_log_filter(QMenu *menu);
    void app_menu_add_theme(QMenu *menu);
    void app_menu_add_lang(QMenu *menu);
    void app_menu_add_style(QMenu *menu);
    void app_menu_add_confirm_exit(QMenu *menu);
    void app_menu_add_show_on_top(QMenu *menu);
    void app_menu_add_about(QMenu *menu);
    void app_menu_add_help(QMenu *menu);

    void app_toolbar_add_separator(void);
    void app_toolbar_add_exit(void);
    void app_toolbar_add_font(void);
    void app_toolbar_add_lang(void);
    void app_toolbar_add_style(void);
    void app_toolbar_add_about(void);
    void app_toolbar_add_help(void);

    void app_updateText(void);

    void show_docs(void);

protected:
    bool flag_close = false;
    bool flag_always_on_top = false;
#ifndef NO_LOG_INFO
    bool flag_show_info = false;
#endif
#ifndef NO_LOG_DEBUG
    bool flag_show_debug = false;
#endif
#ifndef NO_LOG_ERROR
    bool flag_show_error = false;
#endif
#ifndef NO_LOG_TRACE
    bool flag_show_trace = false;
#endif

    void changeEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);
#ifndef NO_RESIZE
    void resizeEvent (QResizeEvent * event);
#endif
};
//--------------------------------------------------------------------------------
#endif //MAINWINDOW_HPP
