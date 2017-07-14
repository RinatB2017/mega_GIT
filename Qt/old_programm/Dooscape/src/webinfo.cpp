#include "webinfo.h"

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

WebInfo::WebInfo(QWidget *parent) :
    Panel(parent)
{
    setTitle(tr("Informations"));

    QVBoxLayout *centerLayout = new QVBoxLayout;
    centerLayout->setContentsMargins(10,10,10,10);
    centerLayout->setSpacing(0);
    setLayout(centerLayout);

    btnMenu = new QPushButton(QIcon(":/img/file.png"),"");
    btnMenu->setObjectName("BtnIcon");
    btnMenu->setIconSize(QSize(100,100));
    btnMenu->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
    btnMenu->setStyleSheet("#BtnIcon {background: transparent; border: none; padding: 0px; margin-top: 20px; margin-bottom: 30px;}");

    QLabel *titleLbl = new QLabel(tr("Title page"));
    titleLbl->setStyleSheet("margin-bottom: 5px;");

    titleBar = new QLineEdit;
    titleBar->setObjectName("TitleBar");
    titleBar->setToolTip(tr("Please enter a few words you want to search"));
    titleBar->setPlaceholderText(tr("Search"));
    connect(titleBar,SIGNAL(returnPressed()),this,SIGNAL(searchToWeb()));

    QLabel *addressBarLbl = new QLabel(tr("URL Address"));
    addressBarLbl->setStyleSheet("margin-bottom: 5px;");

    urlBar = new QLineEdit;
    urlBar->setToolTip(tr("Please enter the correct URL address of the site you want to visit"));
    urlBar->setPlaceholderText(tr("URL address"));
    connect(urlBar,SIGNAL(returnPressed()),this,SIGNAL(goToUrl()));

    //------------------------------------
    //Grille de btn (source, fav, inspect, etc...)

    QToolButton *btnAddBookmarks = new QToolButton;
    btnAddBookmarks->setIcon(QIcon(":/img/add-bookmark.png"));
    btnAddBookmarks->setText(tr("Add to bookmarks"));
    btnAddBookmarks->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnAddBookmarks->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    connect(btnAddBookmarks,SIGNAL(clicked()),this,SIGNAL(addBookmark()));


    QToolButton *btnSource = new QToolButton;
    btnSource->setIcon(QIcon(":/img/file.png"));
    btnSource->setText(tr("Show source"));
    btnSource->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnSource->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);

    QToolButton *btnInspect = new QToolButton;
    btnInspect->setIcon(QIcon(":/img/menu.png"));
    btnInspect->setText(tr("Inspect page"));
    btnInspect->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnInspect->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    connect(btnInspect,SIGNAL(clicked()),this,SIGNAL(inspectPage()));

    QGridLayout *grid = new QGridLayout;
    grid->setContentsMargins(0,30,0,0);
    grid->setSpacing(0);
    grid->addWidget(btnAddBookmarks);
    //grid->addWidget(btnSource);
    grid->addWidget(btnInspect);

    // --------

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    centerLayout->addWidget(btnMenu);
    centerLayout->setAlignment(btnMenu,Qt::AlignCenter);
    centerLayout->addWidget(titleLbl);
    centerLayout->addWidget(titleBar);
    centerLayout->addWidget(addressBarLbl);
    centerLayout->addWidget(urlBar);
    centerLayout->addLayout(grid);
    centerLayout->addWidget(spacer);

    setDefaultColor("orange");
}

void WebInfo::setIcon(QIcon icon)
{
   // btnMenu->setIcon(icon.pixmap(btnMenu->iconSize()));
   // btnMenu->setIcon(icon.pixmap(65,65).scaled(65,65,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    btnMenu->setIcon(icon.pixmap(100,100).scaled(100,100,Qt::IgnoreAspectRatio,Qt::FastTransformation));
}

void WebInfo::setWebTitle(QString title)
{
    titleBar->setText(title);
}

void WebInfo::setUrl(QUrl url)
{
    urlBar->setText(url.toString());
}

void WebInfo::setFocus()
{
    titleBar->selectAll();
    titleBar->setFocus();
}

QString WebInfo::getText()
{
    return titleBar->text();
}

QString WebInfo::getUrl()
{
    return urlBar->text();
}

void WebInfo::setDefaultColor(QString colorName)
{
    color = colorName;
    setColor(color);
}
