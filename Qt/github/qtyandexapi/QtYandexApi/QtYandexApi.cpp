#include "Precompiled.h"
#include "QtYandexApi.h"

QtYandexApi::QtYandexApi(QObject *parent)
    : QObject(parent)
{
}

QString QtYandexApi::getYandexKeyFromFile(const QString &filePath)
{
    QString result;
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly))
        result = file.readAll();
    result = result.trimmed();
    return result;
}
