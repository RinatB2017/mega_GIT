#ifndef TQTYANDEXAPI_H
#define TQTYANDEXAPI_H

#include <QObject>

class QtYandexDictionary;
class QNetworkAccessManager;

class TQtYandexApi : public QObject
{
    Q_OBJECT
public:
    explicit TQtYandexApi(QObject *parent = 0);

private slots:
    void init();
    void testTranslation();
    void testInvalidWordErrorTranslation();
    void testInvalidYandexKeyErrorTranslation();
    void testLanguagesGot();

private:
    QNetworkAccessManager* const m_pNetworkAccessManager;
    QtYandexDictionary* m_pYandexDictionary;

};

#endif // TQTYANDEXAPI_H
