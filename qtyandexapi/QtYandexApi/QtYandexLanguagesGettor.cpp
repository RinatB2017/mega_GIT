#include "Precompiled.h"
#include "QtYandexLanguagesGettor.h"

const QString g_yandexGetLanguagesUrl("https://dictionary.yandex.net/api/v1/dicservice.json/getLangs");

QtYandexLanguagesGettor::QtYandexLanguagesGettor(const QString &yandexApiKey, QNetworkAccessManager *pNetworkAccessManager, QObject *pParent)
    : QObject(pParent)
    , m_pNetworkAccessManager(pNetworkAccessManager)
{
    QNetworkRequest request(getLanguagesRequest(yandexApiKey));
    QNetworkReply* pNetworkReply = m_pNetworkAccessManager->get(request);
    connect(pNetworkReply, &QNetworkReply::finished, this, &QtYandexLanguagesGettor::onGetLanguagesRequestFinished);
}

void QtYandexLanguagesGettor::onGetLanguagesRequestFinished() const
{
    QNetworkReply* pNetworkReply = qobject_cast<QNetworkReply*>(sender());
    QtYaLanguages languages;
    if (pNetworkReply->error() == QNetworkReply::NoError) {
        languages = getLanguages(pNetworkReply->readAll());
    } else {
        languages = QtYaLanguages(pNetworkReply->errorString());
    }
    pNetworkReply->deleteLater();
    emit languagesGot(languages);
}

QUrl QtYandexLanguagesGettor::getLanguagesRequest(const QString &yandexApiKey) const
{
    QUrl result(g_yandexGetLanguagesUrl);
    QUrlQuery query;
    query.addQueryItem("key", yandexApiKey);
    result.setQuery(query);
    return result;
}

QtYaLanguages QtYandexLanguagesGettor::getLanguages(const QByteArray &json) const
{
    QtYaLanguages result;
    QJsonParseError parseError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(json, &parseError);
    QStringList languagesDirections;
    if (parseError.error == QJsonParseError::NoError) {
        for (QJsonValue value : jsonDocument.array())
            languagesDirections.push_back(value.toString());
    result = QtYaLanguages(languagesDirections);
    } else {
        result = QtYaLanguages(parseError.errorString());
    }
    return result;
}
