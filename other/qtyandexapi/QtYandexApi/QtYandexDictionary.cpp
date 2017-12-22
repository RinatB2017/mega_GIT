#include "Precompiled.h"
#include "QtYandexDictionary.h"

#include "QtYaTranslationGettor.h"
#include "QtYandexLanguagesGettor.h"

QtYandexDictionary::QtYandexDictionary(const QString &yandexApiKey, QNetworkAccessManager *pNetworkAccessManager)
    : QObject(pNetworkAccessManager)
    , m_pNetworkAccessManager(pNetworkAccessManager)
    , m_yandexApiKey(yandexApiKey)
{
}

void QtYandexDictionary::translate(const QtYaWord &word)
{
    QtYaTranslationGettor* pYandexTranslationGettor = new QtYaTranslationGettor(m_yandexApiKey, word, m_pNetworkAccessManager, this);
    connect(pYandexTranslationGettor, &QtYaTranslationGettor::translated, this, &QtYandexDictionary::translated);
    connect(pYandexTranslationGettor, &QtYaTranslationGettor::translated, pYandexTranslationGettor, &QtYaTranslationGettor::deleteLater);
}

void QtYandexDictionary::getLanguages()
{
    QtYandexLanguagesGettor* pYandexLanguagesGettor = new QtYandexLanguagesGettor(m_yandexApiKey, m_pNetworkAccessManager, this);
    connect(pYandexLanguagesGettor, &QtYandexLanguagesGettor::languagesGot, this, &QtYandexDictionary::languagesGot);
    connect(pYandexLanguagesGettor, &QtYandexLanguagesGettor::languagesGot, pYandexLanguagesGettor, &QtYandexLanguagesGettor::deleteLater);
}
