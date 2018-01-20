//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QRadioButton>
#   include <QPushButton>
#   include <QVBoxLayout>
#   include <QHBoxLayout>
#   include <QStringList>
#   include <QGroupBox>
#   include <QSettings>
#   include <QSlider>
#   include <qmath.h>
#   include <QEvent>
#   include <QFrame>
#   include <QTimer>
#   include <QIcon>
#endif
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "generator_curve.hpp"
#include "mainwindow.hpp"
#include "defines.hpp"
#include "qwt_knob.h"
//--------------------------------------------------------------------------------
union INT_TO_BYTES
{
    unsigned int value;
    struct
    {
        unsigned char d;
        unsigned char c;
        unsigned char b;
        unsigned char a;
    } bytes;
};
union SHORT_TO_BYTES
{
    unsigned short value;
    struct
    {
        unsigned char b;
        unsigned char a;
    } bytes;
};
//--------------------------------------------------------------------------------
Generator_Curve::Generator_Curve(QWidget *parent) :
    MyWidget(parent)
{
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setMargin(0);
    hbox->setSpacing(0);

    hbox->addWidget(add_frame());
    hbox->addWidget(add_grapher());
    setLayout(hbox);

    load_setting();
    init_timer();
    updateText();
}
//--------------------------------------------------------------------------------
Generator_Curve::~Generator_Curve(void)
{
    save_setting();
}
//--------------------------------------------------------------------------------
QWidget *Generator_Curve::add_frame(void)
{
    QFrame *frame = new QFrame(this);

    btnPower = new QPushButton(this);
    btnPower->setCheckable(true);
    connect(btnPower, SIGNAL(toggled(bool)), this, SLOT(start(bool)));

    btnSinus    = new QPushButton(this);
    btnTriangle = new QPushButton(this);
    btnSaw      = new QPushButton(this);
    btnMeandr   = new QPushButton(this);

    group = new QGroupBox(this);
    btn_1bytes  = new QRadioButton(this);
    btn_2bytes  = new QRadioButton(this);
    btn_2bytes->setChecked(true);

    connect(btn_1bytes, SIGNAL(toggled(bool)),  this,   SLOT(update_sliders()));
    connect(btn_2bytes, SIGNAL(toggled(bool)),  this,   SLOT(update_sliders()));

    QVBoxLayout *group_vbox = new QVBoxLayout;
    group_vbox->addWidget(btn_1bytes);
    group_vbox->addWidget(btn_2bytes);
    group->setLayout(group_vbox);

    knob_Interval = new QwtKnob(this);
    knob_Interval->setUpperBound(1000);
    knob_Interval->setScaleMaxMajor(10);
    knob_Interval->setScaleMaxMinor(10);
    knob_Interval->setValue(1000);

    connect(btnSinus, SIGNAL(clicked()), this, SLOT(gen_sinus()));
    connect(btnTriangle, SIGNAL(clicked()), this, SLOT(gen_triangle()));
    connect(btnSaw, SIGNAL(clicked()), this, SLOT(gen_saw()));
    connect(btnMeandr, SIGNAL(clicked()), this, SLOT(gen_meandr()));

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(btnPower);
    vbox->addWidget(btnSinus);
    vbox->addWidget(btnTriangle);
    vbox->addWidget(btnSaw);
    vbox->addWidget(btnMeandr);
    vbox->addWidget(group);
    vbox->addWidget(knob_Interval);

    frame->setLayout(vbox);
    frame->setFixedSize(frame->sizeHint());

    frame->setFrameStyle(QFrame::Box | QFrame::Raised);

    Q_CHECK_PTR(frame);

    return frame;
}
//--------------------------------------------------------------------------------
void Generator_Curve::update_sliders(void)
{
    foreach (QSlider *slider, sliders)
    {
        if(btn_1bytes->isChecked())
        {
            slider->setRange(0, 0xFF);
        }
        if(btn_2bytes->isChecked())
        {
            slider->setRange(0, 0xFFFF);
        }
    }
}
//--------------------------------------------------------------------------------
QWidget *Generator_Curve::add_grapher(void)
{
    QWidget *w = new QWidget(this);
    QHBoxLayout *sl = new QHBoxLayout;
    for(int n=0; n<MAX_SLIDER; n++)
    {
        QSlider *slider = new QSlider(Qt::Vertical);
        if(btn_1bytes->isChecked()) slider->setRange(0, 0xFF);
        if(btn_2bytes->isChecked()) slider->setRange(0, 0xFFFF);
        sl->addWidget(slider);

        sliders.append(slider);
    }
    sl->addStretch(1);
    w->setLayout(sl);

    QScrollArea *area = new QScrollArea(this);
    area->setWidgetResizable(true);
    area->setWidget(w);

    Q_CHECK_PTR(area);

    return area;
}
//--------------------------------------------------------------------------------
void Generator_Curve::start(bool state)
{
    QPushButton *button = (QPushButton *)sender();
    if(state)
    {
        timer->start(knob_Interval->value());
        button->setText(tr("stop"));
    }
    else
    {
        timer->stop();
        button->setText(tr("start"));
    }
}
//--------------------------------------------------------------------------------
void Generator_Curve::gen_sinus(void)
{
    int n = 0;
    double y;

    foreach (QSlider *slider, sliders)
    {
        y = double(slider->maximum() / 2)*qSin(double(n)*double(M_PI)/double(16));
        slider->setValue(y+(slider->maximum() / 2)+0.5f);
        n++;
    }
}
//--------------------------------------------------------------------------------
void Generator_Curve::gen_triangle(void)
{
    int n;
    double y, delta;

    y = 0;
    n = 0;
    foreach (QSlider *slider, sliders)
    {
        delta = slider->maximum() / (MAX_SLIDER / 2);
        if(n<(MAX_SLIDER / 2))
            y += delta;
        else
            y -= delta;
        n++;
        slider->setValue(y);
    }
}
//--------------------------------------------------------------------------------
void Generator_Curve::gen_saw(void)
{
    double y, delta;

    y = 0;
    foreach (QSlider *slider, sliders)
    {
        delta = slider->maximum() / MAX_SLIDER;
        y += delta;
        slider->setValue(y);
    }
}
//--------------------------------------------------------------------------------
void Generator_Curve::gen_meandr(void)
{
    int n = 0;
    double y;

    y = 0;
    foreach (QSlider *slider, sliders)
    {
        if(n<(MAX_SLIDER / 2))
            y = 0;
        else
            y = slider->maximum();
        slider->setValue(y);
        n++;
    }
}
//--------------------------------------------------------------------------------
void Generator_Curve::init_timer(void)
{
    timer = new QTimer(this);
    connect(timer,  SIGNAL(timeout()),  this,   SLOT(update()));
}
//--------------------------------------------------------------------------------
void Generator_Curve::update(void)
{
    if(btn_1bytes->isChecked())
    {
        send_1_bytes();
        return;
    }
    if(btn_2bytes->isChecked())
    {
        send_2_bytes();
        return;
    }
}
//--------------------------------------------------------------------------------
void Generator_Curve::send_1_bytes(void)
{
    QByteArray ba;
    ba.clear();
    foreach (QSlider *slider, sliders)
    {
        ba.append((char)slider->value());
    }
    QString temp = QString(":%1\n").arg(ba.toHex().toUpper().data());
    emit send(temp);
}
//--------------------------------------------------------------------------------
void Generator_Curve::send_2_bytes(void)
{
    union SHORT_TO_BYTES data;

    QByteArray ba;
    ba.clear();
    foreach (QSlider *slider, sliders)
    {
        data.value = slider->value();
        ba.append((char)data.bytes.b);
        ba.append((char)data.bytes.a);
    }
    QString temp = QString(":%1\n").arg(ba.toHex().toUpper().data());
    emit send(temp);
}
//--------------------------------------------------------------------------------
void Generator_Curve::close_gen(void)
{
    close();
}
//--------------------------------------------------------------------------------
void Generator_Curve::load_setting(void)
{
    QStringList sl;
    QString temp;
    int n = 0;
    int y = 0;

    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);

    temp = settings->value("SlidersValue").toString();
    sl = temp.split(',');

    n = 0;
    foreach (QSlider *slider, sliders)
    {
        y = sl.at(n).toInt(); //1024 / MAX_SLIDER) * n;
        slider->setValue(y);
        n++;
    }

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void Generator_Curve::save_setting(void)
{
    QString temp;

    temp.clear();
    foreach (QSlider *slider, sliders)
    {
        temp.append(QString("%1,").arg(slider->value()));
    }

    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    settings->setValue("SlidersValue", QVariant(temp));
    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void Generator_Curve::updateText()
{
    btnPower->setText(btnPower->isChecked() ? tr("stop") : tr("start"));
    btnSinus->setText(tr("sinus"));
    btnTriangle->setText(tr("triangle"));
    btnSaw->setText(tr("saw"));
    btnMeandr->setText(tr("meandr"));

    btn_1bytes->setText(tr("1 byte"));
    btn_2bytes->setText(tr("2 byte"));
}
//--------------------------------------------------------------------------------
