//--------------------------------------------------------------------------------
#include <QDesktopWidget>
#include <QApplication>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStyle>
//--------------------------------------------------------------------------------
#include "helpbrowser.hpp" 
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
HelpBrowser::HelpBrowser(QString &page,
                         QWidget *parent,
                         const char *name)
    : QWidget(parent)
{
    setObjectName(name);

    textBrowser = new QTextBrowser(this);
    homeButton  = new QPushButton(QObject::tr("&Home"), this);
    backButton  = new QPushButton(QObject::tr("&Back"), this);
    closeButton = new QPushButton(QObject::tr("Close"), this);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(homeButton);
    buttonLayout->addWidget(backButton);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(closeButton);

    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(textBrowser);
    setLayout(mainLayout);

    connect(homeButton, SIGNAL(clicked()),  textBrowser, SLOT(home()));
    connect(backButton, SIGNAL(clicked()),  textBrowser, SLOT(backward()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(textBrowser, SIGNAL(sourceChanged(QUrl)), this, SLOT(updateCaption(QUrl)));

    textBrowser->setSource(QUrl(page));

    setGeometry(QStyle::alignedRect(Qt::LeftToRight,
                                    Qt::AlignCenter,
                                    size(),
                                    qApp->desktop()->availableGeometry()));
}
//--------------------------------------------------------------------------------
void HelpBrowser::updateCaption(QUrl)
{ 
    //setCaption(tr("Help: %1").arg(textBrowser->documentTitle()));
    setCaption(textBrowser->documentTitle());
}
//--------------------------------------------------------------------------------
void HelpBrowser::setCaption(const QString &caption)
{
    setWindowTitle(caption);
}
//--------------------------------------------------------------------------------
void HelpBrowser::showPage(QString page, bool is_maximized)
{ 
    HelpBrowser *browser = new HelpBrowser(page);
    browser->resize(640, 480);
    if(is_maximized)
        browser->showMaximized();
    else
        browser->show();
}
//--------------------------------------------------------------------------------
