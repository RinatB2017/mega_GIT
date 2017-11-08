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
#include "helpbrowser.hpp"
#include "mainwindow.hpp"
#include "aboutbox.hpp"
#include "defines.hpp"
#include "version.hpp"
//--------------------------------------------------------------------------------
#include <QDebug>
#ifdef QT_DEBUG
#   include <QDebug>
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
MainWindow::MainWindow(QMainWindow *parent)
    : QMainWindow(parent),
      orgName(ORGNAME),
      appName(APPNAME),
      appVersion(QString("%1.%2.%3 (%4)").arg(VER_MAJOR).arg(VER_MINOR).arg(VER_PATCH).arg(VER_BUILD))
      //TODO app_left(-1),
      //TODO app_top(-1),
      //TODO app_width(-1),
      //TODO app_height(-1)
{
    init();
}
//--------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    qDebug() << "~MainWindow()";

#ifndef NO_LOG
    MyWidget::set_param("Main", "flag_show_info",   a_is_shows_info->isChecked());
    MyWidget::set_param("Main", "flag_show_debug",  a_is_shows_debug->isChecked());
    MyWidget::set_param("Main", "flag_show_error",  a_is_shows_error->isChecked());
    MyWidget::set_param("Main", "flag_show_trace",  a_is_shows_trace->isChecked());
    if(ld)
    {
        ld->deleteLater();
    }
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
#else
    Q_UNUSED(event)
#endif
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
    connect(this, SIGNAL(trace(QString)),   this, SLOT(log(QString)));
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

#ifdef  DEMO
    check_date();
