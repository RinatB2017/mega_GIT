#include "Precompiled.h"
#include "QtYaTranslatedWord.h"

QtYaTranslatedWord::QtYaTranslatedWord()
{

}

QtYaTranslatedWord::QtYaTranslatedWord(const QJsonObject &yandexJsonWord)
{
    QJsonValue code = yandexJsonWord["code"];
    if (code.isUndefined()) {
        QJsonObject def = yandexJsonWord["def"].toArray().at(0).toObject();
        QJsonObject tr = def["tr"].toArray().at(0).toObject();
        if (!tr.isEmpty()) {
            m_mainTranslation = tr["text"].toString();
            for (const QJsonValue& value : tr["syn"].toArray())
                m_synonyms.push_back(value.toObject()["text"].toString());
            m_examples = getExamples(tr["ex"].toArray());
        }
    } else {
        setYandexError(code.toString().toInt());
    }
}

const QString &QtYaTranslatedWord::mainTranslation() const
{
    return m_mainTranslation;
}

const QStringList &QtYaTranslatedWord::synonyms() const
{
    return m_synonyms;
}

const QVector<QPair<QString, QString> > &QtYaTranslatedWord::examples() const
{
    return m_examples;
}

bool QtYaTranslatedWord::isNull() const
{
    return m_mainTranslation.isNull();
}

QVector<QPair<QString, QString> > QtYaTranslatedWord::getExamples(const QJsonArray &jsonExamples)
{
    QVector<QPair<QString, QString> > result;
    for (const QJsonValue& value : jsonExamples) {
        QJsonObject jsonObject = value.toObject();
        QPair<QString, QString> pair;
        pair.first = jsonObject["text"].toString();
        pair.second = jsonObject["tr"].toArray().at(0).toObject()["text"].toString();
        result.push_back(pair);
    }
    return result;
}

void QtYaTranslatedWord::setYandexError(int errorCode)
{
    switch (errorCode) {
    case 401:
        setError(QObject::tr("Invalid API key."));
        break;
    case 402:
        setError(QObject::tr("This API key has been blocked."));
        break;
    case 403:
        setError(QObject::tr("Exceeded the daily limit on the number of requests."));
        break;
    case 413:
        setError(QObject::tr("The text size exceeds the maximum."));
        break;
    case 501:
        setError(QObject::tr("The specified translation direction is not supported."));
        break;
    default:
        break;
    }
}
