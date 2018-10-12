#ifndef CUSTOMPAGE_H
#define CUSTOMPAGE_H
//--------------------------------------------------------------------------------
#include <QWebEnginePage>
//--------------------------------------------------------------------------------
class CustomPage : public QWebEnginePage
{
    Q_OBJECT

signals:
    void err_output(const QString &);

public:
    CustomPage(QObject* parent = 0) :
        QWebEnginePage(parent)
    {

    }

    virtual void javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level,
                                          const QString &message,
                                          int lineNumber,
                                          const QString &sourceID)
    {
        Q_UNUSED(level);
        Q_UNUSED(sourceID);

        emit err_output(QString("msg [%1] line_number %2")
                        .arg(message)
                        .arg(lineNumber));
    }
};
//--------------------------------------------------------------------------------
#endif // CUSTOMPAGE_H
