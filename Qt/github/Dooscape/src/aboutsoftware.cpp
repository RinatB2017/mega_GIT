#include "aboutsoftware.h"

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

AboutSoftware::AboutSoftware(QWidget *parent) :
    Panel(parent)
{
    setTitle(tr("About ")+QCoreApplication::applicationName());

    QLabel *textView = new QLabel;
    textView->setWordWrap(true);
    textView->setStyleSheet("border-bottom: 10px solid white; padding-bottom: 15px; margin-bottom: 15px;");
    textView->setText(tr("<b>%1 %2 by %3 :</b><br/>Dooscape is a web browser writed with the Qt(Creator) framework and using Webkit as web engine. It is different compared to its interface, modern and innovative.<br /><br /><b>Licence :</b><br />Licence LGPL v.3<br /><br /><b>Developpers :</b><ul><li>aero15 : Creator</li></ul><br /><b>Contributors :</b> <ul><li></li><!--<li>Item</li>--></ul>").arg(QCoreApplication::applicationName(),QCoreApplication::applicationVersion(),QCoreApplication::organizationName()));
    textView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QToolButton *btnQt = new QToolButton;
    btnQt->setText(tr("About Qt"));
    btnQt->setIcon(QIcon(":/img/information.png"));
    btnQt->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnQt->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    connect(btnQt,SIGNAL(clicked()),qApp,SLOT(aboutQt()));

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QPushButton *lblIcon = new QPushButton(QIcon(":/res/icon.ico"),"");
    lblIcon->setObjectName("BtnIcon");
    lblIcon->setIconSize(QSize(150,150));
    lblIcon->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
    lblIcon->setStyleSheet("#BtnIcon {background: transparent; border: none; padding: 0px; margin-top: 20px; margin-bottom: 30px;}");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(10,10,10,10);
    layout->addWidget(lblIcon);
    layout->setAlignment(lblIcon,Qt::AlignCenter);
    layout->addWidget(textView);
    layout->addWidget(btnQt);
    layout->addWidget(spacer);
    layout->setSpacing(0);
    setLayout(layout);
}
