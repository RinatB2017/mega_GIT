#include "mainwindow.h"

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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    color = "rgb(0,160,245)";

    tabs = new QTabWidget;
    tabs->setDocumentMode(true);
    tabs->setTabsClosable(true);
    tabs->setMovable(true);

    settings = new QSettings;

    setup();
    loadConfig();
}

MainWindow::~MainWindow()
{
}

void MainWindow::createPanels()
{
    btnQuitArea = new QPushButton(tabs);
    btnQuitArea->move(0,0);
    btnQuitArea->setStyleSheet("border: none; background: rgba(0,0,0,.3);");
    btnQuitArea->hide();
    connect(btnQuitArea,SIGNAL(clicked()),this,SLOT(hideAllPanels()));

    progress = new ProgressView(tabs);

    tabBar = new TabView(tabs);
    tabBar->enableShadow(false);
    tabBar->setGeometry(QRect(-tabBar->geometry().width(),0,tabBar->geometry().width(),tabs->geometry().height()));
    tabsMapper = new QSignalMapper;
    connect(tabsMapper,SIGNAL(mapped(int)),tabs,SLOT(setCurrentIndex(int)));

    field = new CommandBar(tabs);
    field->setGeometry(0,-field->geometry().height(),tabs->geometry().width(),field->geometry().height());
    field->enableShadow(false);

    infoWidget = new WebInfo(tabs);
    infoWidget->enableShadow(false);
    infoWidget->setGeometry(QRect(-infoWidget->geometry().width(),0,infoWidget->geometry().width(),tabs->geometry().height()));

    aboutSoft = new AboutSoftware(tabs);
    aboutSoft->enableShadow(false);
    aboutSoft->setGeometry(QRect(-aboutSoft->geometry().width(),0,aboutSoft->geometry().width(),tabs->geometry().height()));
}

