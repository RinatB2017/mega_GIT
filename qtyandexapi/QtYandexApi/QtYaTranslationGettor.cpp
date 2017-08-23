#include "Precompiled.h"
#include "QtYaTranslationGettor.h"

const QString g_yandexTranslationUrl("https://dictionary.yandex.net/api/v1/dicservice.json/lookup");

QtYaTranslationGettor::QtYaTranslationGettor(const QString &yandexApiKey, const QtYaWord &wordForTranslation, QNetworkAccessManager *pNetworkAccessManager, QObject *pParent)
    : QObject(pParent)
    , m_wordForTranslation(wordForTranslation)
    , m_pNetworkAccessManager(pNetworkAccessManager)
{
    QNetworkRequest networkRequest(translateRequest(m_wordForTranslation, yandexApiKey));
    QNetworkReply* pNetworkReply = m_pNetworkAccessManager->get(networkRequest);
    connect(pNetworkReply, &QNetworkReply::finished, this, &QtYaTranslationGettor::onTranslateFinished);
}

void QtYaTranslationGettor::onTranslateFinished() const
{
    QNetworkReply* pNetworkReply = qobject_cast<QNetworkReply*>(sender());
    QtYaWordTranslation result(m_wordForTranslation, pNetworkReply->errorString());
    if (pNetworkReply->error() == QNetworkReply::NoError) {
        QJsonParseError parseError;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(pNetworkReply->readAll(), &parseError);
        if (parseError.error == QJsonParseError::NoError) {
            result = QtYaWordTranslation(m_wordForTranslation, QtYaTranslatedWord(jsonDocument.object()));
        } else
            result = QtYaWordTranslation(m_wordForTranslation, parseError.errorString());
    } else {
        result = QtYaWordTranslation(m_wordForTranslation, pNetworkReply->errorString());
    }
    pNetworkReply->deleteLater();
    emit translated(result);
}

QUrl QtYaTranslationGettor::translateRequest(const QtYaWord &wordForTranslation, const QString &yandexApiKey) const
{
    QUrl result(g_yandexTranslationUrl);
    QString lang = wordForTranslation.fromLanguage() + "-" + wordForTranslation.toLanguage();
    QUrlQuery query;
    query.addQueryItem("key", yandexApiKey);
    query.addQueryItem("lang", lang);
    query.addQueryItem("text", wordForTranslation.wordName());
    result.setQuery(query);
    return result;
}
