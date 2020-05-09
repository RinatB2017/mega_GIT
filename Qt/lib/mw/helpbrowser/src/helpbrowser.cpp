//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "helpbrowser.hpp" 
#include "defines.hpp"
//--------------------------------------------------------------------------------
HelpBrowser::HelpBrowser(QString &page,
                         QWidget *parent)
    : QWidget(parent)
{
    textBrowser = new QTextBrowser(this);
    homeButton  = new QPushButton(QObject::tr("Home"), this);
    backButton  = new QPushButton(QObject::tr("Back"), this);
    closeButton = new QPushButton(QObject::tr("Close"), this);

    QVBoxLayout *mainLayout = new QVBoxLayout();

    QHBoxLayout *buttonLayout = new QHBoxLayout();
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
        textBrowser->disconnect();
        textBrowser->deleteLater();
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
        break;
    }
}
//--------------------------------------------------------------------------------
