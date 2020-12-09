#include "d3webpage.h"
#include <QDebug>

D3WebPage::D3WebPage(QWidget *parent) :
    QWebPage(parent)
{
}

void D3WebPage::javaScriptConsoleMessage(const QString &message, int lineNumber, const QString &sourceID)
{
    qDebug() << "source:" << sourceID << " line: " << lineNumber << "  " << message;
}
