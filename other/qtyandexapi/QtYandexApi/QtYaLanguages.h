#ifndef QTYALANGUAGES_H
#define QTYALANGUAGES_H

#include <QSet>
#include <QString>
#include <QStringList>

#include "QtYaError.h"
#include "qtyandexapi_global.h"

class QTYANDEXAPISHARED_EXPORT QtYaLanguages : public QtYaError
{
public:
    QtYaLanguages();
    QtYaLanguages(const QString& errorString);
    QtYaLanguages(const QStringList& languagesDirections);


    bool isDirectionSupported(const QString& fromLanguage, const QString& toLanguage) const;
    QSet<QString> languages() const;
    const QSet<QString> &languagesDirections() const;

    bool isNull() const;

private:
    QSet<QString> m_languagesDirections;
};

#endif // QTYALANGUAGES_H
