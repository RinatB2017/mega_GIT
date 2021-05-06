﻿/*********************************************************************************
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
#ifndef CREATORWINDOW_HPP
#define CREATORWINDOW_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif

#include <QtGlobal>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "dock_position.hpp"
#include "helpbrowser.hpp"
#include "aboutbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "syslog.hpp"
#include "logbox.hpp"

#include "mysettings.hpp"
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
#define P_ICON_HELP       ":/mainwindow/system-help.png"
#define P_ICON_STYLE      ":/mainwindow/style.png"
#define P_ICON_EXIT       ":/mainwindow/system-shutdown.png"
#define P_ICON_FONT       ":/mainwindow/kfontview.png"
#define P_ICON_LANG       ":/mainwindow/applications-education-language.png"

#define P_ICON_RU         ":/flags/ru.png"
#define P_ICON_US         ":/flags/us.png"
#define P_ICON_IT         ":/flags/it.png"
//--------------------------------------------------------------------------------
#define P_APP_ENG_TEXT           "app_eng_text"
#define P_DOCKWIDGET_ENG_TEXT    "dockwidget_eng_text"

#define P_IS_EXIT "is_exit"

#define P_EMAIL_STR       "tux4096@gmail.com"
#define P_AUTHOR_STR      "Author: Bikbao Rinat Zinorovich"
#define P_TELEGRAM_STR    "https://telegram.im/TrueProgrammer"
//#define P_TELEGRAM_STR    "https://t-do.ru/TrueProgrammer"

#define P_FONT_WEIGHT "FontWeight"
#define P_FONT_SIZE   "FontSize"
#define P_FONT_NAME   "FontName"

#define P_STYLE_NAME          "StyleName"
#define P_NO_ANSWER_FROM_EXIT "NoAnswerFromExit"
#define P_ALWAYS_ON_TOP       "AlwaysOnTop"

#define P_THEME   "Theme"

#define P_WINDOW_STATE "windowState"
#define P_GEOMETRY     "geometry"

#define P_MAIN    "Main"

#define P_FLAG_READ_ONLY          "flag_ReadOnly"
#define P_FLAG_ACCEPT_RICH_TEXT   "flag_AcceptRichText"
#define P_FLAG_NO_CRLF            "flag_NoCRLF"
#define P_FLAG_ADD_DATETIME       "flag_AddDateTime"
#define P_FLAG_COLOR              "flag_Color"
#define P_FLAG_ERROR_AS_MESSAGE   "flag_ErrorAsMessage"
#define P_FLAG_TEXT_IS_WINDOWS    "flag_TextIsWindows"
#define P_FLAG_AUTOSIZE           "flag_AutoSave"
#define P_FILE_AUTOSIZE           "file_AutoSave"

#define P_LANG  "Language"
#define P_US    "US"
#define P_IT    "IT"
#define P_RU    "RU"
//--------------------------------------------------------------------------------
typedef struct {
    QPointer<QTranslator> translator_obj;   // new QTranslator
    QString translator_file;                // :/lang/lang_ru.qm
    QString locale_name;                    // ru_RU
    QString language;                       // Russian
    QString property;                       // RU
    QString icon_name;                      // P_ICON_RU
} TRANSLATOR;
//--------------------------------------------------------------------------------
class CreatorWindow : public QMainWindow, public MySettings
{
    Q_OBJECT

public:
    explicit CreatorWindow(QMainWindow* parent = nullptr);
    virtual ~CreatorWindow();

    void setCentralWidget(MyWidget *widget);
    void setCentralWidget(QWidget *widget);
    void set_c_widget(MyWidget *widget);   //TODO проба

    void setWindowTitle(const QString &title);

    bool add_action(QMenu *menu,
                    int pos_y,
                    QAction *action);

    bool add_separator(QMenu *menu,
                       int pos_y);

    bool add_mdi_sorting(void);

    bool add_dock_widget(QString title,
                         QString objectname,
                         Qt::DockWidgetArea area,
                         QWidget *widget,
                         bool no_dock_position = false);

    void load_setting(void);
    void save_setting(void);

signals:
    void updateLanguage(void);

    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void clear_log(void);

    void colorLog(const QString &, const QColor, const QColor);

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

//private slots:
    void log(const QString &data);
    void set_app_font(void);
    void setStyles(void);
    void closeOnExit(bool state);
    void alwaysOnTop(bool state);
    void about(void);
    void help(void);

    void choice_translator(QAction *menu);

    void set_system_palette(void);
    void set_blue_palette(void);
    void set_dark_palette(void);
    void set_light_palette(void);

    void showHide(QSystemTrayIcon::ActivationReason r);
    //void iconActivated(QSystemTrayIcon::ActivationReason reason);

    void setToolBarLanguage(void);
    void setMenuLanguage(void);

    void change_value(void);

#ifndef NO_STYLETOOLBAR
#ifdef USE_CUSTOM_STYLE
    void set_norton_style(void);
#endif
#endif

#if defined (DEMO) && defined (DEMO_YEAR) && defined (DEMO_MONTH) && defined (DEMO_DAY)
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

//    bool add_windowsmenu_action(QWidget *widget, QAction *action);
    typedef void (CreatorWindow::*v_saveSlot)(void);
    typedef bool (CreatorWindow::*b_saveSlot)(void);
    bool add_new_action(QMenu   *parent,
                        QString text,
                        QIcon   *icon,
                        v_saveSlot slot);
    bool add_new_action(QMenu   *parent,
                        QString text,
                        QIcon   *icon,
                        b_saveSlot slot);
#ifdef USE_CUSTOM_STYLE
    void app_menu_add_custom_style(QMenu *menu);
#endif

    QString orgName;
    QString appName;
    QString appVersion;
    QString style_name;

    QPointer<QLabel> statusLabel1;
    QPointer<QLabel> statusLabel2;

    QPointer<QSystemTrayIcon> trayIcon;
    QPointer<QMenu> trayIconMenu;

//    QPointer<QMenu> m_app_windowsmenu;

    //TODO тест
    QPointer<MyWidget> c_widget;

    void load_main(void);
    void save_main(void);

    void init(void);

    void load_translations(void);

    void setToolBarStyles(void);

    void createStatusBar(void);
    void createTrayIcon(void);

    void set_tileSubWindows(void);
    void set_cascadeSubWindows(void);

#ifndef NO_STYLETOOLBAR
    QPointer<QToolBar> styletoolbar;
    void createStyleToolBar(void);
#ifdef USE_CUSTOM_STYLE
    void createCustomStyleToolBar(void);
#endif
#endif

#ifndef NO_LOG
    QPointer<LogBox> lb;
#endif
    QPointer<SysLog> w_syslog;

#ifndef NO_LOG
    void createLog(void);
#endif
    void createSysLog_dock(void);

    int state_theme = DARK_THEME;

#if defined (DEMO) && defined (DEMO_YEAR) && defined (DEMO_MONTH) && defined (DEMO_DAY)
    void check_date(void);
#endif

    QPointer<QMenuBar> app_mainBar;

    QList<QAction *> app_actions;
    QList<QAbstractButton *> app_buttons;

    void app_updateText(void);
    void dockwidget_updateText(void);

    // void show_docs(void);

    void dockLocationChanged(Qt::DockWidgetArea area);

    bool check_exit(void);

protected:
    bool flag_close = false;
    bool flag_always_on_top = false;

    QPointer<QTranslator> translator_system;
    QList<TRANSLATOR> l_translators;

    void changeEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);
#ifdef SHOW_SIZE
    void resizeEvent (QResizeEvent * event);
#endif
};
//--------------------------------------------------------------------------------
#endif //CREATORWINDOW_HPP
