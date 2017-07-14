#include "settingswindow.h"

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

SettingsWindow::SettingsWindow(QWidget *parent) :
    QFrame(parent)
{
    settings = new QSettings;

    genCustomise();
    genNavigation();
    genInterface();
    genBookmarks();
    genSecurity();
    genOptions();
    genReset();

    mainLayout = new QStackedWidget;
    mainLayout->addWidget(tabCustomise);
    mainLayout->addWidget(tabNavigation);
    mainLayout->addWidget(tabInterface);
    mainLayout->addWidget(tabBookmarks);
    mainLayout->addWidget(tabSecurity);
    mainLayout->addWidget(tabOptions);
    mainLayout->addWidget(tabReset);
   // connect(mainLayout,SIGNAL(currentChanged(int)),this,SLOT(indexChanged(int)));

    tabBar = new TabList;
    tabBar->setMinimumWidth(150);
    tabBar->addTab(tabCustomise->title(),0);
    tabBar->addTab(tabNavigation->title(),1);
    tabBar->addTab(tabInterface->title(),2);
    tabBar->addTab(tabBookmarks->title(),3);
    tabBar->addTab(tabSecurity->title(),4);
   /* tabBar->addTab(tabOptions->title(),5);
    tabBar->addTab("Downloads",5);
    tabBar->addTab("History",5);*/
    tabBar->addTab(tabReset->title(),6);
    connect(tabBar,SIGNAL(showTab(int)),mainLayout,SLOT(setCurrentIndex(int)));

    QSplitter *layout = new QSplitter;
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(tabBar);
    layout->addWidget(mainLayout);

    QHBoxLayout *center = new QHBoxLayout;
    center->setContentsMargins(0,0,0,0);
    center->addWidget(layout);
    setLayout(center);

    btnClose = new QPushButton(QIcon(":/img/close.png"),"",this);
    btnClose->setObjectName("TopBtn");
    btnClose->setIconSize(QSize(20,20));
    btnClose->move(this->geometry().width()-btnClose->geometry().width(),0);
    connect(btnClose,SIGNAL(clicked()),this,SIGNAL(closeRequested()));

    loadSettings();
}

void SettingsWindow::resizeEvent(QResizeEvent *event)
{
    btnClose->move(this->geometry().width()-btnClose->geometry().width(),0);
    event->accept();
}

void SettingsWindow::genCustomise()
{
    QLabel *lblBgColor = new QLabel(tr("Background color"));
    lblBgColor->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    editBgColor = new QLineEdit;
    editBgColor->setEnabled(false);
    editBgColor->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    QToolButton *btnBgColor = new QToolButton;
    btnBgColor->setText(tr("Change color"));
    btnBgColor->setIcon(QIcon(":/img/config.png"));
    btnBgColor->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnBgColor->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    connect(btnBgColor,SIGNAL(clicked()),this,SLOT(chooseColor()));

    QLabel *lblBgImageMain = new QLabel(tr("Background image of the main window"));
    editBgImageMain = new QLineEdit;
    editBgImageMain->setEnabled(false);
    editBgImageMain->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    btnBgImageMain = new QToolButton;
    btnBgImageMain->setText(tr("Explore"));
    btnBgImageMain->setIcon(QIcon(":/img/config.png"));
    btnBgImageMain->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnBgImageMain->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    connect(btnBgImageMain,SIGNAL(clicked()),this,SLOT(chooseBgImage()));

    QLabel *lblBgImageLock = new QLabel(tr("Background image of the lock screen"));
    editBgImageLock = new QLineEdit;
    editBgImageLock->setEnabled(false);
    editBgImageLock->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    btnBgImageLock = new QToolButton;
    btnBgImageLock->setText(tr("Explore"));
    btnBgImageLock->setIcon(QIcon(":/img/config.png"));
    btnBgImageLock->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnBgImageLock->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    connect(btnBgImageLock,SIGNAL(clicked()),this,SLOT(chooseBgImage()));

    QLabel *lblAnimation = new QLabel(tr("Animations Effects / Transitions"));
    checkAnimation = new QCheckBox(tr("Enable"));
    checkAnimation->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);

    QGridLayout *layCustomise = new QGridLayout;
    layCustomise->setContentsMargins(10,10,10,10);
    layCustomise->setSpacing(10);

    layCustomise->addWidget(lblAnimation,0,0);
    layCustomise->addWidget(checkAnimation,0,1,1,2);

    layCustomise->addWidget(lblBgColor,1,0);
    layCustomise->addWidget(editBgColor,1,1);
    layCustomise->addWidget(btnBgColor,1,2);

    layCustomise->addWidget(lblBgImageMain,2,0);
    layCustomise->addWidget(editBgImageMain,2,1);
    layCustomise->addWidget(btnBgImageMain,2,2);

    layCustomise->addWidget(lblBgImageLock,3,0);
    layCustomise->addWidget(editBgImageLock,3,1);
    layCustomise->addWidget(btnBgImageLock,3,2);

    QGroupBox *boxCustomize = new QGroupBox(tr("Customize"));
    boxCustomize->setLayout(layCustomise);

    // ----------------------------

    previewBgImageMain = new QToolButton;
    previewBgImageMain->setText(tr("Background image of the main window"));
    previewBgImageMain->setIconSize(QSize(400,300));
    previewBgImageMain->setIcon(QIcon(":/img/file.png"));
    previewBgImageMain->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    previewBgImageMain->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    connect(previewBgImageMain,SIGNAL(clicked()),this,SLOT(chooseBgImage()));

    previewBgImageLock = new QToolButton;
    previewBgImageLock->setText(tr("Background image of the lock screen"));
    previewBgImageLock->setIconSize(QSize(400,300));
    previewBgImageLock->setIcon(QIcon(":/img/file.png"));
    previewBgImageLock->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    previewBgImageLock->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    connect(previewBgImageLock,SIGNAL(clicked()),this,SLOT(chooseBgImage()));

    QGridLayout *layBgImageMain = new QGridLayout;
    layBgImageMain->setContentsMargins(10,10,10,10);
    layBgImageMain->setSpacing(10);

    layBgImageMain->addWidget(previewBgImageMain,0,0);
    layBgImageMain->addWidget(previewBgImageLock,0,1);

    HideBlock *boxBgImage = new HideBlock(tr("Background images preview"));
    boxBgImage->setBlockLayout(layBgImageMain);

    // ----------------------------

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(boxCustomize,0,0);
    mainLayout->addWidget(boxBgImage,1,0);
    mainLayout->addWidget(spacer,2,0);

    tabCustomise = new SettingsPanel;
    tabCustomise->setTitle(tr("Customise"));
    tabCustomise->setMainLayout(mainLayout);
}

