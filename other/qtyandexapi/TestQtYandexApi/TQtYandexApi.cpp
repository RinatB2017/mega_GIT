#include "Precompiled.h"
#include "TQtYandexApi.h"

#include <QtYandexApi/QtYandexApi.h>

#define END_ASYNC_FUNCTION(signalSpy, connection) \
    QVERIFY(signalSpy.wait()); \
    QObject::disconnect(connection);

TQtYandexApi::TQtYandexApi(QObject *parent)
    : QObject(parent)
    , m_pNetworkAccessManager(new QNetworkAccessManager(this))
{
}

void TQtYandexApi::init()
{
    m_pYandexDictionary = new QtYandexDictionary(QtYandexApi::getYandexKeyFromFile("dictKey"), m_pNetworkAccessManager);
}

void TQtYandexApi::testTranslation()
{
    const QString rusHome = QString("Home");
    QSignalSpy signalSpy(m_pYandexDictionary, SIGNAL(translated(QtYaWordTranslation)));
    QMetaObject::Connection connection = connect(m_pYandexDictionary, &QtYandexDictionary::translated,
                                                 [&](const QtYaWordTranslation& translation)
    {
        QVERIFY2(translation.isError() == false, translation.errorString().toStdString().c_str());
        QVERIFY(translation.wordForTranslation().wordName() == rusHome);
        QVERIFY(translation.wordForTranslation().fromLanguage() == "en");
        QVERIFY(translation.wordForTranslation().toLanguage() == "ru");
        QVERIFY(translation.translatedWord().mainTranslation().toLower() == QString::fromUtf8("дом"));
    });
    m_pYandexDictionary->translate(QtYaWord(rusHome, "en", "ru"));
    END_ASYNC_FUNCTION(signalSpy, connection)
}

void TQtYandexApi::testInvalidWordErrorTranslation()
{
    const QString rusHome = QString("hm");
    QSignalSpy signalSpy(m_pYandexDictionary, SIGNAL(translated(QtYaWordTranslation)));
    QMetaObject::Connection connection = connect(m_pYandexDictionary, &QtYandexDictionary::translated,
                                                 [&](const QtYaWordTranslation& translation)
    {
        QVERIFY(translation.translatedWord().mainTranslation().isEmpty());
        QVERIFY(translation.translatedWord().synonyms().isEmpty());
    });
    m_pYandexDictionary->translate(QtYaWord(rusHome, "en", "ru"));
    END_ASYNC_FUNCTION(signalSpy, connection)
}

void TQtYandexApi::testInvalidYandexKeyErrorTranslation()
{
    const QString rusHome = QString("home");
    QtYandexDictionary yandexTranslator("0", m_pNetworkAccessManager);
    QSignalSpy signalSpy(&yandexTranslator, SIGNAL(translated(QtYaWordTranslation)));
    QMetaObject::Connection connection = connect(&yandexTranslator, &QtYandexDictionary::translated,
                                                 [&](const QtYaWordTranslation& translation)
    {
        QVERIFY(translation.isError() == true);
    });
    yandexTranslator.translate(QtYaWord(rusHome, "en", "ru"));
    END_ASYNC_FUNCTION(signalSpy, connection)
}

void TQtYandexApi::testLanguagesGot()
{
    QSignalSpy signalSpy(m_pYandexDictionary, SIGNAL(languagesGot(QtYaLanguages)));
    QMetaObject::Connection connection = connect(m_pYandexDictionary, &QtYandexDictionary::languagesGot,
                                                 [](const QtYaLanguages& languages)
    {
        QVERIFY2(languages.languagesDirections().size() == 100, qPrintable(QString("%1").arg(languages.languagesDirections().size())));
        QVERIFY2(languages.languages().size() == 27, qPrintable(QString("%1").arg(languages.languages().size())));
        qDebug() << languages.languages();
    });
    m_pYandexDictionary->getLanguages();
    END_ASYNC_FUNCTION(signalSpy, connection)
}
