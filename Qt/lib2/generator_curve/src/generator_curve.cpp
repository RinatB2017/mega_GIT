//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QRadioButton>
#   include <QPushButton>
#   include <QVBoxLayout>
#   include <QHBoxLayout>
#   include <QGroupBox>
#   include <QSettings>
#   include <QSlider>
#   include <QtMath>
#   include <QFrame>
#endif
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "generator_curve.hpp"
#include "mainwindow.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
Generator_Curve::Generator_Curve(QWidget *parent) :
    MyWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
Generator_Curve::~Generator_Curve(void)
{
    save_setting();
}
//--------------------------------------------------------------------------------
void Generator_Curve::init(void)
{
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setMargin(0);
    hbox->setSpacing(0);

    hbox->addWidget(add_frame());
    hbox->addWidget(add_grapher());
    setLayout(hbox);

    load_setting();
    updateText();
}
//--------------------------------------------------------------------------------
QWidget *Generator_Curve::add_frame(void)
{
    QFrame *frame = new QFrame(this);

    btnPower = new QPushButton(this);
    connect(btnPower,   SIGNAL(clicked(bool)),  this,   SLOT(start()));

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

    connect(btnSinus,       SIGNAL(clicked()),  this,   SLOT(gen_sinus()));
    connect(btnTriangle,    SIGNAL(clicked()),  this,   SLOT(gen_triangle()));
    connect(btnSaw,         SIGNAL(clicked()),  this,   SLOT(gen_saw()));
    connect(btnMeandr,      SIGNAL(clicked()),  this,   SLOT(gen_meandr()));

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(btnPower);
    vbox->addWidget(btnSinus);
    vbox->addWidget(btnTriangle);
    vbox->addWidget(btnSaw);
    vbox->addWidget(btnMeandr);
    vbox->addWidget(group);
    vbox->addStretch(1);

    frame->setLayout(vbox);

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
        connect(slider, SIGNAL(valueChanged(int)),  this, SLOT(set_slider_tooltip(int)));
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
void Generator_Curve::set_slider_tooltip(int value)
{
    QSlider *slider = (QSlider *)sender();
    if(slider)
    {
        slider->setToolTip(QString("%1").arg(value));
    }
}
//--------------------------------------------------------------------------------
void Generator_Curve::start(void)
{
    emit info("Start");
    emit trace(Q_FUNC_INFO);

    QString temp;

    temp = ":";
    foreach (QSlider *slider, sliders)
    {
        QString data = QString("%1").arg(slider->value(), 4, 16, QChar('0')).toUpper();
        temp.append(data);
    }
    temp.append("\n");

    if(temp.length() != (1 + (MAX_SLIDER * 4) + 1))
    {
        emit error(QString("ERROR: len=%1").arg(temp.length()));
        return;
    }

    emit debug(temp);
    emit debug(QString("send len=%1").arg(temp.length()));

    emit send(temp);
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
    Q_CHECK_PTR(settings);

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