void MainWindow::createActs()
{
    actBack = new QAction(QIcon(":/img/back.png"),tr("Back"),this);
    actBack->setVisible(false);
    actBack->setShortcut(QKeySequence::Back);
    connect(actBack,SIGNAL(triggered()),this,SLOT(goBack()));

    actForward = new QAction(QIcon(":/img/forward.png"),tr("Forward"),this);
    actForward->setVisible(false);
    actForward->setShortcut(QKeySequence::Forward);
    connect(actForward,SIGNAL(triggered()),this,SLOT(goForward()));

    actReload = new QAction(QIcon(":/img/reload.png"),tr("Reload"),this);
    actReload->setVisible(false);
    actReload->setShortcut(QKeySequence::Refresh);
    connect(actReload,SIGNAL(triggered()),this,SLOT(reload()));

    actStop = new QAction(QIcon(":/img/stop.png"),tr("Stop"),this);
    actStop->setVisible(false);
    connect(actStop,SIGNAL(triggered()),this,SLOT(stop()));

    actHome = new QAction(QIcon(":/img/home.png"),tr("Home"),this);
    actStop->setShortcut(QKeySequence("Ctrl+H"));
    connect(actHome,SIGNAL(triggered()),this,SLOT(goHome()));


    actNewTab = new QAction(QIcon(":/img/add-tab.png"), tr("New Tab"), this);
    actNewTab->setShortcut(QKeySequence::AddTab);
    connect(actNewTab, SIGNAL(triggered()), this, SLOT(newTab()));

    actCloseTab = new QAction(QIcon(":/img/close-tab.png"), tr("Close Tab"), this);
    actCloseTab->setShortcut(QKeySequence("Ctrl+W"));
    connect(actCloseTab, SIGNAL(triggered()), this, SLOT(closeTab()));


    actOpenFile = new QAction(QIcon(":/img/open-file.png"), tr("Open File"), this);
    actOpenFile->setShortcut(QKeySequence::Open);
    connect(actOpenFile, SIGNAL(triggered()), this, SLOT(openFile()));

    actSaveFile = new QAction(QIcon(":/img/save-file.png"), tr("Save File"), this);
    actSaveFile->setShortcut(QKeySequence::Save);
    connect(actSaveFile, SIGNAL(triggered()), this, SLOT(saveFile()));

    actPrint = new QAction(QIcon(":/img/print.png"), tr("Print Page"), this);
    actPrint->setShortcut(QKeySequence::Print);
    actPrint->setVisible(false); // A desactiver tant que bug non resolu
    connect(actPrint, SIGNAL(triggered()), this, SLOT(printPage()));


    actViewWebInfo = new QAction(QIcon(":/img/page-information.png"), tr("Show Information Bar"), this);
    actViewWebInfo->setShortcut(QKeySequence("F8"));
    actViewWebInfo->setCheckable(true);
    connect(actViewWebInfo, SIGNAL(triggered()), this, SLOT(showWebInfo()));

    actViewField = new QAction(QIcon(":/img/cmd.png"), tr("Show Command Bar"), this);
    actViewField->setShortcut(QKeySequence("F6"));
    actViewField->setCheckable(true);
    connect(actViewField, SIGNAL(triggered()), this, SLOT(showField()));

    actViewTabs = new QAction(QIcon(":/img/tabs.png"), tr("Show Tab Panel"), this);
    actViewTabs->setShortcut(QKeySequence("F4"));
    actViewTabs->setCheckable(true);
    connect(actViewTabs, SIGNAL(triggered()), this, SLOT(showTabView()));

    actAboutSoft = new QAction(QIcon(":/img/information.png"), tr("About"), this);
    actAboutSoft->setShortcut(QKeySequence("F1"));
    actAboutSoft->setCheckable(true);
    connect(actAboutSoft, SIGNAL(triggered()), this, SLOT(showAboutSoft()));

    actViewFullScreen = new QAction(QIcon(":/img/full-screen.png"), tr("Show Fullscreen"), this);
    actViewFullScreen->setShortcut(QKeySequence::FullScreen);
    actViewFullScreen->setCheckable(true);
    connect(actViewFullScreen, SIGNAL(triggered()), this, SLOT(setFullScreen()));

    actZoomIn = new QAction(QIcon(":/img/zoom-in.png"), tr("Zoom In"), this);
    actZoomIn->setShortcut(QKeySequence::ZoomIn);
    connect(actZoomIn, SIGNAL(triggered()), this, SLOT(zoomIn()));

    actZoomOut = new QAction(QIcon(":/img/zoom-out.png"), tr("Zoom Out"), this);
    actZoomOut->setShortcut(QKeySequence::ZoomOut);
    connect(actZoomOut, SIGNAL(triggered()), this, SLOT(zoomOut()));
    
    actResetZoom = new QAction(QIcon(":/img/zoom-reset.png"), tr("Reset Zoom"), this);
    actResetZoom->setShortcut(QKeySequence("Ctrl+0"));
    connect(actResetZoom, SIGNAL(triggered()), this, SLOT(resetZoom()));


    actAddBookmark = new QAction(QIcon(":/img/add-bookmark.png"), tr("Add to bookmark"), this);
    actAddBookmark->setShortcut(QKeySequence("Ctrl+D"));
    connect(actAddBookmark,SIGNAL(triggered()),this,SLOT(addBookmark()));

    actViewBookmarks = new QAction(QIcon(":/img/bookmarks.png"), tr("Show bookmarks"), this);

    actManageBookmarks = new QAction(QIcon(":/img/config.png"), tr("Manage bookmarks"), this);
    connect(actManageBookmarks,SIGNAL(triggered()),this,SLOT(manageBookmarks()));


    actScreenShoot = new QAction(QIcon(":/img/screen-shoot.png"), tr("Screen Shoot"), this);
    connect(actScreenShoot, SIGNAL(triggered()), this, SLOT(takeScreenShoot()));

    actLock = new QAction(QIcon(":/img/lock.png"), tr("Lock"), this);
    actLock->setShortcut(QKeySequence("Ctrl+L"));
    connect(actLock, SIGNAL(triggered()), this, SLOT(lockWindow()));
    
    
    actViewSettings = new QAction(QIcon(":/img/config.png"), tr("Settings"), this);
    connect(actViewSettings,SIGNAL(triggered()),this,SLOT(showSettings()));


    actQuit = new QAction(QIcon(":/img/quit.png"), tr("Quit"), this);
    actQuit->setShortcut(QKeySequence("Alt+F4"));
    connect(actQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::createMenu()
{
    mainMenu = new QMenu;

    menuFile = mainMenu->addMenu(QIcon(":/img/file.png"),tr("File"));
    menuFile->addAction(actOpenFile);
    menuFile->addAction(actSaveFile);
    menuFile->addAction(actPrint);

    QMenu *menuNavigation = mainMenu->addMenu(QIcon(":/img/navigation.png"),tr("Navigation"));
    menuNavigation->addAction(actBack);
    menuNavigation->addAction(actForward);
    menuNavigation->addAction(actReload);
    menuNavigation->addAction(actStop);
    menuNavigation->addAction(actHome);

    menuTabs = mainMenu->addMenu(QIcon(":/img/tabs.png"),tr("Tabs"));
    menuTabs->addAction(actNewTab);
    menuTabs->addAction(actCloseTab);

    //  menuClosedTabs = menuTabs->addMenu(QIcon(":/img/closed-tabs.png"),tr("Recently Closed Tabs"));
      menuTabs->addSeparator();

    QMenu *menuView = mainMenu->addMenu(QIcon(":/img/view.png"),tr("View"));
    menuView->addAction(actViewTabs);
    menuView->addAction(actViewField);
    menuView->addAction(actViewWebInfo);
    menuView->addAction(actViewBookmarks);
    menuView->addAction(actViewFullScreen);
    menuView->addSeparator();
    menuView->addAction(actZoomIn);
    menuView->addAction(actZoomOut);
    menuView->addAction(actResetZoom);

    menuBookmarks = mainMenu->addMenu(QIcon(":/img/bookmarks.png"),tr("Bookmarks"));
    menuBookmarks->addAction(actAddBookmark);
    menuBookmarks->addAction(actManageBookmarks);
    menuBookmarks->addSeparator();/*
    menuBookmarks->addAction(actViewBookmarks);
    menuBookmarks->addSeparator();*/
    menuBookmarks->addAction(tr("(You have no bookmarks)"));

    QMenu *menuTools = mainMenu->addMenu(QIcon(":/img/tools.png"),tr("Tools"));
    menuTools->addAction(actScreenShoot);
    menuTools->addAction(actLock);

    mainMenu->addSeparator();
    mainMenu->addAction(actViewSettings);
    mainMenu->addAction(actAboutSoft);
    mainMenu->addAction(actQuit);
}

void MainWindow::createToolBar()
{
    mainToolBar = new QToolBar;
    mainToolBar->setMovable(true);
    mainToolBar->setIconSize(QSize(18,18));
    mainToolBar->hide();
    mainToolBar->setStyleSheet("QToolBar {background: transparent; border: none; padding: 0px; spacing: 0px; border-bottom: 1px solid rgba(0,0,0,.5);} QToolButton {padding: 10px; border-radius: 0px;} QToolButton:hover {background: rgba(0,0,0,.2);} QToolButton:checked, QToolButton:pressed {background: rgba(0,0,0,.5);}");

    if(settings->value("Interface/ToolBar/AddTab").toBool())
        mainToolBar->addAction(actNewTab);
    if(settings->value("Interface/ToolBar/Back").toBool())
        mainToolBar->addAction(actBack);
    if(settings->value("Interface/ToolBar/Forward").toBool())
        mainToolBar->addAction(actForward);
    if(settings->value("Interface/ToolBar/Reload").toBool())
        mainToolBar->addAction(actReload);
    if(settings->value("Interface/ToolBar/Reload").toBool())
        mainToolBar->addAction(actStop);
    if(settings->value("Interface/ToolBar/Home").toBool())
        mainToolBar->addAction(actHome);

    spotLight = new QLineEdit;
    mainToolBar->addWidget(spotLight);
    connect(spotLight,SIGNAL(returnPressed()),this,SLOT(research()));

    if(settings->value("Interface/ToolBar/Tabs").toBool())
        mainToolBar->addAction(actViewTabs);
    if(settings->value("Interface/ToolBar/Cmd").toBool())
        mainToolBar->addAction(actViewField);
    if(settings->value("Interface/ToolBar/Info").toBool())
        mainToolBar->addAction(actViewWebInfo);

    QToolButton *btnMenu = new QToolButton;
    btnMenu->setText(tr("Main Menu"));
    btnMenu->setIcon(QIcon(":/img/menu.png"));
    btnMenu->setMenu(mainMenu);
    btnMenu->setPopupMode(QToolButton::MenuButtonPopup);
    mainToolBar->addWidget(btnMenu);
}

void MainWindow::createCentral()
{
    QVBoxLayout *layoutCentral = new QVBoxLayout;
    layoutCentral->setContentsMargins(0,0,0,0);
    layoutCentral->setSpacing(0);
    layoutCentral->addWidget(mainToolBar);
    layoutCentral->addWidget(tabs);
    central = new QFrame;
    central->setObjectName("MainFrame");
    central->setLayout(layoutCentral);

    ano = new SidePanel(central);
    ano->setMainMenu(mainMenu,QIcon(":/img/menu.png"));

    config = new SettingsWindow;
    config->setMaximumWidth(16777215);

    lockWidget = new LockScreen;
    connect(lockWidget,SIGNAL(canUnlock()),this,SLOT(lockWindow()));

    widgetManager = new QStackedWidget;
    widgetManager->addWidget(ano);
    widgetManager->addWidget(config);
    widgetManager->addWidget(lockWidget);
    widgetManager->setCurrentWidget(ano);
    setCentralWidget(widgetManager);
}

void MainWindow::createJumpList()
{
#ifdef Q_OS_WIN
    QWinJumpList *jumplist = new QWinJumpList(this);
    jumplist->recent()->setVisible(true);
#endif
}

void MainWindow::setup()
{
    createPanels();
    createActs();
    createMenu();
    createToolBar();
    createCentral();
    createJumpList();

    bookmarksMapper = new QSignalMapper;
    connect(bookmarksMapper,SIGNAL(mapped(QString)),this,SLOT(goToBookmark(QString)));

    connect(tabs,SIGNAL(currentChanged(int)),this,SLOT(tabChanged()));
    connect(tabs,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));

    connect(infoWidget,SIGNAL(goToUrl()),this,SLOT(goToUrl()));
    connect(infoWidget,SIGNAL(searchToWeb()),this,SLOT(research()));
    connect(infoWidget,SIGNAL(inspectPage()),this,SLOT(inspectPage()));
    connect(infoWidget,SIGNAL(addBookmark()),this,SLOT(addBookmark()));

    connect(field,SIGNAL(goToWeb()),this,SLOT(goToUrl()));
    connect(field,SIGNAL(searchWeb()),this,SLOT(research()));

    connect(tabBar,SIGNAL(addTab()),this,SLOT(newTab()));
    connect(tabBar,SIGNAL(closeCurrentTab()),this,SLOT(closeTab()));
    connect(tabBar,SIGNAL(setCurrentTab(int)),tabs,SLOT(setCurrentIndex(int)));
    connect(tabBar,SIGNAL(setCurrentTab(int)),this,SLOT(showTabView()));
    connect(tabBar,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
    connect(tabBar,SIGNAL(viewModeChanged()),this,SLOT(updateTabs()));

    connect(aboutSoft,SIGNAL(closeRequested()),this,SLOT(showAboutSoft()));
    connect(infoWidget,SIGNAL(closeRequested()),this,SLOT(showWebInfo()));
    connect(tabBar,SIGNAL(closeRequested()),this,SLOT(showTabView()));
    connect(field,SIGNAL(closeRequested()),this,SLOT(showField()));
    connect(progress,SIGNAL(closeRequested()),this,SLOT(showProgress()));

    connect(config,SIGNAL(canClose()),this,SLOT(showSettings()));
    connect(config,SIGNAL(closeRequested()),config,SLOT(askSave()));

    connect(config,SIGNAL(colorChanged()),this,SLOT(updateColor()));
}

void MainWindow::loadConfig()
{
    // ColorStyle

    color = settings->value("Apparence/BgColor").toString();
    setColor(color);

    ano->setToolBarVisible(false);
    mainToolBar->hide();
    tabs->tabBar()->hide();

    if(settings->value("Interface/Display/SideBar").toBool())
        ano->setToolBarVisible(true);
    if(settings->value("Interface/Display/ToolBar").toBool())
        mainToolBar->show();
    if(settings->value("Interface/Display/TabBar").toBool())
        tabs->tabBar()->show();


    // ToolBar

    mainToolBar->clear();
    if(settings->value("Interface/ToolBar/AddTab").toBool())
        mainToolBar->addAction(actNewTab);
    if(settings->value("Interface/ToolBar/Back").toBool())
        mainToolBar->addAction(actBack);
    if(settings->value("Interface/ToolBar/Forward").toBool())
        mainToolBar->addAction(actForward);
    if(settings->value("Interface/ToolBar/Reload").toBool())
        mainToolBar->addAction(actReload);
    if(settings->value("Interface/ToolBar/Reload").toBool())
        mainToolBar->addAction(actStop);
    if(settings->value("Interface/ToolBar/Home").toBool())
        mainToolBar->addAction(actHome);

    spotLight = new QLineEdit;
    mainToolBar->addWidget(spotLight);
    connect(spotLight,SIGNAL(returnPressed()),this,SLOT(research()));

    if(settings->value("Interface/ToolBar/AddBookmark").toBool())
        mainToolBar->addAction(actAddBookmark);

    QToolButton *btnBookmarksMenu = new QToolButton;
    btnBookmarksMenu->setText(tr("Bookmarks Menu"));
    btnBookmarksMenu->setIcon(QIcon(":/img/bookmarks.png"));
    btnBookmarksMenu->setMenu(menuBookmarks);
    btnBookmarksMenu->setPopupMode(QToolButton::MenuButtonPopup);

    if(settings->value("Interface/ToolBar/BookmarksMenu").toBool())
        mainToolBar->addWidget(btnBookmarksMenu);

    if(settings->value("Interface/ToolBar/Tabs").toBool())
        mainToolBar->addAction(actViewTabs);
    if(settings->value("Interface/ToolBar/Cmd").toBool())
        mainToolBar->addAction(actViewField);
    if(settings->value("Interface/ToolBar/Info").toBool())
        mainToolBar->addAction(actViewWebInfo);
    if(settings->value("Interface/ToolBar/OpenFile").toBool())
        mainToolBar->addAction(actOpenFile);
    if(settings->value("Interface/ToolBar/SaveFile").toBool())
        mainToolBar->addAction(actSaveFile);
    if(settings->value("Interface/ToolBar/Lock").toBool())
        mainToolBar->addAction(actLock);
    if(settings->value("Interface/ToolBar/Screen").toBool())
        mainToolBar->addAction(actScreenShoot);
    if(settings->value("Interface/ToolBar/Full").toBool())
        mainToolBar->addAction(actViewFullScreen);
    if(settings->value("Interface/ToolBar/Config").toBool())
        mainToolBar->addAction(actViewSettings);

    QToolButton *btnMenu = new QToolButton;
    btnMenu->setText(tr("Main Menu"));
    btnMenu->setIcon(QIcon(":/img/menu.png"));
    btnMenu->setMenu(mainMenu);
    btnMenu->setPopupMode(QToolButton::MenuButtonPopup);
    mainToolBar->addWidget(btnMenu);

    // SideBar

    ano->clearSideBar();
    if(settings->value("Interface/SideBar/AddTab").toBool())
        ano->addAction(actNewTab);
    if(settings->value("Interface/SideBar/Tabs").toBool())
        ano->addAction(actViewTabs);
    if(settings->value("Interface/SideBar/Cmd").toBool())
        ano->addAction(actViewField);
    if(settings->value("Interface/SideBar/Back").toBool())
        ano->addAction(actBack);
    if(settings->value("Interface/SideBar/Forward").toBool())
        ano->addAction(actForward);
    if(settings->value("Interface/SideBar/Reload").toBool())
        ano->addAction(actReload);
    if(settings->value("Interface/SideBar/Reload").toBool())
        ano->addAction(actStop);
    if(settings->value("Interface/SideBar/Home").toBool())
        ano->addAction(actHome);
    if(settings->value("Interface/SideBar/Info").toBool())
        ano->addAction(actViewWebInfo);
    if(settings->value("Interface/SideBar/AddBookmark").toBool())
        ano->addAction(actAddBookmark);
    if(settings->value("Interface/SideBar/BookmarksMenu").toBool())
        ano->addMenu(menuBookmarks,QIcon(":/img/bookmarks.png"));
    if(settings->value("Interface/SideBar/OpenFile").toBool())
        ano->addAction(actOpenFile);
    if(settings->value("Interface/SideBar/SaveFile").toBool())
        ano->addAction(actSaveFile);
    if(settings->value("Interface/SideBar/Lock").toBool())
        ano->addAction(actLock);
    if(settings->value("Interface/SideBar/Screen").toBool())
        ano->addAction(actScreenShoot);
    if(settings->value("Interface/SideBar/Full").toBool())
        ano->addAction(actViewFullScreen);
    if(settings->value("Interface/SideBar/Config").toBool())
        ano->addAction(actViewSettings);

    updateBookmarks();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if(field->pos()==QPoint(0,0))
        field->setGeometry(0,0,tabs->geometry().width(),250);

    progress->move(progress->pos().x(),tabs->geometry().height()-progress->height());
    btnQuitArea->setGeometry(0,0,tabs->geometry().width(),tabs->geometry().height());
    infoWidget->setGeometry(infoWidget->pos().x(),infoWidget->pos().y(),infoWidget->geometry().width(),tabs->geometry().height());
    aboutSoft->setGeometry(aboutSoft->pos().x(),aboutSoft->pos().y(),aboutSoft->geometry().width(),tabs->geometry().height());
    tabBar->setGeometry(tabBar->pos().x(),tabBar->pos().y(),tabBar->geometry().width(),tabs->geometry().height());

    event->accept();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(widgetManager->currentWidget()==ano)
    {
        if(settings->value("Security/Lock/AtClose").toBool()&&!settings->value("Account/Password").toString().isEmpty())
        {
            // Si mdp
            QString password = QInputDialog::getText(this,tr("Enter password"),tr("Please enter your password"),QLineEdit::Password);
            if(password==settings->value("Account/Password").toString())
                event->accept();
            else if(password.isNull())
                event->ignore();
            else
                event->ignore();
        }
        else
            event->accept();
    }
    else if(widgetManager->currentWidget()==config)
    {
        event->ignore();
        config->askSave();
    }
    else if(widgetManager->currentWidget()==lockWidget)
    {
        if(settings->value("Account/Password").toString().isEmpty())
            event->accept();
        else
        {
            event->ignore();
            qApp->beep();
        }
    }
}

WebView *MainWindow::currentView()
{
    return tabs->currentWidget()->findChild<WebView *>();
}

WebWidget *MainWindow::currentWidget()
{
    return qobject_cast<WebWidget *>(tabs->currentWidget());
}

void MainWindow::loadFile(QString file)
{
    newTab();
    currentView()->load(QUrl::fromUserInput(file));

    currentView()->setFocus();
}

void MainWindow::loadStartup()
{
    this->showMaximized();

    WebWidget *view = new WebWidget;

    connect(view->view(),SIGNAL(urlChanged(QUrl)),this,SLOT(update()));
    connect(view->view(),SIGNAL(loadStarted()),this,SLOT(loadStarted()));
    connect(view->view(),SIGNAL(loadProgress(int)),this,SLOT(loadProgress(int)));
    connect(view->view(),SIGNAL(loadFinished(bool)),this,SLOT(loadFinished()));

    connect(view->view(),SIGNAL(urlChanged(QUrl)),this,SLOT(urlChanged(QUrl)));
    connect(view->view(),SIGNAL(titleChanged(QString)),this,SLOT(titleChanged(QString)));
    connect(view->view(),SIGNAL(iconChanged()),this,SLOT(iconChanged()));

    int index = tabs->addTab(view,QIcon(":/img/tabs.png"),tr("Startup Tab"));
    tabs->setCurrentIndex(index);

    if(settings->value("Navigation/Startup/Home").toBool())
        goHome();
    else if(settings->value("Navigation/Startup/Specific").toBool())
        view->view()->load(QUrl(settings->value("Navigation/Startup/SpecificPage").toString()));

    if(settings->value("Security/Lock/AtStart").toBool()==true)
        lockWindow();

    updateTabs();

    currentView()->setFocus();
}

void MainWindow::openFile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("All files (*);;Web Page (*.html *.xhtlm *.htm *.shtm);;Text File (*.txt);;Picture (*.bmp *.gif *.jpeg *.jpg *.png)"));
    if(!path.isEmpty())
        currentView()->load(QUrl::fromUserInput(path));

    currentView()->setFocus();
}