void SettingsWindow::genNavigation()
{
    radioEmptyStartup = new QRadioButton(tr("Show empty page"));
    radioHomeStartup = new QRadioButton(tr("Show home page"));
    radioSpecificStartup = new QRadioButton(tr("Show specific page"));
    editSpecificStartup = new QLineEdit;
    editSpecificStartup->hide();

    QGridLayout *layStartup = new QGridLayout;
    layStartup->setContentsMargins(10,10,10,10);
    layStartup->setSpacing(10);

    layStartup->addWidget(radioEmptyStartup,0,0);
    layStartup->addWidget(radioHomeStartup,1,0);
    layStartup->addWidget(radioSpecificStartup,2,0);
    layStartup->addWidget(editSpecificStartup,3,1);

    connect(radioSpecificStartup,SIGNAL(toggled(bool)),editSpecificStartup,SLOT(setVisible(bool)));

    HideBlock *boxStartup = new HideBlock(tr("At startup"));
    boxStartup->setBlockLayout(layStartup);

    // -----------------------------------------

    radioEmptyNewTab = new QRadioButton(tr("Show empty page"));
    radioHomeNewTab = new QRadioButton(tr("Show home page"));
    radioSpecificNewTab = new QRadioButton(tr("Show specific page"));
    editSpecificNewTab = new QLineEdit;
    editSpecificNewTab->hide();

    QGridLayout *layNewTab = new QGridLayout;
    layNewTab->setContentsMargins(10,10,10,10);
    layNewTab->setSpacing(10);

    layNewTab->addWidget(radioEmptyNewTab,0,0);
    layNewTab->addWidget(radioHomeNewTab,1,0);
    layNewTab->addWidget(radioSpecificNewTab,2,0);
    layNewTab->addWidget(editSpecificNewTab,3,1);

    connect(radioSpecificNewTab,SIGNAL(toggled(bool)),editSpecificNewTab,SLOT(setVisible(bool)));

    HideBlock *boxNewTab = new HideBlock(tr("When I open a new tab"));
    boxNewTab->setBlockLayout(layNewTab);

    // -----------------------------------------

    QLabel *lblHomePage = new QLabel(tr("Home page"));
    lblHomePage->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    editHomePage = new QLineEdit;

    QLabel *lblSearchUrl = new QLabel(tr("Search URL"));
    editPrefixSearchUrl = new QLineEdit;
    QLabel *lblSearch = new QLabel(tr("search words"));
    editSuffixSearchUrl = new QLineEdit;

    QGridLayout *layNavigationUrl = new QGridLayout;
    layNavigationUrl->setContentsMargins(10,10,10,10);
    layNavigationUrl->setSpacing(10);
    layNavigationUrl->addWidget(lblHomePage,0,0);
    layNavigationUrl->addWidget(editHomePage,0,1,1,3);

    layNavigationUrl->addWidget(lblSearchUrl,1,0);
    layNavigationUrl->addWidget(editPrefixSearchUrl,1,1);
    layNavigationUrl->addWidget(lblSearch,1,2);
    layNavigationUrl->addWidget(editSuffixSearchUrl,1,3);

    QGroupBox *boxNavigationUrl = new QGroupBox(tr("URL Navigations"));
    boxNavigationUrl->setLayout(layNavigationUrl);

    // -------------------------------------

    // GrpBox Navigation (normal/private/perso)

    // ---------------------------------------

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(boxNavigationUrl);
    mainLayout->addWidget(boxStartup);
    mainLayout->addWidget(boxNewTab);
    mainLayout->addWidget(spacer);

    tabNavigation = new SettingsPanel;
    tabNavigation->setTitle(tr("Navigation"));
    tabNavigation->setMainLayout(mainLayout);
}

