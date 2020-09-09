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
#include "mainwindow.hpp"
//--------------------------------------------------------------------------------
MainWindow::MainWindow(QMainWindow *parent)
    : QMainWindow(parent),
      orgName(ORGNAME),
      appName(APPNAME),
      appVersion(QString("%1.%2.%3 (%4)")
                 .arg(VER_MAJOR)
                 .arg(VER_MINOR)
                 .arg(VER_PATCH)
                 .arg(VER_BUILD))
{
    init();
}
//--------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
#ifdef QT_DEBUG
    qDebug() << "~MainWindow()";
#endif

#if 1
    QList<MyWidget *> l_widgets = findChildren<MyWidget *>();
    foreach(MyWidget *widget, l_widgets)
    {
        widget->save_setting();
    }
#endif

#ifndef NO_LOG
    if(lb != nullptr)
    {
        lb->deleteLater();
    }
#endif
}
//--------------------------------------------------------------------------------
void MainWindow::setCentralWidget(MyWidget *widget)
{
    Q_CHECK_PTR(widget);
    c_widget = widget;

    QMainWindow::setCentralWidget(c_widget);

#if 1
    QList<MyWidget *> l_widgets = findChildren<MyWidget *>();
    foreach(MyWidget *widget, l_widgets)
    {
        widget->load_setting();
    }
#endif

#ifdef FIXED_SIZE
    setFixedSize(sizeHint());
#endif
}
//--------------------------------------------------------------------------------
void MainWindow::setCentralWidget(QWidget *widget)
{
    Q_CHECK_PTR(widget);

    QMainWindow::setCentralWidget(widget);

#ifdef FIXED_SIZE
    setFixedSize(sizeHint());
#endif
}
//--------------------------------------------------------------------------------
void MainWindow::set_c_widget(MyWidget *widget)
{
    Q_CHECK_PTR(widget);
    c_widget = widget;
}
//--------------------------------------------------------------------------------
void MainWindow::changeEvent(QEvent *event)
{
    if(event == nullptr)
    {
        return;
    }

    switch (event->type())
    {
    case QEvent::LanguageChange:
        app_updateText();
        dockwidget_updateText();
#ifndef NO_TOOLBAR
        if(toolbar)
        {
            toolbar->setWindowTitle(tr("toolbar"));
        }
#endif
#ifndef NO_STYLETOOLBAR
        if(styletoolbar)
        {
            styletoolbar->setWindowTitle(tr("styletoolbar"));
        }
#endif
//#ifndef NO_LOG
//        if(ld)
//        {
//            ld->setWindowTitle(tr("log"));
//        }
//#endif
        break;

    default:
        QWidget::changeEvent(event);
        break;
    }
}
//--------------------------------------------------------------------------------
bool MainWindow::check_exit(void)
{
    QWidgetList wl = qApp->allWidgets();
    foreach (QWidget *widget, wl)
    {
        bool ok = widget->property(P_IS_EXIT).toBool();
        if(ok)
        {
            return false;
        }
    }
    return true;
}
//--------------------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent *event)
{
    bool ok = check_exit();
    if(!ok)
    {
        emit debug("output blocked");
        event->ignore();
        return;
    }
#if 1
    QList<MyWidget *> l_widgets = findChildren<MyWidget *>();
    foreach(MyWidget *widget, l_widgets)
    {
        if(!widget->programm_is_exit())
        {
            event->ignore();
            return;
        }
    }

#else
    if(c_widget)
    {
        Q_CHECK_PTR(c_widget);
        if(!c_widget->programm_is_exit())
        {
            event->ignore();
            return;
        }
    }
#endif

    if(flag_close)
    {
        save_setting();
        event->accept();
        return;
    }

    QMessageBox msgBox;
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
#ifdef SHOW_SIZE
void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_CHECK_PTR(event);
#ifndef NO_STATUSBAR
    statusLabel1->setText(QString(QLatin1String("%1 %2"))
                          .arg(event->size().width())
                          .arg(event->size().height()));
#else
    Q_UNUSED(event)
#endif
}
#endif
//--------------------------------------------------------------------------------
void MainWindow::init(void)
{
    flag_close = true;

    load_translations();
    setWindowTitle(QString("%1 (ver. %2)")
                   .arg(appName)
                   .arg(appVersion));

    load_setting();

#ifndef NO_MENU
    createMenus();
#endif

#ifndef NO_LOG
    createLog();
#else
    connect(this, &MainWindow::info,    this, &MainWindow::log);
    connect(this, &MainWindow::debug,   this, &MainWindow::log);
    connect(this, &MainWindow::error,   this, &MainWindow::log);
    connect(this, &MainWindow::trace,   this, &MainWindow::log);
#endif

#ifdef SYSLOG_LOG
    createSysLog_dock();
#endif

#ifndef NO_TOOLBAR
    createToolBar();
#endif
#ifndef NO_STYLETOOLBAR
    createStyleToolBar();
#ifdef USE_CUSTOM_STYLE
    createCustomStyleToolBar();
#endif
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
    //setAttribute(Qt::WA_QuitOnClose);
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
        QMessageBox::critical(nullptr, "Error", "sysTranslator not loaded");
#ifdef QT_DEBUG
        qDebug() << "translator_system not loaded!";
#endif
    }
    else
    {
        qApp->installTranslator(translator_system);
    }
    //---
    translator_common = new QTranslator(this);
    res = translator_common->load(":/common");
    if(!res)
    {
        QMessageBox::critical(nullptr, "Error", "appTranslator (common) not loaded!");
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
        QMessageBox::critical(nullptr, "Error", "appTranslator (programm) not loaded!");
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
    m_app_filemenu->setProperty(P_APP_ENG_TEXT,      "File");

    m_app_optionsmenu->setTitle("Settings");
    m_app_optionsmenu->setProperty(P_APP_ENG_TEXT,   "Settings");

    m_app_windowsmenu->setTitle("Windows");
    m_app_windowsmenu->setProperty(P_APP_ENG_TEXT,   "Windows");

    m_app_helpmenu->setTitle("Help");
    m_app_helpmenu->setProperty(P_APP_ENG_TEXT,      "Help");

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
#ifndef NO_LOG
    app_menu_add_font_log(m_app_optionsmenu);
#endif
    app_menu_add_separator(m_app_optionsmenu);
    app_menu_add_theme(m_app_optionsmenu);
    app_menu_add_separator(m_app_optionsmenu);
#ifndef ONLY_ENGLISH
    app_menu_add_lang(m_app_optionsmenu);
#endif
    app_menu_add_style(m_app_optionsmenu);
    app_menu_add_separator(m_app_optionsmenu);

#ifdef USE_CUSTOM_STYLE
    app_menu_add_custom_style(m_app_optionsmenu);
    app_menu_add_separator(m_app_optionsmenu);
#endif

    //FIXME под виндой это нормально не работает, почему то
#ifndef Q_OS_WIN
    app_menu_add_show_on_top(m_app_optionsmenu);
    app_menu_add_separator(m_app_optionsmenu);
#else
    flag_always_on_top = false;
#endif

    app_menu_add_confirm_exit(m_app_optionsmenu);

    app_menu_add_about(m_app_helpmenu);
    app_menu_add_help(m_app_helpmenu);
}
//--------------------------------------------------------------------------------
void MainWindow::setMenuLanguage(void)
{
#ifndef ONLY_ENGLISH
    QAction* menu = static_cast<QAction *>(sender());
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
    QToolButton* button = reinterpret_cast<QToolButton*>(sender());
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

    MainWindow *mw = reinterpret_cast<MainWindow *>(topLevelWidget());
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
    QAction* menu = static_cast<QAction *>(sender());
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
    QPushButton *button = reinterpret_cast<QPushButton*>(sender());
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
    AboutBox *about = new AboutBox(orgName,
                                   appName,
                                   appVersion,
                                   P_EMAIL_STR,
                                   tr(P_AUTHOR_STR),
                                   P_TELEGRAM_STR);
    Q_CHECK_PTR(about);

    about->exec();
}
//--------------------------------------------------------------------------------
void MainWindow::load_main(void)
{
#ifdef QT_DEBUG
    qDebug() << "MainWindow::load_main";
#endif

    QFont font = qApp->font();
    QString font_name;
    int font_weight;
    int font_size;

    beginGroup(P_MAIN);
    font_weight = load_value(P_FONT_WEIGHT,   QFont::Normal).toInt();
    font_size   = load_value(P_FONT_SIZE,     9).toInt();
    font_name   = load_value(P_FONT_NAME,     "Liberation Sans").toString();

    if(font_size > 72) font_size = 72;
    if(font_size < 6)  font_size = 6;

    font.setFamily(font_name);
    font.setWeight(font_weight);
    font.setPointSize(font_size);

    font.setBold(false);
    font.setItalic(false);
    font.setKerning(false);

    QApplication::setFont(font);

    style_name = load_value(P_STYLE_NAME,             "Breeze").toString();
    flag_close = load_value(P_NO_ANSWER_FROM_EXIT,    true).toBool();
    flag_always_on_top = load_value(P_ALWAYS_ON_TOP,  false).toBool();

    QApplication::setStyle(QStyleFactory::create(style_name));

    //---
    state_theme = load_value(P_THEME,  SYSTEM_THEME).toInt();
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
    default:
        set_system_palette();
        break;
    }
    //---

    endGroup();

    restoreState(load_value(P_WINDOW_STATE).toByteArray());
    restoreGeometry(load_value(P_GEOMETRY).toByteArray());
}
//--------------------------------------------------------------------------------
void MainWindow::save_main(void)
{
#ifdef QT_DEBUG
    qDebug() << "MainWindow::save_main";
#endif

    beginGroup(P_MAIN);
    save_value(P_FONT_NAME,      QApplication::font().family());
    save_value(P_FONT_WEIGHT,    QApplication::font().weight());
    save_value(P_FONT_SIZE,      QApplication::font().pointSize());
    save_value(P_STYLE_NAME,     style_name);

    save_value(P_NO_ANSWER_FROM_EXIT, flag_close);
    save_value(P_ALWAYS_ON_TOP,   flag_always_on_top);

    save_value(P_THEME,         state_theme);

    endGroup();

    save_value(P_GEOMETRY,      saveGeometry());
    save_value(P_WINDOW_STATE,   saveState());
}
//--------------------------------------------------------------------------------
void MainWindow::load_setting(void)
{
#ifdef QT_DEBUG
    qDebug() << "MainWindow::load_setting";
#endif
    load_main();
}
//--------------------------------------------------------------------------------
void MainWindow::save_setting(void)
{
#ifdef QT_DEBUG
    qDebug() << "MainWindow::save_setting";
#endif
    save_main();
}
//--------------------------------------------------------------------------------
#ifndef NO_LOG
void MainWindow::createLog(void)
{
    lb = new LogBox(this);
    Q_CHECK_PTR(lb);

    lb->setObjectName("log_widget");

    connect(this,   &MainWindow::clear_log, lb, &LogBox::clear);

    connect(this,   &MainWindow::info,      lb, &LogBox::infoLog);
    connect(this,   &MainWindow::debug,     lb, &LogBox::debugLog);
    connect(this,   &MainWindow::error,     lb, &LogBox::errorLog);
    connect(this,   &MainWindow::trace,     lb, &LogBox::traceLog);

    connect(this,   &MainWindow::colorLog,  lb, &LogBox::colorLog);

    add_dock_widget("log", "log", Qt::BottomDockWidgetArea, lb);
}
#endif
//--------------------------------------------------------------------------------
void MainWindow::createSysLog_dock(void)
{
    w_syslog = new SysLog("syslog", this);
    Q_CHECK_PTR(w_syslog);

    connect(w_syslog,   &SysLog::info,  this,   &MainWindow::info);
    connect(w_syslog,   &SysLog::debug, this,   &MainWindow::debug);
    connect(w_syslog,   &SysLog::error, this,   &MainWindow::error);
    connect(w_syslog,   &SysLog::trace, this,   &MainWindow::trace);

    connect(this,   SIGNAL(syslog(int,int,QString)),            w_syslog,   SLOT(syslog(int,int,QString)));
    connect(this,   SIGNAL(syslog(QDateTime,int,int,QString)),  w_syslog,   SLOT(syslog(QDateTime,int,int,QString)));

//    addDockWidget(Qt::BottomDockWidgetArea, syslog_dock);
    add_dock_widget("syslog", "syslog", Qt::BottomDockWidgetArea, w_syslog);
}
//--------------------------------------------------------------------------------
#ifndef NO_TOOLBAR
void MainWindow::createToolBar(void)
{
    toolbar = new QToolBar("toolbar", this);
    Q_CHECK_PTR(toolbar);

    toolbar->setObjectName("toolbar");
    toolbar->setMovable(true);

    addToolBar(Qt::TopToolBarArea, toolbar);

#ifndef NO_TOOLBAR_BUTTON_EXIT
    app_toolbar_add_exit();
#ifndef NO_TOOLBAR_SEPARATORS
    app_toolbar_add_separator();
#endif
#endif

#ifndef NO_TOOLBAR_BUTTON_FONT
#ifndef NO_LOG
    app_toolbar_add_font();
#endif
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
#ifdef USE_CUSTOM_STYLE
    app_toolbar_add_custom_style();
#endif
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

#ifdef TOOLBAR_ORIENTATION
    toolbar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    connect(toolbar, &QToolBar::orientationChanged, [this]()
    {
        switch(toolbar->orientation())
        {
        case Qt::Horizontal:
            toolbar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
            break;

        case Qt::Vertical:
            toolbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            break;
        }
    });
#endif


#ifndef NO_TOOLBAR
    add_windowsmenu_action(toolbar, toolbar->toggleViewAction());
#endif
}
#endif
//--------------------------------------------------------------------------------
#ifndef NO_STYLETOOLBAR
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