void MainWindow::saveFile()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save File"), QString(QDir::homePath()+QDir::separator()+currentView()->url().fileName()), tr("All Files (*);;HTML Files (*.html);;PHP Files (*.php);;CSS Files (*.css);;JavaScript Files (*.js);;Text Files (*.txt)"));
    if (!path.isEmpty())
    {
        QFile fileToSave(path);
        if(fileToSave.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream flux(&fileToSave);
            flux << currentView()->page()->mainFrame()->toHtml();
            fileToSave.close();
        }
    }

    currentView()->setFocus();
}

void MainWindow::printPage()
{
/*#ifndef QT_NO_PRINTDIALOG
    QPrinter printer;
    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print Document"));
    if (dialog->exec() != QDialog::Accepted)
        return;
    currentView()->page()->mainFrame()->print(&printer);
#endif*/

    currentView()->setFocus();
}

void MainWindow::goBack()
{
    currentView()->back();
    currentView()->setFocus();
}

void MainWindow::goForward()
{
    currentView()->forward();
    currentView()->setFocus();
}

void MainWindow::reload()
{
    currentView()->reload();
    currentView()->setFocus();
}

void MainWindow::stop()
{
    currentView()->stop();
    currentView()->setFocus();
}

void MainWindow::goHome()
{
    currentView()->load(QUrl(settings->value("Navigation/Home").toString()));

    currentView()->setFocus();
}

