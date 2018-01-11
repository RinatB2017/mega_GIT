#include "tabview.h"

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

TabView::TabView(QWidget *parent) :
    Panel(parent)
{
    setTitle(tr("Tabs"));
    compact = false;

    QPushButton *btnAddTab = new QPushButton(this);
    btnAddTab->setIcon(QIcon(":/img/add-tab.png"));
    btnAddTab->setToolTip(tr("New Tab"));
    btnAddTab->setObjectName("TopBtn");
    connect(btnAddTab,SIGNAL(clicked()),this,SIGNAL(addTab()));
    btnAddTab->move(150,0);
    btnAddTab->setIconSize(QSize(19,19));

    QPushButton *btnMenu = new QPushButton(this);
    btnMenu->setIcon(QIcon(":/img/menu.png"));
    btnMenu->setToolTip(tr("Menu"));
    btnMenu->setObjectName("TopBtn");
    btnMenu->move(200,0);
    btnMenu->setIconSize(QSize(20,20));

    actViewNormal = new QAction(QIcon(":/img/tabs.png"),tr("View Normal"),this);
    actViewNormal->setVisible(false);
    connect(actViewNormal,SIGNAL(triggered()),this,SLOT(changeView()));
    actViewCompact = new QAction(QIcon(":/img/menu.png"),tr("View Compact"),this);
    connect(actViewCompact,SIGNAL(triggered()),this,SLOT(changeView()));

    QMenu *mainMenu = new QMenu;
    mainMenu->addAction(actViewNormal);
    mainMenu->addAction(actViewCompact);

    btnMenu->setMenu(mainMenu);

    layout = new QVBoxLayout;
    layout->setContentsMargins(10,0,10,10);
    layout->setSpacing(10);
    layout->setSpacing(0);

    QFrame *widget = new QFrame;
    widget->setStyleSheet("QFrame {border: none; background: transparent;}");
    widget->setLayout(layout);

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout->addWidget(spacer);

    QScrollArea *view = new QScrollArea;
    view->setStyleSheet("QScrollArea {background: transparent;}");
    view->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    view->setWidget(widget);
    view->setWidgetResizable(true);

    QGridLayout *center = new QGridLayout;
    center->setContentsMargins(0,0,0,0);
    center->setSpacing(0);
    center->addWidget(view,0,0);
    setLayout(center);

    viewSignal = new QSignalMapper(this);
    connect(viewSignal,SIGNAL(mapped(int)),this,SIGNAL(setCurrentTab(int)));

    closeSignal = new QSignalMapper(this);
    connect(closeSignal,SIGNAL(mapped(int)),this,SIGNAL(tabCloseRequested(int)));
}

void TabView::setTabWidget(QTabWidget *tabs)
{
    // Clean Layout
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != 0)
    {
        item->widget()->deleteLater();
        delete item;
    }

    // Update Tabs
    for(int i(0);i<tabs->count();i++)
    {
        QIcon icon = tabs->widget(i)->findChild<WebView *>()->icon();
        if(icon.isNull())
            icon = QIcon(":/img/file.png");

        QString title = tabs->widget(i)->findChild<WebView *>()->title();
        if(title.isEmpty())
            title = tabs->widget(i)->findChild<WebView *>()->url().fileName();
     /*   if(title.size()>=20)
            title = title.left(20) + "...";*/

        TabItem *tab = new TabItem;
        tab->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        tab->setIcon(icon);
        tab->setTitle(" "+title);
        layout->addWidget(tab);

        if(i == tabs->currentIndex())
            tab->setActive(true);
        else
            tab->setActive(false);

        tab->setCompactMode(compact);

        viewSignal->setMapping(tab,i);
        connect(tab,SIGNAL(clicked()),viewSignal,SLOT(map()));

        closeSignal->setMapping(tab,i);
        connect(tab,SIGNAL(closeRequested()),closeSignal,SLOT(map()));
    }

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout->addWidget(spacer);
}

void TabView::changeView()
{
    if(sender()==actViewCompact)
    {
        actViewCompact->setVisible(false);
        actViewNormal->setVisible(true);
        compact = true;
    }
    if(sender()==actViewNormal)
    {
        actViewNormal->setVisible(false);
        actViewCompact->setVisible(true);
        compact = false;
    }

    emit viewModeChanged();
}

TabItem::TabItem(QWidget *parent) :
    QFrame(parent)
{
    QVBoxLayout *center = new QVBoxLayout;
    setLayout(center);
    center->setContentsMargins(0,0,0,0);
    center->setSpacing(0);

    btnIcon = new QPushButton(QIcon(":/img/file.png"),tr(""));
    btnIcon->setIconSize(QSize(40,40));
   // btnIcon->setText(" •");
   // btnIcon->setIconSize(QSize(240,240));
    btnIcon->setFixedHeight(100);
    btnIcon->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    center->addWidget(btnIcon);
    connect(btnIcon,SIGNAL(clicked()),this,SIGNAL(clicked()));

    btnTitle = new QToolButton;
    btnTitle->setText(tr("New Tab"));
    btnTitle->setIcon(QIcon(":/img/file.png"));
    btnTitle->setFixedHeight(32);
    btnTitle->setMaximumWidth(230);
    btnTitle->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnTitle->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    center->addWidget(btnTitle);
    connect(btnTitle,SIGNAL(clicked()),this,SIGNAL(clicked()));

    setFixedHeight(132);
    setMaximumWidth(280);

    btnClose = new QPushButton(QIcon(":/img/close.png"),"",this);
    btnClose->setObjectName("BtnClose");
    btnClose->setIconSize(QSize(20,20));
    btnClose->setIconSize(QSize(15,15));
    btnClose->move(this->geometry().width()-(3+btnClose->geometry().width()),3);
    btnClose->move(this->geometry().width()-(0+btnClose->geometry().width()),0);
    connect(btnClose,SIGNAL(clicked()),this,SIGNAL(closeRequested()));
}

void TabItem::resizeEvent(QResizeEvent *event)
{
    btnClose->move(this->geometry().width()-(3+btnClose->geometry().width()),3);
    btnClose->move(this->geometry().width()-(0+btnClose->geometry().width()),0);
    event->accept();
}

void TabItem::setCompactMode(bool compact)
{
    if(compact)
    {
        btnIcon->setVisible(false);
        setFixedHeight(35);
    }
    else
    {
        btnIcon->setVisible(true);
        setFixedHeight(132);
    }
}

void TabItem::setTitle(QString title)
{
    btnTitle->setText(title);
}

void TabItem::setIcon(QIcon icon)
{
    if(icon.isNull())
        icon = QIcon(":/img/file.png");

    btnTitle->setIcon(icon);
}

void TabItem::setActive(bool active)
{
    int background(0);

    if(active)
        background = 2;

    this->setStyleSheet("\
                  QFrame {\
                      border: none;\
                      border-bottom: 1px solid white;\
                      background: rgba(255,255,255,." + QString::number(background) + ");\
                      margin: 0px;\
                  }\
                  QFrame:hover {\
                      background: rgba(255,255,255,.4);\
                  }\
                  QPushButton {font-size: 55px;}\
                  QToolButton,QPushButton {\
                      border: none;\
                      background: transparent;\
                      padding: 5px;\
                      margin: 0px;\
                      }\
                  #BtnClose {\
                      min-height: 25px;\
                      max-width: 25px;\
                      border: none;\
                      background: none;\
                      padding: 5px;\
                  }\
                  #BtnClose:hover {\
                      background: rgba(0,0,0,.2);\
                  }\
                  #BtnClose:pressed {\
                      background: rgba(0,0,0,.5);\
                  }");
}
