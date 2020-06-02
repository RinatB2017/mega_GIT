#ifndef D3WEBPAGE_H
#define D3WEBPAGE_H

#include <QWebPage>

class D3WebPage : public QWebPage
{
    Q_OBJECT
public:
    explicit D3WebPage(QWidget *parent = 0);

protected:
    void javaScriptConsoleMessage(const QString &message, int lineNumber, const QString &sourceID);

};

#endif // D3WEBPAGE_H
