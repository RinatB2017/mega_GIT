#ifndef QTYAERROR_H
#define QTYAERROR_H

#include <QString>

#include "qtyandexapi_global.h"

class QTYANDEXAPISHARED_EXPORT QtYaError
{
public:
    bool isError() const;
    const QString& errorString() const;

protected:
    QtYaError();
    QtYaError(const QString& errorString);


    void setError(const QString& errorString);

private:
    QString m_errorString;
};

#endif // QTYAERROR_H