#endif

    QTimer::singleShot(100, this, SLOT(load_setting()));
}
//--------------------------------------------------------------------------------
#ifdef  DEMO
void MainWindow::check_date(void)
{
    QDateTime now = QDateTime::currentDateTime();
    if(now.date().year() >= DEMO_YEAR &&
            now.date().month() >= DEMO_MONTH &&
            now.date().day() > DEMO_DAY)
    {
        QTimer::singleShot(3000, this, SLOT(shot()));
    }
}
#endif
//--------------------------------------------------------------------------------
void MainWindow::shot(void)
{
    //QMessageBox::information(this, "Информация", "Всё!");
    int a = 5;
    int b = 0;
    int c = a / b;
    qDebug() << a << b << c;
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

    mainBar = menuBar();

    m_fileMenu = new QMenu(mainBar);
    m_optionsMenu = new QMenu(mainBar);
#ifndef NO_LOG
    m_windowsMenu = new QMenu(mainBar);
#endif
    m_helpMenu = new QMenu(mainBar);

#ifndef ONLY_ENGLISH
    m_langMenu = new QMenu(m_optionsMenu);
#endif
    m_styleMenu = new QMenu(m_optionsMenu);

#ifndef ONLY_ENGLISH
    m_langMenu->setIcon(QPixmap(QLatin1String(ICON_LANG)));
#endif
    m_styleMenu->setIcon(QPixmap(QLatin1String(ICON_STYLE)));

    a_Exit  = new QAction(m_fileMenu);
    a_About = new QAction(m_helpMenu);
    a_Help = new QAction(m_helpMenu);
    a_Font  = new QAction(m_optionsMenu);
#ifndef NO_LOG
    a_Log_Font = new QAction(m_optionsMenu);
#endif
#ifndef ONLY_ENGLISH
    a_Rus = new QAction(m_langMenu);
    a_Eng = new QAction(m_langMenu);
#endif
    a_AskExit = new QAction(m_optionsMenu);

    a_Exit->setShortcuts(QKeySequence::Quit);
    a_Exit->setIcon(QPixmap(QLatin1String(ICON_EXIT)));

    a_Font->setIcon(QPixmap(QLatin1String(ICON_FONT)));
#ifndef NO_LOG
    a_Log_Font->setIcon(QPixmap(QLatin1String(ICON_FONT)));
#endif

    a_Help->setIcon(QPixmap(QLatin1String(ICON_HELP)));
    a_About->setIcon(QPixmap(QLatin1String(ICON_ABOUT)));

#ifndef ONLY_ENGLISH
    a_Rus->setIcon(QPixmap(QLatin1String(ICON_RU)));
    a_Eng->setIcon(QPixmap(QLatin1String(ICON_US)));
#endif

    a_AskExit->setCheckable(true);
    a_AskExit->setChecked(flag_close);

    m_fileMenu->addAction(a_Exit);

    m_helpMenu->addAction(a_Help);
    m_helpMenu->addAction(a_About);

#ifndef ONLY_ENGLISH
    m_langMenu->addAction(a_Rus);
    m_langMenu->addAction(a_Eng);
#endif

    mainBar->addMenu(m_fileMenu);
    mainBar->addMenu(m_optionsMenu);
#ifndef NO_LOG
    mainBar->addMenu(m_windowsMenu);
#endif
    mainBar->addMenu(m_helpMenu);

    sl.append(QStyleFactory::keys());
    foreach (QString style, sl)
    {
        QAction *action = m_styleMenu->addAction(style, this, SLOT(setStyles()));
        action->setIcon(QPixmap(QLatin1String(ICON_STYLE)));
    }

    m_optionsMenu->addAction(a_Font);
#ifndef NO_LOG
    m_optionsMenu->addAction(a_Log_Font);
    m_optionsMenu->addSeparator();

    a_is_shows_info  = new QAction(m_optionsMenu);
    a_is_shows_debug = new QAction(m_optionsMenu);
    a_is_shows_error = new QAction(m_optionsMenu);
    a_is_shows_trace = new QAction(m_optionsMenu);

    connect(a_is_shows_info,    SIGNAL(triggered(bool)),    this,   SLOT(slot_is_shows_info(bool)));
    connect(a_is_shows_debug,   SIGNAL(triggered(bool)),    this,   SLOT(slot_is_shows_debug(bool)));
    connect(a_is_shows_error,   SIGNAL(triggered(bool)),    this,   SLOT(slot_is_shows_error(bool)));
    connect(a_is_shows_trace,   SIGNAL(triggered(bool)),    this,   SLOT(slot_is_shows_trace(bool)));

    QVariant flag_show_info  = true;
    QVariant flag_show_debug = true;
    QVariant flag_show_error = true;
    QVariant flag_show_trace = true;
    MyWidget::get_param("Main", "flag_show_info",   true,   &flag_show_info);
    MyWidget::get_param("Main", "flag_show_debug",  true,   &flag_show_debug);
    MyWidget::get_param("Main", "flag_show_error",  true,   &flag_show_error);
    MyWidget::get_param("Main", "flag_show_trace",  true,   &flag_show_trace);

    a_is_shows_info->setCheckable(true);
    a_is_shows_info->setChecked(flag_show_info.toBool());

    a_is_shows_debug->setCheckable(true);
    a_is_shows_debug->setChecked(flag_show_debug.toBool());

    a_is_shows_error->setCheckable(true);
    a_is_shows_error->setChecked(flag_show_error.toBool());

    a_is_shows_trace->setCheckable(true);
    a_is_shows_trace->setChecked(flag_show_trace.toBool());

    m_optionsMenu->addAction(a_is_shows_info);
    m_optionsMenu->addAction(a_is_shows_debug);
    m_optionsMenu->addAction(a_is_shows_error);
    m_optionsMenu->addAction(a_is_shows_trace);
#endif
    m_optionsMenu->addSeparator();
#ifndef ONLY_ENGLISH
    m_optionsMenu->addMenu(m_langMenu);
#endif
    m_optionsMenu->addMenu(m_styleMenu);
    m_optionsMenu->addSeparator();
    m_optionsMenu->addAction(a_AskExit);

    connect(a_Exit,     SIGNAL(triggered()),    this, SLOT(close()));
    connect(a_Help,     SIGNAL(triggered()),    this, SLOT(help()));
    connect(a_About,    SIGNAL(triggered()),    this, SLOT(about()));
    connect(a_Font,     SIGNAL(triggered()),    this, SLOT(set_app_font()));
#ifndef NO_LOG
    connect(a_Log_Font, SIGNAL(triggered()),    this, SLOT(set_log_font()));
#endif
#ifndef ONLY_ENGLISH
    connect(a_Rus,      SIGNAL(triggered()),    this, SLOT(setMenuLanguage()));
    connect(a_Eng,      SIGNAL(triggered()),    this, SLOT(setMenuLanguage()));
#endif
    connect(a_AskExit,  SIGNAL(triggered()),    this, SLOT(closeOnExit()));
    connect(this,       SIGNAL(updateLanguage()), this, SLOT(updateText()));
}
#endif
//--------------------------------------------------------------------------------
void MainWindow::updateText(void)
{
#ifndef NO_MENU
    m_fileMenu->setTitle(tr("File"));
    m_fileMenu->setStatusTip(tr("File"));
    m_fileMenu->setToolTip(tr("File"));

#ifndef NO_LOG
    m_windowsMenu->setTitle((tr("Windows")));
    m_windowsMenu->setStatusTip((tr("Windows")));
    m_windowsMenu->setToolTip((tr("Windows")));
#endif

    m_optionsMenu->setTitle(tr("&Settings"));
    m_optionsMenu->setStatusTip(tr("&Settings"));
    m_optionsMenu->setToolTip(tr("&Settings"));
    m_helpMenu->setTitle(tr("&Help"));
    m_helpMenu->setStatusTip(tr("&Help"));
    m_helpMenu->setToolTip(tr("&Help"));
#ifndef ONLY_ENGLISH
    m_langMenu->setTitle(tr("Language"));
    m_langMenu->setStatusTip(tr("Language"));
    m_langMenu->setToolTip(tr("Language"));
#endif
    m_styleMenu->setTitle(tr("Style"));
    m_styleMenu->setStatusTip(tr("Style"));
    m_styleMenu->setToolTip(tr("Style"));

#ifndef NO_LOG
    a_is_shows_info->setText(tr("is_shows_info"));
    a_is_shows_debug->setText(tr("is_shows_debug"));
    a_is_shows_error->setText(tr("is_shows_error"));
    a_is_shows_trace->setText(tr("is_shows_trace"));
#endif

    a_Exit->setText(tr("Exit"));
    a_About->setText(tr("About"));
    a_Help->setText(tr("Help"));
    a_Font->setText(tr("Select the font program"));

    a_Exit->setStatusTip(tr("Exit"));\
    a_About->setStatusTip(tr("About"));
    a_Help->setStatusTip(tr("Help"));
    a_Font->setStatusTip(tr("Select the font program"));

    a_Exit->setToolTip(tr("Exit"));\
    a_About->setToolTip(tr("About"));
    a_Help->setToolTip(tr("Help"));
    a_Font->setToolTip(tr("Select the font program"));

#ifndef NO_LOG
    a_Log_Font->setText(tr("Select the font logging"));
    a_Log_Font->setStatusTip(tr("Select the font logging"));
    a_Log_Font->setToolTip(tr("Select the font logging"));
#endif
#ifndef ONLY_ENGLISH
    a_Rus->setText(tr("Russian"));
    a_Rus->setStatusTip(tr("Russian"));
    a_Rus->setToolTip(tr("Russian"));
    a_Eng->setText(tr("English"));
    a_Eng->setStatusTip(tr("English"));
    a_Eng->setToolTip(tr("English"));
#endif
    a_AskExit->setText(tr("Do not ask when you exit"));
#endif

#ifndef NO_TRAYICON
    a_minimizeAction->setText(tr("Minimize"));
    a_maximizeAction->setText(tr("Maximize"));
    a_restoreAction->setText(tr("Restore"));
    a_quitAction->setText(tr("Quit"));
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
    qDebug() << "language = " << language;
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
    flag_close = a_AskExit->isChecked();
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
    about->setWindowIcon(QIcon(ICON_PROGRAMM));
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

    //TODO app_left    = settings->value("Left",   100).toInt();
    //TODO app_top     = settings->value("Top",    100).toInt();
    //TODO app_width   = settings->value("Width",  640).toInt();
    //TODO app_height  = settings->value("Height", 380).toInt();

#ifndef NO_MENU
    a_AskExit->setChecked(flag_close);
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
    settings->setValue("NoAnswerFromExit", a_AskExit->isChecked());
#endif

    //TODO settings->setValue("Left", geometry().x());
    //TODO settings->setValue("Top",  geometry().y());
    //TODO settings->setValue("Width", width());
    //TODO settings->setValue("Height",height());

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

    if(m_windowsMenu)
    {
        m_windowsMenu->addAction(ld->toggleViewAction());
    }

    connect(this,   SIGNAL(info(QString)),  ld, SLOT(infoLog(QString)));
    connect(this,   SIGNAL(debug(QString)), ld, SLOT(debugLog(QString)));
    connect(this,   SIGNAL(error(QString)), ld, SLOT(errorLog(QString)));
    connect(this,   SIGNAL(trace(QString)), ld, SLOT(traceLog(QString)));

    connect(this,   SIGNAL(signal_is_shows_info(bool)),     ld, SIGNAL(signal_is_shows_info(bool)));
    connect(this,   SIGNAL(signal_is_shows_debug(bool)),    ld, SIGNAL(signal_is_shows_debug(bool)));
    connect(this,   SIGNAL(signal_is_shows_error(bool)),    ld, SIGNAL(signal_is_shows_error(bool)));
    connect(this,   SIGNAL(signal_is_shows_trace(bool)),    ld, SIGNAL(signal_is_shows_trace(bool)));

    connect(this,   SIGNAL(syslog(int,QString,QString)),    ld,  SLOT(syslog(int,QString,QString)));

    slot_is_shows_info(a_is_shows_info->isChecked());
    slot_is_shows_debug(a_is_shows_debug->isChecked());
    slot_is_shows_error(a_is_shows_error->isChecked());
    slot_is_shows_trace(a_is_shows_trace->isChecked());

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
    toolBar->setMovable(true);  //TODO

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

    //toolBar->adjustSize();
    //toolBar->setMinimumWidth(toolBar->sizeHint().width());
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
    return mainBar;
}
#endif
//--------------------------------------------------------------------------------
#ifndef NO_MENU
QMenu *MainWindow::get_file_menu(void)
{
    return m_fileMenu;
}
#endif
//--------------------------------------------------------------------------------
#ifndef NO_MENU
QMenu *MainWindow::get_help_menu(void)
{
    return m_helpMenu;
}
#endif
//--------------------------------------------------------------------------------
#ifndef NO_MENU
QMenu *MainWindow::get_options_menu(void)
{
    return m_optionsMenu;
}
#endif
//--------------------------------------------------------------------------------
#ifndef NO_MENU
QMenu *MainWindow::get_windows_menu(void)
{
#ifndef NO_LOG
    return m_windowsMenu;
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

    a_minimizeAction = new QAction(this);
    connect(a_minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    a_maximizeAction = new QAction(this);
    connect(a_maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    a_restoreAction = new QAction(this);
    connect(a_restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    a_quitAction = new QAction(this);
    a_quitAction->setIcon(QIcon(ICON_EXIT));
    connect(a_quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(a_minimizeAction);
    trayIconMenu->addAction(a_maximizeAction);
    trayIconMenu->addAction(a_restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(a_quitAction);

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
    QList<QAction *> menus = mainBar->actions();
    if(menus.count() == 0)
    {
        emit error("add_menu: menu is empty!");
        return false;
    }

    if(pos_x >= menus.count())
    {
        mainBar->addMenu(menu);
        return true;
    }

    int pos = 0;
    foreach (QAction *current_menu, menus)
    {
        if(pos == pos_x)
        {
            mainBar->insertMenu(current_menu, menu);
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
#ifndef NO_LOG
void MainWindow::slot_is_shows_info(bool state)
{
    Q_CHECK_PTR(ld);
    ld->signal_is_shows_info(state);
}
#endif
//--------------------------------------------------------------------------------
#ifndef NO_LOG
void MainWindow::slot_is_shows_debug(bool state)
{
    Q_CHECK_PTR(ld);
    ld->signal_is_shows_debug(state);
}
#endif
//--------------------------------------------------------------------------------
#ifndef NO_LOG
void MainWindow::slot_is_shows_error(bool state)
{
    Q_CHECK_PTR(ld);
    ld->signal_is_shows_error(state);
}
#endif
//--------------------------------------------------------------------------------
#ifndef NO_LOG
void MainWindow::slot_is_shows_trace(bool state)
{
    Q_CHECK_PTR(ld);
    ld->signal_is_shows_trace(state);
}
#endif
//--------------------------------------------------------------------------------

