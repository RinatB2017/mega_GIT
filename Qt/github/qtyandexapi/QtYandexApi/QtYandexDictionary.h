#ifndef QTYANDEXDICTIONARY_H
#define QTYANDEXDICTIONARY_H

#include <QObject>

#include "qtyandexapi_global.h"

#include "QtYaWord.h"
#include "QtYaWordTranslation.h"
#include "QtYaLanguages.h"

class QNetworkAccessManager;

class QTYANDEXAPISHARED_EXPORT QtYandexDictionary : public QObject
{
    Q_OBJECT
public:
    explicit QtYandexDictionary(const QString& yandexApiKey, QNetworkAccessManager* pNetworkAccessManager);

    void translate(const QtYaWord& word);
    void getLanguages();

signals:
    void translated(const QtYaWordTranslation& translation) const;
    void languagesGot(const QtYaLanguages& langs) const;

private:
    QNetworkAccessManager* const m_pNetworkAccessManager;
    QString m_yandexApiKey;
};

#endif // QTYANDEXDICTIONARY_H
