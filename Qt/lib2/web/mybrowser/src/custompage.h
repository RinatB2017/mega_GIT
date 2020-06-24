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
    CustomPage(QWebEngineProfile *profile, QObject* parent = nullptr) :
        QWebEnginePage(profile, parent)
    {

    }

    virtual void javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level,
                                          const QString &message,
                                          int lineNumber,
                                          const QString &sourceID)
    {
        Q_UNUSED(sourceID);

        switch(level)
        {
        case JavaScriptConsoleMessageLevel::InfoMessageLevel:
            emit err_output(QString("INFO: msg [%1] line_number %2")
                            .arg(message)
                            .arg(lineNumber));
            break;

        case JavaScriptConsoleMessageLevel::WarningMessageLevel:
#if 0
            emit err_output(QString("WARNING: msg [%1] line_number %2")
                            .arg(message)
                            .arg(lineNumber));
#endif
            break;

        case JavaScriptConsoleMessageLevel::ErrorMessageLevel:
            emit err_output(QString("ERROR: msg [%1] line_number %2")
                            .arg(message)
                            .arg(lineNumber));
            break;
        }
    }
};
//--------------------------------------------------------------------------------
#endif // CUSTOMPAGE_H
