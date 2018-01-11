#include "starteruser.h"

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

StarterUser::StarterUser(QWidget *parent) :
    QWidget(parent)
{
    setFixedSize(600,450);

    mapBack = new QSignalMapper;
    mapNext = new QSignalMapper;

    createWelcome();
    createAccount();
    createCustomize();
    createFinish();

    layout = new QStackedLayout;
    layout->addWidget(tabWelcome);
    layout->addWidget(tabAccount);
    layout->addWidget(tabCustomize);
    layout->addWidget(tabFinish);
    setLayout(layout);

    connect(mapBack,SIGNAL(mapped(int)),layout,SLOT(setCurrentIndex(int)));
    connect(mapNext,SIGNAL(mapped(int)),layout,SLOT(setCurrentIndex(int)));
    connect(tabWelcome,SIGNAL(goBackRequest()),qApp,SLOT(quit()));
}

void StarterUser::start()
{
    this->show();
}

void StarterUser::createWelcome()
{
    tabWelcome = new TabWizard(tr("Welcome"));

    QLabel *text = new QLabel;
    text->setWordWrap(true);
   // text->setText(tr("Bienvenue dans l'assistant de configuration rapide de Dooscape. <br />Nous allons vous guider dans la configuration et la <br />personnalisation de votre navigateur. Pour commencer, cliquez <br />sur Continuer."));
    text->setText(tr("Welcome to the Setup Wizard of Dooscape. We will guide you through setting up and customizing your browser. To begin, click Continue."));

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(text);
    mainLayout->addWidget(spacer);
    tabWelcome->setBoxLayout(mainLayout);

    connect(tabWelcome,SIGNAL(goForwardRequest()),mapNext,SLOT(map()));

    mapNext->setMapping(tabWelcome,1);

    color = QColor::fromRgb(0,160,245);
}

void StarterUser::createAccount()
{
    tabAccount = new TabWizard(tr("Account"));

    QLabel *text = new QLabel;
    text->setWordWrap(true);
   // text->setText(tr("Comme c'est un navigateur personnel, vous devez remplir <br />certaines informations qui vont permettre aux autres de <br />reconnaître à qui appartient ce navigateur."));
    text->setText(tr("As it is a personal navigator, you must meet certain information that will allow others to recognize who owns this browser.<br />"));

    QLabel *lblPseudo = new QLabel(tr("Pseudo"));
    editPseudo = new QLineEdit;
    // Remplir le champ Pseudo par le nom d'user
    editPseudo->setText(QDir::home().dirName());

    QLabel *lblPass1 = new QLabel(tr("Password"));
    editPass1 = new QLineEdit;
    editPass1->setEchoMode(QLineEdit::Password);

    QLabel *lblPass2 = new QLabel(tr("Retype the password"));
    editPass2 = new QLineEdit;
    editPass2->setEchoMode(QLineEdit::Password);

    QLabel *lblIndice = new QLabel(tr("Indication"));
    editIndice = new QLineEdit;

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(text,0,0,1,2);

    mainLayout->addWidget(lblPseudo,1,0);
    mainLayout->addWidget(editPseudo,1,1);

    mainLayout->addWidget(lblPass1,2,0);
    mainLayout->addWidget(editPass1,2,1);

    mainLayout->addWidget(lblPass2,3,0);
    mainLayout->addWidget(editPass2,3,1);

    mainLayout->addWidget(lblIndice,4,0);
    mainLayout->addWidget(editIndice,4,1);

    mainLayout->addWidget(spacer);
    tabAccount->setBoxLayout(mainLayout);

    connect(tabAccount,SIGNAL(goBackRequest()),mapBack,SLOT(map()));
    connect(tabAccount,SIGNAL(goForwardRequest()),this,SLOT(lookFields()));

    mapBack->setMapping(tabAccount,0);
}

void StarterUser::createCustomize()
{
    tabCustomize = new TabWizard(tr("Customize"));

    QLabel *text = new QLabel;
    text->setText(tr("Choose a color for the background.<br />"));

    QPushButton *btnChoose = new QPushButton(tr("Change color"));

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(text);
    mainLayout->addWidget(btnChoose);
    mainLayout->setAlignment(btnChoose,Qt::AlignLeft);
    mainLayout->addWidget(spacer);
    tabCustomize->setBoxLayout(mainLayout);

    connect(btnChoose,SIGNAL(clicked()),this,SLOT(editColor()));
    connect(tabCustomize,SIGNAL(goBackRequest()),mapBack,SLOT(map()));
    connect(tabCustomize,SIGNAL(goForwardRequest()),mapNext,SLOT(map()));

    mapBack->setMapping(tabCustomize,1);
    mapNext->setMapping(tabCustomize,3);
}

