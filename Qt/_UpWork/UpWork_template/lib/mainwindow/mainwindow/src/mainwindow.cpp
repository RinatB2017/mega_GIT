/*********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#include "helpbrowser.hpp"
#include "mainwindow.hpp"
#include "aboutbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "logdock.hpp"
//--------------------------------------------------------------------------------
#define ICON_HELP       ":/mainwindow/system-help.png"
#define ICON_ABOUT      ":/mainwindow/computer.png"
#define ICON_RU         ":/mainwindow/ru.png"
#define ICON_US         ":/mainwindow/us.png"
#define ICON_STYLE      ":/mainwindow/style.png"
#define ICON_EXIT       ":/mainwindow/system-shutdown.png"
#define ICON_FONT       ":/mainwindow/kfontview.png"
#define ICON_LANG       ":/mainwindow/applications-education-language.png"
//--------------------------------------------------------------------------------
MainWindow::MainWindow(const QString &orgName,
                       const QString &appName,
                       const QString &appVersion,
                       QMainWindow *parent)
    : QMainWindow(parent),
      orgName(orgName),
      appName(appName),
      appVersion(appVersion),
      app_left(-1),
      app_top(-1),
      app_width(-1),
      app_height(-1)
{
    init();
}
//--------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
#ifndef NO_LOG
    if(ld) ld->deleteLater();
#endif
}
//--------------------------------------------------------------------------------
void MainWindow::setCentralWidget(QWidget *widget)
{
    if(!widget) return;

    mainWidget = widget;

    QMainWindow::setCentralWidget(mainWidget);

#ifdef FIXED_SIZE
    setFixedSize(sizeHint());
#endif
}
//--------------------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox;

    if(flag_close)
    {
        if(mainWidget)
        {
            //TODO
            bool flag_no_close = mainWidget->property("flag_no_close").toBool();
            if(flag_no_close)
            {
                emit error("flag_no_close is true");
                event->ignore();
                return;
            }

            //---
            MyWidget *w = dynamic_cast<MyWidget *>(mainWidget);
            if(w)
            {
                bool ok = w->close();
                if(!ok)
                {
                    emit error("mainWidget is busy");
                    event->ignore();
                    return;
                }
            }
            //---

            save_setting();
            mainWidget->deleteLater();
        }
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
        if(mainWidget)
        {
            save_setting();
            mainWidget->deleteLater();
        }
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
#endif
    Q_UNUSED(event)
}
#endif
//--------------------------------------------------------------------------------
void MainWindow::init(void)
{
    flag_close = false;

    load_translations();
    setWindowTitle(QString("%1 (ver. %2)")
                   .arg(appName)
                   .arg(appVersion));

#ifndef NO_MENU
    createMenus();
#endif
#ifndef NO_LOG
    createLog();
#else
    connect(this, SIGNAL(info(QString)),    this, SLOT(log(QString)));
    connect(this, SIGNAL(debug(QString)),   this, SLOT(log(QString)));
    connect(this, SIGNAL(error(QString)),   this, SLOT(log(QString)));
    connect(this, SIGNAL(message(QString)), this, SLOT(log(QString)));
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

    updateText();

    QTimer::singleShot(100, this, SLOT(load_setting()));
}
//--------------------------------------------------------------------------------
void MainWindow::load_translations()
{
#ifdef ONLY_ENGLISH
    return;
#else
    sysTranslator = new QTranslator(this);
    //res = sysTranslator->load(QLatin1String("qt_ru"), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    bool res = sysTranslator->load("qt_ru", ":/system");
    if(!res)
    {
        QMessageBox::critical(0, "Error", "sysTranslator not loaded");
#ifdef QT_DEBUG
        qDebug() << "sysTranslator not loaded!";
#endif
        return;
    }
    qApp->installTranslator(sysTranslator);
    //---
    appTranslator = new QTranslator(this);
    res = appTranslator->load(QLatin1String(":/programm"));
    if(!res)
    {
        QMessageBox::critical(0, "Error", "appTranslator not loaded!");
#ifdef QT_DEBUG
        qDebug() << "appTranslator not loaded!";
#endif
        return;
    }
    qApp->installTranslator(appTranslator);
    //---
#endif
}
//--------------------------------------------------------------------------------
#ifndef NO_MENU
void MainWindow::createMenus(void)
{
    QStringList sl;

    mainMenu = menuBar();

    fileMenu = new QMenu(mainMenu);
    optionsMenu = new QMenu(mainMenu);
#ifndef NO_LOG
    windowsMenu = new QMenu(mainMenu);
#endif
    helpMenu = new QMenu(mainMenu);

#ifndef ONLY_ENGLISH
    langMenu = new QMenu(optionsMenu);
#endif
    styleMenu = new QMenu(optionsMenu);

#ifndef ONLY_ENGLISH
    langMenu->setIcon(QPixmap(QLatin1String(ICON_LANG)));
#endif
    styleMenu->setIcon(QPixmap(QLatin1String(ICON_STYLE)));

    m_Exit  = new QAction(fileMenu);
    m_About = new QAction(helpMenu);
    m_Help = new QAction(helpMenu);
    m_Font  = new QAction(optionsMenu);
#ifndef NO_LOG
    m_Log_Font = new QAction(optionsMenu);
#endif
#ifndef ONLY_ENGLISH
    m_Rus = new QAction(langMenu);
    m_Eng = new QAction(langMenu);
#endif
    m_AskExit = new QAction(optionsMenu);

    m_Exit->setShortcuts(QKeySequence::Quit);
    m_Exit->setIcon(QPixmap(QLatin1String(ICON_EXIT)));

    m_Font->setIcon(QPixmap(QLatin1String(ICON_FONT)));
#ifndef NO_LOG
    m_Log_Font->setIcon(QPixmap(QLatin1String(ICON_FONT)));
#endif

    m_Help->setIcon(QPixmap(QLatin1String(ICON_HELP)));
    m_About->setIcon(QPixmap(QLatin1String(ICON_ABOUT)));

#ifndef ONLY_ENGLISH
    m_Rus->setIcon(QPixmap(QLatin1String(ICON_RU)));
    m_Eng->setIcon(QPixmap(QLatin1String(ICON_US)));
#endif

    m_AskExit->setCheckable(true);
    m_AskExit->setChecked(flag_close);

    fileMenu->addAction(m_Exit);

    helpMenu->addAction(m_Help);
    helpMenu->addAction(m_About);

#ifndef ONLY_ENGLISH
    langMenu->addAction(m_Rus);
    langMenu->addAction(m_Eng);
#endif

    mainMenu->addMenu(fileMenu);
    mainMenu->addMenu(optionsMenu);
#ifndef NO_LOG
    mainMenu->addMenu(windowsMenu);
#endif
    mainMenu->addMenu(helpMenu);

    sl.append(QStyleFactory::keys());
    foreach (QString style, sl)
    {
        QAction *action = styleMenu->addAction(style, this, SLOT(setStyles()));
        action->setIcon(QPixmap(QLatin1String(ICON_STYLE)));
    }

    optionsMenu->addAction(m_Font);
#ifndef NO_LOG
    optionsMenu->addAction(m_Log_Font);
#endif
    optionsMenu->addSeparator();
#ifndef ONLY_ENGLISH
    optionsMenu->addMenu(langMenu);
#endif
    optionsMenu->addMenu(styleMenu);
    optionsMenu->addSeparator();
    optionsMenu->addAction(m_AskExit);

    connect(m_Exit,     SIGNAL(triggered()),    this, SLOT(close()));
    connect(m_Help,     SIGNAL(triggered()),    this, SLOT(help()));
    connect(m_About,    SIGNAL(triggered()),    this, SLOT(about()));
    connect(m_Font,     SIGNAL(triggered()),    this, SLOT(set_app_font()));
#ifndef NO_LOG
    connect(m_Log_Font, SIGNAL(triggered()),    this, SLOT(set_log_font()));
#endif
#ifndef ONLY_ENGLISH
    connect(m_Rus,      SIGNAL(triggered()),    this, SLOT(setMenuLanguage()));
    connect(m_Eng,      SIGNAL(triggered()),    this, SLOT(setMenuLanguage()));
#endif
    connect(m_AskExit,  SIGNAL(triggered()),    this, SLOT(closeOnExit()));
    connect(this,       SIGNAL(updateLanguage()), this, SLOT(updateText()));
}
#endif
//--------------------------------------------------------------------------------
void MainWindow::updateText(void)
{
#ifndef NO_MENU
    fileMenu->setTitle(tr("File"));
    fileMenu->setStatusTip(tr("File"));
    fileMenu->setToolTip(tr("File"));

#ifndef NO_LOG
    windowsMenu->setTitle((tr("Windows")));
    windowsMenu->setStatusTip((tr("Windows")));
    windowsMenu->setToolTip((tr("Windows")));
#endif

    optionsMenu->setTitle(tr("&Settings"));
    optionsMenu->setStatusTip(tr("&Settings"));
    optionsMenu->setToolTip(tr("&Settings"));
    helpMenu->setTitle(tr("&Help"));
    helpMenu->setStatusTip(tr("&Help"));
    helpMenu->setToolTip(tr("&Help"));
#ifndef ONLY_ENGLISH
    langMenu->setTitle(tr("Language"));
    langMenu->setStatusTip(tr("Language"));
    langMenu->setToolTip(tr("Language"));
#endif
    styleMenu->setTitle(tr("Style"));
    styleMenu->setStatusTip(tr("Style"));
    styleMenu->setToolTip(tr("Style"));

    m_Exit->setText(tr("Exit"));
    m_About->setText(tr("About"));
    m_Help->setText(tr("Help"));
    m_Font->setText(tr("Select the font program"));

    m_Exit->setStatusTip(tr("Exit"));\
    m_About->setStatusTip(tr("About"));
    m_Help->setStatusTip(tr("Help"));
    m_Font->setStatusTip(tr("Select the font program"));

    m_Exit->setToolTip(tr("Exit"));\
    m_About->setToolTip(tr("About"));
    m_Help->setToolTip(tr("Help"));
    m_Font->setToolTip(tr("Select the font program"));

#ifndef NO_LOG
    m_Log_Font->setText(tr("Select the font logging"));
    m_Log_Font->setStatusTip(tr("Select the font logging"));
    m_Log_Font->setToolTip(tr("Select the font logging"));
#endif
#ifndef ONLY_ENGLISH
    m_Rus->setText(tr("Russian"));
    m_Rus->setStatusTip(tr("Russian"));
    m_Rus->setToolTip(tr("Russian"));
    m_Eng->setText(tr("English"));
    m_Eng->setStatusTip(tr("English"));
    m_Eng->setToolTip(tr("English"));
#endif
    m_AskExit->setText(tr("Do not ask when you exit"));
#endif

#ifndef NO_TRAYICON
    minimizeAction->setText(tr("Minimize"));
    maximizeAction->setText(tr("Maximize"));
    restoreAction->setText(tr("Restore"));
    quitAction->setText(tr("Quit"));
#endif

#ifndef NO_TOOLBAR
    btnExit->setToolTip(tr("Exit"));
    btnFont->setToolTip(tr("Select the font program"));
#ifndef ONLY_ENGLISH
    btnRus->setToolTip(tr("Russian"));
    btnEng->setToolTip(tr("English"));
#endif
    btnStyle->setToolTip(tr("Style"));
    btnAbout->setToolTip(tr("About"));
    btnHelp->setToolTip(tr("Help"));
#endif
}
//--------------------------------------------------------------------------------
void MainWindow::setMenuLanguage(void)
{
#ifndef ONLY_ENGLISH
    QAction* menu =(QAction*)sender();
    if(!menu) return;

    QString language;
    language = menu->text();
#ifdef QT_DEBUG
    qDebug() << language;
#endif
    if(language.contains("Russian") || language.contains("Русский"))
    {
        if(appTranslator) qApp->installTranslator(appTranslator);
        if(sysTranslator) qApp->installTranslator(sysTranslator);
        emit updateLanguage();
    }
    if(language.contains("English"))
    {
        if(appTranslator) qApp->removeTranslator(appTranslator);
        if(sysTranslator) qApp->removeTranslator(sysTranslator);
        emit updateLanguage();
    }
#endif
}
//--------------------------------------------------------------------------------
void MainWindow::setToolBarLanguage(void)
{
#ifndef ONLY_ENGLISH
    QToolButton* button = dynamic_cast<QToolButton*>(sender());
    if(!button)
    {
        emit error("setToolBarLanguage: button");
        return;
    }

    QString language;
    language = button->text().remove("&");
    if(language.contains("Russian") || language.contains("Русский"))
    {
        if(appTranslator) qApp->installTranslator(appTranslator);
        if(sysTranslator) qApp->installTranslator(sysTranslator);
        emit updateLanguage();
        return;
    }
    if(language.contains("English"))
    {
        if(appTranslator) qApp->removeTranslator(appTranslator);
        if(sysTranslator) qApp->removeTranslator(sysTranslator);
        emit updateLanguage();
        return;
    }
#endif
}
//--------------------------------------------------------------------------------
void MainWindow::closeOnExit(void)
{
#ifndef NO_MENU
    flag_close = m_AskExit->isChecked();
#endif
}
//--------------------------------------------------------------------------------
#ifndef NO_STATUSBAR
void MainWindow::set_status1_text(const QString &data)
{
    statusLabel1->setText(data);
}
#endif
//--------------------------------------------------------------------------------
#ifndef NO_STATUSBAR
void MainWindow::set_status2_text(const QString &data)
{
    statusLabel2->setText(data);
}
#endif
//--------------------------------------------------------------------------------
#ifndef NO_STATUSBAR
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
#endif
//--------------------------------------------------------------------------------
void MainWindow::setStyles(void)
{
    QAction* menu =(QAction*)sender();
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
#ifndef NO_STYLETOOLBAR
void MainWindow::setToolBarStyles(void)
{
    QPushButton *button = dynamic_cast<QPushButton*>(sender());
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
#endif
//--------------------------------------------------------------------------------
void MainWindow::about(void)
{
    AboutBox *about = new AboutBox(orgName, appName, appVersion, tr("Author: Bikbao Rinat Zinorovich"));
    about->exec();
}
//--------------------------------------------------------------------------------
void MainWindow::load_main(void)
{
    QFont font = qApp->font();
    QString font_name;
    int font_weight;
    int font_size;

    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
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

    style_name = settings->value("StyleName", "Oxygen").toString();
    flag_close = settings->value("NoAnswerFromExit", false).toBool();

    QApplication::setStyle(QStyleFactory::create(style_name));

    app_left    = settings->value("Left",   100).toInt();
    app_top     = settings->value("Top",    100).toInt();
    app_width   = settings->value("Width",  640).toInt();
    app_height  = settings->value("Height", 380).toInt();

#ifndef NO_MENU
    m_AskExit->setChecked(flag_close);
#endif

    settings->endGroup();

    restoreGeometry(settings->value("geometry").toByteArray());
    restoreState(settings->value("windowState").toByteArray());

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MainWindow::save_main(void)
{
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup("Main");
    settings->setValue("FontName",      QApplication::font().family());
    settings->setValue("FontWeight",    QApplication::font().weight());
    settings->setValue("FontSize",      QApplication::font().pointSize());
    settings->setValue("StyleName",     style_name);

#ifndef NO_MENU
    settings->setValue("NoAnswerFromExit", m_AskExit->isChecked());
#endif

    settings->setValue("Left", geometry().x());
    settings->setValue("Top",  geometry().y());
    settings->setValue("Width", width());
    settings->setValue("Height",height());

    settings->endGroup();

    settings->setValue("geometry",      saveGeometry());
    settings->setValue("windowState",   saveState());

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MainWindow::load_setting(void)
{
    load_main();

#ifndef NO_LOG
    if(ld)
    {
        ld->load_settings();
    }
#endif
}
//--------------------------------------------------------------------------------
void MainWindow::save_setting(void)
{
    save_main();

#ifndef NO_LOG
    if(ld)
    {
        ld->save_settings();
    }
#endif
}
//--------------------------------------------------------------------------------
#ifndef NO_LOG
void MainWindow::createLog(void)
{
    ld = new LogDock(QObject::tr("log"), this);

    if(windowsMenu)
    {
        windowsMenu->addAction(ld->toggleViewAction());
    }

    connect(this,   SIGNAL(info(QString)),      ld, SLOT(infoLog(QString)));
    connect(this,   SIGNAL(debug(QString)),     ld, SLOT(debugLog(QString)));
    connect(this,   SIGNAL(error(QString)),     ld, SLOT(errorLog(QString)));
    connect(this,   SIGNAL(message(QString)),   ld, SLOT(messageLog(QString)));

    connect(this,   SIGNAL(syslog(int,QString,QString)),    ld,  SLOT(syslog(int,QString,QString)));

    addDockWidget(Qt::BottomDockWidgetArea, ld);
}
#endif
//--------------------------------------------------------------------------------
#ifndef NO_TOOLBAR
void MainWindow::createToolBar(void)
{
    QToolBar *toolBar = new QToolBar("toolbar", this);
    if(toolBar == nullptr)
    {
        return;
    }

    toolBar->setObjectName("toolBar");

    addToolBar(Qt::TopToolBarArea, toolBar);

    btnExit    = new QToolButton(this);
    btnFont    = new QToolButton(this);
#ifndef ONLY_ENGLISH
    btnRus     = new QToolButton(this);
    btnEng     = new QToolButton(this);
#endif
    btnStyle   = new QToolButton(this);
    btnHelp    = new QToolButton(this);
    btnAbout   = new QToolButton(this);

    btnExit->setObjectName("btnExit");
    btnFont->setObjectName("btnFont");
#ifndef ONLY_ENGLISH
    btnRus->setObjectName("btnRus");
    btnEng->setObjectName("btnEng");
#endif
    btnHelp->setObjectName("btnHelp");
    btnAbout->setObjectName("btnAbout");

#ifndef ONLY_ENGLISH
    btnRus->setObjectName("btnRus");
    btnEng->setObjectName("btnEng");

    btnRus->setText("Russian");
    btnEng->setText("English");
#endif

    btnStyle->setText("Style");

    btnExit->setIcon(QPixmap(QLatin1String(ICON_EXIT)));
    btnFont->setIcon(QPixmap(QLatin1String(ICON_FONT)));
#ifndef ONLY_ENGLISH
    btnRus->setIcon(QPixmap(QLatin1String(ICON_RU)));
    btnEng->setIcon(QPixmap(QLatin1String(ICON_US)));
#endif
    btnStyle->setIcon(QPixmap(QLatin1String(ICON_STYLE)));
    btnHelp->setIcon(QPixmap(QLatin1String(ICON_HELP)));
    btnAbout->setIcon(QPixmap(QLatin1String(ICON_ABOUT)));

    btnHelp->setShortcut(Qt::Key_F1);

    toolBar->addWidget(btnExit);
    toolBar->addSeparator();
    toolBar->addWidget(btnFont);
#ifndef ONLY_ENGLISH
    toolBar->addSeparator();
    toolBar->addWidget(btnRus);
    toolBar->addWidget(btnEng);
#endif
    toolBar->addSeparator();
    toolBar->addWidget(btnStyle);
    toolBar->addSeparator();
    toolBar->addWidget(btnHelp);
    toolBar->addWidget(btnAbout);

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
    btnStyle->setMenu(menu);
    btnStyle->setPopupMode(QToolButton::InstantPopup);

    connect(btnExit,    SIGNAL(clicked(bool)),    this, SLOT(close()));
    connect(btnFont,    SIGNAL(clicked(bool)),    this, SLOT(set_app_font()));
#ifndef ONLY_ENGLISH
    connect(btnRus,     SIGNAL(clicked(bool)),    this, SLOT(setToolBarLanguage()));
    connect(btnEng,     SIGNAL(clicked(bool)),    this, SLOT(setToolBarLanguage()));
#endif
    connect(btnHelp,    SIGNAL(clicked(bool)),    this, SLOT(help()));
    connect(btnAbout,   SIGNAL(clicked(bool)),    this, SLOT(about()));

    toolBar->adjustSize();
    toolBar->setMinimumWidth(toolBar->sizeHint().width());
}
#endif
//--------------------------------------------------------------------------------
#ifndef NO_STYLETOOLBAR
void MainWindow::createStyleToolBar(void)
{
    toolBar = new QToolBar("styletoolbar", this);
    if(toolBar == nullptr)
    {
        return;
    }

    toolBar->setObjectName("styletoolbar");

    QStringList sl;
    sl.append(QStyleFactory::keys());

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setMargin(0);
    vbox->setSpacing(0);
    foreach (QString style, sl)
    {
        QPushButton *btnTemp = new QPushButton(this);
        btnTemp->setText(style);

        vbox->addWidget(btnTemp);
        connect(btnTemp, SIGNAL(clicked()), this, SLOT(setToolBarStyles()));
    }
    vbox->addStretch(1);

    QFrame *frame = new QFrame;
    frame->setLayout(vbox);
    toolBar->addWidget(frame);
    addToolBar(Qt::LeftToolBarArea, toolBar);
}
#endif
//--------------------------------------------------------------------------------
void MainWindow::help(void)
{
    HelpBrowser::showPage("qrc:/index.html", false);
}
//--------------------------------------------------------------------------------
MainWindow *MainWindow::getThis(void)
{
    return this;
}
//--------------------------------------------------------------------------------
#ifndef NO_MENU
QMenuBar *MainWindow::get_menubar(void)
{
    return mainMenu;
}
#endif
//--------------------------------------------------------------------------------
#ifndef NO_MENU
QMenu *MainWindow::get_file_menu(void)
{
    return fileMenu;
}
#endif
//--------------------------------------------------------------------------------
#ifndef NO_MENU
QMenu *MainWindow::get_help_menu(void)
{
    return helpMenu;
}
#endif
//--------------------------------------------------------------------------------
#ifndef NO_MENU
QMenu *MainWindow::get_options_menu(void)
{
    return optionsMenu;
}
#endif
//--------------------------------------------------------------------------------
#ifndef NO_MENU
QMenu *MainWindow::get_windows_menu(void)
{
#ifndef NO_LOG
    return windowsMenu;
#else
    return 0;
#endif
}
#endif
//--------------------------------------------------------------------------------
#ifndef NO_TRAYICON
void MainWindow::createTrayIcon(void)
{
    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        QMessageBox::critical(0,
                              QObject::tr("Systray"),
                              QObject::tr("I couldn't detect any system tray on this system."));
        return;
    }

    minimizeAction = new QAction(this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    maximizeAction = new QAction(this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    restoreAction = new QAction(this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(this);
    quitAction->setIcon(QIcon(ICON_EXIT));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(ICON_PROGRAMM));

    connect(trayIcon,
            SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,
            SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    trayIcon->show();
}
#endif
//--------------------------------------------------------------------------------
#ifndef NO_TRAYICON
void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick)
    {
        if(isHidden()) show();
        else hide();
    }
}
#endif
//--------------------------------------------------------------------------------
bool MainWindow::add_menu(int pos_x,
                          QMenu *menu)
{
    if(menu == nullptr)
    {
        emit error("add_menu: menu is empty!");
        return false;
    }

#ifndef NO_MENU
    QList<QAction *> menus = mainMenu->actions();
    if(menus.count() == 0)
    {
        emit error("add_menu: menu is empty!");
        return false;
    }

    if(pos_x >= menus.count())
    {
        mainMenu->addMenu(menu);
        return true;
    }

    int pos = 0;
    foreach (QAction *current_menu, menus)
    {
        if(pos == pos_x)
        {
            mainMenu->insertMenu(current_menu, menu);
            return true;
        }
        pos++;
    }
#else
    Q_UNUSED(pos_x)
#endif

    return false;
}
//--------------------------------------------------------------------------------
bool MainWindow::add_action(QMenu *menu,
                            int pos_y,
                            QAction *action)
{
    if(!menu)
    {
        emit error("add_action: menu is empty!");
        return false;
    }

    if(!action)
    {
        emit error("add_action: action is empty!");
        return false;
    }

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
bool MainWindow::add_separator(QMenu *menu,
                               int pos_y)
{
    Q_CHECK_PTR(menu);

    if(menu == nullptr)
    {
        emit error("add_separator: menu is empty!");
        return false;
    }

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
#ifndef NO_LOG
void MainWindow::load_html(void)
{
    ld->load_html();
}
#endif
//--------------------------------------------------------------------------------
#ifndef NO_LOG
void MainWindow::save_html(void)
{
    ld->save_html();
}
#endif
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
#if 1
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok);
    if(ok)
    {
        QApplication::setFont(font);
    }
#else
    QFont font = qApp->font();
    QFontDialog *dlg = new QFontDialog(font, this);
    int btn = dlg->exec();
    if(btn == QFontDialog::Accepted)
    {
        QFont font = dlg->currentFont();
        QApplication::setFont(font);
    }
#endif
}
//--------------------------------------------------------------------------------
#ifndef NO_LOG
void MainWindow::set_log_font(void)
{
#if 1
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, ld->get_font());
    if(ok)
    {
        QApplication::setFont(font);
    }
#else
    QFontDialog *dlg = new QFontDialog(this);
    dlg->setCurrentFont(ld->get_font());

    int btn = dlg->exec();
    if(btn == QFontDialog::Accepted)
    {
        ld->set_font(dlg->currentFont());
    }
#endif
}
#endif
//--------------------------------------------------------------------------------