void MainWindow::goToUrl()
{
    if(sender()==infoWidget)
        currentView()->load(QUrl::fromUserInput(infoWidget->getUrl()));

    else if(sender()==field)
    {
        currentView()->load(QUrl::fromUserInput(field->textEdit->text()));
        field->reset();
        showField();
    }

    currentView()->setFocus();
}

void MainWindow::research()
{
    if(sender()==infoWidget)
        currentView()->load(QUrl(settings->value("Navigation/PrefixSearch").toString() + infoWidget->getText() + settings->value("Navigation/SuffixSearch").toString()));
    if(sender()==field)
    {
        currentView()->load(QUrl(settings->value("Navigation/PrefixSearch").toString() + field->textEdit->text() + settings->value("Navigation/SuffixSearch").toString()));
        field->reset();
        showField();
    }
    if(sender()==spotLight)
    {
        if(spotLight->text().left(7)=="http://" || spotLight->text().left(8)=="https://" || spotLight->text().left(9)=="localhost" || spotLight->text().contains("."))
            currentView()->load(QUrl::fromUserInput(spotLight->text()));
        else
            currentView()->load(QUrl(settings->value("Navigation/PrefixSearch").toString() + spotLight->text() + settings->value("Navigation/SuffixSearch").toString()));
    }

    currentView()->setFocus();
}

