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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "helpbrowser.hpp"
#include "mainwindow.hpp"
#include "aboutbox.hpp"
#include "defines.hpp"
#include "version.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "syslog_dock.hpp"
#include "logdock.hpp"
//--------------------------------------------------------------------------------
MainWindow::MainWindow(QMainWindow *parent)
    : QMainWindow(parent),
      orgName(ORGNAME),
      appName(APPNAME),
      appVersion(QString("%1.%2.%3 (%4)").arg(VER_MAJOR).arg(VER_MINOR).arg(VER_PATCH).arg(VER_BUILD))
{
    init();
}
//--------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
#ifdef QT_DEBUG
    qDebug() << "~MainWindow()";
#endif
    save_setting();

#ifndef NO_LOG_INFO
    MyWidget::set_param("Main", "flag_show_info",   flag_show_info);
#endif
#ifndef NO_LOG_ERROR
    MyWidget::set_param("Main", "flag_show_error",  flag_show_error);
#endif
#ifndef NO_LOG_DEBUG
    MyWidget::set_param("Main", "flag_show_debug",  flag_show_debug);
#endif
#ifndef NO_LOG_TRACE
    MyWidget::set_param("Main", "flag_show_trace",  flag_show_trace);
#endif
    if(ld)
    {
        ld->deleteLater();
    }

    if(settings)
    {
        settings->deleteLater();
    }
}
//--------------------------------------------------------------------------------
void MainWindow::setCentralWidget(QWidget *widget)
{
    Q_CHECK_PTR(widget);
    QMainWindow::setCentralWidget(widget);

    load_setting();

#ifdef FIXED_SIZE
    setFixedSize(sizeHint());
#endif
}
//--------------------------------------------------------------------------------
void MainWindow::changeEvent(QEvent *event)
{
    if(event == nullptr)
    {
        return;
    }

    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        app_updateText();
        if(toolbar)
        {
            toolbar->setWindowTitle(tr("toolbar"));
        }
        if(styletoolbar)
        {
            styletoolbar->setWindowTitle(tr("styletoolbar"));
        }
        if(ld)
        {
            ld->setWindowTitle(tr("log"));
        }
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox;

    //---
    MyWidget *cw = (MyWidget *)centralWidget();
    if(cw)
    {
#if 0
        //TODO как нибудь потом
        if(cw->no_exit())
        {
            event->ignore();
            return;
        }
#endif
    }
    //---

    if(flag_close)
    {
        save_setting();
        event->accept();
        return;
    }

    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(tr("Exit the program"));
    msgBox.setText(tr("Exit the program?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int btn = msgBox.exec();

    if(btn == QMessageBox::Yes)
    {
        save_setting();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}
//--------------------------------------------------------------------------------
#ifndef NO_RESIZE
void MainWindow::resizeEvent(QResizeEvent *event)
{
#ifndef NO_STATUSBAR
    statusLabel1->setText(QString(QLatin1String("%1 %2")).arg(event->size().width()).arg(event->size().height()));
#else
    Q_UNUSED(event)
#endif
}
#endif
//--------------------------------------------------------------------------------
void MainWindow::init(void)
{
    flag_close = true;

#ifndef SAVE_INI
    settings = new QSettings(ORGNAME, APPNAME);
#else
    settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
#endif

    load_translations();
    setWindowTitle(QString("%1 (ver. %2)")
                   .arg(appName)
                   .arg(appVersion));

#ifndef NO_LOG
    createLog();
#else
    connect(this, SIGNAL(info(QString)),    this, SLOT(log(QString)));
    connect(this, SIGNAL(debug(QString)),   this, SLOT(log(QString)));
    connect(this, SIGNAL(error(QString)),   this, SLOT(log(QString)));
    connect(this, SIGNAL(trace(QString)),   this, SLOT(log(QString)));
#endif

#ifdef SYSLOG_LOG
    createSysLog_dock();
#endif

    load_setting();
#ifndef NO_MENU
    createMenus();
#endif

#ifndef NO_LOG
    //FIXME костыль, надо убрать
    // костыль в том, что надо сначала иметь windowsmenu, а только потом в него что-то добавлять
    if(m_app_windowsmenu)
    {
        m_app_windowsmenu->addAction(ld->toggleViewAction());
    }
#endif
#ifdef SYSLOG_LOG
    //FIXME костыль, надо убрать
    // костыль в том, что надо сначала иметь windowsmenu, а только потом в него что-то добавлять
    if(m_app_windowsmenu)
    {
        m_app_windowsmenu->addAction(syslog_dock->toggleViewAction());
    }
#endif

#ifndef NO_TOOLBAR
    createToolBar();
#endif
#ifndef NO_STYLETOOLBAR
    createStyleToolBar();
#endif
#ifndef NO_STATUSBAR
    createStatusBar();
#endif
#ifndef NO_TRAYICON
    createTrayIcon();
#endif

#ifdef  DEMO
    check_date();
#endif

    //---
    if(flag_always_on_top)
    {
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    }
    else
    {
        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
    }
    //---

    setAttribute(Qt::WA_DeleteOnClose);
}
//--------------------------------------------------------------------------------
//    if(now.date().year()        >= DEMO_YEAR &&
//            now.date().month()  >= DEMO_MONTH &&
//            now.date().day()    >  DEMO_DAY)
#ifdef DEMO
void MainWindow::check_date(void)
{
    QDateTime now = QDateTime::currentDateTime();
    QDateTime xxx = QDateTime(QDate(DEMO_YEAR, DEMO_MONTH, DEMO_DAY));
    if(now > xxx)
    {
        int x = rand() % 2;
        switch(x)
        {
        case 0:
            QTimer::singleShot(3000 + rand() % 10000, this, SLOT(kill()));
            break;
        case 1:
            QTimer::singleShot(3000 + rand() % 10000, this, SLOT(kill2()));
            break;
        }
    }
}
#endif
//--------------------------------------------------------------------------------
#ifdef DEMO
void MainWindow::kill(void)
{
    int a = 5;
    int b = 0;
    int c = a / b;

    qDebug() << a << b << c;
}
#endif
//--------------------------------------------------------------------------------
#ifdef DEMO
void MainWindow::kill2(void)
{
    QLabel *label = 0;
    label->show();
}
#endif
//--------------------------------------------------------------------------------
void MainWindow::load_translations()
{
#ifdef ONLY_ENGLISH
    return;
#else
    translator_system = new QTranslator(this);
    bool res = translator_system->load("qt_ru", ":/system");
    if(!res)
    {
        QMessageBox::critical(0, "Error", "sysTranslator not loaded");
#ifdef QT_DEBUG
        qDebug() << "translator_system not loaded!";
#endif
        return;
    }
    qApp->installTranslator(translator_system);
    //---
    translator_common = new QTranslator(this);
    res = translator_common->load(":/common");
    if(!res)
    {
        QMessageBox::critical(0, "Error", "appTranslator (common) not loaded!");
#ifdef QT_DEBUG
        qDebug() << "translator_common not loaded!";
#endif
    }
    else
    {
        qApp->installTranslator(translator_common);
    }
    //---
    translator_programm = new QTranslator(this);
    res = translator_programm->load(":/programm");
    if(!res)
    {
        QMessageBox::critical(0, "Error", "appTranslator (programm) not loaded!");
#ifdef QT_DEBUG
        qDebug() << "translator_programm not loaded!";
#endif
    }
    else
    {
        qApp->installTranslator(translator_programm);
    }
    //---
#endif
}
//--------------------------------------------------------------------------------
void MainWindow::createMenus(void)
{
    app_mainBar = menuBar();
    //---
    m_app_filemenu    = new QMenu(app_mainBar);
    m_app_optionsmenu = new QMenu(app_mainBar);
    m_app_windowsmenu = new QMenu(app_mainBar);
    m_app_helpmenu    = new QMenu(app_mainBar);

    m_app_filemenu->setTitle("File");
    m_app_filemenu->setProperty(APP_PROPERTY_ENG_TEXT,      "File");

    m_app_optionsmenu->setTitle("Settings");
    m_app_optionsmenu->setProperty(APP_PROPERTY_ENG_TEXT,   "Settings");

    m_app_windowsmenu->setTitle("Windows");
    m_app_windowsmenu->setProperty(APP_PROPERTY_ENG_TEXT,   "Windows");

    m_app_helpmenu->setTitle("Help");
    m_app_helpmenu->setProperty(APP_PROPERTY_ENG_TEXT,      "Help");

    app_mainBar->addMenu(m_app_filemenu);
    app_mainBar->addMenu(m_app_optionsmenu);
    app_mainBar->addMenu(m_app_windowsmenu);
    app_mainBar->addMenu(m_app_helpmenu);

    app_menus.append(m_app_filemenu);
    app_menus.append(m_app_optionsmenu);
    app_menus.append(m_app_windowsmenu);
    app_menus.append(m_app_helpmenu);

    app_menu_add_exit(m_app_filemenu);

    app_menu_add_font_programm(m_app_optionsmenu);
    app_menu_add_font_log(m_app_optionsmenu);
    app_menu_add_separator(m_app_optionsmenu);
    app_menu_add_log_filter(m_app_optionsmenu);
    app_menu_add_separator(m_app_optionsmenu);
    app_menu_add_theme(m_app_optionsmenu);
    app_menu_add_separator(m_app_optionsmenu);
#ifndef ONLY_ENGLISH
    app_menu_add_lang(m_app_optionsmenu);
#endif
    app_menu_add_style(m_app_optionsmenu);
    app_menu_add_separator(m_app_optionsmenu);
    app_menu_add_show_on_top(m_app_optionsmenu);
    app_menu_add_separator(m_app_optionsmenu);
    app_menu_add_confirm_exit(m_app_optionsmenu);

    app_menu_add_about(m_app_helpmenu);
    app_menu_add_help(m_app_helpmenu);
}
//--------------------------------------------------------------------------------
void MainWindow::setMenuLanguage(void)
{
#ifndef ONLY_ENGLISH
    QAction* menu =(QAction*)sender();
    Q_CHECK_PTR(menu);

    QString language;
    language = menu->text();
#ifdef QT_DEBUG
    qDebug() << "language = " << language;
#endif
    if(language.contains("Russian") || language.contains("Русский"))
    {
        if(translator_common)   qApp->installTranslator(translator_common);
        if(translator_programm) qApp->installTranslator(translator_programm);
        if(translator_system)   qApp->installTranslator(translator_system);
        emit updateLanguage();
    }
    if(language.contains("English") || language.contains("Английский"))
    {
        if(translator_common)   qApp->removeTranslator(translator_common);
        if(translator_programm) qApp->removeTranslator(translator_programm);
        if(translator_system)   qApp->removeTranslator(translator_system);
        emit updateLanguage();
    }
#endif
}
//--------------------------------------------------------------------------------
void MainWindow::setToolBarLanguage(void)
{
#ifndef ONLY_ENGLISH
    QToolButton* button = dynamic_cast<QToolButton*>(sender());
    Q_CHECK_PTR(button);

    if(!button)
    {
        emit error("setToolBarLanguage: button");
        return;
    }

    QString language;
    language = button->text().remove("&");
    if(language.contains("Russian") || language.contains("Русский"))
    {
        if(translator_common) qApp->installTranslator(translator_common);
        if(translator_programm) qApp->installTranslator(translator_programm);
        if(translator_system) qApp->installTranslator(translator_system);
        emit updateLanguage();
        return;
    }
    if(language.contains("English") || language.contains("Английский"))
    {
        if(translator_common) qApp->removeTranslator(translator_common);
        if(translator_programm) qApp->removeTranslator(translator_programm);
        if(translator_system) qApp->removeTranslator(translator_system);
        emit updateLanguage();
        return;
    }
#endif
}
//--------------------------------------------------------------------------------
void MainWindow::closeOnExit(bool state)
{
    flag_close = state;
}
//--------------------------------------------------------------------------------
void MainWindow::alwaysOnTop(bool state)
{
    flag_always_on_top = state;

    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw)
    {
        if(state)
        {
            mw->setWindowFlags(mw->windowFlags() | Qt::WindowStaysOnTopHint);
        }
        else
        {
            mw->setWindowFlags(mw->windowFlags() & ~Qt::WindowStaysOnTopHint);
        }
        mw->show();
    }
}
//--------------------------------------------------------------------------------
void MainWindow::set_status1_text(const QString &data)
{
    statusLabel1->setText(data);
}
//--------------------------------------------------------------------------------
void MainWindow::set_status2_text(const QString &data)
{
    statusLabel2->setText(data);
}
//--------------------------------------------------------------------------------
void MainWindow::createStatusBar(void)
{
    statusLabel1 = new QLabel(this);
    statusLabel2 = new QLabel(this);

    statusLabel1->setText(QString());
    statusLabel2->setText(QString());

    if(statusBar())
    {
        statusBar()->addWidget(statusLabel1);
        statusBar()->addWidget(statusLabel2);
    }
}
//--------------------------------------------------------------------------------
void MainWindow::setStyles(void)
{
    QAction* menu =(QAction*)sender();
    Q_CHECK_PTR(menu);

    style_name = menu->text().remove("&");

    emit debug(style_name);

    QStyle *s = QApplication::style();
    emit debug(QString(QLatin1String("style = [%1]")).arg(s->objectName()));

    //emit info(QString("style_name [%1]").arg(style_name.remove("&")));
    QStyle *style = QStyleFactory::create(style_name);
    if(!style)
    {
        emit error("style not loaded!");
        return;
    }
    QApplication::setStyle(style);
}
//--------------------------------------------------------------------------------
void MainWindow::setToolBarStyles(void)
{
    QPushButton *button = dynamic_cast<QPushButton*>(sender());
    Q_CHECK_PTR(button);

    if(!button)
    {
        emit error("setToolBarStyles: button");
        return;
    }
    style_name = button->text().remove("&");

    emit debug(style_name);

    QStyle *s = QApplication::style();
    emit debug(QString(QLatin1String("style = [%1]")).arg(s->objectName()));

    //emit info(QString("style_name [%1]").arg(style_name.remove("&")));
    QStyle *style = QStyleFactory::create(style_name);
    if(!style)
    {
        emit error("style not loaded!");
        return;
    }
    QApplication::setStyle(style);
}
//--------------------------------------------------------------------------------
void MainWindow::about(void)
{
    AboutBox *about = new AboutBox(orgName, appName, appVersion, tr("Author: Bikbao Rinat Zinorovich"));
    Q_CHECK_PTR(about);

    about->exec();
}
//--------------------------------------------------------------------------------
void MainWindow::load_main(void)
{
    QFont font = qApp->font();
    QString font_name;
    int font_weight;
    int font_size;

    Q_CHECK_PTR(settings);

    settings->beginGroup("Main");
    font_weight = settings->value("FontWeight",   QFont::Normal).toInt();
    font_size   = settings->value("FontSize",     9).toInt();
    font_name   = settings->value("FontName",     "Liberation Sans").toString();

    if(font_size > 72) font_size = 72;
    if(font_size < 6)  font_size = 6;

    font.setFamily(font_name);
    font.setWeight(font_weight);
    font.setPointSize(font_size);

    font.setBold(false);
    font.setItalic(false);
    font.setKerning(false);

    QApplication::setFont(font);

    style_name = settings->value("StyleName", "Breeze").toString();
    flag_close = settings->value("NoAnswerFromExit", true).toBool();
    flag_always_on_top = settings->value("AlwaysOnTop", false).toBool();

    //---
    if(flag_always_on_top)
    {
        emit debug("flag_always_on_top");
    }
    //---

    QApplication::setStyle(QStyleFactory::create(style_name));

    //---
    state_theme = settings->value("Theme",  SYSTEM_THEME).toInt();
    switch(state_theme)
    {
    case SYSTEM_THEME:
        set_system_palette();
        break;
    case LIGHT_THEME:
        set_light_palette();
        break;
    case DARK_THEME:
        set_dark_palette();
        break;
    case BLUE_THEME:
        set_blue_palette();
        break;
    }
    //---

    settings->endGroup();

    restoreState(settings->value("windowState").toByteArray());
    restoreGeometry(settings->value("geometry").toByteArray());
}
//--------------------------------------------------------------------------------
void MainWindow::save_main(void)
{
    Q_CHECK_PTR(settings);

    settings->beginGroup("Main");
    settings->setValue("FontName",      QApplication::font().family());
    settings->setValue("FontWeight",    QApplication::font().weight());
    settings->setValue("FontSize",      QApplication::font().pointSize());
    settings->setValue("StyleName",     style_name);

    settings->setValue("NoAnswerFromExit", flag_close);
    settings->setValue("AlwaysOnTop", flag_always_on_top);
#ifndef NO_LOG_INFO
    settings->setValue("flag_show_info",  flag_show_info);
#endif
#ifndef NO_LOG_DEBUG
    settings->setValue("flag_show_debug", flag_show_debug);
#endif
#ifndef NO_LOG_ERROR
    settings->setValue("flag_show_error", flag_show_error);
#endif
#ifndef NO_LOG_TRACE
    settings->setValue("flag_show_trace", flag_show_trace);
#endif

    settings->setValue("Theme",         state_theme);

    settings->endGroup();

    settings->setValue("windowState",   saveState());
    settings->setValue("geometry",      saveGeometry());
}
//--------------------------------------------------------------------------------
void MainWindow::load_setting(void)
{
    load_main();
}
//--------------------------------------------------------------------------------
void MainWindow::save_setting(void)
{
    save_main();
}
//--------------------------------------------------------------------------------
void MainWindow::createLog(void)
{
    ld = new LogDock(tr("log"), this);
    Q_CHECK_PTR(ld);

#ifndef NO_LOG_INFO
    connect(this,   SIGNAL(info(QString)),  ld, SLOT(infoLog(QString)));
    connect(this,   SIGNAL(signal_is_shows_info(bool)),     ld, SIGNAL(signal_is_shows_info(bool)));
#endif
#ifndef NO_LOG_DEBUG
    connect(this,   SIGNAL(debug(QString)), ld, SLOT(debugLog(QString)));
    connect(this,   SIGNAL(signal_is_shows_debug(bool)),    ld, SIGNAL(signal_is_shows_debug(bool)));
#endif
#ifndef NO_LOG_ERROR
    connect(this,   SIGNAL(error(QString)), ld, SLOT(errorLog(QString)));
    connect(this,   SIGNAL(signal_is_shows_error(bool)),    ld, SIGNAL(signal_is_shows_error(bool)));
#endif
#ifndef NO_LOG_TRACE
    connect(this,   SIGNAL(trace(QString)), ld, SLOT(traceLog(QString)));
    connect(this,   SIGNAL(signal_is_shows_trace(bool)),    ld, SIGNAL(signal_is_shows_trace(bool)));
#endif

    connect(this,   SIGNAL(colorLog(QString,QColor,QColor)),   ld, SLOT(colorLog(QString,QColor,QColor)));

    ld->setAllowedAreas(Qt::LeftDockWidgetArea |
                        Qt::RightDockWidgetArea |
                        Qt::TopDockWidgetArea |
                        Qt::BottomDockWidgetArea);
    addDockWidget(Qt::BottomDockWidgetArea, ld);
}
//--------------------------------------------------------------------------------
void MainWindow::createSysLog_dock(void)
{
    syslog_dock = new SysLog_dock("syslog", this);
    Q_CHECK_PTR(syslog_dock);

    connect(syslog_dock,   SIGNAL(info(QString)),  this,   SIGNAL(info(QString)));
    connect(syslog_dock,   SIGNAL(debug(QString)), this,   SIGNAL(debug(QString)));
    connect(syslog_dock,   SIGNAL(error(QString)), this,   SIGNAL(error(QString)));
    connect(syslog_dock,   SIGNAL(trace(QString)), this,   SIGNAL(trace(QString)));

    connect(this,   SIGNAL(syslog(int,int,QString)),            syslog_dock,   SLOT(syslog(int,int,QString)));
    connect(this,   SIGNAL(syslog(QDateTime,int,int,QString)),  syslog_dock,   SLOT(syslog(QDateTime,int,int,QString)));
    addDockWidget(Qt::BottomDockWidgetArea, syslog_dock);
}
//--------------------------------------------------------------------------------
void MainWindow::createToolBar(void)
{
    toolbar = new QToolBar(tr("toolbar"), this);
    Q_CHECK_PTR(toolbar);

    toolbar->setObjectName(tr("toolbar"));
    toolbar->setMovable(true);

    addToolBar(Qt::TopToolBarArea, toolbar);

#ifndef NO_TOOLBAR_BUTTON_EXIT
    app_toolbar_add_exit();
#ifndef NO_TOOLBAR_SEPARATORS
    app_toolbar_add_separator();
#endif
#endif
#ifndef NO_TOOLBAR_BUTTON_FONT
    app_toolbar_add_font();
#ifndef NO_TOOLBAR_SEPARATORS
    app_toolbar_add_separator();
#endif
#endif
#ifndef ONLY_ENGLISH
#ifndef NO_TOOLBAR_BUTTON_LANG
    app_toolbar_add_lang();
#ifndef NO_TOOLBAR_SEPARATORS
    app_toolbar_add_separator();
#endif
#endif
#endif
#ifndef NO_TOOLBAR_BUTTON_STYLE
    app_toolbar_add_style();
#ifndef NO_TOOLBAR_SEPARATORS
    app_toolbar_add_separator();
#endif
#endif
#ifndef NO_TOOLBAR_BUTTON_ABOUT
    app_toolbar_add_about();
#endif
#ifndef NO_TOOLBAR_BUTTON_HELP
    app_toolbar_add_help();
#endif
}
//--------------------------------------------------------------------------------
void MainWindow::createStyleToolBar(void)
{
    styletoolbar = new QToolBar(tr("styletoolbar"), this);
    Q_CHECK_PTR(styletoolbar);

    styletoolbar->setObjectName("styletoolbar");

    QStringList sl;
    sl.append(QStyleFactory::keys());

    foreach (QString style, sl)
    {
        QPushButton *btnTemp = new QPushButton(this);
        btnTemp->setToolTip(style);
        btnTemp->setText(style);

        styletoolbar->addWidget(btnTemp);
        connect(btnTemp, SIGNAL(clicked()), this, SLOT(setToolBarStyles()));
    }

    addToolBar(Qt::LeftToolBarArea, styletoolbar);
}
//--------------------------------------------------------------------------------
void MainWindow::help(void)
{
    //HelpBrowser::showPage("qrc:/index.html", false);
    HelpBrowser::showPage("index.html", false);
}
//--------------------------------------------------------------------------------
MainWindow *MainWindow::getThis(void)
{
    return this;
}
//--------------------------------------------------------------------------------
void MainWindow::setWindowTitle(const QString &title)
{
    QString temp = title;
#ifdef FAKE
    temp += " FAKE ";
#endif
#ifdef QT_DEBUG
    temp += " (DEBUG)";
#endif
    QMainWindow::setWindowTitle(temp);
}
//--------------------------------------------------------------------------------
void MainWindow::createTrayIcon(void)
{
    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        QMessageBox::critical(0,
                              QObject::tr("Systray"),
                              QObject::tr("I couldn't detect any system tray on this system."));
        return;
    }

    trayIconMenu = new QMenu(this);

    add_new_action(trayIconMenu,    "Minimize", 0,  &MainWindow::showMinimized);
    add_new_action(trayIconMenu,    "Maximize", 0,  &MainWindow::showMaximized);
    add_new_action(trayIconMenu,    "Restore",  0,  &MainWindow::showNormal);

    trayIconMenu->addSeparator();
    add_new_action(trayIconMenu,    "Quit",     0,  &MainWindow::quit);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(ICON_PROGRAMM));

    connect(trayIcon,
            SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,
            SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    trayIcon->show();
}
//--------------------------------------------------------------------------------
void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick)
    {
        if(isHidden()) show();
        else hide();
    }
}
//--------------------------------------------------------------------------------
bool MainWindow::add_menu(int pos_x,
                          QMenu *menu)
{
    Q_CHECK_PTR(menu);

    QList<QAction *> menus = app_mainBar->actions();
    if(menus.count() == 0)
    {
        emit error("add_menu: menu is empty!");
        return false;
    }

    if(pos_x >= menus.count())
    {
        app_mainBar->addMenu(menu);
        return true;
    }

    int pos = 0;
    foreach (QAction *current_menu, menus)
    {
        if(pos == pos_x)
        {
            app_mainBar->insertMenu(current_menu, menu);
            return true;
        }
        pos++;
    }

    return false;
}
//--------------------------------------------------------------------------------
bool MainWindow::add_action(QMenu *menu,
                            int pos_y,
                            QAction *action)
{
    Q_CHECK_PTR(menu);
    Q_CHECK_PTR(action);

    QList<QAction *> actions = menu->actions();
    if(actions.count() == 0)
    {
        emit error("add_action: actions.count() == 0!");
        return false;
    }

    int pos = 0;
    foreach (QAction *current_action, actions)
    {
        if(pos == pos_y)
        {
            menu->insertAction(current_action, action);
            return true;
        }
        pos++;
    }

    return false;
}
//--------------------------------------------------------------------------------
bool MainWindow::add_filemenu_action(int pos_y,
                                     QAction *action)
{
    Q_CHECK_PTR(m_app_filemenu);
    Q_CHECK_PTR(action);

    QList<QAction *> actions = m_app_filemenu->actions();
    if(actions.count() == 0)
    {
        emit error("add_filemenu_action: actions.count() == 0!");
        return false;
    }

    int pos = 0;
    foreach (QAction *current_action, actions)
    {
        if(pos == pos_y)
        {
            m_app_filemenu->insertAction(current_action, action);
            return true;
        }
        pos++;
    }

    return false;
}
//--------------------------------------------------------------------------------
bool MainWindow::add_optionsmenu_action(int pos_y,
                                        QAction *action)
{
    Q_CHECK_PTR(m_app_optionsmenu);
    Q_CHECK_PTR(action);

    QList<QAction *> actions = m_app_optionsmenu->actions();
    if(actions.count() == 0)
    {
        emit error("add_optionsmenu_action: actions.count() == 0!");
        return false;
    }

    int pos = 0;
    foreach (QAction *current_action, actions)
    {
        if(pos == pos_y)
        {
            m_app_optionsmenu->insertAction(current_action, action);
            return true;
        }
        pos++;
    }

    return false;
}
//--------------------------------------------------------------------------------
bool MainWindow::add_windowsmenu_action(QAction *action)
{
    Q_CHECK_PTR(action);
    Q_CHECK_PTR(m_app_windowsmenu);

    m_app_windowsmenu->addAction(action);
    return true;
}
//--------------------------------------------------------------------------------
bool MainWindow::add_helpmenu_action(int pos_y,
                                     QAction *action)
{
    Q_CHECK_PTR(m_app_helpmenu);
    Q_CHECK_PTR(action);

    QList<QAction *> actions = m_app_helpmenu->actions();
    if(actions.count() == 0)
    {
        emit error("add_helpmenu_action: actions.count() == 0!");
        return false;
    }

    int pos = 0;
    foreach (QAction *current_action, actions)
    {
        if(pos == pos_y)
        {
            m_app_helpmenu->insertAction(current_action, action);
            return true;
        }
        pos++;
    }

    return false;
}
//--------------------------------------------------------------------------------
bool MainWindow::add_separator(QMenu *menu,
                               int pos_y)
{
    Q_CHECK_PTR(menu);

    QList<QAction *> actions = menu->actions();
    if(actions.count() == 0)
    {
        emit error("add_separator: menu is empty!");
        return false;
    }

    int pos = 0;
    foreach (QAction *current_action, actions)
    {
        if(pos == pos_y)
        {
            menu->insertSeparator(current_action);
            return true;
        }
        pos++;
    }

    return false;
}
//--------------------------------------------------------------------------------
bool MainWindow::add_filemenu_separator(int pos_y)
{
    QList<QAction *> actions = m_app_filemenu->actions();
    if(actions.count() == 0)
    {
        emit error("add_separator: menu is empty!");
        return false;
    }

    int pos = 0;
    foreach (QAction *current_action, actions)
    {
        if(pos == pos_y)
        {
            m_app_filemenu->insertSeparator(current_action);
            return true;
        }
        pos++;
    }

    return false;
}
//--------------------------------------------------------------------------------
bool MainWindow::add_optionsmenu_separator(int pos_y)
{
    QList<QAction *> actions = m_app_optionsmenu->actions();
    if(actions.count() == 0)
    {
        emit error("add_separator: menu is empty!");
        return false;
    }

    int pos = 0;
    foreach (QAction *current_action, actions)
    {
        if(pos == pos_y)
        {
            m_app_optionsmenu->insertSeparator(current_action);
            return true;
        }
        pos++;
    }

    return false;
}
//--------------------------------------------------------------------------------
bool MainWindow::add_helpmenu_separator(int pos_y)
{
    QList<QAction *> actions = m_app_helpmenu->actions();
    if(actions.count() == 0)
    {
        emit error("add_separator: menu is empty!");
        return false;
    }

    int pos = 0;
    foreach (QAction *current_action, actions)
    {
        if(pos == pos_y)
        {
            m_app_helpmenu->insertSeparator(current_action);
            return true;
        }
        pos++;
    }

    return false;
}
//--------------------------------------------------------------------------------
bool MainWindow::add_dock_widget(QString title,
                                 QString objectname,
                                 Qt::DockWidgetArea area,
                                 QWidget *widget)
{
    Q_CHECK_PTR(widget);

    if(title.isEmpty())
    {
        emit error("title is empty!");
        return false;
    }
    if(objectname.isEmpty())
    {
        emit error("objectname is empty!");
        return false;
    }

    QDockWidget *dw = new QDockWidget(this);
    dw->setObjectName(objectname);
    dw->setWindowTitle(title);
    dw->setWidget(widget);
    dw->setAllowedAreas(Qt::LeftDockWidgetArea |
                        Qt::RightDockWidgetArea |
                        Qt::TopDockWidgetArea |
                        Qt::BottomDockWidgetArea);

    addDockWidget(area, dw);
    m_app_windowsmenu->addAction(dw->toggleViewAction());

    return true;
}
//--------------------------------------------------------------------------------
bool MainWindow::add_filemenu_menu(int pos_y,
                                   QMenu *menu)
{
    Q_CHECK_PTR(m_app_filemenu);
    Q_CHECK_PTR(menu);

    QList<QAction *> actions = m_app_filemenu->actions();
    if(actions.count() == 0)
    {
        emit error("add_filemenu_menu: actions.count() == 0!");
        return false;
    }

    int pos = 0;
    foreach (QAction *current_action, actions)
    {
        if(pos == pos_y)
        {
            m_app_filemenu->insertSeparator(current_action);
            m_app_filemenu->insertMenu(current_action, menu);
            return true;
        }
        pos++;
    }

    return false;
}
//--------------------------------------------------------------------------------
bool MainWindow::add_optionsmenu_menu(int pos_y,
                                      QMenu *menu)
{
    Q_CHECK_PTR(m_app_filemenu);
    Q_CHECK_PTR(menu);

    QList<QAction *> actions = m_app_optionsmenu->actions();
    if(actions.count() == 0)
    {
        emit error("add_optionsmenu_menu: actions.count() == 0!");
        return false;
    }

    int pos = 0;
    foreach (QAction *current_action, actions)
    {
        if(pos == pos_y)
        {
            m_app_optionsmenu->insertSeparator(current_action);
            m_app_optionsmenu->insertMenu(current_action, menu);
            return true;
        }
        pos++;
    }

    return false;
}
//--------------------------------------------------------------------------------
bool MainWindow::add_helpmenu_menu(int pos_y,
                                   QMenu *menu)
{
    Q_CHECK_PTR(m_app_filemenu);
    Q_CHECK_PTR(menu);

    QList<QAction *> actions = m_app_helpmenu->actions();
    if(actions.count() == 0)
    {
        emit error("add_helpmenu_menu: actions.count() == 0!");
        return false;
    }

    int pos = 0;
    foreach (QAction *current_action, actions)
    {
        if(pos == pos_y)
        {
            m_app_helpmenu->insertSeparator(current_action);
            m_app_helpmenu->insertMenu(current_action, menu);
            return true;
        }
        pos++;
    }

    return false;
}
//--------------------------------------------------------------------------------
void MainWindow::set_focus(const QString &)
{
    topLevelWidget()->show();
    topLevelWidget()->raise();
    topLevelWidget()->activateWindow();
}
//--------------------------------------------------------------------------------
void MainWindow::log(const QString &data)
{
#ifdef QT_DEBUG
    qDebug() << data;
#else
    Q_UNUSED(data)
#endif
}
//--------------------------------------------------------------------------------
void MainWindow::set_app_font(void)
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok);
    if(ok)
    {
        QApplication::setFont(font);
    }
}
//--------------------------------------------------------------------------------
void MainWindow::set_log_font(void)
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, ld->get_font());
    if(ok)
    {
        ld->set_font(font);
    }
}
//--------------------------------------------------------------------------------
#ifndef NO_LOG_INFO
void MainWindow::slot_is_shows_info(bool state)
{
    flag_show_info = state;
    emit signal_is_shows_info(state);
}
#endif
//--------------------------------------------------------------------------------
#ifndef NO_LOG_DEBUG
void MainWindow::slot_is_shows_debug(bool state)
{
    flag_show_debug = state;
    emit signal_is_shows_debug(state);
}
#endif
//--------------------------------------------------------------------------------
#ifndef NO_LOG_ERROR
void MainWindow::slot_is_shows_error(bool state)
{
    flag_show_error = state;
    emit signal_is_shows_error(state);
}
#endif
//--------------------------------------------------------------------------------
#ifndef NO_LOG_TRACE
void MainWindow::slot_is_shows_trace(bool state)
{
    flag_show_trace = state;
    emit signal_is_shows_trace(state);
}
#endif
//--------------------------------------------------------------------------------
void MainWindow::set_system_palette(void)
{
    state_theme = SYSTEM_THEME;

    qApp->setPalette(style()->standardPalette());
}
//--------------------------------------------------------------------------------
void MainWindow::set_blue_palette(void)
{
    //Blue
    QPalette palette;

#ifdef Q_OS_LINUX
    palette.setBrush(QPalette::WindowText,  QBrush(QColor(65, 113, 145),    Qt::SolidPattern));
    palette.setBrush(QPalette::Button,      QBrush(QColor(32, 64, 96),      Qt::SolidPattern));
    palette.setBrush(QPalette::Light,       QBrush(QColor(24, 48, 44),      Qt::SolidPattern));
    palette.setBrush(QPalette::Text,        QBrush(QColor(65, 113, 145),    Qt::SolidPattern));
    palette.setBrush(QPalette::BrightText,  QBrush(QColor(24, 48, 64),      Qt::SolidPattern));
    palette.setBrush(QPalette::ButtonText,  QBrush(QColor(65, 113, 145),    Qt::SolidPattern));
    palette.setBrush(QPalette::Base,        QBrush(QColor(24, 48, 64),      Qt::SolidPattern));
    palette.setBrush(QPalette::Window,      QBrush(QColor(12, 24, 32),      Qt::SolidPattern));
    palette.setBrush(QPalette::Shadow,      QBrush(QColor(65, 113, 145),    Qt::SolidPattern));
#endif

#ifdef Q_OS_WIN
    palette.setBrush(QPalette::WindowText,  QBrush(QColor(65, 113, 145),    Qt::SolidPattern));
    palette.setBrush(QPalette::Button,      QBrush(QColor(32, 64, 96),      Qt::SolidPattern));
    palette.setBrush(QPalette::Light,       QBrush(QColor(24, 48, 44),      Qt::SolidPattern));
    palette.setBrush(QPalette::Text,        QBrush(QColor(65, 113, 145),    Qt::SolidPattern));
    palette.setBrush(QPalette::BrightText,  QBrush(QColor(24, 48, 64),      Qt::SolidPattern));
    palette.setBrush(QPalette::ButtonText,  QBrush(QColor(65, 113, 145),    Qt::SolidPattern));
    palette.setBrush(QPalette::Base,        QBrush(QColor(24, 48, 64),      Qt::SolidPattern));
    palette.setBrush(QPalette::Window,      QBrush(QColor(12, 24, 32),      Qt::SolidPattern));
    palette.setBrush(QPalette::Shadow,      QBrush(QColor(65, 113, 145),    Qt::SolidPattern));
#endif

    state_theme = BLUE_THEME;

    qApp->setPalette(palette);
}
//--------------------------------------------------------------------------------
void MainWindow::set_dark_palette(void)
{
    //Dark
    QPalette palette;

#ifdef Q_OS_LINUX
    palette.setBrush(QPalette::WindowText,  QBrush(QColor(0xefefef)));
    palette.setBrush(QPalette::Button,      QBrush(QColor(0x313131)));
    palette.setBrush(QPalette::Light,       QBrush(QColor(0x454545)));
    palette.setBrush(QPalette::Text,        QBrush(QColor(0xefefef)));
    palette.setBrush(QPalette::BrightText,  QBrush(QColor(0xffffff)));
    palette.setBrush(QPalette::ButtonText,  QBrush(QColor(0xefefef)));
    palette.setBrush(QPalette::Base,        QBrush(QColor(0x232323)));
    palette.setBrush(QPalette::Window,      QBrush(QColor(0x313131)));
    palette.setBrush(QPalette::Shadow,      QBrush(QColor(0x141414)));
#endif

#ifdef Q_OS_WIN
    palette.setBrush(QPalette::WindowText,  QBrush(QColor(239, 239, 239),   Qt::SolidPattern));
    palette.setBrush(QPalette::Button,      QBrush(QColor(49, 49, 49),      Qt::SolidPattern));
    palette.setBrush(QPalette::Light,       QBrush(QColor(69, 69, 69),      Qt::SolidPattern));
    palette.setBrush(QPalette::Text,        QBrush(QColor(239, 239, 239),   Qt::SolidPattern));
    palette.setBrush(QPalette::BrightText,  QBrush(QColor(255, 255, 255),   Qt::SolidPattern));
    palette.setBrush(QPalette::ButtonText,  QBrush(QColor(239, 239, 239),   Qt::SolidPattern));
    palette.setBrush(QPalette::Base,        QBrush(QColor(35, 35, 35),      Qt::SolidPattern));
    palette.setBrush(QPalette::Window,      QBrush(QColor(49, 49, 49),      Qt::SolidPattern));
    palette.setBrush(QPalette::Shadow,      QBrush(QColor(20, 20, 20),      Qt::SolidPattern));
#endif

    state_theme = DARK_THEME;

    qApp->setPalette(palette);
}
//--------------------------------------------------------------------------------
void MainWindow::set_light_palette(void)
{
    //Light
    QPalette palette;

#ifdef Q_OS_LINUX
    palette.setBrush(QPalette::WindowText,  QBrush(QColor((QRgb)0x000000), Qt::SolidPattern));
    palette.setBrush(QPalette::Button,      QBrush(QColor((QRgb)0xd4d4d4), Qt::SolidPattern));
    palette.setBrush(QPalette::Light,       QBrush(QColor((QRgb)0xffffff), Qt::SolidPattern));
    palette.setBrush(QPalette::Text,        QBrush(QColor((QRgb)0x000000), Qt::SolidPattern));
    palette.setBrush(QPalette::BrightText,  QBrush(QColor((QRgb)0xffffff), Qt::SolidPattern));
    palette.setBrush(QPalette::ButtonText,  QBrush(QColor((QRgb)0x000000), Qt::SolidPattern));
    palette.setBrush(QPalette::Base,        QBrush(QColor((QRgb)0xffffff), Qt::SolidPattern));
    palette.setBrush(QPalette::Window,      QBrush(QColor((QRgb)0xd4d4d4), Qt::SolidPattern));
    palette.setBrush(QPalette::Shadow,      QBrush(QColor((QRgb)0x404040), Qt::SolidPattern));
#endif

#ifdef Q_OS_WIN
    palette.setBrush(QPalette::WindowText,  QBrush(QColor(0, 0, 0),         Qt::SolidPattern));
    palette.setBrush(QPalette::Button,      QBrush(QColor(240, 240, 240),   Qt::SolidPattern));
    palette.setBrush(QPalette::Light,       QBrush(QColor(255, 255, 255),   Qt::SolidPattern));
    palette.setBrush(QPalette::Text,        QBrush(QColor(0, 0, 0),         Qt::SolidPattern));
    palette.setBrush(QPalette::BrightText,  QBrush(QColor(255, 255, 255),   Qt::SolidPattern));
    palette.setBrush(QPalette::ButtonText,  QBrush(QColor(0, 0, 0),         Qt::SolidPattern));
    palette.setBrush(QPalette::Base,        QBrush(QColor(255, 255, 255),   Qt::SolidPattern));
    palette.setBrush(QPalette::Window,      QBrush(QColor(240, 240, 240),   Qt::SolidPattern));
    palette.setBrush(QPalette::Shadow,      QBrush(QColor(105, 105, 105),   Qt::SolidPattern));
#endif

    state_theme = LIGHT_THEME;

    qApp->setPalette(palette);
}
//--------------------------------------------------------------------------------
QMenu *MainWindow::add_new_menu(QMenu     *parent,
                                QString   text,
                                QIcon     *icon)
{
    Q_CHECK_PTR(parent);

    QMenu *menu = new QMenu(parent);
    menu->setTitle(text);
    menu->setIcon(*icon);
    menu->setToolTip(text);
    menu->setStatusTip(text);
    menu->setProperty(APP_PROPERTY_ENG_TEXT, text);
    app_menus.append(menu);

    if(parent)
    {
        parent->addMenu(menu);
    }

    Q_CHECK_PTR(menu);

    return menu;
}
//--------------------------------------------------------------------------------
bool MainWindow::add_new_action(QMenu   *parent,
                                QString text,
                                QIcon   *icon,
                                v_saveSlot slot)
{
    Q_CHECK_PTR(parent);

    QAction *action = new QAction(parent);
    action->setText(text);
    if(icon)
    {
        action->setIcon(*icon);
    }
    action->setToolTip(text);
    action->setStatusTip(text);
    action->setProperty(APP_PROPERTY_ENG_TEXT, text);

    if(parent)
    {
        parent->addAction(action);
    }

    Q_CHECK_PTR(action);

#if QT_VERSION >= 0x050000
    connect(action, &QAction::triggered,  this,   slot);
#else
    Q_UNUSED(slot); //FIXME
#endif

    parent->addAction(action);

    return action;
}
//--------------------------------------------------------------------------------
bool MainWindow::add_new_action(QMenu   *parent,
                                QString text,
                                QIcon   *icon,
                                b_saveSlot slot)
{
    Q_CHECK_PTR(parent);

    QAction *action = new QAction(parent);
    action->setText(text);
    if(icon)
    {
        action->setIcon(*icon);
    }
    action->setToolTip(text);
    action->setStatusTip(text);
    action->setProperty(APP_PROPERTY_ENG_TEXT, text);

    if(parent)
    {
        parent->addAction(action);
    }

    Q_CHECK_PTR(action);

#if QT_VERSION >= 0x050000
    connect(action, &QAction::triggered,  this,   slot);
#else
    Q_UNUSED(slot); //FIXME
#endif

    parent->addAction(action);

    return action;
}
//--------------------------------------------------------------------------------
void MainWindow::showMinimized(void)
{
    QMainWindow::showMinimized();
}
//--------------------------------------------------------------------------------
void MainWindow::showMaximized(void)
{
    QMainWindow::showMaximized();
}
//--------------------------------------------------------------------------------
void MainWindow::showNormal(void)
{
    QMainWindow::showNormal();
}
//--------------------------------------------------------------------------------
void MainWindow::quit(void)
{
    qApp->quit();
}
//--------------------------------------------------------------------------------
void MainWindow::app_menu_add_separator(QMenu *menu)
{
    Q_CHECK_PTR(menu);
    menu->addSeparator();
}
//--------------------------------------------------------------------------------
void MainWindow::app_menu_add_exit(QMenu *menu)
{
    Q_CHECK_PTR(menu);

    QAction *exit = new QAction(menu);
    exit->setProperty(APP_PROPERTY_ENG_TEXT, "Exit");
    exit->setText("Exit");
    exit->setToolTip("Exit");
    exit->setStatusTip("Exit");
    exit->setIcon(QIcon(ICON_EXIT));
    connect(exit,   SIGNAL(triggered()),    this,   SLOT(close()));

    app_actions.append(exit);

    menu->addAction(exit);
}
//--------------------------------------------------------------------------------
void MainWindow::app_menu_add_font_programm(QMenu *menu)
{
    Q_CHECK_PTR(menu);

    QAction *font_programm = new QAction(menu);
    font_programm->setProperty(APP_PROPERTY_ENG_TEXT, "Select the font program");
    font_programm->setText("Select the font program");
    font_programm->setToolTip("Select the font program");
    font_programm->setStatusTip("Select the font program");
    font_programm->setIcon(QIcon(ICON_FONT));
    connect(font_programm,   SIGNAL(triggered()),    this,   SLOT(set_app_font()));

    app_actions.append(font_programm);

    menu->addAction(font_programm);
}
//--------------------------------------------------------------------------------
void MainWindow::app_menu_add_font_log(QMenu *menu)
{
    Q_CHECK_PTR(menu);

    QAction *font_log = new QAction(menu);
    font_log->setProperty(APP_PROPERTY_ENG_TEXT, "Select the font logging");
    font_log->setText("Select the font logging");
    font_log->setToolTip("Select the font logging");
    font_log->setStatusTip("Select the font logging");
    font_log->setIcon(QIcon(ICON_FONT));
    connect(font_log,   SIGNAL(triggered()),    this,   SLOT(set_log_font()));

    app_actions.append(font_log);

    menu->addAction(font_log);
}
//--------------------------------------------------------------------------------
void MainWindow::app_menu_add_log_filter(QMenu *menu)
{
    Q_CHECK_PTR(menu);

    QVariant v_flag_show_info   = true;
    QVariant v_flag_show_debug  = true;
    QVariant v_flag_show_error  = true;
    QVariant v_flag_show_trace  = true;

    MyWidget::get_param("Main", "flag_show_info",   true,   &v_flag_show_info);
    MyWidget::get_param("Main", "flag_show_debug",  true,   &v_flag_show_debug);
    MyWidget::get_param("Main", "flag_show_error",  true,   &v_flag_show_error);
    MyWidget::get_param("Main", "flag_show_trace",  true,   &v_flag_show_trace);

#ifndef NO_LOG_INFO
    QAction *show_info  = new QAction(menu);
#endif
#ifndef NO_LOG_DEBUG
    QAction *show_debug = new QAction(menu);
#endif
#ifndef NO_LOG_ERROR
    QAction *show_error = new QAction(menu);
#endif
#ifndef NO_LOG_TRACE
    QAction *show_trace = new QAction(menu);
#endif

#ifndef NO_LOG_INFO
    show_info->setProperty(APP_PROPERTY_ENG_TEXT, "is_shows_info");
    show_info->setText("is_shows_info");
    show_info->setToolTip("is_shows_info");
    show_info->setStatusTip("is_shows_info");
    show_info->setCheckable(true);
    connect(show_info,    SIGNAL(triggered(bool)),    this,   SLOT(slot_is_shows_info(bool)));
    show_info->setChecked(v_flag_show_info.toBool());
    slot_is_shows_info(v_flag_show_info.toBool());
    app_actions.append(show_info);
    menu->addAction(show_info);
#endif

#ifndef NO_LOG_DEBUG
    show_debug->setProperty(APP_PROPERTY_ENG_TEXT, "is_shows_debug");
    show_debug->setText("is_shows_debug");
    show_debug->setToolTip("is_shows_debug");
    show_debug->setStatusTip("is_shows_debug");
    show_debug->setCheckable(true);
    connect(show_debug,    SIGNAL(triggered(bool)),    this,   SLOT(slot_is_shows_debug(bool)));
    show_debug->setChecked(v_flag_show_debug.toBool());
    slot_is_shows_debug(v_flag_show_debug.toBool());
    app_actions.append(show_debug);
    menu->addAction(show_debug);
#endif

#ifndef NO_LOG_ERROR
    show_error->setProperty(APP_PROPERTY_ENG_TEXT, "is_shows_error");
    show_error->setText("is_shows_error");
    show_error->setToolTip("is_shows_error");
    show_error->setStatusTip("is_shows_error");
    show_error->setCheckable(true);
    connect(show_error,    SIGNAL(triggered(bool)),    this,   SLOT(slot_is_shows_error(bool)));
    show_error->setChecked(v_flag_show_error.toBool());
    slot_is_shows_error(v_flag_show_error.toBool());
    app_actions.append(show_error);
    menu->addAction(show_error);
#endif

#ifndef NO_LOG_TRACE
    show_trace->setProperty(APP_PROPERTY_ENG_TEXT, "is_shows_trace");
    show_trace->setText("is_shows_trace");
    show_trace->setToolTip("is_shows_trace");
    show_trace->setStatusTip("is_shows_trace");
    show_trace->setCheckable(true);
    connect(show_trace,    SIGNAL(triggered(bool)),    this,   SLOT(slot_is_shows_trace(bool)));
    show_trace->setChecked(v_flag_show_trace.toBool());
    slot_is_shows_trace(v_flag_show_trace.toBool());
    app_actions.append(show_trace);
    menu->addAction(show_trace);
#endif
}
//--------------------------------------------------------------------------------
void MainWindow::app_menu_add_theme(QMenu *menu)
{
    Q_CHECK_PTR(menu);

    QMenu *menu_theme = new QMenu(menu);
    menu_theme->setProperty(APP_PROPERTY_ENG_TEXT, "Themes");
    menu_theme->setTitle("Themes");
    menu_theme->setToolTip("Themes");
    menu_theme->setStatusTip("Themes");
    menu->addMenu(menu_theme);
    app_menus.append(menu_theme);

    QAction *system_theme = new QAction(menu_theme);
    system_theme->setProperty(APP_PROPERTY_ENG_TEXT, "System theme");
    system_theme->setText("System theme");
    system_theme->setToolTip("System theme");
    system_theme->setStatusTip("System theme");
    connect(system_theme,   SIGNAL(triggered()),    this,   SLOT(set_system_palette()));
    menu_theme->addAction(system_theme);
    app_actions.append(system_theme);

    QAction *light_theme = new QAction(menu_theme);
    light_theme->setProperty(APP_PROPERTY_ENG_TEXT, "Light theme");
    light_theme->setText("Light theme");
    light_theme->setToolTip("Light theme");
    light_theme->setStatusTip("Light theme");
    connect(light_theme,   SIGNAL(triggered()),    this,   SLOT(set_light_palette()));
    menu_theme->addAction(light_theme);
    app_actions.append(light_theme);

    QAction *dark_theme = new QAction(menu_theme);
    dark_theme->setProperty(APP_PROPERTY_ENG_TEXT, "Dark theme");
    dark_theme->setText("Dark theme");
    dark_theme->setToolTip("Dark theme");
    dark_theme->setStatusTip("Dark theme");
    connect(dark_theme,   SIGNAL(triggered()),    this,   SLOT(set_dark_palette()));
    menu_theme->addAction(dark_theme);
    app_actions.append(dark_theme);

    QAction *blue_theme = new QAction(menu_theme);
    blue_theme->setProperty(APP_PROPERTY_ENG_TEXT, "Blue theme");
    blue_theme->setText("Blue theme");
    blue_theme->setToolTip("Blue theme");
    blue_theme->setStatusTip("Blue theme");
    connect(blue_theme,   SIGNAL(triggered()),    this,   SLOT(set_blue_palette()));
    menu_theme->addAction(blue_theme);
    app_actions.append(blue_theme);
}
//--------------------------------------------------------------------------------
void MainWindow::app_menu_add_lang(QMenu *menu)
{
    Q_CHECK_PTR(menu);

    QMenu *menu_language = new QMenu(menu);
    menu_language->setProperty(APP_PROPERTY_ENG_TEXT, "Language");
    menu_language->setTitle("Language");
    menu_language->setToolTip("Language");
    menu_language->setStatusTip("Language");
    menu_language->setIcon(QIcon(ICON_LANG));
    menu->addMenu(menu_language);
    app_menus.append(menu_language);

    QAction *rus_language = new QAction(menu_language);
    rus_language->setProperty(APP_PROPERTY_ENG_TEXT, "Russian");
    rus_language->setText("Russian");
    rus_language->setToolTip("Russian");
    rus_language->setStatusTip("Russian");
    rus_language->setIcon(QIcon(ICON_RU));
    connect(rus_language,   SIGNAL(triggered()),    this,   SLOT(setMenuLanguage()));
    menu_language->addAction(rus_language);
    app_actions.append(rus_language);

    QAction *eng_language = new QAction(menu_language);
    eng_language->setProperty(APP_PROPERTY_ENG_TEXT, "English");
    eng_language->setText("English");
    eng_language->setToolTip("English");
    eng_language->setStatusTip("English");
    eng_language->setIcon(QIcon(ICON_US));
    connect(eng_language,   SIGNAL(triggered()),    this,   SLOT(setMenuLanguage()));
    menu_language->addAction(eng_language);
    app_actions.append(eng_language);
}
//--------------------------------------------------------------------------------
void MainWindow::app_menu_add_style(QMenu *menu)
{
    Q_CHECK_PTR(menu);

    QMenu *menu_style = new QMenu(menu);
    menu_style->setProperty(APP_PROPERTY_ENG_TEXT, "Style");
    menu_style->setTitle("Style");
    menu_style->setToolTip("Style");
    menu_style->setStatusTip("Style");
    menu_style->setIcon(QIcon(ICON_STYLE));
    menu->addMenu(menu_style);
    app_menus.append(menu_style);

    QStringList sl;
    sl.clear();
    sl.append(QStyleFactory::keys());
    foreach (QString style, sl)
    {
        QAction *temp = new QAction(style, menu_style);
        temp->setIcon(QIcon(ICON_STYLE));
        menu_style->addAction(temp);
        connect(temp,   SIGNAL(triggered()),    this,   SLOT(setStyles()));
    }
}
//--------------------------------------------------------------------------------
void MainWindow::app_menu_add_confirm_exit(QMenu *menu)
{
    Q_CHECK_PTR(menu);

    QAction *exit = new QAction(menu);
    exit->setProperty(APP_PROPERTY_ENG_TEXT, "Do not ask when you exit");
    exit->setText("Do not ask when you exit");
    exit->setToolTip("Do not ask when you exit");
    exit->setStatusTip("Do not ask when you exit");
    exit->setCheckable(true);
    connect(exit,   SIGNAL(triggered(bool)),    this,   SLOT(closeOnExit(bool)));
    exit->setChecked(flag_close);

    app_actions.append(exit);

    menu->addAction(exit);
}
//--------------------------------------------------------------------------------
void MainWindow::app_menu_add_show_on_top(QMenu *menu)
{
    Q_CHECK_PTR(menu);

    QAction *on_top = new QAction(menu);
    on_top->setProperty(APP_PROPERTY_ENG_TEXT, "Always on top");
    on_top->setText("Always on top");
    on_top->setToolTip("Always on top");
    on_top->setStatusTip("Always on top");
    on_top->setCheckable(true);
    connect(on_top,   SIGNAL(triggered(bool)),    this,   SLOT(alwaysOnTop(bool)));
    on_top->setChecked(flag_always_on_top);

    app_actions.append(on_top);

    menu->addAction(on_top);
}
//--------------------------------------------------------------------------------
void MainWindow::app_menu_add_about(QMenu *menu)
{
    Q_CHECK_PTR(menu);

    QAction *about = new QAction(menu);
    about->setProperty(APP_PROPERTY_ENG_TEXT, "About");
    about->setText("About");
    about->setToolTip("About");
    about->setStatusTip("About");
    about->setIcon(QIcon(ICON_ABOUT));
    connect(about,   SIGNAL(triggered()),    this,   SLOT(about()));

    app_actions.append(about);

    menu->addAction(about);
}
//--------------------------------------------------------------------------------
void MainWindow::app_menu_add_help(QMenu *menu)
{
    Q_CHECK_PTR(menu);

    QAction *help = new QAction(menu);
    help->setProperty(APP_PROPERTY_ENG_TEXT, "Help");
    help->setText("Help");
    help->setToolTip("Help");
    help->setStatusTip("Help");
    help->setIcon(QIcon(ICON_ABOUT));
    connect(help,   SIGNAL(triggered()),    this,   SLOT(help()));

    app_actions.append(help);

    menu->addAction(help);
}
//--------------------------------------------------------------------------------
void MainWindow::app_toolbar_add_separator(void)
{
    Q_CHECK_PTR(toolbar);
    toolbar->addSeparator();
}
//--------------------------------------------------------------------------------
void MainWindow::app_toolbar_add_exit(void)
{
    QToolButton *btnExit = new QToolButton(this);
    btnExit->setObjectName("btnExit");
    btnExit->setIcon(QPixmap(ICON_EXIT));
    btnExit->setToolTip("Exit");
    btnExit->setStatusTip("Exit");
    btnExit->setProperty(APP_PROPERTY_ENG_TEXT, "Exit");
    connect(btnExit,    SIGNAL(clicked(bool)),  this,   SLOT(close()));

    toolbar->addWidget(btnExit);

    app_buttons.append(btnExit);
}
//--------------------------------------------------------------------------------
void MainWindow::app_toolbar_add_font(void)
{
    QToolButton *btnAppFont = new QToolButton(this);
    QToolButton *btnLogFont = new QToolButton(this);

    btnAppFont->setObjectName("btnAppFont");
    btnAppFont->setIcon(QPixmap(ICON_FONT));
    btnAppFont->setToolTip("Select the font program");
    btnAppFont->setStatusTip("Select the font program");
    btnAppFont->setProperty(APP_PROPERTY_ENG_TEXT, "Select the font program");
    connect(btnAppFont,    SIGNAL(clicked(bool)),  this,   SLOT(set_app_font()));

    btnLogFont->setObjectName("btnLogFont");
    btnLogFont->setIcon(QPixmap(ICON_FONT));
    btnLogFont->setToolTip("Select the font logging");
    btnLogFont->setStatusTip("Select the font logging");
    btnLogFont->setProperty(APP_PROPERTY_ENG_TEXT, "Select the font logging");
    connect(btnLogFont,    SIGNAL(clicked(bool)),  this,   SLOT(set_log_font()));

    toolbar->addWidget(btnAppFont);
    toolbar->addWidget(btnLogFont);

    app_buttons.append(btnAppFont);
    app_buttons.append(btnLogFont);
}
//--------------------------------------------------------------------------------
void MainWindow::app_toolbar_add_lang(void)
{
    QToolButton *btnRus = new QToolButton(this);
    QToolButton *btnEng = new QToolButton(this);

    btnRus->setObjectName("btnRus");
    btnRus->setIcon(QPixmap(ICON_RU));
    btnRus->setToolTip("Russian");
    btnRus->setStatusTip("Russian");
    btnRus->setProperty(APP_PROPERTY_ENG_TEXT, "Russian");
    connect(btnRus,    SIGNAL(clicked(bool)),  this,   SLOT(setToolBarLanguage()));

    btnEng->setObjectName("btnEng");
    btnEng->setIcon(QPixmap(ICON_US));
    btnEng->setToolTip("English");
    btnEng->setStatusTip("English");
    btnEng->setProperty(APP_PROPERTY_ENG_TEXT, "English");
    connect(btnEng,    SIGNAL(clicked(bool)),  this,   SLOT(setToolBarLanguage()));

    toolbar->addWidget(btnRus);
    toolbar->addWidget(btnEng);

    app_buttons.append(btnRus);
    app_buttons.append(btnEng);
}
//--------------------------------------------------------------------------------
void MainWindow::app_toolbar_add_style(void)
{
    QMenu *menu = new QMenu(this);

    QStringList sl;
    sl.clear();
    sl.append(QStyleFactory::keys());
    foreach (QString style, sl)
    {
        QAction *temp = new QAction(style, menu);
        menu->addAction(temp);
        connect(temp, SIGNAL(triggered()), this, SLOT(setStyles()));
    }

    QToolButton *btnStyle = new QToolButton(this);
    btnStyle->setObjectName("btnStyle");
    btnStyle->setIcon(QPixmap(ICON_STYLE));
    btnStyle->setToolTip("Style");
    btnStyle->setStatusTip("Style");
    btnStyle->setProperty(APP_PROPERTY_ENG_TEXT, "Style");
    btnStyle->setMenu(menu);
    btnStyle->setPopupMode(QToolButton::InstantPopup);
    connect(btnStyle,    SIGNAL(clicked(bool)),  this,   SLOT(close()));

    toolbar->addWidget(btnStyle);

    app_buttons.append(btnStyle);
}
//--------------------------------------------------------------------------------
void MainWindow::app_toolbar_add_about(void)
{
    QToolButton *btnAbout = new QToolButton(this);
    btnAbout->setObjectName("btnExit");
    btnAbout->setIcon(QPixmap(ICON_ABOUT));
    btnAbout->setToolTip("About");
    btnAbout->setStatusTip("About");
    btnAbout->setProperty(APP_PROPERTY_ENG_TEXT, "About");
    connect(btnAbout,    SIGNAL(clicked(bool)),  this,   SLOT(about()));

    toolbar->addWidget(btnAbout);
    app_buttons.append(btnAbout);
}
//--------------------------------------------------------------------------------
void MainWindow::app_toolbar_add_help(void)
{
    QToolButton *btnHelp = new QToolButton(this);
    btnHelp->setObjectName("btnHelp");
    btnHelp->setIcon(QPixmap(ICON_HELP));
    btnHelp->setToolTip("Help");
    btnHelp->setStatusTip("Help");
    btnHelp->setProperty(APP_PROPERTY_ENG_TEXT, "Help");
    btnHelp->setShortcut(Qt::Key_F1);
    connect(btnHelp,    SIGNAL(clicked(bool)),  this,   SLOT(help()));

    toolbar->addWidget(btnHelp);
    app_buttons.append(btnHelp);
}
//--------------------------------------------------------------------------------
void MainWindow::app_updateText(void)
{
    foreach (auto menu, app_menus)
    {
        menu->setTitle(tr(menu->property(APP_PROPERTY_ENG_TEXT).toString().toLatin1()));
    }
    foreach (auto action, app_actions)
    {
        action->setText(tr(action->property(APP_PROPERTY_ENG_TEXT).toString().toLatin1()));
        action->setToolTip(tr(action->property(APP_PROPERTY_ENG_TEXT).toString().toLatin1()));
        action->setStatusTip(tr(action->property(APP_PROPERTY_ENG_TEXT).toString().toLatin1()));
    }
    foreach (auto btn, app_buttons)
    {
        btn->setText(tr(btn->property(APP_PROPERTY_ENG_TEXT).toString().toLatin1()));
        btn->setToolTip(tr(btn->property(APP_PROPERTY_ENG_TEXT).toString().toLatin1()));
        btn->setStatusTip(tr(btn->property(APP_PROPERTY_ENG_TEXT).toString().toLatin1()));
    }
}
//--------------------------------------------------------------------------------
