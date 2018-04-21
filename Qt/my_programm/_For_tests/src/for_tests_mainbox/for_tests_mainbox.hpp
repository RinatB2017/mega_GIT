/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#endif
//--------------------------------------------------------------------------------
#include "for_tests_mainbox.hpp"
#include "ui_for_tests_mainbox.h"

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class LockButton : public QWidget
{
    Q_OBJECT

public:
    LockButton(QWidget *parent = 0) : QWidget(parent)
    {
        box = new QCheckBox(this);
        box->setToolTip("lock");

        btn = new QPushButton(this);
        btn->setText("Test");
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        connect(box,    SIGNAL(toggled(bool)),  btn,    SLOT(setDisabled(bool)));

        QHBoxLayout *hbox = new QHBoxLayout;
        hbox->setSpacing(0);
        hbox->setMargin(0);
        hbox->addWidget(box);
        hbox->addWidget(btn);
        hbox->setStretchFactor(box, 0);
        hbox->setStretchFactor(btn, 1);

        setLayout(hbox);
    }

    void setText(QString text)
    {
        btn->setText(text);
    }

private:
    QCheckBox *box   = 0;
    QPushButton *btn = 0;
};
//--------------------------------------------------------------------------------
class C
{
public:
  C()
  {
      qDebug() << "C";
  }
  void test()
  {
      qDebug() << "test";
  }
};

class B : public C
{
public:
    B()
    {
        qDebug() << "B";
    }
};

class A : public B
{
public:
    A()
    {
        qDebug() << "A";
    }
    void test()
    {
        qDebug() << "test_A";
    }
};
//--------------------------------------------------------------------------------
class IPV4 : public QWidget
{
    Q_OBJECT

public:
    IPV4(QWidget *parent) : QWidget(parent)
    {
        a = new QSpinBox(this);
        b = new QSpinBox(this);
        c = new QSpinBox(this);
        d = new QSpinBox(this);

        a->setObjectName("ipv4_a");
        b->setObjectName("ipv4_b");
        c->setObjectName("ipv4_c");
        d->setObjectName("ipv4_d");

        port = new QSpinBox(this);

        port->setObjectName("ipv4_port");

        a->setRange(0, 0xFF);
        b->setRange(0, 0xFF);
        c->setRange(0, 0xFF);
        d->setRange(0, 0xFF);

        port->setRange(0, 0xFFFF);
        port->setToolTip("Порт");

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
    }

    QUrl get_url(void)
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
    void set_url(QUrl url)
    {
        QString host = url.host();
        QStringList sl = host.split(".");
        if(sl.count() == 4)
        {
            a->setValue(QString(sl[0]).toInt());
            b->setValue(QString(sl[1]).toInt());
            c->setValue(QString(sl[2]).toInt());
            d->setValue(QString(sl[3]).toInt());
        }
        port->setValue(url.port());
    }

private:
    QSpinBox *a = 0;
    QSpinBox *b = 0;
    QSpinBox *c = 0;
    QSpinBox *d = 0;

    QSpinBox *port = 0;
};
//--------------------------------------------------------------------------------
class MySplashScreen;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

    Q_PROPERTY(int xxx READ getValue WRITE setValue)

public:
    explicit MainBox(QWidget *parent,
            MySplashScreen *splash);
    ~MainBox();

    typedef void (MainBox::*saveSlot)(void);
    void inFunc(QPushButton *btn, saveSlot slot);

signals:
    void dpi_set(int);

public slots:
    void choice_test(void);
    bool test_0(void);
    bool test_1(void);
    bool test_2(void);
    bool test_3(void);
    bool test_4(void);
    bool test_5(void);

    void s_inFunc(void);
    void new_test(void);

    void check_in(void);
    void victory(void);

private:
    enum {
        ID_TEST_0 = 1000,
        ID_TEST_1,
        ID_TEST_2,
        ID_TEST_3,
        ID_TEST_4,
        ID_TEST_5,
        ID_TEST_6
    };

    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (MainBox::*func)(void);
    } CMD_t;

    MySplashScreen *splash = 0;
    Ui::MainBox *ui = 0;

    QComboBox *cb_test = 0;
    QList<CMD> commands;

    void init(void);
    void createTestBar(void);
    void updateText(void);

    int getValue(void)
    {
        return ui->spinBox->value();
    }
    void setValue(int value)
    {
        ui->spinBox->setValue(value);
    }

    quint32 test(const QByteArray ba);
    int get_cnt(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
