/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <QDebug>
//---------------------------------------------------------------------------
#include <qendian.h>
#include <qmath.h>
//---------------------------------------------------------------------------
#include "generator.h"
//---------------------------------------------------------------------------
Generator::Generator(QByteArray data,
                     QObject *parent)
    : QIODevice(parent)
    , m_pos(0)
{
    m_buffer.clear();
    while(m_buffer.size() < 32768)
    {
        m_buffer.append(data);
    }
}
//---------------------------------------------------------------------------
Generator::~Generator()
{

}
//---------------------------------------------------------------------------
void Generator::start(void)
{
    open(QIODevice::ReadOnly);
}
//---------------------------------------------------------------------------
void Generator::stop(void)
{
    m_pos = 0;
    close();
}
//---------------------------------------------------------------------------
void Generator::generateData(QByteArray data)
{
    m_buffer.clear();
    while(m_buffer.size() < 32768)
    {
        m_buffer.append(data);
    }
//    m_buffer.append(data);
}
//---------------------------------------------------------------------------
qint64 Generator::readData(char *data,
                           qint64 len)
{
    qint64 total = 0;
    if (!m_buffer.isEmpty())
    {
        while (len - total > 0)
        {
            const qint64 chunk = qMin((m_buffer.size() - m_pos), len - total);
            memcpy(data + total, m_buffer.constData() + m_pos, static_cast<size_t>(chunk));
            m_pos = (m_pos + chunk) % m_buffer.size();
            total += chunk;
        }
    }
    return total;
}
//---------------------------------------------------------------------------
qint64 Generator::writeData(const char *data,
                            qint64 len)
{
    m_buffer.append(data, static_cast<int>(len));
    return len;
}
//---------------------------------------------------------------------------
void Generator::clear_buffer(void)
{
    m_buffer.clear();
}
//---------------------------------------------------------------------------
qint64 Generator::bytesAvailable(void) const
{
    return m_buffer.size() + QIODevice::bytesAvailable();
}
//---------------------------------------------------------------------------
