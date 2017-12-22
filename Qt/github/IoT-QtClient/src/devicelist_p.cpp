/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Wapice Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "devicelist_p.h"
#include "device.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace iot
{

DeviceListPrivate::DeviceListPrivate(DeviceList* parent): ItemPrivate(), q_ptr(parent)
{

}

DeviceListPrivate::~DeviceListPrivate()
{

}

void DeviceListPrivate::get()
{
    m_getResponse.reset( RequestHandlerProvider::instance()->getRequest("/devices") );
    QObject::connect( m_getResponse.data(), &Response::finished, this, &DeviceListPrivate::onGetFinished);
}

void DeviceListPrivate::onGetFinished()
{
    Q_Q(DeviceList);
    bool error = true;
    m_devices.clear();
    try {
        if (isValidResponse(*m_getResponse, m_getError, 200)) {
            error = false;
            m_getError.reset();

            const QJsonObject rootObject = getObject(m_getResponse->document());
            int offset = getValue(rootObject, "offset", QJsonValue::Double).toInt();
            Q_UNUSED(offset);
            int limit = getValue(rootObject, "limit", QJsonValue::Double).toInt();
            Q_UNUSED(limit);
            int fullSize = getValue(rootObject, "fullSize", QJsonValue::Double).toInt();
            const QJsonArray items = getValue(rootObject, "items", QJsonValue::Array, fullSize != 0).toArray();
            foreach (auto item, items) {
                Device* device = Device::fromJson(getObject(item));
                if (device) m_devices << device;
                else {
                    // TODO what error type??
                    error = true;
                    break;
                }
            }

        }
    } catch (std::exception& exception) {
        m_getError.setErrorType(Error::ErrorType::GenericError);
        m_getError.setDescription(exception.what());
        error = true;
    }

    emit q->getFinished(!error);
}

} // namespace iot
