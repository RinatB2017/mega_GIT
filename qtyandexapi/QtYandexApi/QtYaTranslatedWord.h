#ifndef QTYATRANSLATEDWORD_H
#define QTYATRANSLATEDWORD_H

#include <QString>
#include <QStringList>
#include <QVector>
#include <QPair>
#include <QJsonObject>

#include "QtYaError.h"

#include "qtyandexapi_global.h"

class QTYANDEXAPISHARED_EXPORT QtYaTranslatedWord : public QtYaError
{
public:
    QtYaTranslatedWord();
    QtYaTranslatedWord(const QJsonObject& yandexJsonWord);

    const QString& mainTranslation() const;
    const QStringList& synonyms() const;
    const QVector<QPair<QString, QString>>& examples() const;

    bool isNull() const;

private:
    QVector<QPair<QString, QString> > getExamples(const QJsonArray &jsonExamples);
    void setYandexError(int errorCode);

    QString m_mainTranslation;
    QStringList m_synonyms;
    QVector<QPair<QString, QString>> m_examples;
};

#endif // QTYATRANSLATEDWORD_H
