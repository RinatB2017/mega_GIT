#include "Precompiled.h"
#include "QtYaWordTranslation.h"

#include "Common.h"

MetaTypeRegister<QtYaWordTranslation> g_qtYaWordTranslationRegister("QtYaWordTranslation");

QtYaWordTranslation::QtYaWordTranslation()
{

}

QtYaWordTranslation::QtYaWordTranslation(const QtYaWord &wordForTranslation, const QString &errorString)
    : QtYaError(errorString)
    , m_wordForTranslation(wordForTranslation)
{

}

QtYaWordTranslation::QtYaWordTranslation(const QtYaWord &wordForTranslation, const QtYaTranslatedWord &translatedWord)
    : QtYaError(translatedWord.errorString())
    , m_wordForTranslation(wordForTranslation)
    , m_translatedWord(translatedWord)
{
}

const QtYaWord &QtYaWordTranslation::wordForTranslation() const
{
    return m_wordForTranslation;
}

const QtYaTranslatedWord &QtYaWordTranslation::translatedWord() const
{
    return m_translatedWord;
}

bool QtYaWordTranslation::isNull() const
{
    return m_wordForTranslation.isNull() || m_translatedWord.isNull();
}
