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
#include <QVBoxLayout>
#include <QDebug>

#include "audiooutput.h"

#define PUSH_MODE_LABEL "Enable push mode"
#define PULL_MODE_LABEL "Enable pull mode"
#define SUSPEND_LABEL   "Suspend playback"
#define RESUME_LABEL    "Resume playback"
#define VOLUME_LABEL    "Volume:"

const int DurationSeconds   = 1;
const int ToneSampleRateHz  = 141;  //440;
const int DataSampleRateHz  = 44100;
const int BufferSize        = 32768;
//---------------------------------------------------------------------------
AudioTest::AudioTest()
    :   m_pushTimer(new QTimer(this))
    ,   m_modeButton(0)
    ,   m_suspendResumeButton(0)
    ,   m_deviceBox(0)
    ,   m_device(QAudioDeviceInfo::defaultOutputDevice())
    ,   m_generator(0)
    ,   m_audioOutput(0)
    ,   m_output(0)
    ,   m_buffer(BufferSize, 0)
{
    initializeWindow();
    initializeAudio();
}
//---------------------------------------------------------------------------
void AudioTest::initializeWindow(void)
{
    QScopedPointer<QWidget> window(new QWidget);
    QScopedPointer<QVBoxLayout> layout(new QVBoxLayout);

    m_deviceBox = new QComboBox(this);
    const QAudioDeviceInfo &defaultDeviceInfo = QAudioDeviceInfo::defaultOutputDevice();
    m_deviceBox->addItem(defaultDeviceInfo.deviceName(), qVariantFromValue(defaultDeviceInfo));
    foreach (const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
    {
        if (deviceInfo != defaultDeviceInfo)
        {
            m_deviceBox->addItem(deviceInfo.deviceName(), qVariantFromValue(deviceInfo));
        }
    }
    connect(m_deviceBox,SIGNAL(activated(int)),SLOT(deviceChanged(int)));
    layout->addWidget(m_deviceBox);

    m_modeButton = new QPushButton(this);
    m_modeButton->setText(tr(PUSH_MODE_LABEL));
    connect(m_modeButton, SIGNAL(clicked()), SLOT(toggleMode()));
    layout->addWidget(m_modeButton);

    m_suspendResumeButton = new QPushButton(this);
    m_suspendResumeButton->setText(tr(SUSPEND_LABEL));
    connect(m_suspendResumeButton, SIGNAL(clicked()), SLOT(toggleSuspendResume()));
    layout->addWidget(m_suspendResumeButton);

    QHBoxLayout *volumeBox = new QHBoxLayout;
    m_volumeLabel = new QLabel;
    m_volumeLabel->setText(tr(VOLUME_LABEL));
    m_volumeSlider = new QSlider(Qt::Horizontal);
    m_volumeSlider->setMinimum(0);
    m_volumeSlider->setMaximum(100);
    m_volumeSlider->setSingleStep(10);
    connect(m_volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(volumeChanged(int)));
    volumeBox->addWidget(m_volumeLabel);
    volumeBox->addWidget(m_volumeSlider);
    layout->addLayout(volumeBox);

    window->setLayout(layout.data());
    layout.take(); // ownership transferred

    setCentralWidget(window.data());
    QWidget *const windowPtr = window.take(); // ownership transferred
    windowPtr->show();
}
//---------------------------------------------------------------------------
void AudioTest::initializeAudio(void)
{
    connect(m_pushTimer, SIGNAL(timeout()), SLOT(pushTimerExpired()));

    m_pullMode = true;

    m_format.setSampleRate(DataSampleRateHz);
    m_format.setChannelCount(1);
    m_format.setSampleSize(16);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(m_format))
    {
        qWarning() << "Default format not supported - trying to use nearest";
        m_format = info.nearestFormat(m_format);
    }

    m_generator = new Generator(m_format,
                                DurationSeconds*1000000,
                                ToneSampleRateHz,
                                this);

    createAudioOutput();
}
//---------------------------------------------------------------------------
void AudioTest::createAudioOutput(void)
{
    delete m_audioOutput;
    m_audioOutput = 0;
    m_audioOutput = new QAudioOutput(m_device, m_format, this);
    m_generator->start();
    m_audioOutput->start(m_generator);
    m_volumeSlider->setValue(int(m_audioOutput->volume()*100.0f));
}
//---------------------------------------------------------------------------
AudioTest::~AudioTest()
{

}
//---------------------------------------------------------------------------
void AudioTest::deviceChanged(int index)
{
    m_pushTimer->stop();
    m_generator->stop();
    m_audioOutput->stop();
    m_audioOutput->disconnect(this);
    m_device = m_deviceBox->itemData(index).value<QAudioDeviceInfo>();
    createAudioOutput();
}
//---------------------------------------------------------------------------
void AudioTest::volumeChanged(int value)
{
    if (m_audioOutput)
    {
        m_audioOutput->setVolume(qreal(value/100.0f));
    }
}
//---------------------------------------------------------------------------
void AudioTest::pushTimerExpired(void)
{
    if (m_audioOutput && m_audioOutput->state() != QAudio::StoppedState)
    {
        int chunks = m_audioOutput->bytesFree()/m_audioOutput->periodSize();
        while (chunks)
        {
            const qint64 len = m_generator->read(m_buffer.data(), m_audioOutput->periodSize());
            if (len)
            {
                m_output->write(m_buffer.data(), len);
            }
            if (len != m_audioOutput->periodSize())
            {
                break;
            }
            --chunks;
        }
    }
}
//---------------------------------------------------------------------------
void AudioTest::toggleMode(void)
{
    m_pushTimer->stop();
    m_audioOutput->stop();

    if (m_pullMode)
    {
        //switch to push mode (periodically push to QAudioOutput using a timer)
        m_modeButton->setText(tr(PULL_MODE_LABEL));
        m_output = m_audioOutput->start();
        m_pullMode = false;
        m_pushTimer->start(0);  //20
    }
    else
    {
        //switch to pull mode (QAudioOutput pulls from Generator as needed)
        m_modeButton->setText(tr(PUSH_MODE_LABEL));
        m_pullMode = true;
        m_audioOutput->start(m_generator);
    }

    m_suspendResumeButton->setText(tr(SUSPEND_LABEL));
}
//---------------------------------------------------------------------------
void AudioTest::toggleSuspendResume(void)
{
    if (m_audioOutput->state() == QAudio::SuspendedState)
    {
        m_audioOutput->resume();
        m_suspendResumeButton->setText(tr(SUSPEND_LABEL));
    }
    else if (m_audioOutput->state() == QAudio::ActiveState)
    {
        m_audioOutput->suspend();
        m_suspendResumeButton->setText(tr(RESUME_LABEL));
    }
    else if (m_audioOutput->state() == QAudio::StoppedState)
    {
        m_audioOutput->resume();
        m_suspendResumeButton->setText(tr(SUSPEND_LABEL));
    }
    else if (m_audioOutput->state() == QAudio::IdleState)
    {
        // no-op
    }
}
//---------------------------------------------------------------------------
