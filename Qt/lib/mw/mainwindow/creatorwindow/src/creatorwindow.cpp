/*********************************************************************************
**                                                                              **
**     Copyright (C) 2023                                                       **
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
#include "creatorwindow.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "mylogger.hpp"
//--------------------------------------------------------------------------------
MyLogger *logger = nullptr;
//--------------------------------------------------------------------------------
CreatorWindow::CreatorWindow(QWidget *parent)
    : QMainWindow(parent),
    orgName(ORGNAME),
    appName(APPNAME),
    appVersion(QString("%1.%2.%3.%4")
                   .arg(VER_MAJOR)
                   .arg(VER_MINOR)
                   .arg(VER_PATCH)
                   .arg(VER_BUILD))
{
    init();
}
//--------------------------------------------------------------------------------
CreatorWindow::~CreatorWindow()
{
    qInstallMessageHandler(0);

#ifdef QT_DEBUG
    qDebug() << "~CreatorWindow()";
#endif

    QList<MyWidget *> l_widgets = findChildren<MyWidget *>();
    foreach(MyWidget *widget, l_widgets)
    {
        Q_ASSERT(widget);
        widget->save_setting();
    }

#ifndef NO_LOG
    if(lb)
    {
        delete lb;
    }
#endif

#ifdef LOGGER_ON
    if(logger)
    {
        QObject::disconnect(logger,    &MyLogger::info,    this,   &CreatorWindow::info);
        QObject::disconnect(logger,    &MyLogger::debug,   this,   &CreatorWindow::debug);
        QObject::disconnect(logger,    &MyLogger::error,   this,   &CreatorWindow::error);
        QObject::disconnect(logger,    &MyLogger::trace,   this,   &CreatorWindow::trace);
        delete logger;
    }
#endif
}
//--------------------------------------------------------------------------------
void CreatorWindow::setCentralWidget(MyWidget *widget)
{
    Q_ASSERT(widget);
    c_widget = widget;

    QMainWindow::setCentralWidget(c_widget);

    QList<MyWidget *> l_widgets = findChildren<MyWidget *>();
    foreach(MyWidget *w, l_widgets)
    {
        w->load_setting();
    }

#ifdef FIXED_SIZE
    setFixedSize(sizeHint());
#endif
}
//--------------------------------------------------------------------------------
void CreatorWindow::setCentralWidget(QWidget *widget)
{
    Q_ASSERT(widget);

    QMainWindow::setCentralWidget(widget);

#ifdef FIXED_SIZE
    setFixedSize(sizeHint());
#endif
}
//--------------------------------------------------------------------------------
void CreatorWindow::set_c_widget(MyWidget *widget)
{
    Q_ASSERT(widget);
    c_widget = widget;
}
//--------------------------------------------------------------------------------
void CreatorWindow::changeEvent(QEvent *event)
{
    if(event == nullptr)
    {
        return;
    }

    switch (event->type())
    {
    case QEvent::LanguageChange:
        dockwidget_updateText();
#ifndef NO_STYLETOOLBAR
        if(styletoolbar)
        {
            styletoolbar->setWindowTitle(tr("styletoolbar"));
        }
#endif
        break;

    default:
        QWidget::changeEvent(event);
        break;
    }
}
//--------------------------------------------------------------------------------
bool CreatorWindow::check_exit(void)
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
void CreatorWindow::closeEvent(QCloseEvent *event)
{
    bool ok = check_exit();
    if(!ok)
    {
        emit debug("output blocked");
        event->ignore();
        return;
    }

    QList<MyWidget *> l_widgets = findChildren<MyWidget *>();
    foreach(MyWidget *widget, l_widgets)
    {
        if(!widget->programm_is_exit())
        {
            event->ignore();
            return;
        }
    }

    if(flag_close)
    {
        save_setting();
        event->accept();
        return;
    }

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(CreatorWindow::tr("Exit the program"));
    msgBox.setText(CreatorWindow::tr("Exit the program?"));
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
void CreatorWindow::resizeEvent(QResizeEvent *event)
{
    Q_ASSERT(event);
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
void CreatorWindow::init(void)
{
    flag_close = true;

    app_mainBar = menuBar();
    // m_app_windowsmenu = new QMenu(app_mainBar);

#ifdef APPTITLE
    appTitle = APPTITLE;
#else
    appTitle = APPNAME;
#endif

    QShortcut *keyF1 = new QShortcut(this);
    keyF1->setKey(Qt::Key_F1);
    connect(keyF1,  &QShortcut::activated,
            this,   &CreatorWindow::help);

    load_translations();
    setWindowTitle(QString("%1 (ver. %2)")
                       .arg(appTitle)
                       .arg(appVersion));
    setObjectName("CreatorWindow");

    load_setting();

#ifndef NO_LOG
    createLog();
#else
    connect(this, &CreatorWindow::info,    this, &CreatorWindow::log);
    connect(this, &CreatorWindow::debug,   this, &CreatorWindow::log);
    connect(this, &CreatorWindow::error,   this, &CreatorWindow::log);
    connect(this, &CreatorWindow::trace,   this, &CreatorWindow::log);
#endif

#ifdef SYSLOG_LOG
    createSysLog_dock();
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

#if defined (DEMO) && defined (DEMO_YEAR) && defined (DEMO_MONTH) && defined (DEMO_DAY)
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

#ifdef LOGGER_ON
    logger = new MyLogger();

    qInstallMessageHandler(myMessageOutput);
    if(logger)
    {
        QObject::connect(logger,    &MyLogger::info,    this,   &CreatorWindow::info);
        QObject::connect(logger,    &MyLogger::debug,   this,   &CreatorWindow::debug);
        QObject::connect(logger,    &MyLogger::error,   this,   &CreatorWindow::error);
        QObject::connect(logger,    &MyLogger::trace,   this,   &CreatorWindow::trace);
    }
#endif

    // не стоит именно здесь взводить эти аттрибуты, лучше в mywidget
    setAttribute(Qt::WA_DeleteOnClose);
    //setAttribute(Qt::WA_QuitOnClose);
}
//--------------------------------------------------------------------------------
//    if(now.date().year()        >= DEMO_YEAR &&
//            now.date().month()  >= DEMO_MONTH &&
//            now.date().day()    >  DEMO_DAY)
#if defined (DEMO) && defined (DEMO_YEAR) && defined (DEMO_MONTH) && defined (DEMO_DAY)
void CreatorWindow::check_date(void)
{
    QDateTime now = QDateTime::currentDateTime();
    QDateTime xxx = QDateTime(QDate(DEMO_YEAR, DEMO_MONTH, DEMO_DAY));
    if(now > xxx)
    {
        int x = rand() % 2;
        switch(x)
        {
        case 0:
            QTimer::singleShot(3000 + rand() % 10000, this, &CreatorWindow::kill);
            break;
        case 1:
            QTimer::singleShot(3000 + rand() % 10000, this, &CreatorWindow::kill2);
            break;
        }
    }
}
#endif
//--------------------------------------------------------------------------------
#if defined (DEMO) && defined (DEMO_YEAR) && defined (DEMO_MONTH) && defined (DEMO_DAY)
void CreatorWindow::kill(void)
{
    int a = 5;
    int b = 0;
    int c = a / b;

    qDebug() << a << b << c;
}
#endif
//--------------------------------------------------------------------------------
#if defined (DEMO) && defined (DEMO_YEAR) && defined (DEMO_MONTH) && defined (DEMO_DAY)
void CreatorWindow::kill2(void)
{
    QLabel *label = 0;
    label->show();
}
#endif
//--------------------------------------------------------------------------------
void CreatorWindow::load_translations()
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
    TRANSLATOR translator_ru;
    translator_ru.translator_obj = new QTranslator(this);
    translator_ru.translator_file = ":/lang/lang_ru.qm";
    translator_ru.locale_name = "ru_RU";
    translator_ru.language = "Russian";
    translator_ru.property = P_RU;
    translator_ru.icon_name = P_ICON_RU;
    l_translators.append(translator_ru);

    TRANSLATOR translator_it;
    translator_it.translator_obj = new QTranslator(this);
    translator_it.translator_file = ":/lang/lang_it.qm";
    translator_it.locale_name = "it_IT";
    translator_it.language = "Italiano";
    translator_it.property = P_IT;
    translator_it.icon_name = P_ICON_IT;
    l_translators.append(translator_it);

    foreach (TRANSLATOR translator, l_translators)
    {
        res = translator.translator_obj->load(QLocale(), translator.translator_file);
        if(!res)
        {
            QMessageBox::critical(nullptr,
                                  "Error",
                                  QString("appTranslator (%1) not loaded!")
                                      .arg(translator.translator_file));
#ifdef QT_DEBUG
            qDebug() << QString("appTranslator (%1) not loaded!")
                            .arg(translator.translator_file);

#endif
        }
    }

    QLocale locale = QLocale();
#ifdef QT_DEBUG
    qDebug() << "### locale" << locale.name();
#endif
    foreach (TRANSLATOR translator, l_translators)
    {
        if(locale.name() == translator.locale_name)
        {
            qApp->installTranslator(translator.translator_obj);
            emit updateLanguage();
        }
    }
#endif
}
//--------------------------------------------------------------------------------
void CreatorWindow::choice_translator(QAction *menu)
{
    foreach (TRANSLATOR translator, l_translators)
    {
        qApp->removeTranslator(translator.translator_obj);
    }
    qApp->installTranslator(translator_system);

    QString p_lang = menu->property(P_LANG).toString();

    auto trans_it = std::find_if(
        l_translators.begin(),
        l_translators.end(),
        [p_lang](TRANSLATOR trans){ return trans.property == p_lang; }
        );
    if (trans_it != l_translators.end())
    {
        qApp->installTranslator(trans_it->translator_obj);
        emit updateLanguage();
    }
}
//--------------------------------------------------------------------------------
void CreatorWindow::setMenuLanguage(void)
{
#ifndef ONLY_ENGLISH
    QAction* menu = static_cast<QAction *>(sender());
    Q_ASSERT(menu);

    choice_translator(menu);
#endif
}
//--------------------------------------------------------------------------------
void CreatorWindow::setToolBarLanguage(void)
{
#ifndef ONLY_ENGLISH
    QAction* menu = reinterpret_cast<QAction*>(sender());
    Q_ASSERT(menu);

    choice_translator(menu);
#endif
}
//--------------------------------------------------------------------------------
void CreatorWindow::closeOnExit(bool state)
{
    flag_close = state;
}
//--------------------------------------------------------------------------------
void CreatorWindow::alwaysOnTop(bool state)
{
    flag_always_on_top = state;

    CreatorWindow *mw = reinterpret_cast<CreatorWindow *>(QApplication::activeWindow());
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
void CreatorWindow::move_to_center(void)
{
    CreatorWindow *mw = reinterpret_cast<CreatorWindow *>(QApplication::activeWindow());
    if(mw)
    {
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QDesktopWidget *desktop = QApplication::desktop();
        int screenWidth = desktop->width();
        int screenHeight = desktop->height();
#else
        QScreen *desktop = QGuiApplication::primaryScreen();
        int screenWidth = desktop->geometry().width();
        int screenHeight = desktop->geometry().height();
#endif
        int x = (screenWidth - width()) / 2;
        int y = (screenHeight - height()) / 2;

        mw->move(x, y);
    }
}
//--------------------------------------------------------------------------------
void CreatorWindow::set_status1_text(const QString &data)
{
    statusLabel1->setText(data);
}
//--------------------------------------------------------------------------------
void CreatorWindow::set_status2_text(const QString &data)
{
    statusLabel2->setText(data);
}
//--------------------------------------------------------------------------------
void CreatorWindow::setToolBarStyles(void)
{
    QPushButton *button = reinterpret_cast<QPushButton*>(sender());
    Q_ASSERT(button);

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
void CreatorWindow::createStatusBar(void)
{
    statusLabel1 = new QLabel(this);
    statusLabel2 = new QLabel(this);

#ifdef SHOW_IP
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    foreach (const QNetworkInterface &interface, interfaces)
    {
        QList<QNetworkAddressEntry> entries = interface.addressEntries();
        foreach (const QNetworkAddressEntry &entry, entries)
        {
            QHostAddress ip = entry.ip();
            // Проверяем, что это IPv4 адрес и не является адресом loopback
            if (ip.protocol() == QAbstractSocket::IPv4Protocol && !ip.isLoopback())
            {
                statusLabel1->setText(ip.toString());
            }
        }
    }
#else
    statusLabel1->setText(QString());
#endif

    statusLabel2->setText(QString());

    if(statusBar())
    {
        statusBar()->addWidget(statusLabel1);
        statusBar()->addWidget(statusLabel2);
    }
}
//--------------------------------------------------------------------------------
void CreatorWindow::setStyles(void)
{
    QAction* menu = static_cast<QAction *>(sender());
    Q_ASSERT(menu);

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
void CreatorWindow::about(void)
{
    AboutBox *about = new AboutBox();
    Q_ASSERT(about);

    about->setProperty(P_AVATAR,        ":/logo/avatar.png");
    about->setProperty(P_TELEGRAM_QR,   ":/logo/telegram.png");
    about->setProperty(P_ORGNAME,       orgName);
    about->setProperty(P_PROGRAMMNAME,  appName);
    about->setProperty(P_VERSION,       appVersion);
    about->setProperty(P_AUTHOR,        P_AUTHOR_STR);
    about->setProperty(P_EMAIL,         P_EMAIL_STR);
    about->setProperty(P_TELEGRAM,      P_TELEGRAM_STR);
    about->setProperty(P_GITHUB,        P_GITHUB_STR);
    about->exec();
}
//--------------------------------------------------------------------------------
void CreatorWindow::load_main(void)
{
#ifdef QT_DEBUG
    qDebug() << "CreatorWindow::load_main";
#endif

    QFont font = qApp->font();
    QString font_name;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    QFont::Weight font_weight;
#else
    int font_weight;
#endif
    int font_size;

    beginGroup(P_MAIN);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    font_weight = QFont::Normal;
#else
    font_weight = load_value(P_FONT_WEIGHT,   QFont::Normal).toInt();
#endif
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

#ifdef ALWAYS_CONFIRM_EXIT
    flag_close = false;
#else
    flag_close = load_value(P_NO_ANSWER_FROM_EXIT,    true).toBool();
#endif

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
    case MFC_THEME:
        set_mfc_palette();
        break;
    default:
        set_system_palette();
        break;
    }
    //---

    endGroup();

    QTimer::singleShot(100, [this]() {
        restoreGeometry(load_value(P_GEOMETRY).toByteArray());
        restoreState(load_value(P_WINDOW_STATE).toByteArray());
        if(load_value(P_IS_MAXIMIZED).toBool())
        {
            showMaximized();
        }
    });
}
//--------------------------------------------------------------------------------
void CreatorWindow::save_main(void)
{
#ifdef QT_DEBUG
    qDebug() << "CreatorWindow::save_main";
#endif

    beginGroup(P_MAIN);
    save_value(P_FONT_NAME,      QApplication::font().family());
    save_value(P_FONT_WEIGHT,    QApplication::font().weight());
    save_value(P_FONT_SIZE,      QApplication::font().pointSize());
    save_value(P_STYLE_NAME,     style_name);

#ifdef ALWAYS_CONFIRM_EXIT
    save_value(P_NO_ANSWER_FROM_EXIT, false);
#else
    save_value(P_NO_ANSWER_FROM_EXIT, flag_close);
#endif

    save_value(P_ALWAYS_ON_TOP,   flag_always_on_top);

    save_value(P_THEME,         state_theme);

    endGroup();

    save_value(P_GEOMETRY,      saveGeometry());
    save_value(P_WINDOW_STATE,  saveState());

    save_value(P_IS_MAXIMIZED, isMaximized());
}
//--------------------------------------------------------------------------------
void CreatorWindow::load_setting(void)
{
#ifdef QT_DEBUG
    qDebug() << "CreatorWindow::load_setting";
#endif
    load_main();
}
//--------------------------------------------------------------------------------
void CreatorWindow::save_setting(void)
{
#ifdef QT_DEBUG
    qDebug() << "CreatorWindow::save_setting";
#endif
    save_main();
}
//--------------------------------------------------------------------------------
#ifndef NO_LOG
void CreatorWindow::createLog(void)
{
    lb = new LogBox(this);
    Q_ASSERT(lb);

    lb->setObjectName("log_widget");

    connect(this,   &CreatorWindow::clear_log, lb, &LogBox::clear);

    connect(this,   &CreatorWindow::info,      lb, &LogBox::infoLog);
    connect(this,   &CreatorWindow::debug,     lb, &LogBox::debugLog);
    connect(this,   &CreatorWindow::error,     lb, &LogBox::errorLog);
    connect(this,   &CreatorWindow::trace,     lb, &LogBox::traceLog);

    connect(this,   &CreatorWindow::colorLog,  lb, &LogBox::colorLog);

    add_dock_widget("log", "log", Qt::BottomDockWidgetArea, lb);
}
#endif
//--------------------------------------------------------------------------------
void CreatorWindow::createSysLog_dock(void)
{
    w_syslog = new SysLog("syslog", this);
    Q_ASSERT(w_syslog);

    connect(w_syslog,   &SysLog::info,  this,   &CreatorWindow::info);
    connect(w_syslog,   &SysLog::debug, this,   &CreatorWindow::debug);
    connect(w_syslog,   &SysLog::error, this,   &CreatorWindow::error);
    connect(w_syslog,   &SysLog::trace, this,   &CreatorWindow::trace);

    connect(this,       static_cast<void (CreatorWindow::*)(int,int,QString)>(&CreatorWindow::syslog),
            w_syslog,   static_cast<void (SysLog::*)(int,int,QString)>(&SysLog::s_syslog));
    connect(this,       static_cast<void (CreatorWindow::*)(QDateTime,int,int,QString)>(&CreatorWindow::syslog),
            w_syslog,   static_cast<void (SysLog::*)(QDateTime,int,int,QString)>(&SysLog::s_syslog));

    add_dock_widget("syslog", "syslog", Qt::BottomDockWidgetArea, w_syslog);
}
//--------------------------------------------------------------------------------
#ifndef NO_STYLETOOLBAR
void CreatorWindow::createStyleToolBar(void)
{
    styletoolbar = new QToolBar(tr("styletoolbar"), this);
    Q_ASSERT(styletoolbar);

    styletoolbar->setObjectName("styletoolbar");

    QStringList sl;
    sl.append(QStyleFactory::keys());

    foreach (QString style, sl)
    {
        QPushButton *btnTemp = new QPushButton(this);
        btnTemp->setToolTip(style);
        btnTemp->setText(style);

        styletoolbar->addWidget(btnTemp);
        connect(btnTemp, &QPushButton::clicked, this, &CreatorWindow::setToolBarStyles);
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
}
#endif
//--------------------------------------------------------------------------------
#ifndef NO_STYLETOOLBAR
#ifdef USE_CUSTOM_STYLE
void CreatorWindow::set_norton_style(void)
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
void CreatorWindow::createCustomStyleToolBar(void)
{
    // пробую самописные стили
    QToolBar *customStyletoolbar = new QToolBar(tr("customstyletoolbar"), this);
    Q_ASSERT(customStyletoolbar);

    QPushButton *btnTemp = new QPushButton(this);
    btnTemp->setToolTip("Norton");
    btnTemp->setText("Norton");
    connect(btnTemp,    &QPushButton::clicked,  this,   &CreatorWindow::set_norton_style);

    customStyletoolbar->addWidget(btnTemp);

    addToolBar(Qt::LeftToolBarArea, customStyletoolbar);
    //#ifndef NO_MENU
    //    add_windowsmenu_action(customStyletoolbar, customStyletoolbar->toggleViewAction());
    //#endif
}
#endif
#endif
//--------------------------------------------------------------------------------
void CreatorWindow::help(void)
{
    QString help_filename = QString("%1.html").arg(APPNAME);
    if(QFile::exists(help_filename))
        HelpBrowser::showPage(help_filename, false);
    else
        HelpBrowser::showPage("qrc:/index.html", false);
}
//--------------------------------------------------------------------------------
void CreatorWindow::setWindowTitle(const QString &title)
{
    QString temp = title;
#ifdef TEST
    temp += " TEST ";
#endif
#ifdef QT_DEBUG
    temp += " (DEBUG)";
#endif
    QMainWindow::setWindowTitle(temp);
}
//--------------------------------------------------------------------------------
void CreatorWindow::createTrayIcon(void)
{
    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        QMessageBox::critical(nullptr,
                              QObject::tr("Systray"),
                              QObject::tr("I couldn't detect any system tray on this system."));
        return;
    }

    trayIconMenu = new QMenu(this);

    add_new_action(trayIconMenu,    "Minimize", nullptr,    &CreatorWindow::showMinimized);
    add_new_action(trayIconMenu,    "Maximize", nullptr,    &CreatorWindow::showMaximized);
    add_new_action(trayIconMenu,    "Restore",  nullptr,    &CreatorWindow::showNormal);

    trayIconMenu->addSeparator();
    add_new_action(trayIconMenu,    "Quit",     nullptr,    &CreatorWindow::quit);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(ICON_PROGRAMM));

    connect(trayIcon,
            &QSystemTrayIcon::activated,
            this,
            &CreatorWindow::showHide);

    trayIcon->show();
}
//--------------------------------------------------------------------------------
void CreatorWindow::showHide(QSystemTrayIcon::ActivationReason r)
{
    //qDebug() sl.append("### showHide ###" << r;
    if (r == QSystemTrayIcon::Trigger)
    {
        if (!this->isVisible())
        {
            this->show();
        }
        else
        {
            this->hide();
        }
    }
}
//--------------------------------------------------------------------------------
void CreatorWindow::change_value(void)
{
    emit info(QString("change_value %1").arg(sender()->metaObject()->className()));
}
//--------------------------------------------------------------------------------
bool CreatorWindow::add_separator(QMenu *menu,
                                  int pos_y)
{
    Q_ASSERT(menu);

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
bool CreatorWindow::add_mdi_sorting(void)
{
    QList<QMdiSubWindow *> l_obj = findChildren<QMdiSubWindow *>();
    emit debug(QString("find %1 mdi").arg(l_obj.count()));
    if(l_obj.count() <= 0)
    {
        return false;
    }

    Q_ASSERT(app_mainBar);

    QMenu *menu = new QMenu(app_mainBar);
    menu->setProperty(P_APP_ENG_TEXT, "MDI");
    menu->setTitle("MDI");
    menu->setToolTip("MDI");
    menu->setStatusTip("MDI");

    QAction *a_tile = new QAction(this);
    a_tile->setObjectName("a_tile");
    a_tile->setText("Tile");
    a_tile->setToolTip("Tile");
    a_tile->setStatusTip("Tile");
    a_tile->setProperty(P_APP_ENG_TEXT, "Tile");
    connect(a_tile, &QAction::triggered,
            this,   &CreatorWindow::set_tileSubWindows);

    QAction *a_cascade = new QAction(this);
    a_cascade->setObjectName("a_cascade");
    a_cascade->setText("Cascade");
    a_cascade->setToolTip("Cascade");
    a_cascade->setStatusTip("Cascade");
    a_cascade->setProperty(P_APP_ENG_TEXT, "Cascade");
    connect(a_cascade,  &QAction::triggered,
            this,       &CreatorWindow::set_cascadeSubWindows);

    menu->addAction(a_tile);
    menu->addAction(a_cascade);

    return true;
}
//--------------------------------------------------------------------------------
void CreatorWindow::set_tileSubWindows(void)
{
    QList<QMdiArea *> l_obj = findChildren<QMdiArea *>();
    if(l_obj.count() <= 1)
    {
        l_obj.at(0)->tileSubWindows();
    }
}
//--------------------------------------------------------------------------------
void CreatorWindow::set_cascadeSubWindows(void)
{
    QList<QMdiArea *> l_obj = findChildren<QMdiArea *>();
    if(l_obj.count() <= 1)
    {
        l_obj.at(0)->cascadeSubWindows();
    }
}
//--------------------------------------------------------------------------------
QDockWidget * CreatorWindow::add_dock_widget(QString title,
                                            QString objectname,
                                            Qt::DockWidgetArea area,
                                            QWidget *widget,
                                            bool no_dock_position)
{
    Q_ASSERT(widget);

    if(title.isEmpty())
    {
        emit error("title is empty!");
        return nullptr;
    }
    if(objectname.isEmpty())
    {
        emit error("objectname is empty!");
        return nullptr;
    }

    QDockWidget *dw = new QDockWidget(this);
    Q_ASSERT(dw);

    dw->setObjectName(objectname);
    dw->setWindowTitle(tr(title.toLocal8Bit()));
    dw->setProperty(P_DOCKWIDGET_ENG_TEXT, title);

    if(no_dock_position == false)
    {
        // Dock_position - это стрелки сбоку
        Dock_position *dp = new Dock_position(objectname, this);
        dp->setObjectName("dock_position");
        Q_ASSERT(dp);

        QWidget *nw = new QWidget(this);
        QHBoxLayout *hbox = new QHBoxLayout();
        hbox->setContentsMargins(0, 0, 0, 0);
        hbox->setSpacing(0);

        widget->setParent(dw);

        widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

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
    dw->setAttribute(Qt::WA_DeleteOnClose);

    connect(dw, &QDockWidget::dockLocationChanged, this, &CreatorWindow::dockLocationChanged);

    addDockWidget(area, dw);
    load_setting();

    return dw;
}
//--------------------------------------------------------------------------------
void CreatorWindow::tabify_all_docs(void)
{
    QList<QDockWidget *> l_docs = findChildren<QDockWidget *>();
    bool find_first = false;
    QDockWidget *f_w = nullptr;
    foreach(QDockWidget *widget, l_docs)
    {
        if(!find_first)
        {
            find_first = true;
            f_w = widget;
        }
        else
        {
            tabifyDockWidget(f_w, widget);
        }
    }
}
//--------------------------------------------------------------------------------
void CreatorWindow::dockLocationChanged(Qt::DockWidgetArea area)
{
    //qDebug() << area;
    Q_UNUSED(area)
}
//--------------------------------------------------------------------------------
void CreatorWindow::set_focus(const QString &)
{
    topLevelWidget()->show();
    topLevelWidget()->raise();
    topLevelWidget()->activateWindow();
}
//--------------------------------------------------------------------------------
void CreatorWindow::log(const QString &data)
{
#ifdef QT_DEBUG
    qDebug() << data;
#else
    Q_UNUSED(data)
#endif
}
//--------------------------------------------------------------------------------
void CreatorWindow::set_app_font(void)
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
void CreatorWindow::set_log_font(void)
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
void CreatorWindow::set_system_palette(void)
{
    state_theme = SYSTEM_THEME;
    qApp->setPalette(style()->standardPalette());
}
//--------------------------------------------------------------------------------
void CreatorWindow::set_blue_palette(void)
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
void CreatorWindow::set_dark_palette(void)
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
void CreatorWindow::set_light_palette(void)
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
void CreatorWindow::set_mfc_palette(void)
{
    //MFC
    QPalette palette;

    palette.setColor(QPalette::Window, QColor(240, 240, 240));
    palette.setColor(QPalette::WindowText, Qt::black);
    palette.setColor(QPalette::Base, Qt::white);
    palette.setColor(QPalette::AlternateBase, QColor(233, 231, 227));
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, Qt::black);
    palette.setColor(QPalette::Text, Qt::black);
    palette.setColor(QPalette::Button, QColor(240, 240, 240));
    palette.setColor(QPalette::ButtonText, Qt::black);
    palette.setColor(QPalette::Highlight, QColor(51, 153, 255));
    palette.setColor(QPalette::HighlightedText, Qt::white);

    state_theme = MFC_THEME;

    qApp->setPalette(palette);
}
//--------------------------------------------------------------------------------
//bool CreatorWindow::add_windowsmenu_action(QWidget *widget, QAction *action)
//{
//    Q_ASSERT(widget);
//    Q_ASSERT(action);
//    Q_ASSERT(m_app_windowsmenu);

//    m_app_windowsmenu->addAction(action);
//    return true;
//}
//--------------------------------------------------------------------------------
bool CreatorWindow::add_new_action(QMenu   *parent,
                                   QString text,
                                   QIcon   *icon,
                                   v_saveSlot slot)
{
    Q_ASSERT(parent);

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

    Q_ASSERT(action);

#if QT_VERSION >= 0x050000
    connect(action, &QAction::triggered,  this,   slot);
#else
    Q_UNUSED(slot);
#endif

    parent->addAction(action);
    return action;
}
//--------------------------------------------------------------------------------
bool CreatorWindow::add_new_action(QMenu   *parent,
                                   QString text,
                                   QIcon   *icon,
                                   b_saveSlot slot)
{
    Q_ASSERT(parent);

    QAction *action = new QAction(parent);
    Q_ASSERT(action);
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

    Q_ASSERT(action);

#if QT_VERSION >= 0x050000
    connect(action, &QAction::triggered,  this,   slot);
#else
    Q_UNUSED(slot);
#endif

    parent->addAction(action);
    return action;
}
//--------------------------------------------------------------------------------
void CreatorWindow::showMinimized(void)
{
    QMainWindow::showMinimized();
}
//--------------------------------------------------------------------------------
void CreatorWindow::showMaximized(void)
{
    QMainWindow::showMaximized();
}
//--------------------------------------------------------------------------------
void CreatorWindow::showNormal(void)
{
    QMainWindow::showNormal();
}
//--------------------------------------------------------------------------------
void CreatorWindow::quit(void)
{
    qApp->quit();
}
//--------------------------------------------------------------------------------
void CreatorWindow::dockwidget_updateText(void)
{
    QList<QDockWidget *> ldw = findChildren<QDockWidget *>();
    foreach (QDockWidget *dock, ldw)
    {
        dock->setWindowTitle(tr(dock->property(P_DOCKWIDGET_ENG_TEXT).toString().toLocal8Bit()));
    }
}
//--------------------------------------------------------------------------------