void SettingsWindow::genInterface()
{
    checkSideBar = new QCheckBox(tr("Side bar"));
    checkToolBar = new QCheckBox(tr("Tool bar"));
    checkTabBar = new QCheckBox(tr("Tab bar"));

    // ------------------------------------------

    checkSideBarAddTab = new QCheckBox(tr("Add Tab"));
    checkSideBarBack = new QCheckBox(tr("Back"));
    checkSideBarForward = new QCheckBox(tr("Forward"));
    checkSideBarReload = new QCheckBox(tr("Reload/Stop"));
    checkSideBarHome = new QCheckBox(tr("Home"));
    checkSideBarAddBookmark = new QCheckBox(tr("Add bookmark"));
    checkSideBarBookmarksMenu = new QCheckBox(tr("Bookmarks menu"));

    checkSideBarTabs = new QCheckBox(tr("Tabs"));
    checkSideBarCmd = new QCheckBox(tr("Command Bar"));
    checkSideBarInformations = new QCheckBox(tr("Informations"));

    checkSideBarOpenFile = new QCheckBox(tr("Open File"));
    checkSideBarSaveFile = new QCheckBox(tr("Save File"));
    checkSideBarLock = new QCheckBox(tr("Lock"));
    checkSideBarScreen = new QCheckBox(tr("Screen Shoot"));
    checkSideBarFull = new QCheckBox(tr("Fullscreen"));
    checkSideBarConfig = new QCheckBox(tr("Settings"));

    btnCheckAllSideBar = new QToolButton;
    btnCheckAllSideBar->setText(tr("Select all"));
    btnCheckAllSideBar->setIcon(QIcon(":/img/menu.png"));
    btnCheckAllSideBar->setIconSize(QSize(0,0));
    btnCheckAllSideBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnCheckAllSideBar->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    connect(btnCheckAllSideBar,SIGNAL(clicked()),this,SLOT(selectAll()));

    QGridLayout *laySideBar = new QGridLayout;
    laySideBar->setContentsMargins(10,10,10,10);
    laySideBar->setSpacing(10);

    laySideBar->addWidget(checkSideBarAddTab);
    laySideBar->addWidget(checkSideBarTabs);
    laySideBar->addWidget(checkSideBarCmd);

    laySideBar->addWidget(checkSideBarBack);
    laySideBar->addWidget(checkSideBarForward);
    laySideBar->addWidget(checkSideBarReload);
    laySideBar->addWidget(checkSideBarHome);
    laySideBar->addWidget(checkSideBarAddBookmark);
    laySideBar->addWidget(checkSideBarBookmarksMenu);

    laySideBar->addWidget(checkSideBarInformations);

    laySideBar->addWidget(checkSideBarOpenFile);
    laySideBar->addWidget(checkSideBarSaveFile);
    laySideBar->addWidget(checkSideBarLock);
    laySideBar->addWidget(checkSideBarScreen);
    laySideBar->addWidget(checkSideBarFull);
    laySideBar->addWidget(checkSideBarConfig);

    laySideBar->addWidget(btnCheckAllSideBar);

    HideBlock *blockSideBar = new HideBlock(tr("Icons on side bar"));
    blockSideBar->setBlockLayout(laySideBar);

    // -----------------------------------------

    checkToolBarAddTab = new QCheckBox(tr("Add Tab"));
    checkToolBarBack = new QCheckBox(tr("Back"));
    checkToolBarForward = new QCheckBox(tr("Forward"));
    checkToolBarReload = new QCheckBox(tr("Reload/Stop"));
    checkToolBarHome = new QCheckBox(tr("Home"));

    checkToolBarAddBookmark = new QCheckBox(tr("Add bookmark"));
    checkToolBarBookmarksMenu = new QCheckBox(tr("Bookmarks menu"));
    checkToolBarTabs = new QCheckBox(tr("Tabs"));
    checkToolBarCmd = new QCheckBox(tr("Command Bar"));
    checkToolBarInformations = new QCheckBox(tr("Informations"));

    checkToolBarOpenFile = new QCheckBox(tr("Open File"));
    checkToolBarSaveFile = new QCheckBox(tr("Save File"));
    checkToolBarLock = new QCheckBox(tr("Lock"));
    checkToolBarScreen = new QCheckBox(tr("Screen Shoot"));
    checkToolBarFull = new QCheckBox(tr("Fullscreen"));
    checkToolBarConfig = new QCheckBox(tr("Settings"));

    btnCheckAllToolBar = new QToolButton;
    btnCheckAllToolBar->setText(tr("Select all"));
    btnCheckAllToolBar->setIcon(QIcon(":/img/menu.png"));
    btnCheckAllToolBar->setIconSize(QSize(0,0));
    btnCheckAllToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnCheckAllToolBar->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    connect(btnCheckAllToolBar,SIGNAL(clicked()),this,SLOT(selectAll()));

    QGridLayout *layToolBar = new QGridLayout;
    layToolBar->setContentsMargins(10,10,10,10);
    layToolBar->setSpacing(10);

    layToolBar->addWidget(checkToolBarAddTab);
    layToolBar->addWidget(checkToolBarBack);
    layToolBar->addWidget(checkToolBarForward);
    layToolBar->addWidget(checkToolBarReload);
    layToolBar->addWidget(checkToolBarHome);

    layToolBar->addWidget(checkToolBarAddBookmark);
    layToolBar->addWidget(checkToolBarBookmarksMenu);
    layToolBar->addWidget(checkToolBarTabs);
    layToolBar->addWidget(checkToolBarCmd);
    layToolBar->addWidget(checkToolBarInformations);

    layToolBar->addWidget(checkToolBarOpenFile);
    layToolBar->addWidget(checkToolBarSaveFile);
    layToolBar->addWidget(checkToolBarLock);
    layToolBar->addWidget(checkToolBarScreen);
    layToolBar->addWidget(checkToolBarFull);
    layToolBar->addWidget(checkToolBarConfig);

    layToolBar->addWidget(btnCheckAllToolBar);

    HideBlock *blockToolBar = new HideBlock(tr("Icons on tool bar"));
    blockToolBar->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    blockToolBar->setBlockLayout(layToolBar);

    // --------------------------------------------

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(checkSideBar);
    mainLayout->addWidget(blockSideBar);
    mainLayout->addWidget(checkToolBar);
    mainLayout->addWidget(blockToolBar);
    mainLayout->addWidget(checkTabBar);

    mainLayout->addWidget(spacer);

    tabInterface = new SettingsPanel;
    tabInterface->setTitle(tr("Interface"));
    tabInterface->setMainLayout(mainLayout);
}

void SettingsWindow::genBookmarks()
{
    bkmManager = new BookmarksManager;
    bkmManager->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(bkmManager);

    tabBookmarks = new SettingsPanel;
    tabBookmarks->setTitle(tr("Bookmarks"));
    tabBookmarks->setMainLayout(mainLayout);
}

