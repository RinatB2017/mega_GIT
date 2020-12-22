/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
**                                                                              **
**     This program is free software: you can redistribute it and/or modify     **
**     it under the terms of the GNU General Public License as published by     **
**     the Free Software Foundation, either version 3 of the License, or        **
**     (at your option) any later version.                                      **
**                                                                              **
**     This program is distributed in the hope that it will be useful,          **
**     but WITHOUT ANY WARRANTY; without even the implied warranty of           **
**     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **
**     GNU General Public License for more details.                             **
**                                                                              **
**     You should have received a copy of the GNU General Public License        **
**     along with this program.  If not, see http://www.gnu.org/licenses/.      **
**                                                                              **
**********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#include "ipv4.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
IPV4::IPV4(QWidget *parent) :
    MyWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
IPV4::~IPV4()
{
    if(a) delete a;
    if(b) delete b;
    if(c) delete c;
    if(d) delete d;

    if(port) delete port;
}
//--------------------------------------------------------------------------------
void IPV4::init(void)
{
    a = new QSpinBox(this);
    b = new QSpinBox(this);
    c = new QSpinBox(this);
    d = new QSpinBox(this);

    a->installEventFilter(this);
    b->installEventFilter(this);
    c->installEventFilter(this);
    d->installEventFilter(this);

    a->setObjectName("IPV4_a");
    b->setObjectName("IPV4_b");
    c->setObjectName("IPV4_c");
    d->setObjectName("IPV4_d");

    port = new QSpinBox(this);

    port->setObjectName("IPV4_port");

    a->setRange(0, 0xFF);
    b->setRange(0, 0xFF);
    c->setRange(0, 0xFF);
    d->setRange(0, 0xFF);

    port->setRange(0, 0xFFFF);
    port->setToolTip("Порт");

    a->setProperty("NO_SAVE", true);
    b->setProperty("NO_SAVE", true);
    c->setProperty("NO_SAVE", true);
    d->setProperty("NO_SAVE", true);
    port->setProperty("NO_SAVE", true);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setMargin(0);
    hbox->setSpacing(0);
    hbox->addWidget(a);
    hbox->addWidget(new QLabel("."));
    hbox->addWidget(b);
    hbox->addWidget(new QLabel("."));
    hbox->addWidget(c);
    hbox->addWidget(new QLabel("."));
    hbox->addWidget(d);
    hbox->addWidget(new QLabel(":"));
    hbox->addWidget(port);
    setLayout(hbox);

    connect(a,  SIGNAL(valueChanged(int)),  this,   SLOT(work()));
    connect(b,  SIGNAL(valueChanged(int)),  this,   SLOT(work()));
    connect(c,  SIGNAL(valueChanged(int)),  this,   SLOT(work()));
    connect(d,  SIGNAL(valueChanged(int)),  this,   SLOT(work()));
    connect(port,   SIGNAL(valueChanged(int)),  this,   SLOT(work()));

    connect(a, &QSpinBox::editingFinished,  this,   &IPV4::editingFinished);
    connect(b, &QSpinBox::editingFinished,  this,   &IPV4::editingFinished);
    connect(c, &QSpinBox::editingFinished,  this,   &IPV4::editingFinished);
    connect(d, &QSpinBox::editingFinished,  this,   &IPV4::editingFinished);

    connect(port, &QSpinBox::editingFinished,  this,   &IPV4::editingFinished);

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}
//--------------------------------------------------------------------------------
void IPV4::setObjectName(const QString &name)
{
    a->setObjectName(QString("%1_IPV4_a").arg(name));
    b->setObjectName(QString("%1_IPV4_b").arg(name));
    c->setObjectName(QString("%1_IPV4_c").arg(name));
    d->setObjectName(QString("%1_IPV4_d").arg(name));

    QWidget::setObjectName(name);
}
//--------------------------------------------------------------------------------
void IPV4::work(void)
{
    emit s_get_url(get_url());
}
//--------------------------------------------------------------------------------
QUrl IPV4::get_url(void)
{
    QUrl url;
    url.setHost(QString("%1.%2.%3.%4")
                .arg(a->value())
                .arg(b->value())
                .arg(c->value())
                .arg(d->value()));
    url.setPort(port->value());

    return url;
}
//--------------------------------------------------------------------------------
void IPV4::set_url(QUrl url)
{
    QString host = url.host();
    if(host.isEmpty())
    {
        host = url.path();
    }
    QStringList sl = host.split(".");
    if(sl.count() == 4)
    {
        a->setValue(QString(sl[0]).toInt());
        b->setValue(QString(sl[1]).toInt());
        c->setValue(QString(sl[2]).toInt());
        d->setValue(QString(sl[3]).toInt());
    }
    else
    {
        emit error(QString("set_url: ip.count = %1").arg(sl.count()));
    }
    port->setValue(url.port());
}
//--------------------------------------------------------------------------------
bool IPV4::eventFilter(QObject*, QEvent* event)
{
    if(event->type() == QEvent::Wheel)
    {
        qDebug() << "Wheel event blocked";
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
void IPV4::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool IPV4::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void IPV4::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void IPV4::save_setting(void)
{

}
//--------------------------------------------------------------------------------
