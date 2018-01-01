//--------------------------------------------------------------------------------
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStringList>
#include <QGroupBox>
#include <QSettings>
#include <QSlider>
#include <qmath.h>
#include <QEvent>
#include <QFrame>
#include <QDebug>
#include <QTimer>
#include <QIcon>
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
    btn_2bytes  = new QRadioButton(this);
    btn_4bytes  = new QRadioButton(this);
    btn_2bytes->setChecked(true);

    QVBoxLayout *group_vbox = new QVBoxLayout;
    group_vbox->addWidget(btn_2bytes);
    group_vbox->addWidget(btn_4bytes);
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

    return frame;
}
//--------------------------------------------------------------------------------
QWidget *Generator_Curve::add_grapher(void)
{
    QWidget *w = new QWidget(this);
    QHBoxLayout *sl = new QHBoxLayout;
    for(int n=0; n<MAX_SLIDER; n++)
    {
        sliders[n] = new QSlider(Qt::Vertical);
        sliders[n]->setMinimum(MIN_VALUE);
        sliders[n]->setMaximum(MAX_VALUE);
        sl->addWidget(sliders[n]);

        connect(sliders[n], SIGNAL(sliderReleased()), this, SLOT(update_values()));
    }
    w->setLayout(sl);

    QScrollArea *area = new QScrollArea(this);
    area->setWidgetResizable(true);
    area->setWidget(w);

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
    int n;
    double y;

    for(n=0; n<MAX_SLIDER; n++)
    {
        y = double(MAX_VALUE / 2)*qSin(double(n)*double(M_PI)/double(16));
        sliders[n]->setValue(y+(MAX_VALUE / 2)+0.5f);
    }
    update_values();
}
//--------------------------------------------------------------------------------
void Generator_Curve::gen_triangle(void)
{
    int n;
    double y, delta;

    delta = MAX_VALUE / (MAX_SLIDER / 2);
    y = 0;
    for(n=0; n<MAX_SLIDER; n++)
    {
        if(n<(MAX_SLIDER / 2))
            y += delta;
        else
            y -= delta;
        sliders[n]->setValue(y);
    }
    update_values();
}
//--------------------------------------------------------------------------------
void Generator_Curve::gen_saw(void)
{
    int n;
    double y, delta;

    delta = MAX_VALUE / MAX_SLIDER;
    y = 0;
    for(n=0; n<MAX_SLIDER; n++)
    {
        y += delta;
        sliders[n]->setValue(y);
    }
    update_values();
}
//--------------------------------------------------------------------------------
void Generator_Curve::gen_meandr(void)
{
    int n;
    double y;

    y = 0;
    for(n=0; n<MAX_SLIDER; n++)
    {
        if(n<(MAX_SLIDER / 2))
            y = 0;
        else
            y = MAX_VALUE;
        sliders[n]->setValue(y);
    }
    update_values();
}
//--------------------------------------------------------------------------------
void Generator_Curve::init_timer(void)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    // timer->start(10);
}
//--------------------------------------------------------------------------------
void Generator_Curve::update(void)
{
    if(btn_2bytes->isChecked())
    {
        send_2_bytes();
    }
    else
    {
        send_4_bytes();
    }
}
//--------------------------------------------------------------------------------
void Generator_Curve::send_4_bytes(void)
{
    union INT_TO_BYTES data;
    QByteArray ba;

    double y = values[index];
    if(index < (MAX_SLIDER - 1)) index++;
    else index = 0;

    data.value = y;

    ba.clear();
    ba.append((char)data.bytes.d);
    ba.append((char)data.bytes.c);
    ba.append((char)data.bytes.b);
    ba.append((char)data.bytes.a);
    emit send(ba);
}
//--------------------------------------------------------------------------------
void Generator_Curve::send_2_bytes(void)
{
    union SHORT_TO_BYTES data;
    QByteArray ba;

    double y = values[index];
    if(index < (MAX_SLIDER - 1))
    {
        index++;
    }
    else
    {
        index = 0;
    }

    data.value = y;

    ba.clear();
    ba.append((char)data.bytes.b);
    ba.append((char)data.bytes.a);
    emit send(ba);
}
//--------------------------------------------------------------------------------
void Generator_Curve::close_gen(void)
{
    close();
}
//--------------------------------------------------------------------------------
void Generator_Curve::update_values(void)
{
    for(int n=0; n<MAX_SLIDER; n++)
    {
        values[n] = sliders[n]->value();
        sliders[n]->setToolTip(QString("%1").arg(sliders[n]->value()));
    }
    save_setting();
}
//--------------------------------------------------------------------------------
void Generator_Curve::load_setting(void)
{
    QStringList sl;
    QString temp;
    int n,y;

    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);

    temp = "0,32,64,96,128,160,192,224,256,288,320,352,384,416,448,480,512,544,576,608,640,672,704,736,768,800,832,864,896,928,960,992";
    temp = settings->value("SlidersValue").toString();
    sl = temp.split(',');

    for(n=0; n<MAX_SLIDER; n++)
    {
        if(n < sl.count())
        {
            y = sl.at(n).toInt(); //1024 / MAX_SLIDER) * n;
            sliders[n]->setValue(y);
            values[n] = y;
        }
    }

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void Generator_Curve::save_setting(void)
{
    QString temp;
    int n;

    temp.clear();
    for(n=0; n<(MAX_SLIDER - 1); n++)
    {
        temp.append(QString("%1,").arg(values[n]));
    }
    temp.append(QString("%1").arg(values[n]));

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

    btn_2bytes->setText(tr("short"));
    btn_4bytes->setText(tr("uint"));
}
//--------------------------------------------------------------------------------