void MainWindow::newTab()
{
    WebWidget *view = new WebWidget;

    connect(view->view(),SIGNAL(urlChanged(QUrl)),this,SLOT(update()));
    connect(view->view(),SIGNAL(loadStarted()),this,SLOT(loadStarted()));
    connect(view->view(),SIGNAL(loadProgress(int)),this,SLOT(loadProgress(int)));
    connect(view->view(),SIGNAL(loadFinished(bool)),this,SLOT(loadFinished()));

    connect(view->view(),SIGNAL(urlChanged(QUrl)),this,SLOT(urlChanged(QUrl)));
    connect(view->view(),SIGNAL(titleChanged(QString)),this,SLOT(titleChanged(QString)));
    connect(view->view(),SIGNAL(iconChanged()),this,SLOT(iconChanged()));

    int index = tabs->addTab(view,QIcon(":/img/add-tab.png"),tr("New Tab"));
    tabs->setCurrentIndex(index);

    if(settings->value("Navigation/NewTab/Home").toBool())
        goHome();
    else if(settings->value("Navigation/NewTab/Specific").toBool())
        view->view()->load(QUrl(settings->value("Navigation/NewTab/SpecificPage").toString()));
    
    updateTabs();

    currentView()->setFocus();
}

void MainWindow::closeTab()
{
    if (tabs->count() > 1)
    {
        delete currentWidget();
        updateTabs();
    }
    else
    {
        qApp->quit();
    }
}

void MainWindow::closeTab(int index)
{
    if (tabs->count() > 1)
    {
        delete tabs->widget(index);
        updateTabs();
    }
    else
    {
        qApp->quit();
    }
}

void MainWindow::zoomIn()
{
    if(settings->value("Apparence/Animations").toBool())
    {
        QPropertyAnimation *animation = new QPropertyAnimation(currentView(), "zoomFactor");
        animation->setDuration(250);
        animation->setStartValue(currentView()->zoomFactor());
        animation->setEndValue(currentView()->zoomFactor()+0.25);

        animation->start();
    }
    else
        currentView()->setZoomFactor(currentView()->zoomFactor()+0.25);

    currentView()->setFocus();
}

void MainWindow::zoomOut()
{
    if(settings->value("Apparence/Animations").toBool())
    {
        QPropertyAnimation *animation = new QPropertyAnimation(currentView(), "zoomFactor");
        animation->setDuration(250);
        animation->setStartValue(currentView()->zoomFactor());
        animation->setEndValue(currentView()->zoomFactor()-0.25);

        animation->start();
    }
    else
        currentView()->setZoomFactor(currentView()->zoomFactor()-0.25);

    currentView()->setFocus();
}

void MainWindow::resetZoom()
{

    if(settings->value("Apparence/Animations").toBool())
    {
        QPropertyAnimation *animation = new QPropertyAnimation(currentView(), "zoomFactor");
        animation->setDuration(250);
        animation->setStartValue(currentView()->zoomFactor());
        animation->setEndValue(1);

        animation->start();
    }
    else
        currentView()->setZoomFactor(1);

    currentView()->setFocus();
}