void SettingsWindow::genSecurity()
{
    /*QLabel *lblPhoto = new QLabel(tr("Your photo"));
    QLineEdit *editPhoto = new QLineEdit;*/

    QLabel *lblUserName = new QLabel(tr("Pseudo"));
    editUserName = new QLineEdit;

    QLabel *lblPass1 = new QLabel(tr("Password"));
    editPass1 = new QLineEdit;
    editPass1->setEchoMode(QLineEdit::Password);

    QLabel *lblPass2 = new QLabel(tr("Retype your Password"));
    lblPass2->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    editPass2 = new QLineEdit;
    editPass2->setEchoMode(QLineEdit::Password);

    QLabel *lblIndice = new QLabel(tr("Indication"));
    editIndice = new QLineEdit;

    QFormLayout *layUser = new QFormLayout;
    layUser->setContentsMargins(10,10,10,10);
    layUser->setSpacing(10);
   /* layUser->addWidget(lblPhoto,0,0);
    layUser->addWidget(editPhoto,0,1);*/

    layUser->addRow(lblUserName,editUserName);
    layUser->addRow(lblPass1,editPass1);
    layUser->addRow(lblPass2,editPass2);
    layUser->addRow(lblIndice,editIndice);

   /* for(int i(0);layUser->count();i++)
        layUser->itemAt(i)->widget()->setAlignment(Qt::AlignHCenter);*/

    /*lblPhoto->setAlignment(Qt::AlignVCenter);
    editPhoto->setAlignment(Qt::AlignVCenter);*/

    QGroupBox *boxUser = new QGroupBox(tr("Account"));
    boxUser->setLayout(layUser);

    // ----------------------------

    checkLockAtStartup = new QCheckBox(tr("At Startup"));
    checkUnlockToClose = new QCheckBox(tr("Closing"));
    checkLockConfig = new QCheckBox(tr("Open Config Window"));
    checkLockLocker = new QCheckBox(tr("To Unlock"));

    QToolButton *btnCheckAll = new QToolButton;
    btnCheckAll->setIcon(QIcon(":/img/menu.png"));
    btnCheckAll->setIconSize(QSize(0,0));
    btnCheckAll->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnCheckAll->setText(tr("Select all"));
    btnCheckAll->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);

    QGridLayout *layAskPass = new QGridLayout;
    layAskPass->setSpacing(10);
    layAskPass->addWidget(checkLockAtStartup);
    layAskPass->addWidget(checkUnlockToClose);
    layAskPass->addWidget(checkLockConfig);
    layAskPass->addWidget(checkLockLocker);
    layAskPass->addWidget(btnCheckAll);

    HideBlock *boxAskPass = new HideBlock(tr("Ask password"));
    boxAskPass->setBlockLayout(layAskPass);

    // --------------------------

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setContentsMargins(10,10,10,10);
    mainLayout->setSpacing(10);
    mainLayout->addWidget(boxUser);
    mainLayout->addWidget(boxAskPass);
    mainLayout->addWidget(spacer);

    tabSecurity = new SettingsPanel;
    tabSecurity->setTitle(tr("Security"));
    tabSecurity->setMainLayout(mainLayout);
}

void SettingsWindow::genOptions()
{
    QCheckBox *checkPlugins = new QCheckBox(tr("Enable Plugins"));
    QCheckBox *checkJava = new QCheckBox(tr("Enable Java"));
    QCheckBox *checkJavaScript = new QCheckBox(tr("Enable JavaScript"));
    QCheckBox *checkJavascriptCanOpenWindows = new QCheckBox(tr("JavaScript can open windows"));
    QCheckBox *checkJavascriptCanAccessClipboard = new QCheckBox(tr("JavaScript can access clipboard"));

    QToolButton *btnCheckAll = new QToolButton;
    btnCheckAll->setIcon(QIcon(":/img/menu.png"));
    btnCheckAll->setIconSize(QSize(0,0));
    btnCheckAll->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnCheckAll->setText(tr("Select all"));
    btnCheckAll->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);

    // --------------------------

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(0,10,0,0);
    mainLayout->addWidget(checkPlugins);
    mainLayout->addWidget(checkJava);
    mainLayout->addWidget(checkJavaScript);
    mainLayout->addWidget(checkJavascriptCanOpenWindows);
    mainLayout->addWidget(checkJavascriptCanAccessClipboard);
    mainLayout->addWidget(btnCheckAll);
    mainLayout->addWidget(spacer);

    tabOptions = new SettingsPanel;
    tabOptions->setTitle(tr("Options"));
    tabOptions->setMainLayout(mainLayout);
}

void SettingsWindow::genReset()
{
    QLabel *textView = new QLabel;
    textView->setWordWrap(true);
    textView->setText(tr("If you click \"Start Reset\", you accept close browser (loose all opened tabs) and delete :<ul><li>Account</li><li>Color background</li><li>Home Page</li><li>Elements of Interface</li><li>Account</li></ul>Do you continue ?"));
    textView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);

    QToolButton *btnReset = new QToolButton;
    btnReset->setIcon(QIcon(":/img/forward.png"));
    btnReset->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnReset->setText(tr("Start Reset"));
    btnReset->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    connect(btnReset,SIGNAL(clicked()),this,SLOT(reset()));

    // --------------------------

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(0,10,0,0);
    mainLayout->addWidget(textView);
    mainLayout->addWidget(btnReset);
    mainLayout->addWidget(spacer);

    tabReset = new SettingsPanel;
    tabReset->setTitle(tr("Reset"));
    tabReset->setMainLayout(mainLayout);
}

