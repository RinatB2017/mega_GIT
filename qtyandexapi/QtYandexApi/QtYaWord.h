#ifndef QTYAWORD_H
#define QTYAWORD_H

#include <QString>

#include "qtyandexapi_global.h"

class QTYANDEXAPISHARED_EXPORT QtYaWord
{
public:
    QtYaWord();
    QtYaWord(const QString& wordName, const QString& fromLanguage, const QString& toLanguage);

    QString wordName() const;
    QString fromLanguage() const;
    QString toLanguage() const;

    bool isNull() const;

private:
    QString m_wordName;
    QString m_fromLanguage;
    QString m_toLanguage;
};

#endif // QTYAWORD_H
