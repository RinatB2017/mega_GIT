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

#include "devicelist.h"
#include "devicelist_p.h"

namespace iot
{

DeviceList::DeviceList(QObject *parent) : QObject(parent), d_ptr(new DeviceListPrivate(this))
{
}

DeviceList::DeviceList(DeviceListPrivate& dd, QObject* parent) : QObject(parent), d_ptr(&dd)
{
}

DeviceList::~DeviceList()
{
}

QList<Device*> DeviceList::devices()
{
    Q_D(DeviceList);
    return d->m_devices;
}

QList<Device*> DeviceList::takeDevices()
{
    Q_D(DeviceList);
    QList<Device*> devices = d->m_devices;
    d->m_devices.clear();
    return devices;
}

Error* DeviceList::getError()
{
    Q_D(DeviceList);
    return &d->m_getError;
}

void DeviceList::get()
{
    Q_D(DeviceList);
    d->get();
}

} // namespace iot