void SettingsWindow::loadSettings()
{
    // CUSTOMIZE
    oldBgImage = settings->value("Apparence/BgImgMainUrl").toString();
    newColor.setNamedColor(settings->value("Apparence/BgColor").toString());
    editBgColor->setText(settings->value("Apparence/BgColor").toString());
    editBgImageMain->setText(settings->value("Apparence/BgImgMainUrl").toString());
    editBgImageLock->setText(settings->value("Apparence/BgImgLockUrl").toString());
    checkAnimation->setChecked(settings->value("Apparence/Animations").toBool());
    previewBgImageMain->setIcon(QIcon(settings->value("Apparence/BgImgMainUrl").toString()));
    previewBgImageLock->setIcon(QIcon(settings->value("Apparence/BgImgLockUrl").toString()));

    if(settings->value("Apparence/BgImgMainUrl").toString().isEmpty())
        previewBgImageMain->setIcon(QIcon(":/img/file.png"));
    if(settings->value("Apparence/BgImgLockUrl").toString().isEmpty())
        previewBgImageLock->setIcon(QIcon(":/img/file.png"));

    // NAVIGATION
    radioEmptyStartup->setChecked(settings->value("Navigation/Startup/Empty").toBool());
    radioHomeStartup->setChecked(settings->value("Navigation/Startup/Home").toBool());
    radioSpecificStartup->setChecked(settings->value("Navigation/Startup/Specific").toBool());
    editSpecificStartup->setText(settings->value("Navigation/Startup/SpecificPage").toString());

    radioEmptyNewTab->setChecked(settings->value("Navigation/NewTab/Empty").toBool());
    radioHomeNewTab->setChecked(settings->value("Navigation/NewTab/Home").toBool());
    radioSpecificNewTab->setChecked(settings->value("Navigation/NewTab/Specific").toBool());
    editSpecificNewTab->setText(settings->value("Navigation/NewTab/SpecificPage").toString());

    editHomePage->setText(settings->value("Navigation/Home").toString());
    editPrefixSearchUrl->setText(settings->value("Navigation/PrefixSearch").toString());
    editSuffixSearchUrl->setText(settings->value("Navigation/SuffixSearch").toString());
    
    // INTERFACE
    checkSideBar->setChecked(settings->value("Interface/Display/SideBar").toBool());
    checkToolBar->setChecked(settings->value("Interface/Display/ToolBar").toBool());
    checkTabBar->setChecked(settings->value("Interface/Display/TabBar").toBool());

    checkSideBarAddTab->setChecked(settings->value("Interface/SideBar/AddTab").toBool());
    checkSideBarTabs->setChecked(settings->value("Interface/SideBar/Tabs").toBool());
    checkSideBarCmd->setChecked(settings->value("Interface/SideBar/Cmd").toBool());
    checkSideBarBack->setChecked(settings->value("Interface/SideBar/Back").toBool());
    checkSideBarForward->setChecked(settings->value("Interface/SideBar/Forward").toBool());
    checkSideBarReload->setChecked(settings->value("Interface/SideBar/Reload").toBool());
    checkSideBarHome->setChecked(settings->value("Interface/SideBar/Home").toBool());
    checkSideBarAddBookmark->setChecked(settings->value("Interface/SideBar/AddBookmark").toBool());
    checkSideBarBookmarksMenu->setChecked(settings->value("Interface/SideBar/BookmarksMenu").toBool());
    checkSideBarInformations->setChecked(settings->value("Interface/SideBar/Info").toBool());
    checkSideBarOpenFile->setChecked(settings->value("Interface/SideBar/OpenFile").toBool());
    checkSideBarSaveFile->setChecked(settings->value("Interface/SideBar/SaveFile").toBool());
    checkSideBarLock->setChecked(settings->value("Interface/SideBar/Lock").toBool());
    checkSideBarScreen->setChecked(settings->value("Interface/SideBar/Screen").toBool());
    checkSideBarFull->setChecked(settings->value("Interface/SideBar/Full").toBool());
    checkSideBarConfig->setChecked(settings->value("Interface/SideBar/Config").toBool());

    checkToolBarAddTab->setChecked(settings->value("Interface/ToolBar/AddTab").toBool());
    checkToolBarTabs->setChecked(settings->value("Interface/ToolBar/Tabs").toBool());
    checkToolBarCmd->setChecked(settings->value("Interface/ToolBar/Cmd").toBool());
    checkToolBarBack->setChecked(settings->value("Interface/ToolBar/Back").toBool());
    checkToolBarForward->setChecked(settings->value("Interface/ToolBar/Forward").toBool());
    checkToolBarReload->setChecked(settings->value("Interface/ToolBar/Reload").toBool());
    checkToolBarHome->setChecked(settings->value("Interface/ToolBar/Home").toBool());
    checkToolBarAddBookmark->setChecked(settings->value("Interface/ToolBar/AddBookmark").toBool());
    checkToolBarBookmarksMenu->setChecked(settings->value("Interface/ToolBar/BookmarksMenu").toBool());
    checkToolBarInformations->setChecked(settings->value("Interface/ToolBar/Info").toBool());
    checkToolBarOpenFile->setChecked(settings->value("Interface/ToolBar/OpenFile").toBool());
    checkToolBarSaveFile->setChecked(settings->value("Interface/ToolBar/SaveFile").toBool());
    checkToolBarLock->setChecked(settings->value("Interface/ToolBar/Lock").toBool());
    checkToolBarScreen->setChecked(settings->value("Interface/ToolBar/Screen").toBool());
    checkToolBarFull->setChecked(settings->value("Interface/ToolBar/Full").toBool());
    checkToolBarConfig->setChecked(settings->value("Interface/ToolBar/Config").toBool());

    // SECURITY
    editUserName->setText(settings->value("Account/Pseudo").toString());
    editPass1->setText(settings->value("Account/Password").toString());
    editPass2->setText(settings->value("Account/Password").toString());
    editIndice->setText(settings->value("Account/Indice").toString());

    checkLockAtStartup->setChecked(settings->value("Security/Lock/AtStart").toBool());
    checkUnlockToClose->setChecked(settings->value("Security/Lock/AtClose").toBool());
    checkLockConfig->setChecked(settings->value("Security/Lock/ToConfig").toBool());
    checkLockLocker->setChecked(settings->value("Security/Lock/Locker").toBool());

    // Bookmarks
    bkmManager->updateItems();
}

