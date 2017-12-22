#ifndef QTYANDEXLANGUAGESGETTOR_H
#define QTYANDEXLANGUAGESGETTOR_H

#include <QObject>

#include "QtYaLanguages.h"

class QNetworkAccessManager;

class QtYandexLanguagesGettor : public QObject
{
    Q_OBJECT
public:
    explicit QtYandexLanguagesGettor(const QString& yandexApiKey, QNetworkAccessManager* pNetworkAccessManager, QObject *pParent = 0);

signals:
    void languagesGot(const QtYaLanguages& langs) const;

private slots:
    void onGetLanguagesRequestFinished() const;

private:
    QUrl getLanguagesRequest(const QString& yandexApiKey) const;
    QtYaLanguages getLanguages(const QByteArray& json) const;

    QNetworkAccessManager* const m_pNetworkAccessManager;

};

#endif // QTYANDEXLANGUAGESGETTOR_H
