#include "Precompiled.h"
#include "QtYaWord.h"

#include "Precompiled.h"
#include "QtYaWord.h"

QtYaWord::QtYaWord()
{

}

QtYaWord::QtYaWord(const QString &word, const QString &fromLanguage, const QString &toLanguage)
    : m_wordName(word)
    , m_fromLanguage(fromLanguage)
    , m_toLanguage(toLanguage)
{
}

QString QtYaWord::wordName() const
{
    return m_wordName;
}

QString QtYaWord::fromLanguage() const
{
    return m_fromLanguage;
}

QString QtYaWord::toLanguage() const
{
    return m_toLanguage;
}

bool QtYaWord::isNull() const
{
    return m_wordName.isNull() || m_fromLanguage.isNull() || m_toLanguage.isNull();
}