void SettingsWindow::indexChanged(int index)
{
    if(tabBar->nameAt(index)==qobject_cast<SettingsPanel *>(mainLayout->currentWidget())->title())
        tabBar->setCurrent(index);
}

void SettingsWindow::goToTab(int tab)
{
    mainLayout->setCurrentIndex(tab);

    if(tab==3)
        bkmManager->updateItems();
}

void SettingsWindow::selectAll()
{
    if(sender()==btnCheckAllSideBar)
    {
        checkSideBarAddTab->setChecked(true);
        checkSideBarTabs->setChecked(true);
        checkSideBarCmd->setChecked(true);
        checkSideBarBack->setChecked(true);
        checkSideBarForward->setChecked(true);
        checkSideBarReload->setChecked(true);
        checkSideBarHome->setChecked(true);
        checkSideBarAddBookmark->setChecked(true);
        checkSideBarBookmarksMenu->setChecked(true);
        checkSideBarInformations->setChecked(true);
        checkSideBarOpenFile->setChecked(true);
        checkSideBarSaveFile->setChecked(true);
        checkSideBarLock->setChecked(true);
        checkSideBarScreen->setChecked(true);
        checkSideBarFull->setChecked(true);
        checkSideBarConfig->setChecked(true);
    }
    else if(sender()==btnCheckAllToolBar)
    {
        checkToolBarAddTab->setChecked(true);
        checkToolBarTabs->setChecked(true);
        checkToolBarCmd->setChecked(true);
        checkToolBarBack->setChecked(true);
        checkToolBarForward->setChecked(true);
        checkToolBarReload->setChecked(true);
        checkToolBarHome->setChecked(true);
        checkToolBarAddBookmark->setChecked(true);
        checkToolBarBookmarksMenu->setChecked(true);
        checkToolBarInformations->setChecked(true);
        checkToolBarOpenFile->setChecked(true);
        checkToolBarSaveFile->setChecked(true);
        checkToolBarLock->setChecked(true);
        checkToolBarScreen->setChecked(true);
        checkToolBarFull->setChecked(true);
        checkToolBarConfig->setChecked(true);
    }
}

void SettingsWindow::chooseColor()
{
    newColor = QColorDialog::getColor(oldColor);
    setColor(newColor.name());
    editBgColor->setText(newColor.name());

    emit colorChanged();
}

void SettingsWindow::chooseBgImage()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("Picture (*.bmp *.gif *.jpeg *.jpg *.png);;All files (*)"));
    if(path.isEmpty())
    {
        if(sender()==btnBgImageMain || sender()==previewBgImageMain)
        {
            editBgImageMain->setText("");
            newBgImage = ""; // Bug Image de fond tjrs présent dans fen config
            previewBgImageMain->setIcon(QIcon(":/img/file.png"));
        }
        else if(sender()==btnBgImageLock || sender()==previewBgImageLock)
        {
            editBgImageLock->setText("");
            previewBgImageLock->setIcon(QIcon(":/img/file.png"));
        }

        return;
    }

    if(sender()==btnBgImageMain || sender()==previewBgImageMain)
    {
        editBgImageMain->setText(path);
        newBgImage = path;
        previewBgImageMain->setIcon(QIcon(path));
    }
    else if(sender()==btnBgImageLock || sender()==previewBgImageLock)
    {
        editBgImageLock->setText(path);
        previewBgImageLock->setIcon(QIcon(path));
    }

    emit colorChanged();
}

void SettingsWindow::reset()
{
    settings->clear();
    qApp->quit();
}

void SettingsWindow::setCurrentColor(QString backgroundColor)
{
    oldColor.setNamedColor(backgroundColor);
}

QColor SettingsWindow::getNewColor()
{
    return newColor;
}

QString SettingsWindow::getNewBgImage()
{
    return newBgImage;
}

void SettingsWindow::askSave()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Settings"));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(tr("Do you want to save the new settings ?"));
    msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int value = msgBox.exec();

    if(value == QMessageBox::Yes)
        saveSettings();
    else if(value == QMessageBox::No)
        emit canClose();
}

