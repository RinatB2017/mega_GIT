#ifndef QTYATRANSLATIONGETTOR_H
#define QTYATRANSLATIONGETTOR_H

#include <QObject>
#include <QUrl>

#include "QtYaWord.h"
#include "QtYaWordTranslation.h"

class QNetworkAccessManager;

class QtYaTranslationGettor : public QObject
{
    Q_OBJECT
public:
    explicit QtYaTranslationGettor(const QString& yandexApiKey, const QtYaWord& wordForTranslation, QNetworkAccessManager* pNetworkAccessManager, QObject *pParent = 0);

signals:
    void translated(const QtYaWordTranslation& translation) const;

private slots:
    void onTranslateFinished() const;

private:
    QUrl translateRequest(const QtYaWord& wordForTranslation, const QString& yandexApiKey) const;

    QtYaWord m_wordForTranslation;
    QNetworkAccessManager* const m_pNetworkAccessManager;

};

#endif // QTYATRANSLATIONGETTOR_H