void MainWindow::addBookmark()
{
    hideAllPanels();

    settings->beginGroup("Bookmarks");

    int item = 1000;
    if(settings->allKeys().count()>0)
    {
        QString lastKey = settings->allKeys().last();
        item = lastKey.left(4).toInt();
    }

    item = item + 1;
    settings->setValue(QString::number(item)+"/Type","Bookmark");
    settings->setValue(QString::number(item)+"/Name",currentView()->title());
    settings->setValue(QString::number(item)+"/Url",currentView()->url());
    settings->setValue(QString::number(item)+"/Icon",currentView()->icon());

    QMessageBox::information(this,tr("Bookmark added"),tr("Bookmark added !"),QMessageBox::Ok);

    settings->endGroup();

    updateBookmarks();
}

void MainWindow::updateBookmarks()
{
    settings->beginGroup("Bookmarks");

    menuBookmarks->clear();
    menuBookmarks->addAction(actAddBookmark);
    menuBookmarks->addAction(actManageBookmarks);
    menuBookmarks->addSeparator();/*
    menuBookmarks->addAction(actViewBookmarks);
    menuBookmarks->addSeparator();*/

    if(settings->childGroups().count()==0)
        menuBookmarks->addAction(tr("(You have no bookmarks)"));
    else
    {
       // for(int i(0);i<=settings->childGroups().count();i++)
        for(int i(1);i<=settings->childGroups().count();i++)
        {
            int item = i+1000;

            QString type = settings->value(QString::number(item)+"/Type").toString();
            QString title = settings->value(QString::number(item)+"/Name").toString();

            if(type=="Bookmark") // Si c'est un favoris
            {
                QString url = settings->value(QString::number(item)+"/Url").toUrl().toString();
                QIcon icon = settings->value(QString::number(item)+"/Icon").value<QIcon>();

                QAction *bookmark = menuBookmarks->addAction(icon,title);
                bookmark->setToolTip(url);

                bookmarksMapper->setMapping(bookmark,url);
                connect(bookmark,SIGNAL(triggered()),bookmarksMapper,SLOT(map()));
            }
            else if(type=="Folder") // Si c'est un dossier
            {
                // Recreer la boucle ici
            }
        }
    }

   // QMessageBox::information(this,"Count All Keys Bookmarks",QString::number(settings->childGroups().count()),QMessageBox::Ok);

    settings->endGroup();
}

void MainWindow::manageBookmarks()
{
    QSettings *settings = new QSettings;
    if(!settings->value("Account/Password").toString().isEmpty()&&settings->value("Security/Lock/ToConfig").toBool()==true)
    {
        QString password = QInputDialog::getText(this,tr("Enter password"),tr("Please enter your password"),QLineEdit::Password);
        if(password.isEmpty())
            return;
        else if(password==settings->value("Account/Password").toString())
        {
            widgetManager->setCurrentWidget(config);
            config->loadSettings();
        }
        else if(password!=settings->value("Account/Password").toString())
            QMessageBox::critical(this,tr("Password"),tr("Incorrect Password !"),QMessageBox::Ok);
    }
    else
    {
        widgetManager->setCurrentWidget(config);
        config->loadSettings();
    }

    config->goToTab(3);
    loadConfig();
}

void MainWindow::goToBookmark(QString url)
{
    currentView()->load(QUrl(url));

    currentView()->setFocus();
}

void MainWindow::takeScreenShoot()
{
    QPixmap pixmap(currentView()->width(), currentView()->height());
    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    currentView()->render(&painter);

    QString chemin = QFileDialog::getSaveFileName(this, tr("Save Screen Shoot"), QString(QDir::homePath()+QDir::separator()+currentView()->title()), tr("PNG Files (*.png);;BMP Files (*.bmp);;GIF Files (*.gif);;All Files (*)"));
    if (!chemin.isEmpty())
    {
        QFile fichierAEnregistrer(chemin);
        if (fichierAEnregistrer.open(QIODevice::WriteOnly | QIODevice::Text))
            pixmap.save(chemin);
    }
    
    currentView()->setFocus();
}

void MainWindow::lockWindow()
{
    if(sender()==actLock)
    {
        if(widgetManager->currentWidget()!=lockWidget)
        {
            widgetManager->setCurrentWidget(lockWidget);
            lockWidget->launch();
        }
        else
            widgetManager->setCurrentWidget(ano);
    }
    else if(sender()==lockWidget)
        widgetManager->setCurrentWidget(ano);
    else
    {
        if(widgetManager->currentWidget()!=lockWidget)
        {
            widgetManager->setCurrentWidget(lockWidget);
            lockWidget->launch();
        }
        else
            widgetManager->setCurrentWidget(ano);
    }

    setColor(color);
}

void MainWindow::inspectPage()
{
    if(currentWidget()->inspector()->isVisible())
        currentWidget()->inspector()->setVisible(false);
    else
        currentWidget()->inspector()->setVisible(true);

    hideAllPanels();
}

void MainWindow::updateColor()
{
    setColor(config->getNewColor().name());

    // Test
    if(!config->getNewBgImage().isEmpty())
    {
        this->setStyleSheet("QMainWindow {background: " + config->getNewColor().name() + ";background: url('"+config->getNewBgImage()+"'); background-position: center center; background-attachment: fixed;}");
    }
}

void MainWindow::setColor(QString colorName)
{
    color = colorName;

    if(widgetManager->currentWidget()!=lockWidget)
    {
        if(!settings->value("Apparence/BgImgMainUrl").toString().isEmpty())
            this->setStyleSheet("QMainWindow {background: " + colorName + ";background: url('"+settings->value("Apparence/BgImgMainUrl").toString()+"'); background-position: center center; background-attachment: fixed;}");
        else
            this->setStyleSheet("QMainWindow {background: " + colorName + ";}");
    }
    else if(widgetManager->currentWidget()==lockWidget)
    {
        if(!settings->value("Apparence/BgImgLockUrl").toString().isEmpty())
            this->setStyleSheet("QMainWindow {background: " + colorName + ";background: url('"+settings->value("Apparence/BgImgLockUrl").toString()+"'); background-position: center center; background-attachment: fixed;}");
        else
            this->setStyleSheet("QMainWindow {background: " + colorName + ";}");
    }

    central->setStyleSheet("#MainFrame {border: none;}");
    infoWidget->setDefaultColor(colorName);
    tabBar->setColor(color);
    ano->setColor(color);
    aboutSoft->setColor(color);
    infoWidget->setDefaultColor(color);
    field->setColor(color);
    config->setColor(color);
    config->setCurrentColor(color);
    progress->setColor(color);
}

