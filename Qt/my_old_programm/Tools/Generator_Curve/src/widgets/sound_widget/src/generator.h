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
#ifndef GENERATOR_H
#define GENERATOR_H
//---------------------------------------------------------------------------
#include <QAudioFormat>
#include <QIODevice>
//---------------------------------------------------------------------------
//int sr = format.sampleRate();
//int cc = format.channelCount();
//int ss = format.sampleSize();

//sr = 44100
//cc = 2
//ss = 16
//length = 1764000
//sampleIndex = 44100
//cnt = 10

//qint64 length = (format.sampleRate() * format.channelCount() * (format.sampleSize() / 8)) * durationUs / 100000;
// 1764000
//---------------------------------------------------------------------------
class Generator : public QIODevice
{
    Q_OBJECT

public:
    Generator(QByteArray data,
              QObject *parent);
    ~Generator();

    qint64 readData(char *data,
                    qint64 maxlen);
    qint64 writeData(const char *data,
                     qint64 len);
    qint64 bytesAvailable() const;
    void clear_buffer(void);

    void generateData(QByteArray data);

public slots:
    void start(void);
    void stop(void);

private:
    qint64 m_pos;
    int length = 1764000;
    int cnt = 10;
    QByteArray m_buffer;

    void init_m_buffer(QByteArray data);
};
//---------------------------------------------------------------------------
#endif
