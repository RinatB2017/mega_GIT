#ifndef QTYAWORDTRANSLATION_H
#define QTYAWORDTRANSLATION_H

#include <QString>
#include <QStringList>
#include <QPair>

#include "QtYaWord.h"
#include "QtYaTranslatedWord.h"
#include "qtyandexapi_global.h"

class QTYANDEXAPISHARED_EXPORT QtYaWordTranslation : public QtYaError
{
public:
    QtYaWordTranslation();
    QtYaWordTranslation(const QtYaWord& wordForTranslation, const QString& errorString);
    QtYaWordTranslation(const QtYaWord& wordForTranslation, const QtYaTranslatedWord& translatedWord);

    const QtYaWord& wordForTranslation() const;
    const QtYaTranslatedWord& translatedWord() const;


    bool isNull() const;

private:
    QtYaWord m_wordForTranslation;
    QtYaTranslatedWord m_translatedWord;
};

#endif // QTYAWORDTRANSLATION_H
