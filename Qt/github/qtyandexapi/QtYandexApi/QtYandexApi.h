#ifndef QTYANDEXAPI_H
#define QTYANDEXAPI_H

#include <QObject>

#include "qtyandexapi_global.h"

#include "QtYandexDictionary.h"

class QTYANDEXAPISHARED_EXPORT QtYandexApi : public QObject
{
    Q_OBJECT
public:
    explicit QtYandexApi(QObject *parent = 0);

    static QString getYandexKeyFromFile(const QString& filePath);

};

#endif // QTYANDEXAPI_H