void StarterUser::createFinish()
{
    tabFinish = new TabWizard(tr("Finish"));

    QLabel *text = new QLabel;
    text->setWordWrap(true);
    text->setText(tr("Now all is OK ! You can start browsing or continue edit settings.<br />"));

    QPushButton *btnEdit = new QPushButton(tr("Edit settings"));

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(text);
    mainLayout->addWidget(btnEdit);
    mainLayout->setAlignment(btnEdit,Qt::AlignLeft);
    mainLayout->addWidget(spacer);
    tabFinish->setBoxLayout(mainLayout);

    connect(tabFinish,SIGNAL(goBackRequest()),mapBack,SLOT(map()));
    connect(tabFinish,SIGNAL(goForwardRequest()),this,SLOT(finish()));
    connect(btnEdit,SIGNAL(clicked()),this,SLOT(showConfig()));

    mapBack->setMapping(tabFinish,2);
}

void StarterUser::lookFields()
{
    if(editPseudo->text().isEmpty())
        QMessageBox::critical(this,tr("Empty field"),tr("The field \"Pseudo\" is empty"),QMessageBox::Ok);
    else
    {
        if(editPass1->text()!=editPass2->text())
            QMessageBox::critical(this,tr("Password incorrect"),tr("Password incorrect !"),QMessageBox::Ok);
        else
            layout->setCurrentIndex(2);
    }
}

void StarterUser::editColor()
{
    QColor newColor = QColorDialog::getColor(color);
    if(newColor!=QColor::fromRgb(0,0,0))
        color = newColor;

    tabAccount->setColor(color.name());
    tabCustomize->setColor(color.name());
    tabFinish->setColor(color.name());
    tabWelcome->setColor(color.name());
}

void StarterUser::finish()
{
    saveSettings();

    emit finished();

    this->close();
}


void StarterUser::showConfig()
{
    saveSettings();

    emit editConfig();

    this->close();
}

void StarterUser::saveSettings()
{
    QSettings *settings = new QSettings;

    // NEW SETTINGS

    settings->setValue("Account/Pseudo",editPseudo->text());
    settings->setValue("Account/Password",editPass1->text());
    settings->setValue("Account/Indice",editIndice->text());

    settings->setValue("Apparence/BgColor",color.name());

    // DEFAULT SETTINGS

    settings->setValue("Version",QCoreApplication::applicationVersion());

    settings->setValue("Apparence/Animations",true);
    settings->setValue("Apparence/BgImgMainUrl","");
    settings->setValue("Apparence/BgImgLockUrl","");

    settings->setValue("Navigation/Startup/Empty",false);
    settings->setValue("Navigation/Startup/Home",true);
    settings->setValue("Navigation/Startup/Specific",false);
    settings->setValue("Navigation/Startup/SpecificPage","");
    settings->setValue("Navigation/NewTab/Empty",false);
    settings->setValue("Navigation/NewTab/Home",true);
    settings->setValue("Navigation/NewTab/Specific",false);
    settings->setValue("Navigation/NewTab/SpecificPage","");
    settings->setValue("Navigation/Home","http://doosearch.esy.es/");
    settings->setValue("Navigation/PrefixSearch","https://duckduckgo.com/?q=");
    settings->setValue("Navigation/SuffixSearch","");

    settings->setValue("Interface/Display/SideBar",true);
    settings->setValue("Interface/Display/ToolBar",true);
    settings->setValue("Interface/Display/TabBar",false);

    settings->setValue("Interface/SideBar/AddTab",false);
    settings->setValue("Interface/SideBar/Tabs",true);
    settings->setValue("Interface/SideBar/Cmd",true);
    settings->setValue("Interface/SideBar/Back",true);
    settings->setValue("Interface/SideBar/Forward",true);
    settings->setValue("Interface/SideBar/Reload",true);
    settings->setValue("Interface/SideBar/Home",true);
    settings->setValue("Interface/SideBar/Info",true);
    settings->setValue("Interface/SideBar/AddBookmark",false);
    settings->setValue("Interface/SideBar/BookmarksMenu",true);
    settings->setValue("Interface/SideBar/OpenFile",false);
    settings->setValue("Interface/SideBar/SaveFile",false);
    settings->setValue("Interface/SideBar/Lock",false);
    settings->setValue("Interface/SideBar/Screen",false);
    settings->setValue("Interface/SideBar/Full",false);
    settings->setValue("Interface/SideBar/Config",true);

    settings->setValue("Interface/ToolBar/AddTab",true);
    settings->setValue("Interface/ToolBar/Back",false);
    settings->setValue("Interface/ToolBar/Forward",false);
    settings->setValue("Interface/ToolBar/Reload",false);
    settings->setValue("Interface/ToolBar/Home",false);
    settings->setValue("Interface/ToolBar/AddBookmark",true);
    settings->setValue("Interface/ToolBar/BookmarksMenu",false);
    settings->setValue("Interface/ToolBar/Tabs",false);
    settings->setValue("Interface/ToolBar/Cmd",false);
    settings->setValue("Interface/ToolBar/Info",false);
    settings->setValue("Interface/ToolBar/OpenFile",false);
    settings->setValue("Interface/ToolBar/SaveFile",false);
    settings->setValue("Interface/ToolBar/Lock",true);
    settings->setValue("Interface/ToolBar/Screen",true);
    settings->setValue("Interface/ToolBar/Full",true);
    settings->setValue("Interface/ToolBar/Config",false);

    settings->setValue("Security/Lock/AtStart",false);
    settings->setValue("Security/Lock/AtClose",false);
    settings->setValue("Security/Lock/ToConfig",true);
    settings->setValue("Security/Lock/Locker",true);
}

