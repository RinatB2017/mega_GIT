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
#include <QSettings>
#include <QGroupBox>
#include <QDebug>

#include "audiooutput.h"

#define PUSH_MODE_LABEL "Enable push mode"
#define PULL_MODE_LABEL "Enable pull mode"
#define SUSPEND_LABEL   "Suspend playback"
#define RESUME_LABEL    "Resume playback"
#define VOLUME_LABEL    "Volume:"

const int DurationSeconds   = 1;
const int DataSampleRateHz  = 44100;
const int BufferSize        = 32768;
//---------------------------------------------------------------------------
#define APPNAME "audiooutput"
//---------------------------------------------------------------------------
AudioTest::AudioTest()
    : m_pushTimer(new QTimer(this))
    , m_deviceBox(0)
    , m_device(QAudioDeviceInfo::defaultOutputDevice())
    , m_generator(0)
    , m_audioOutput(0)
    , m_output(0)
    , m_buffer(BufferSize, 0)
{
    initializeWindow();
    initializeAudio();

    setFixedSize(sizeHint());
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

    //---
    sb_sampleRate1 = new QDoubleSpinBox;
    sb_sampleRate1->setObjectName("sb_sampleRate1");
    sb_sampleRate1->setRange(1, 20000);

    sb_sampleRate2 = new QDoubleSpinBox;
    sb_sampleRate2->setObjectName("sb_sampleRate2");
    sb_sampleRate2->setRange(1, 20000);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel("FREQ1:"));
    hbox->addWidget(sb_sampleRate1);
    hbox->addStretch(1);
    hbox->addWidget(new QLabel("FREQ2:"));
    hbox->addWidget(sb_sampleRate2);

    //sb_sampleRate1->setValue(440);
    //sb_sampleRate2->setValue(440);

    layout->addLayout(hbox);

    //---
    sb_base_freq = new QDoubleSpinBox;
    sb_base_freq->setObjectName("sb_base_freq");
    sb_base_freq->setRange(1, 20000);

    sb_beats_freq = new QDoubleSpinBox;
    sb_beats_freq->setObjectName("sb_beats_freq");
    sb_beats_freq->setRange(0.01, 20000);

    QPushButton *btn_calc_freq = new QPushButton(this);
    btn_calc_freq->setText("CALC");
    connect(sb_base_freq,   SIGNAL(editingFinished()),  this,   SLOT(calc_freq()));
    connect(sb_beats_freq,  SIGNAL(editingFinished()),  this,   SLOT(calc_freq()));
    connect(btn_calc_freq,  SIGNAL(clicked(bool)),      this,   SLOT(calc_freq()));

    QHBoxLayout *cbox = new QHBoxLayout;
    cbox->addWidget(new QLabel("Базовая частота"));
    cbox->addWidget(sb_base_freq);
    cbox->addWidget(new QLabel("Частота биений"));
    cbox->addWidget(sb_beats_freq);
    cbox->addWidget(btn_calc_freq);
    cbox->addStretch(1);

    layout->addLayout(cbox);
    //---

    QGroupBox *gbox = new QGroupBox;
    gbox->setTitle("Volume");

    QGridLayout *grid = new QGridLayout;
    grid->addWidget(new QLabel("L:"), 0, 0);
    grid->addWidget(new QLabel("R:"), 1, 0);

    m_left_volume   = new QSlider(Qt::Horizontal);
    m_right_volume  = new QSlider(Qt::Horizontal);

    m_left_volume->setRange(0, 100);
    m_right_volume->setRange(0, 100);

    grid->addWidget(m_left_volume,  0, 1);
    grid->addWidget(m_right_volume, 1, 1);

    connect(m_left_volume,  SIGNAL(valueChanged(int)),  this,   SLOT(test()));
    connect(m_right_volume, SIGNAL(valueChanged(int)),  this,   SLOT(test()));

    connect(sb_sampleRate1, SIGNAL(editingFinished()),  this,   SLOT(test()));
    connect(sb_sampleRate2, SIGNAL(editingFinished()),  this,   SLOT(test()));

    m_left_volume->setValue(100);
    m_right_volume->setValue(100);

    gbox->setLayout(grid);

    gbox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    layout->addWidget(gbox);

    window->setLayout(layout.data());
    layout.take(); // ownership transferred

    setCentralWidget(window.data());
    QWidget *const windowPtr = window.take(); // ownership transferred
    windowPtr->show();
}
//---------------------------------------------------------------------------
void AudioTest::calc_freq(void)
{
    double delta = sb_beats_freq->value() / 2.0f;

    sb_sampleRate1->setValue(sb_base_freq->value() - delta);
    sb_sampleRate2->setValue(sb_base_freq->value() + delta);

    //qDebug() << delta << sb_sampleRate1->value() << sb_sampleRate2->value();
    test();
}
//---------------------------------------------------------------------------
void AudioTest::load_QDoubleSpinBox(QString group_name)
{
    QList<QDoubleSpinBox *> allobj = findChildren<QDoubleSpinBox *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach (QDoubleSpinBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->setValue(settings->value("value", 0).toFloat());
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//---------------------------------------------------------------------------
void AudioTest::save_QDoubleSpinBox(QString group_name)
{
    QList<QDoubleSpinBox *> allobj = findChildren<QDoubleSpinBox *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach(QDoubleSpinBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("value", QVariant(obj->value()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//---------------------------------------------------------------------------
void AudioTest::test(void)
{
    if(m_generator)
    {
        m_generator->generateData(m_format,
                                  DurationSeconds*1000000,
                                  sb_sampleRate1->value(),
                                  sb_sampleRate2->value(),
                                  m_left_volume->value(),
                                  m_right_volume->value());
    }
}
//---------------------------------------------------------------------------
void AudioTest::initializeAudio(void)
{
    connect(m_pushTimer, SIGNAL(timeout()), SLOT(pushTimerExpired()));

    m_format.setSampleRate(DataSampleRateHz);
    m_format.setChannelCount(2);
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

    load_QDoubleSpinBox("audio");
    m_generator = new Generator(m_format,
                                DurationSeconds*1000000,
                                sb_sampleRate1->value(),
                                sb_sampleRate2->value(),
                                m_left_volume->value(),
                                m_right_volume->value(),
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
}
//---------------------------------------------------------------------------
AudioTest::~AudioTest()
{
    m_generator->stop();
    m_audioOutput->stop();

    save_QDoubleSpinBox("audio");
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
