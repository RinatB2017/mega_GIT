/*********************************************************************************
**                                                                              **
**     Copyright (C) 2025                                                       **
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
#include "defines.hpp"
//--------------------------------------------------------------------------------
HelpBrowser::HelpBrowser(const QString &page,
                         QWidget *parent)
    : QWidget(parent)
{
    textBrowser = new QTextBrowser(this);
    Q_ASSERT(textBrowser);
    homeButton  = new QPushButton(QObject::tr("Home"), this);
    Q_ASSERT(homeButton);
    backButton  = new QPushButton(QObject::tr("Back"), this);
    Q_ASSERT(backButton);
    closeButton = new QPushButton(QObject::tr("Close"), this);
    Q_ASSERT(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    Q_ASSERT(mainLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    Q_ASSERT(buttonLayout);
    buttonLayout->addWidget(homeButton);
    buttonLayout->addWidget(backButton);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(closeButton);

    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(textBrowser);
    setLayout(mainLayout);

    connect(homeButton,     &QPushButton::clicked,          textBrowser,    &QTextBrowser::home);
    connect(backButton,     &QPushButton::clicked,          textBrowser,    &QTextBrowser::backward);
    connect(closeButton,    &QPushButton::clicked,          this,           &HelpBrowser::close);
    connect(textBrowser,    &QTextBrowser::sourceChanged,   this,           &HelpBrowser::updateCaption);

    textBrowser->setSource(QUrl(page));

    setGeometry(QStyle::alignedRect(Qt::LeftToRight,
                                    Qt::AlignCenter,
                                    size(),
                                    QGuiApplication::screens()[0]->geometry()));
//                                    qApp->desktop()->availableGeometry()));
}
//--------------------------------------------------------------------------------
HelpBrowser::~HelpBrowser()
{
    if(textBrowser)
    {
        disconnect(textBrowser,    &QTextBrowser::sourceChanged,   this,           &HelpBrowser::updateCaption);
        delete textBrowser;
    }
}
//--------------------------------------------------------------------------------
void HelpBrowser::updateCaption(QUrl)
{ 
    setCaption(textBrowser->documentTitle());
}
//--------------------------------------------------------------------------------
void HelpBrowser::setCaption(const QString &caption)
{
    setWindowTitle(caption);
}
//--------------------------------------------------------------------------------
void HelpBrowser::showPage(const QString &page,
                           bool is_maximized)
{ 
    HelpBrowser *browser = new HelpBrowser(page);
    browser->resize(640, 480);
    if(is_maximized)
        browser->showMaximized();
    else
        browser->show();
}
//--------------------------------------------------------------------------------
void HelpBrowser::changeEvent(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::LanguageChange:
        homeButton->setText(QObject::tr("Home"));
        backButton->setText(QObject::tr("Back"));
        closeButton->setText(QObject::tr("Close"));
        break;

    default:
        QWidget::changeEvent(event);
        break;
    }
}
//--------------------------------------------------------------------------------