void StarterUser::setMainWindow(MainWindow *window)
{
    connect(this,SIGNAL(finished()),window,SLOT(showMaximized()));
    connect(this,SIGNAL(finished()),window,SLOT(loadConfig()));
    connect(this,SIGNAL(finished()),window,SLOT(showStartupPage()));

    connect(this,SIGNAL(editConfig()),window,SLOT(showMaximized()));
    connect(this,SIGNAL(editConfig()),window,SLOT(showSettings()));
    connect(this,SIGNAL(editConfig()),window,SLOT(showStartupPage()));
}

// -----------------------------------------

TabWizard::TabWizard(QString title, QWidget *parent):
    QFrame(parent)
{
    if(title.isEmpty())
        title = tr("Title");

    lblTitle = new QLabel(title);
    lblTitle->setObjectName("Title");
    
    btnBack = new QPushButton(QIcon(":/img/back.png"),"");
    btnBack->setObjectName("Back");
    btnBack->setIconSize(QSize(15,15));
    btnBack->setIconSize(QSize(20,20));
    connect(btnBack,SIGNAL(clicked()),this,SIGNAL(goBackRequest()));
    
    btnForward = new QPushButton(tr("Continue"));
    btnForward->setObjectName("Forward");
    connect(btnForward,SIGNAL(clicked()),this,SIGNAL(goForwardRequest()));

    mainLayout = new QGridLayout;
    mainLayout->setContentsMargins(30,30,30,30);
    mainLayout->addWidget(btnBack,0,0);
    mainLayout->addWidget(lblTitle,0,1);
    mainLayout->addWidget(btnForward,2,1);
    mainLayout->setAlignment(btnForward,Qt::AlignRight);
    setLayout(mainLayout);

    setColor("rgb(50,50,50)");
}

void TabWizard::setBoxLayout(QLayout *layout)
{
    mainLayout->addLayout(layout,1,1);
}

void TabWizard::showBackButton(bool show)
{
    btnBack->setVisible(show);
}

void TabWizard::setColor(QString color)
{
    setStyleSheet("\
                  QFrame {\
                      background: " + color + ";\
                      border: none;\
                      }\
                  QLabel {\
                      color: white;\
                      font-size: 16px;\
                      font-family: segoe ui light, segoe ui;\
                      margin-top: 10px;\
                  }\
                  QLineEdit {\
                      color: black;\
                      font-size: 16px;\
                      font-family: segoe ui light, segoe ui;\
                      margin-top: 10px;\
                      padding: 4px 8px;\
                      background: white;\
                      border: none;\
                  }\
                  QPushButton {\
                      border: 2px solid white;\
                      background: transparent;\
                      color: white;\
                      font-size: 16px;\
                      padding: 4px 12px;\
                      font-family: segoe ui light, segoe ui;\
                  }\
                  QPushButton:hover {\
                      background: rgba(255,255,255,.2);\
                  }\
                  QPushButton:pressed {\
                      background: rgba(255,255,255,.4);\
                  }\
                  #Title {\
                      color: white;\
                      font-size: 30px;\
                      font-family: segoe ui light, segoe ui;\
                      margin-top: 0px;\
                  }\
                  #Back {\
                      background: transparent;\
                      border: 2px solid white;\
                      padding: 15px 13px;\
                      padding: 12px 10px;\
                      min-width: 10px; min-height: 10px;\
                      border-radius: 24px;\
                      margin-right: 15px;\
                  }");
}