void SettingsWindow::saveSettings()
{
    if(editPass1->text()!=editPass2->text())
        QMessageBox::critical(this,tr("Password incorrect"),tr("Password incorrect !"),QMessageBox::Ok);
    else if(editUserName->text().isEmpty())
        QMessageBox::critical(this,tr("Empty field"),tr("The field \"Pseudo\" is empty"),QMessageBox::Ok);
    else
    {
        settings->setValue("Version",QCoreApplication::applicationVersion());

        settings->setValue("Apparence/BgColor",newColor.name());
        settings->setValue("Apparence/BgImgMainUrl",editBgImageMain->text());
        settings->setValue("Apparence/BgImgLockUrl",editBgImageLock->text());
        settings->setValue("Apparence/Animations",checkAnimation->isChecked());

        settings->setValue("Navigation/Startup/Empty",radioEmptyStartup->isChecked());
        settings->setValue("Navigation/Startup/Home",radioHomeStartup->isChecked());
        settings->setValue("Navigation/Startup/Specific",radioSpecificStartup->isChecked());
        settings->setValue("Navigation/Startup/SpecificPage",editSpecificStartup->text());
        settings->setValue("Navigation/NewTab/Empty",radioEmptyNewTab->isChecked());
        settings->setValue("Navigation/NewTab/Home",radioHomeNewTab->isChecked());
        settings->setValue("Navigation/NewTab/Specific",radioSpecificNewTab->isChecked());
        settings->setValue("Navigation/NewTab/SpecificPage",editSpecificNewTab->text());
        settings->setValue("Navigation/Home",editHomePage->text());
        settings->setValue("Navigation/PrefixSearch",editPrefixSearchUrl->text());
        settings->setValue("Navigation/SuffixSearch",editSuffixSearchUrl->text());

        settings->setValue("Interface/Display/SideBar",checkSideBar->isChecked());
        settings->setValue("Interface/Display/ToolBar",checkToolBar->isChecked());
        settings->setValue("Interface/Display/TabBar",checkTabBar->isChecked());

        settings->setValue("Interface/SideBar/AddTab",checkSideBarAddTab->isChecked());
        settings->setValue("Interface/SideBar/Tabs",checkSideBarTabs->isChecked());
        settings->setValue("Interface/SideBar/Cmd",checkSideBarCmd->isChecked());
        settings->setValue("Interface/SideBar/Back",checkSideBarBack->isChecked());
        settings->setValue("Interface/SideBar/Forward",checkSideBarForward->isChecked());
        settings->setValue("Interface/SideBar/Reload",checkSideBarReload->isChecked());
        settings->setValue("Interface/SideBar/Home",checkSideBarHome->isChecked());
        settings->setValue("Interface/SideBar/Info",checkSideBarInformations->isChecked());
        settings->setValue("Interface/SideBar/AddBookmark",checkSideBarAddBookmark->isChecked());
        settings->setValue("Interface/SideBar/BookmarksMenu",checkSideBarBookmarksMenu->isChecked());
        settings->setValue("Interface/SideBar/OpenFile",checkSideBarOpenFile->isChecked());
        settings->setValue("Interface/SideBar/SaveFile",checkSideBarSaveFile->isChecked());
        settings->setValue("Interface/SideBar/Lock",checkSideBarLock->isChecked());
        settings->setValue("Interface/SideBar/Screen",checkSideBarScreen->isChecked());
        settings->setValue("Interface/SideBar/Full",checkSideBarFull->isChecked());
        settings->setValue("Interface/SideBar/Config",checkSideBarConfig->isChecked());

        settings->setValue("Interface/ToolBar/AddTab",checkToolBarAddTab->isChecked());
        settings->setValue("Interface/ToolBar/Back",checkToolBarBack->isChecked());
        settings->setValue("Interface/ToolBar/Forward",checkToolBarForward->isChecked());
        settings->setValue("Interface/ToolBar/Reload",checkToolBarReload->isChecked());
        settings->setValue("Interface/ToolBar/Home",checkToolBarHome->isChecked());
        settings->setValue("Interface/ToolBar/AddBookmark",checkToolBarAddBookmark->isChecked());
        settings->setValue("Interface/ToolBar/BookmarksMenu",checkToolBarBookmarksMenu->isChecked());
        settings->setValue("Interface/ToolBar/Tabs",checkToolBarTabs->isChecked());
        settings->setValue("Interface/ToolBar/Cmd",checkToolBarCmd->isChecked());
        settings->setValue("Interface/ToolBar/Info",checkToolBarInformations->isChecked());
        settings->setValue("Interface/ToolBar/OpenFile",checkToolBarOpenFile->isChecked());
        settings->setValue("Interface/ToolBar/SaveFile",checkToolBarSaveFile->isChecked());
        settings->setValue("Interface/ToolBar/Lock",checkToolBarLock->isChecked());
        settings->setValue("Interface/ToolBar/Screen",checkToolBarScreen->isChecked());
        settings->setValue("Interface/ToolBar/Full",checkToolBarFull->isChecked());
        settings->setValue("Interface/ToolBar/Config",checkToolBarConfig->isChecked());

        settings->setValue("Account/Pseudo",editUserName->text());
        settings->setValue("Account/Password",editPass1->text());
        settings->setValue("Account/Indice",editIndice->text());

        settings->setValue("Security/Lock/AtStart",checkLockAtStartup->isChecked());
        settings->setValue("Security/Lock/AtClose",checkUnlockToClose->isChecked());
        settings->setValue("Security/Lock/ToConfig",checkLockConfig->isChecked());
        settings->setValue("Security/Lock/Locker",checkLockLocker->isChecked());

        bkmManager->saveBookmarks();

        emit canClose();
    }
}

