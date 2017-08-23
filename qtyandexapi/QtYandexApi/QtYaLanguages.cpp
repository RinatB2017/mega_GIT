#include "Precompiled.h"
#include "QtYaLanguages.h"

#include "Common.h"

MetaTypeRegister<QtYaLanguages> g_qtYaLanguagesRegister("QtYaLanguages");

QtYaLanguages::QtYaLanguages()
{

}

QtYaLanguages::QtYaLanguages(const QString &errorString)
    : QtYaError(errorString)
{

}

QtYaLanguages::QtYaLanguages(const QStringList &languagesDirections)
    : m_languagesDirections(languagesDirections.toSet())
{

}

bool QtYaLanguages::isDirectionSupported(const QString &fromLanguage, const QString &toLanguage) const
{
    QString languagesDirection = fromLanguage.left(2) + "-" + toLanguage.left(2);
    return m_languagesDirections.contains(languagesDirection.toLower());
}

QSet<QString> QtYaLanguages::languages() const
{
    QSet<QString> result;
    for (QString langDirection : m_languagesDirections)
        foreach (QString lang, langDirection.split("-"))
            result.insert(lang.trimmed().toLower());
    return result;
}

const QSet<QString>& QtYaLanguages::languagesDirections() const
{
    return m_languagesDirections;
}

bool QtYaLanguages::isNull() const
{
    return m_languagesDirections.isEmpty();
}
