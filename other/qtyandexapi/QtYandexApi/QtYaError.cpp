#include "Precompiled.h"
#include "QtYaError.h"

QtYaError::QtYaError()
{
}

QtYaError::QtYaError(const QString &errorString)
    : m_errorString(errorString)
{

}

bool QtYaError::isError() const
{
    return !m_errorString.isNull();
}

const QString &QtYaError::errorString() const
{
    return m_errorString;
}

void QtYaError::setError(const QString &errorString)
{
    m_errorString = errorString;
}
