//--------------------------------------------------------------------------------
#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <qobject.h>

#include <qstringlist.h>
//--------------------------------------------------------------------------------
class QTranslator;
class LanguageManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentLanguage READ currentLanguage WRITE setCurrentLanguage)

signals:
    void languageChanged(const QString &language);

public:
    LanguageManager(QObject *parent = nullptr);

    void addLocaleDirectory(const QString &directory);
    QStringList localeDirectories() const;

    QString currentLanguage() const;
    bool setCurrentLanguage(const QString &language);

    QStringList languages() const;
    bool isLanguageAvailable(const QString &language) const;

public slots:
    void loadLanguageFromSettings();
    void chooseNewLanguage();

private:
    QString convertStringToLanguageFile(const QString &string) const;
    void loadAvailableLanguages() const;

    QString m_currentLanguage;
    QTranslator *m_sysTranslator;
    QTranslator *m_appTranslator;
    mutable bool m_loaded;
    mutable QStringList m_languages;
    QStringList m_localeDirectories;
};
//--------------------------------------------------------------------------------
#endif //LANGUAGEMANAGER_H