void SettingsWindow::setColor(QString color)
{
    this->setStyleSheet("\
                  QFrame {\
                      border: none;\
                      background: " + color + ";\
                      background: transparent;\
                      padding: 0px;\
                  }\
                  #Panel {background: rgba(0,0,0,.7);}\
                  QLabel {\
                      margin-bottom: 15px;\
                      color: white;\
                      font-size: 14px;\
                      font-family: segoe ui;\
                  }\
                  #Title {\
                      border: none;\
                      padding: 15px;\
                      padding-top: 15px;\
                      padding-bottom: 20px;\
                      margin: 0px;\
                      font-size: 20px;\
                      font-weight: lighter;\
                      background: rgba(0,0,0,0);\
                  }\
                  QToolButton, QPushButton {\
                      border: none;\
                      background: none;\
                      padding: 5px; color: white;\
                      font-size: 14px;\
                      font-family: segoe ui;\
                  }\
                  QToolButton:hover, QPushButton:hover {\
                      background: rgba(255,255,255,.2);\
                  }\
                  QToolButton:pressed, QPushButton:pressed, QToolButton:checked, QPushButton:checked {\
                      background: rgba(255,255,255,.5);\
                  }\
                  QPushButton::menu-indicator {\
                      image: url(downarrow.png);\
                  }\
                  #TopBtn {\
                      min-height: 42px;\
                      max-width: 42px;\
                  }\
                  QProgressBar {\
                      border: 4px solid white;\
                      background: white;\
                      border-radius: 14px;\
                      }\
                  QProgressBar::chunk {\
                      border: none;\
                      background: " + color + ";\
                      border-radius: 10px;\
                  }\
                  QLineEdit {\
                      margin: 1px;\
                      border: 1px solid white;\
                      background: transparent;\
                      color: white;\
                      padding: 4px 8px;\
                      font-size: 14px;\
                      border-radius: 4px;\
                      font-family: segoe ui;\
                  }\
                  QLineEdit::focus, QLineEdit::hover {\
                      background: rgba(0,0,0,.5);\
                  }\
                  QLineEdit[enabled=\"false\"] {border: 1px solid rgba(255,255,255,.3); color: rgba(255,255,255,.5);}\
                  QTextEdit {\
                      border: none;\
                      background: transparent;\
                      color: white;\
                      padding: 6px 0px;\
                      font-size: 14px;\
                      font-family: segoe ui;\
                  }\
                  QScrollArea {\
                      background: transparent;\
                      border: none;\
                  }\
                  QCheckBox,QRadioButton {\
                      spacing: 10px;\
                      color: white;\
                      font-size: 14px;\
                      font-family: segoe ui;\
                  }\
                  QCheckBox::indicator,QRadioButton::indicator {\
                      width: 13px;\
                      height: 13px;\
                      border: 1px solid rgba(255,255,255,1);\
                      background: rgba(0,0,0,0);\
                      padding: 4px;\
                      border-radius: 4px;\
                  }\
                  QRadioButton::indicator {\
                      border-radius: 11px;\
                      width: 13px;\
                      height: 13px;\
                      padding: 4px;\
                  }\
                  QCheckBox::indicator:hover,QRadioButton::indicator:hover {\
                      background: rgba(255,255,255,.3);\
                  }\
                  QCheckBox::indicator:checked {\
                      image: url(:/img/close.png);\
                  }\
                  QRadioButton::indicator:checked {\
                      image: url(:/img/round.png);\
                  }");
}

// SETTINGSPANEL

SettingsPanel::SettingsPanel(QWidget *parent) :
    QFrame(parent)
{
    lblTitle = new QLabel;
    lblTitle->setObjectName("Title");
    setObjectName("Panel");

    this->setStyleSheet("QFrame QLabel {padding: 5px; margin: 0px; margin-bottom: 0px;} #Title {background: none; font-size: 29px; margin: 0px; margin-top: 2px; padding: 0px 10px;} QGroupBox {border: 1px solid white; border-radius: 10px; margin-bottom: 30px; margin-top: 20px; border-radius: 0px; border: none; border-top: 5px solid white; border-bottom: 5px solid white;} QGroupBox::title {padding-left: 10px; text-transform: uppercase; font-weight: bold; padding-right: 10px; font-size: 28px; color: white; margin-top: 0px; subcontrol-origin: margin; subcontrol-position: top center; color: transparent;} QLineEdit {margin: 0px;}");

    scrollWidget = new QScrollArea;
    scrollWidget->setStyleSheet("QScrollArea,QFrame {background: transparent;}");
    scrollWidget->setWidgetResizable(true);

    mainLayout = new QGridLayout;
    //mainLayout->setContentsMargins();
    mainLayout->addWidget(lblTitle,0,0);
    mainLayout->addWidget(scrollWidget,1,0);
    setLayout(mainLayout);
}

void SettingsPanel::setTitle(QString title)
{
    lblTitle->setText(title);
}

void SettingsPanel::setMainLayout(QLayout *layout)
{
    //mainLayout->addLayout(layout,1,0);

    layout->setContentsMargins(15,15,15,15);
    QFrame *widget = new QFrame;
    widget->setLayout(layout);

    scrollWidget->setWidget(widget);
    scrollWidget->setWidgetResizable(true);
}

QString SettingsPanel::title()
{
    return lblTitle->text();
}

// TABLIST

TabList::TabList(QWidget *parent) :
    QFrame(parent)
{
    QLabel *title = new QLabel(tr("Settings"));

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);

    layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(title);
    layout->addWidget(spacer);
    setLayout(layout);

    map = new QSignalMapper;
    connect(map,SIGNAL(mapped(int)),this,SIGNAL(showTab(int)));

    setStyleSheet("QFrame {background: transparent; border: none;} QLabel {background: transparent; border: none; padding: 10px; font-size: 29px; margin: 0px; margin-bottom: 5px;} QToolButton {margin: 0px; padding: 6px 8px;}");
}

void TabList::addTab(QString title, int index)
{
    QToolButton *btnItem = new QToolButton;
    btnItem->setText(title);
    btnItem->setIcon(QIcon(":/img/menu.png"));
    btnItem->setIconSize(QSize(0,0));
  //  btnItem->setCheckable(true);
    btnItem->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnItem->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    layout->insertWidget(layout->count()-1,btnItem);
    connect(btnItem,SIGNAL(clicked()),map,SLOT(map()));

    map->setMapping(btnItem,index);
}

QString TabList::nameAt(int index)
{
    QToolButton *item = qobject_cast<QToolButton *>(layout->itemAt(index)->widget());
    return item->text();
}

void TabList::setCurrent(int index)
{
    /*for(int i(0);i<=layout->count();i++)
    {
        QToolButton *item = qobject_cast<QToolButton *>(layout->itemAt(i)->widget());
        item->setChecked(false);
    }
                            qApp->beep();*/

    QToolButton *item2 = qobject_cast<QToolButton *>(layout->itemAt(index)->widget());
    item2->setChecked(false);
}
