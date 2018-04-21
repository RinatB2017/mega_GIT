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
#include <QNetworkAccessManager>
#include <QNetworkRequest>
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

    //TODO
    void readFortune(void);
    void s_error(QAbstractSocket::SocketError err);

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

    //TODO
    QTcpSocket *tcpSocket = 0;
    QNetworkRequest request;
    QNetworkAccessManager networkManager;
    //---

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