void MainWindow::hideAllPanels()
{
    if(infoWidget->pos().x()==0)
        showWebInfo();
    if(aboutSoft->pos().x()==0)
        showAboutSoft();
    if(tabBar->pos().x()==0)
        showTabView();
}

void MainWindow::showPanel(Panel *item, bool view)
{
    if(view)
    {
        if(settings->value("Apparence/Animations").toBool())
        {
            QPropertyAnimation *animation = new QPropertyAnimation(item,"geometry");
            animation->setDuration(150);
            animation->setEasingCurve(QEasingCurve::InCubic);
            animation->setStartValue(QRect(item->geometry().x(),item->geometry().y(),item->geometry().width(),item->geometry().height()));
            animation->setEndValue(QRect(-item->geometry().width()-10,0,item->geometry().width(),tabs->geometry().height()));

            animation->start();
        }
        else
            item->setGeometry(QRect(-item->geometry().width()-10,50,item->geometry().width(),tabs->geometry().height()-100));

        item->enableShadow(false);

        btnQuitArea->setVisible(false);
    }
    else
    {
        if(settings->value("Apparence/Animations").toBool())
        {
            QPropertyAnimation *animation = new QPropertyAnimation(item,"geometry");
            animation->setDuration(250);
            animation->setEasingCurve(QEasingCurve::OutCubic);
            animation->setStartValue(QRect(-item->geometry().width()-10,50,item->geometry().width(),tabs->geometry().height()-100));
            animation->setEndValue(QRect(0,0,item->geometry().width(),tabs->geometry().height()));

            animation->start();
        }
        else
            item->setGeometry(QRect(0,0,item->geometry().width(),tabs->geometry().height()));

        item->enableShadow(true);

        btnQuitArea->setVisible(true);
    }

    btnQuitArea->setGeometry(0,0,tabs->geometry().width(),tabs->geometry().height());
}

void MainWindow::showWebInfo()
{
    if(sender()==actViewWebInfo)
    {
        if(actViewWebInfo->isChecked())
            showPanel(infoWidget,false);
        else
            showPanel(infoWidget,true);
    }
    else
    {
        showPanel(infoWidget,true);
        actViewWebInfo->setChecked(false);
    }
}

void MainWindow::showAboutSoft()
{
    if(sender()==actAboutSoft)
    {
        if(actAboutSoft->isChecked())
            showPanel(aboutSoft,false);
        else
            showPanel(aboutSoft,true);
    }
    else
    {
        showPanel(aboutSoft,true);
        actAboutSoft->setChecked(false);
    }
}

void MainWindow::showProgress()
{
    if(progress->pos().x()!=0) // If is'nt visible
    {
        progress->move(0,tabs->geometry().height()-progress->height());
    }
    else
    {
        progress->move(-progress->geometry().width()-10,tabs->geometry().height()-progress->height());
    }
}

void MainWindow::showField()
{
    if(field->pos()==QPoint(0,0))
    {

        if(settings->value("Apparence/Animations").toBool())
        {
            QPropertyAnimation *animation = new QPropertyAnimation(field,"geometry");
            animation->setDuration(150);
            animation->setEasingCurve(QEasingCurve::InCubic);
            animation->setStartValue(QRect(0,0,tabs->geometry().width(),250));
            animation->setEndValue(QRect(0,-field->geometry().height()-10,tabs->geometry().width(),field->geometry().height()));

            animation->start();
        }
        else
            field->setGeometry(QRect(0,-field->geometry().height()-10,tabs->geometry().width(),field->geometry().height()));

        actViewField->setChecked(false);
        field->enableShadow(false);

        currentView()->setFocus();
    }
    else
    {

        if(settings->value("Apparence/Animations").toBool())
        {
            QPropertyAnimation *animation = new QPropertyAnimation(field,"geometry");
            animation->setDuration(250);
            animation->setEasingCurve(QEasingCurve::OutCubic);
            animation->setStartValue(QRect(field->geometry().x()+200,field->geometry().y(),tabs->geometry().width()-400,field->geometry().height()));
            animation->setEndValue(QRect(0,0,tabs->geometry().width(),250));

            animation->start();
        }
        else
            field->setGeometry(QRect(0,0,tabs->geometry().width(),250));

        actViewField->setChecked(true);

        field->setFocus();
        field->enableShadow(true);
        field->reset();
    }
}

void MainWindow::showTabView()
{
    if(sender()==actViewTabs)
    {
        if(actViewTabs->isChecked())
            showPanel(tabBar,false);
        else
            showPanel(tabBar,true);
    }
    else
    {
        showPanel(tabBar,true);
        actViewTabs->setChecked(false);
    }
}

void MainWindow::showSettings()
{
    if(sender()!=config)
    {
        if(widgetManager->currentWidget()!=config)
        {
            QSettings *settings = new QSettings;
            if(!settings->value("Account/Password").toString().isEmpty()&&settings->value("Security/Lock/ToConfig").toBool()==true)
            {
                QString password = QInputDialog::getText(this,tr("Enter password"),tr("Please enter your password"),QLineEdit::Password);
                if(password.isEmpty())
                    return;
                else if(password==settings->value("Account/Password").toString())
                {
                    widgetManager->setCurrentWidget(config);
                    config->loadSettings();
                }
                else if(password!=settings->value("Account/Password").toString())
                    QMessageBox::critical(this,tr("Password"),tr("Incorrect Password !"),QMessageBox::Ok);
            }
            else
            {
                widgetManager->setCurrentWidget(config);
                config->loadSettings();
            }

            config->goToTab(0);
        }
        else
            widgetManager->setCurrentWidget(ano);
    }
    else if(sender()==config)
        widgetManager->setCurrentWidget(ano);

    loadConfig();
}

void MainWindow::setFullScreen()
{
    if(this->isFullScreen())
        this->showMaximized();
    else
        this->showFullScreen();

    currentView()->setFocus();
}