#ifdef TOOLBAR_ORIENTATION
    styletoolbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(styletoolbar, &QToolBar::orientationChanged, [this]()
    {
        switch(styletoolbar->orientation())
        {
        case Qt::Horizontal:
            styletoolbar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
            break;

        case Qt::Vertical:
            styletoolbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            break;
        }
    });
#endif

    addToolBar(Qt::LeftToolBarArea, styletoolbar);
    add_windowsmenu_action(styletoolbar, styletoolbar->toggleViewAction());
}
#endif
//--------------------------------------------------------------------------------
#ifndef NO_STYLETOOLBAR
#ifdef USE_CUSTOM_STYLE
void MainWindow::set_norton_style(void)
{
    QFile file(":/themes_qss/Norton Commander.qss");
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        qApp->setStyleSheet(ba.data());
    }
    else
    {
        emit error("theme file not open");
    }
}
#endif
#endif
//--------------------------------------------------------------------------------
#ifndef NO_STYLETOOLBAR
#ifdef USE_CUSTOM_STYLE
void MainWindow::createCustomStyleToolBar(void)
{
    //TODO пробую самописные стили
    QToolBar *customStyletoolbar = new QToolBar(tr("customstyletoolbar"), this);
    Q_CHECK_PTR(customStyletoolbar);

    QPushButton *btnTemp = new QPushButton(this);
    btnTemp->setToolTip("Norton");
    btnTemp->setText("Norton");
    connect(btnTemp,    &QPushButton::clicked,  this,   &MainWindow::set_norton_style);

    customStyletoolbar->addWidget(btnTemp);

    addToolBar(Qt::LeftToolBarArea, customStyletoolbar);
    add_windowsmenu_action(customStyletoolbar, customStyletoolbar->toggleViewAction());
}
#endif
#endif
//--------------------------------------------------------------------------------
void MainWindow::help(void)
{
    if(QFile::exists("index.html"))
        HelpBrowser::showPage("index.html", false);
    else
        HelpBrowser::showPage("qrc:/index.html", false);
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
        QMessageBox::critical(nullptr,
                              QObject::tr("Systray"),
                              QObject::tr("I couldn't detect any system tray on this system."));
        return;
    }

    trayIconMenu = new QMenu(this);

    add_new_action(trayIconMenu,    "Minimize", nullptr,    &MainWindow::showMinimized);
    add_new_action(trayIconMenu,    "Maximize", nullptr,    &MainWindow::showMaximized);
    add_new_action(trayIconMenu,    "Restore",  nullptr,    &MainWindow::showNormal);

    trayIconMenu->addSeparator();
    add_new_action(trayIconMenu,    "Quit",     nullptr,  &MainWindow::quit);

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
void MainWindow::change_value(void)
{
    emit info(QString("change_value %1").arg(sender()->metaObject()->className()));
}
//--------------------------------------------------------------------------------
bool MainWindow::add_windowsmenu_action(QWidget *widget, QAction *action)
{
    Q_CHECK_PTR(widget);
    Q_CHECK_PTR(action);
    Q_CHECK_PTR(m_app_windowsmenu);

//    l_docs.append(widget);

#if 0
    ToolButtonAction *tb_action = new ToolButtonAction(action);
    connect(tb_action->toolButton(), SIGNAL(clicked()), this, SLOT(change_value()));

    m_app_windowsmenu->addAction(tb_action);
#else
    m_app_windowsmenu->addAction(action);
#endif

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
                                 QWidget *widget,
                                 bool no_dock_position)
{
    Q_CHECK_PTR(widget);

    emit debug(title);

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
    //    dw->setWindowTitle(title);
    dw->setWindowTitle(tr(title.toLatin1()));
    dw->setProperty(P_DOCKWIDGET_ENG_TEXT, title);

    if(no_dock_position == false)
    {
        // Dock_position - это стрелки сбоку
        Dock_position *dp = new Dock_position(objectname, this);

        QWidget *nw = new QWidget(this);
        QHBoxLayout *hbox = new QHBoxLayout();
        hbox->setMargin(0);
        hbox->setSpacing(0);
        hbox->addWidget(widget);
        hbox->addWidget(dp);
        nw->setLayout(hbox);

        dw->setWidget(nw);
    }
    else
    {
        dw->setWidget(widget);
    }

    dw->setAllowedAreas(Qt::AllDockWidgetAreas);
    //    dw->setAllowedAreas(Qt::LeftDockWidgetArea |
    //                        Qt::RightDockWidgetArea |
    //                        Qt::TopDockWidgetArea |
    //                        Qt::BottomDockWidgetArea);

    connect(dw, &QDockWidget::dockLocationChanged, this, &MainWindow::dockLocationChanged);

    addDockWidget(area, dw);
    add_windowsmenu_action(dw, dw->toggleViewAction());
    load_setting();

    return true;
}
//--------------------------------------------------------------------------------
void MainWindow::dockLocationChanged(Qt::DockWidgetArea area)
{
    //qDebug() << area;
    Q_UNUSED(area)
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
#ifndef NO_LOG
void MainWindow::set_log_font(void)
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, lb->get_font());
    if(ok)
    {
        lb->set_font(font);
    }
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
    palette.setBrush(QPalette::WindowText,  QBrush(QColor(static_cast<QRgb>(0x000000)), Qt::SolidPattern));
    palette.setBrush(QPalette::Button,      QBrush(QColor(static_cast<QRgb>(0xd4d4d4)), Qt::SolidPattern));
    palette.setBrush(QPalette::Light,       QBrush(QColor(static_cast<QRgb>(0xffffff)), Qt::SolidPattern));
    palette.setBrush(QPalette::Text,        QBrush(QColor(static_cast<QRgb>(0x000000)), Qt::SolidPattern));
    palette.setBrush(QPalette::BrightText,  QBrush(QColor(static_cast<QRgb>(0xffffff)), Qt::SolidPattern));
    palette.setBrush(QPalette::ButtonText,  QBrush(QColor(static_cast<QRgb>(0x000000)), Qt::SolidPattern));
    palette.setBrush(QPalette::Base,        QBrush(QColor(static_cast<QRgb>(0xffffff)), Qt::SolidPattern));
    palette.setBrush(QPalette::Window,      QBrush(QColor(static_cast<QRgb>(0xd4d4d4)), Qt::SolidPattern));
    palette.setBrush(QPalette::Shadow,      QBrush(QColor(static_cast<QRgb>(0x404040)), Qt::SolidPattern));
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
    menu->setProperty(P_APP_ENG_TEXT, text);
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
    action->setProperty(P_APP_ENG_TEXT, text);

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
    action->setProperty(P_APP_ENG_TEXT, text);

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
    exit->setProperty(P_APP_ENG_TEXT, "Exit");
    exit->setText("Exit");
    exit->setToolTip("Exit");
    exit->setStatusTip("Exit");
    exit->setIcon(QIcon(P_ICON_EXIT));
    connect(exit,   SIGNAL(triggered()),    this,   SLOT(close()));

    app_actions.append(exit);

    menu->addAction(exit);
}
//--------------------------------------------------------------------------------
void MainWindow::app_menu_add_font_programm(QMenu *menu)
{
    Q_CHECK_PTR(menu);

    QAction *font_programm = new QAction(menu);
    font_programm->setProperty(P_APP_ENG_TEXT, "Select the font program");
    font_programm->setText("Select the font program");
    font_programm->setToolTip("Select the font program");
    font_programm->setStatusTip("Select the font program");
    font_programm->setIcon(QIcon(P_ICON_FONT));
    connect(font_programm,   SIGNAL(triggered()),    this,   SLOT(set_app_font()));

    app_actions.append(font_programm);

    menu->addAction(font_programm);
}
//--------------------------------------------------------------------------------
#ifndef NO_LOG
void MainWindow::app_menu_add_font_log(QMenu *menu)
{
    Q_CHECK_PTR(menu);

    QAction *font_log = new QAction(menu);
    font_log->setProperty(P_APP_ENG_TEXT, "Select the font logging");
    font_log->setText("Select the font logging");
    font_log->setToolTip("Select the font logging");
    font_log->setStatusTip("Select the font logging");
    font_log->setIcon(QIcon(P_ICON_FONT));
    connect(font_log,   SIGNAL(triggered()),    this,   SLOT(set_log_font()));

    app_actions.append(font_log);

    menu->addAction(font_log);
}
#endif
//--------------------------------------------------------------------------------
void MainWindow::app_menu_add_theme(QMenu *menu)
{
    Q_CHECK_PTR(menu);

    QMenu *menu_theme = new QMenu(menu);
    menu_theme->setProperty(P_APP_ENG_TEXT, "Themes");
    menu_theme->setTitle("Themes");
    menu_theme->setToolTip("Themes");
    menu_theme->setStatusTip("Themes");
    menu->addMenu(menu_theme);
    app_menus.append(menu_theme);

    QAction *system_theme = new QAction(menu_theme);
    system_theme->setProperty(P_APP_ENG_TEXT, "System theme");
    system_theme->setText("System theme");
    system_theme->setToolTip("System theme");
    system_theme->setStatusTip("System theme");
    connect(system_theme,   SIGNAL(triggered()),    this,   SLOT(set_system_palette()));
    menu_theme->addAction(system_theme);
    app_actions.append(system_theme);

    QAction *light_theme = new QAction(menu_theme);
    light_theme->setProperty(P_APP_ENG_TEXT, "Light theme");
    light_theme->setText("Light theme");
    light_theme->setToolTip("Light theme");
    light_theme->setStatusTip("Light theme");
    connect(light_theme,   SIGNAL(triggered()),    this,   SLOT(set_light_palette()));
    menu_theme->addAction(light_theme);
    app_actions.append(light_theme);

    QAction *dark_theme = new QAction(menu_theme);
    dark_theme->setProperty(P_APP_ENG_TEXT, "Dark theme");
    dark_theme->setText("Dark theme");
    dark_theme->setToolTip("Dark theme");
    dark_theme->setStatusTip("Dark theme");
    connect(dark_theme,   SIGNAL(triggered()),    this,   SLOT(set_dark_palette()));
    menu_theme->addAction(dark_theme);
    app_actions.append(dark_theme);

    QAction *blue_theme = new QAction(menu_theme);
    blue_theme->setProperty(P_APP_ENG_TEXT, "Blue theme");
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
    menu_language->setProperty(P_APP_ENG_TEXT, "Language");
    menu_language->setTitle("Language");
    menu_language->setToolTip("Language");
    menu_language->setStatusTip("Language");
    menu_language->setIcon(QIcon(P_ICON_LANG));
    menu->addMenu(menu_language);
    app_menus.append(menu_language);

    QAction *rus_language = new QAction(menu_language);
    rus_language->setProperty(P_APP_ENG_TEXT, "Russian");
    rus_language->setText("Russian");
    rus_language->setToolTip("Russian");
    rus_language->setStatusTip("Russian");
    rus_language->setIcon(QIcon(P_ICON_RU));
    connect(rus_language,   SIGNAL(triggered()),    this,   SLOT(setMenuLanguage()));
    menu_language->addAction(rus_language);
    app_actions.append(rus_language);

    QAction *eng_language = new QAction(menu_language);
    eng_language->setProperty(P_APP_ENG_TEXT, "English");
    eng_language->setText("English");
    eng_language->setToolTip("English");
    eng_language->setStatusTip("English");
    eng_language->setIcon(QIcon(P_ICON_US));
    connect(eng_language,   SIGNAL(triggered()),    this,   SLOT(setMenuLanguage()));
    menu_language->addAction(eng_language);
    app_actions.append(eng_language);
}
//--------------------------------------------------------------------------------
void MainWindow::app_menu_add_style(QMenu *menu)
{
    Q_CHECK_PTR(menu);

    QMenu *menu_style = new QMenu(menu);
    menu_style->setProperty(P_APP_ENG_TEXT, "Style");
    menu_style->setTitle("Style");
    menu_style->setToolTip("Style");
    menu_style->setStatusTip("Style");
    menu_style->setIcon(QIcon(P_ICON_STYLE));
    menu->addMenu(menu_style);
    app_menus.append(menu_style);

    QStringList sl;
    sl.clear();
    sl.append(QStyleFactory::keys());
    foreach (QString style, sl)
    {
        QAction *temp = new QAction(style, menu_style);
        temp->setIcon(QIcon(P_ICON_STYLE));
        menu_style->addAction(temp);
        connect(temp,   SIGNAL(triggered()),    this,   SLOT(setStyles()));
    }
}
//--------------------------------------------------------------------------------
#ifdef USE_CUSTOM_STYLE
void MainWindow::app_menu_add_custom_style(QMenu *menu)
{
    //TODO пробую самописные стили

    Q_CHECK_PTR(menu);

    QMenu *menu_style = new QMenu(menu);
    menu_style->setProperty(P_APP_ENG_TEXT, "Custom style");
    menu_style->setTitle("Custom style");
    menu_style->setToolTip("Custom style");
    menu_style->setStatusTip("Custom style");
    menu_style->setIcon(QIcon(ICON_STYLE));
    menu->addMenu(menu_style);
    app_menus.append(menu_style);

    QAction *temp = new QAction("Norton", menu_style);
    temp->setIcon(QIcon(ICON_STYLE));

    menu_style->addAction(temp);
    connect(temp,   &QAction::triggered,    this,   &MainWindow::set_norton_style);
}
#endif
//--------------------------------------------------------------------------------
void MainWindow::app_menu_add_confirm_exit(QMenu *menu)
{
    Q_CHECK_PTR(menu);

    QAction *exit = new QAction(menu);
    exit->setProperty(P_APP_ENG_TEXT, "Do not ask when you exit");
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
    on_top->setProperty(P_APP_ENG_TEXT, "Always on top");
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
    about->setProperty(P_APP_ENG_TEXT, "About");
    about->setText("About");
    about->setToolTip("About");
    about->setStatusTip("About");
    about->setIcon(QIcon(ICON_PROGRAMM));
    connect(about,   SIGNAL(triggered()),    this,   SLOT(about()));

    app_actions.append(about);

    menu->addAction(about);
}
//--------------------------------------------------------------------------------
void MainWindow::app_menu_add_help(QMenu *menu)
{
    Q_CHECK_PTR(menu);

    QAction *help = new QAction(menu);
    help->setProperty(P_APP_ENG_TEXT, "Help");
    help->setText("Help");
    help->setToolTip("Help");
    help->setStatusTip("Help");
    help->setIcon(QIcon(P_ICON_HELP));
    connect(help,   SIGNAL(triggered()),    this,   SLOT(help()));

    app_actions.append(help);

    menu->addAction(help);
}
//--------------------------------------------------------------------------------
void MainWindow::app_toolbar_add_separator(void)
{
#ifndef NO_TOOLBAR
    Q_CHECK_PTR(toolbar);
    toolbar->addSeparator();
#endif
}
//--------------------------------------------------------------------------------
void MainWindow::app_toolbar_add_exit(void)
{
    QToolButton *btnExit = new QToolButton(this);
    btnExit->setObjectName("btnExit");
    btnExit->setIcon(QPixmap(P_ICON_EXIT));
    btnExit->setToolTip("Exit");
    btnExit->setStatusTip("Exit");
    btnExit->setProperty(P_APP_ENG_TEXT, "Exit");
    connect(btnExit,    SIGNAL(clicked(bool)),  this,   SLOT(close()));

#ifndef NO_TOOLBAR
    toolbar->addWidget(btnExit);
#endif

    app_buttons.append(btnExit);
}
//--------------------------------------------------------------------------------
#ifndef NO_LOG
void MainWindow::app_toolbar_add_font(void)
{
    QToolButton *btnAppFont = new QToolButton(this);
    QToolButton *btnLogFont = new QToolButton(this);

    btnAppFont->setObjectName("btnAppFont");
    btnAppFont->setIcon(QPixmap(P_ICON_FONT));
    btnAppFont->setToolTip("Select the font program");
    btnAppFont->setStatusTip("Select the font program");
    btnAppFont->setProperty(P_APP_ENG_TEXT, "Select the font program");
    connect(btnAppFont,    SIGNAL(clicked(bool)),  this,   SLOT(set_app_font()));

    btnLogFont->setObjectName("btnLogFont");
    btnLogFont->setIcon(QPixmap(P_ICON_FONT));
    btnLogFont->setToolTip("Select the font logging");
    btnLogFont->setStatusTip("Select the font logging");
    btnLogFont->setProperty(P_APP_ENG_TEXT, "Select the font logging");
    connect(btnLogFont,    SIGNAL(clicked(bool)),  this,   SLOT(set_log_font()));

#ifndef NO_TOOLBAR
    toolbar->addWidget(btnAppFont);
    toolbar->addWidget(btnLogFont);
#endif

    app_buttons.append(btnAppFont);
    app_buttons.append(btnLogFont);
}
#endif
//--------------------------------------------------------------------------------
void MainWindow::app_toolbar_add_lang(void)
{
    QToolButton *btnRus = new QToolButton(this);
    QToolButton *btnEng = new QToolButton(this);

    btnRus->setObjectName("btnRus");
    btnEng->setObjectName("btnEng");

    btnRus->setObjectName("btnRus");
    btnRus->setIcon(QPixmap(P_ICON_RU));
    btnRus->setToolTip("Russian");
    btnRus->setStatusTip("Russian");
    btnRus->setProperty(P_APP_ENG_TEXT, "Russian");
    connect(btnRus,    SIGNAL(clicked(bool)),  this,   SLOT(setToolBarLanguage()));

    btnEng->setObjectName("btnEng");
    btnEng->setIcon(QPixmap(P_ICON_US));
    btnEng->setToolTip("English");
    btnEng->setStatusTip("English");
    btnEng->setProperty(P_APP_ENG_TEXT, "English");
    connect(btnEng,    SIGNAL(clicked(bool)),  this,   SLOT(setToolBarLanguage()));

#ifndef NO_TOOLBAR
    toolbar->addWidget(btnRus);
    toolbar->addWidget(btnEng);
#endif

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
    btnStyle->setIcon(QPixmap(P_ICON_STYLE));
    btnStyle->setToolTip("Style");
    btnStyle->setStatusTip("Style");
    btnStyle->setProperty(P_APP_ENG_TEXT, "Style");
    btnStyle->setMenu(menu);
    btnStyle->setPopupMode(QToolButton::InstantPopup);
    connect(btnStyle,    SIGNAL(clicked(bool)),  this,   SLOT(close()));

#ifndef NO_TOOLBAR
    toolbar->addWidget(btnStyle);
#endif

    app_buttons.append(btnStyle);
}
//--------------------------------------------------------------------------------
#ifdef USE_CUSTOM_STYLE
void MainWindow::app_toolbar_add_custom_style(void)
{
    //TODO пробую самописные стили

    QMenu *menu = new QMenu(this);

    QAction *a_norton = new QAction("Norton", menu);
    menu->addAction(a_norton);

    QToolButton *btnStyle = new QToolButton(this);
    btnStyle->setObjectName("btn_norton");
    btnStyle->setIcon(QPixmap(ICON_STYLE));
    btnStyle->setMenu(menu);
    btnStyle->setPopupMode(QToolButton::InstantPopup);

    connect(a_norton,   &QAction::triggered,    this,   &MainWindow::set_norton_style);

#ifndef NO_TOOLBAR
    toolbar->addWidget(btnStyle);
#endif
}
#endif
//--------------------------------------------------------------------------------
void MainWindow::app_toolbar_add_about(void)
{
    QToolButton *btnAbout = new QToolButton(this);
    btnAbout->setObjectName("btnExit");
    btnAbout->setIcon(QPixmap(ICON_PROGRAMM));
    btnAbout->setToolTip("About");
    btnAbout->setStatusTip("About");
    btnAbout->setProperty(P_APP_ENG_TEXT, "About");
    connect(btnAbout,    SIGNAL(clicked(bool)),  this,   SLOT(about()));

#ifndef NO_TOOLBAR
    toolbar->addWidget(btnAbout);
#endif
    app_buttons.append(btnAbout);
}
//--------------------------------------------------------------------------------
void MainWindow::app_toolbar_add_help(void)
{
    QToolButton *btnHelp = new QToolButton(this);
    btnHelp->setObjectName("btnHelp");
    btnHelp->setIcon(QPixmap(P_ICON_HELP));
    btnHelp->setToolTip("Help");
    btnHelp->setStatusTip("Help");
    btnHelp->setProperty(P_APP_ENG_TEXT, "Help");
    btnHelp->setShortcut(Qt::Key_F1);
    connect(btnHelp,    SIGNAL(clicked(bool)),  this,   SLOT(help()));

#ifndef NO_TOOLBAR
    toolbar->addWidget(btnHelp);
#endif
    app_buttons.append(btnHelp);
}
//--------------------------------------------------------------------------------
void MainWindow::app_updateText(void)
{
    foreach (auto menu, app_menus)
    {
        menu->setTitle(tr(menu->property(P_APP_ENG_TEXT).toString().toLatin1()));
    }
    foreach (auto action, app_actions)
    {
        action->setText(tr(action->property(P_APP_ENG_TEXT).toString().toLatin1()));
        action->setToolTip(tr(action->property(P_APP_ENG_TEXT).toString().toLatin1()));
        action->setStatusTip(tr(action->property(P_APP_ENG_TEXT).toString().toLatin1()));
    }
    foreach (auto btn, app_buttons)
    {
        btn->setText(tr(btn->property(P_APP_ENG_TEXT).toString().toLatin1()));
        btn->setToolTip(tr(btn->property(P_APP_ENG_TEXT).toString().toLatin1()));
        btn->setStatusTip(tr(btn->property(P_APP_ENG_TEXT).toString().toLatin1()));
    }
}
//--------------------------------------------------------------------------------
void MainWindow::dockwidget_updateText(void)
{
    QList<QDockWidget *> ldw = findChildren<QDockWidget *>();
    foreach (QDockWidget *dock, ldw)
    {
        dock->setWindowTitle(tr(dock->property(P_DOCKWIDGET_ENG_TEXT).toString().toLatin1()));
    }
}
//--------------------------------------------------------------------------------