void MainWindow::loadStarted()
{
    if(sender()==currentView())
    {
        actReload->setVisible(false);
        actStop->setVisible(true);

        if(actViewField->isChecked())
            showField();

        if(progress->pos().x()!=0)
            showProgress();

        ano->progress()->show();
        ano->progress()->startAnimation();
    }
}

void MainWindow::loadProgress(int value)
{
    if(sender()==currentView())
    {
        actReload->setVisible(false);
        actStop->setVisible(true);

        if(progress->pos().x()==0)
            progress->move(0,tabs->geometry().height()-progress->height());

        progress->setValue(value);
    }
}

void MainWindow::loadFinished()
{
    if(sender()==currentView())
    {
        actReload->setVisible(true);
        actStop->setVisible(false);

        if(progress->pos().x() == 0)
            showProgress();

        ano->progress()->hide();
        ano->progress()->stopAnimation();
    }
}

void MainWindow::update()
{
    // BACK
    if(currentView()->history()->canGoBack())
        actBack->setVisible(true);
    else
        actBack->setVisible(false);

    // FORWARD
    if(currentView()->history()->canGoForward())
        actForward->setVisible(true);
    else
        actForward->setVisible(false);

    // RELOAD OR STOP
    if(currentWidget()->isLoading())
    {
        actReload->setVisible(false);
        actStop->setVisible(true);

        ano->progress()->show();
        ano->progress()->startAnimation();
    }
    else
    {
        actReload->setVisible(true);
        actStop->setVisible(false);

        ano->progress()->hide();
        ano->progress()->stopAnimation();
    }

    infoWidget->setUrl(currentView()->url());
    infoWidget->setWebTitle(currentView()->title());
    spotLight->setText(currentView()->url().toString());

    QIcon icon = currentView()->icon();
    if(icon.isNull())
        icon = QIcon(":/img/file.png");

    infoWidget->setIcon(icon);
    
    updateTabs();
}

void MainWindow::updateTabs()
{
    menuTabs->clear();
    //menuClosedTabs->clear();

    menuTabs->addAction(actNewTab);
    menuTabs->addAction(actCloseTab);
   // menuTabs->addMenu(menuClosedTabs);
    menuTabs->addSeparator();

    for(int i(0);i<tabs->count();i++)
    {
        QIcon icon = tabs->widget(i)->findChild<WebView *>()->icon();
        if(icon.isNull())
            icon = QIcon(":/img/file.png");

        QString title = tabs->widget(i)->findChild<WebView *>()->title();
        if(title.isEmpty())
            title = tabs->widget(i)->findChild<WebView *>()->url().fileName();
        if(title.size()>=20)
            title = title.left(20) + "...";

        QAction *item = new QAction(icon,title,this);
        item->setCheckable(true);
        connect(item,SIGNAL(triggered()),tabsMapper,SLOT(map()));
        connect(item,SIGNAL(triggered()),this,SLOT(updateTabs()));
        tabsMapper->setMapping(item,i);

        if(tabs->widget(i)->findChild<WebView *>() == currentView())
            item->setChecked(true);

        menuTabs->addAction(item);
    }

    // ===============================================

    tabBar->setTabWidget(tabs);
}

void MainWindow::tabChanged()
{
    update();
    
    updateTabs();
}

void MainWindow::iconChanged()
{
    int senderIndex = tabs->indexOf(qobject_cast<WebWidget *>(sender()->parent()->parent()));

    QIcon icon = tabs->widget(senderIndex)->findChild<WebView*>()->icon();
    if(icon.isNull())
        icon = QIcon(":/img/file.png");

    tabs->setTabIcon(senderIndex, icon);

    if(sender()==currentView())
        infoWidget->setIcon(icon);
    
    updateTabs();
}

void MainWindow::urlChanged(QUrl url)
{
    if(sender()==currentView())
    {
        infoWidget->setUrl(url);
    }
    
    updateTabs();
}

void MainWindow::titleChanged(QString title)
{
    if(title.isNull())
        title = qobject_cast<WebView *>(sender())->url().fileName();

    if(sender()==currentView())
    {
        infoWidget->setWebTitle(title);
    }

    int senderIndex = tabs->indexOf(qobject_cast<WebWidget *>(sender()->parent()->parent()));

    QPixmap pixmap(qobject_cast<WebView *>(sender())->width(),qobject_cast<WebView *>(sender())->height());
    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    qobject_cast<WebView *>(sender())->render(&painter);

/*    QLabel *test = new QLabel; // Afficher l'image dans une fenetre à part
    test->setPixmap(pixmap);   // ...
    test->show();              // ...
*/
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    pixmap.save(&buffer, "PNG");
    QString url = QString("<center><img src=\"data:image/png;base64,") + byteArray.toBase64() + "\"  width=\"400\"/>";

    tabs->setTabToolTip(senderIndex, url +"<br /><h2>"+title+"</h2></center>");

    if(title.size()>=25)
        title = title.left(25) + "...";
    tabs->setTabText(senderIndex, title);
    
    updateTabs();
}

void MainWindow::showStartupPage()
{
    newTab();
    currentView()->setHtml(QString(tr(
                      "<html>"
                      "  <head>"
                      "    <link rel=\"icon\" type=\"image/png\"href=\"qrc:/res/file.png\" />"
                      "    <title>Welcome to your new browser</title>"
                      "    <link rel=\"stylesheet\" href=\"qrc:/res/html.css\" />"
                      "    <style>body {background: %5;}</style>"
                      "  </head>"
                      "  <body>"
                      "    <h1>Welcome to %1 %2</h1>"
                      "    <form action=\"%3\" method=\"get\">"
                      "        <input type=\"text\" name=\"%4\" placeholder=\"Type here your text\" autofocus />"
                      "        <input type=\"submit\" value=\"Go !\" />"
                      "    </form>"
                      "  </body>"
                      "</html>"))
                  .arg(QCoreApplication::applicationName())
                  .arg(QCoreApplication::applicationVersion())
                  .arg(QString("https://duckduckgo.com/"))
                  .arg(QString("q"))
                  .arg(color)
                  ,QUrl(""));

    currentView()->setFocus();
}
