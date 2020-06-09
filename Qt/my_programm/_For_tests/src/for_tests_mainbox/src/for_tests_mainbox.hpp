/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "ui_for_tests_mainbox.h"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
typedef struct PACKET {
    uint16_t a;
    uint8_t  b;
    uint32_t c;
} *packet_t;
//--------------------------------------------------------------------------------
class IMyClass {
    virtual void open(void) = 0;
    virtual void close(void) = 0;
};

class IMyClass2 {
    virtual void open2(void) = 0;
    virtual void close2(void) = 0;
};

class MyClass : public IMyClass, IMyClass2
{
public:
    void open(void)  { open2();  };
    void close(void) { close2(); };

private:
    void open2(void)  { qDebug() << "open2";  };
    void close2(void) { qDebug() << "close2"; };
};

class MyClass2 : MyClass
{
public:
    void open(void)  { MyClass::open(); };
};

class MyLabel : public QWidget
{
    Q_OBJECT

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

public:
    MyLabel()
    {
        picture_label = new QLabel(this);
        QVBoxLayout *vbox = new QVBoxLayout();

#if 0
        scr = new QScrollArea(this);
        scr->setWidgetResizable(true);
        scr->setWidget(picture_label);

        vbox->addWidget(scr);
#else
        vbox->addWidget(picture_label);
#endif
        setLayout(vbox);

        //        installEventFilter(this);
    }

    void setPixmap(const QPixmap &pix)
    {
        orig_pixmap = pix;
        picture_label->setPixmap(orig_pixmap);
    }

private:
    QPixmap orig_pixmap;
    QPointer<QLabel> picture_label;
    QPointer<QScrollArea> scr;
    bool flag = false;

protected:
    void resizeEvent (QResizeEvent * event)
    {
        QWidget::resizeEvent(event);

        if(scr)
        {
            qDebug() << "size" << scr->size();
            qDebug() << "rect" << scr->rect();
        }
    }

    //    bool eventFilter(QObject* obj, QEvent* event)
    //    {
    //        if(event->type() == QEvent::Wheel)
    //        {
    //            emit info("Wheel event blocked");
    //            return false;
    //        }
    //        return QWidget::eventFilter(obj, event);;
    //    }

    void mousePressEvent(QMouseEvent* event)
    {
        flag = true;
        //emit info("press");
        QWidget::mousePressEvent(event);
    }

    void mouseMoveEvent(QMouseEvent* event)
    {
        //emit info("move");
        QWidget::mouseMoveEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent* event)
    {
        flag = false;
        //emit info("release");
        QWidget::mouseReleaseEvent(event);
    }

    void keyPressEvent(QKeyEvent *event)
    {
        if(event->key() ==  Qt::Key_Control)
            flag = true;
        //        emit info(QString("key press %1").arg(event->key()));
        QWidget::keyPressEvent(event);
    }

    void keyReleaseEvent(QKeyEvent *event)
    {
        if(event->key() ==  Qt::Key_Control)
            flag = false;
        //        emit info("key release");
        QWidget::keyReleaseEvent(event);
    }


    void wheelEvent(QWheelEvent *event)
    {
        //        QPoint numPixels = event->pixelDelta();
        QPoint numDegrees = event->angleDelta() / 8;
        if(flag)
        {
            orig_pixmap = orig_pixmap.scaled(orig_pixmap.width()  + numDegrees.ry(),
                                             orig_pixmap.height() + numDegrees.ry(),
                                             Qt::KeepAspectRatio);
            picture_label->setPixmap(orig_pixmap);

            emit info(QString("numDegrees %1").arg(numDegrees.ry()));
        }
    }
};
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class SimpleWidget;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    ~MainBox();

    typedef void (MainBox::*saveSlot)(void);
    void inFunc(QPushButton *btn, saveSlot slot);

    int test_packet(PACKET packet);

public slots:
    void choice_test(void);
    bool test(void);
    bool print_property(void);
    bool timer_start(void);
    bool timer_stop(void);
    void show_timer_count(void);

    void print_mp(QWidget *widget);
    bool load_property(QWidget *widget, const QString &property_name);
    bool save_property(QWidget *widget, const QString &property_name, QVariant value);
    void delete_string(void);

    void s_inFunc(void);

    void check_in(void);
    void victory(void);

private:
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (MainBox::*func)(void);
    } *cmd_t;
    QList<CMD> commands;

    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;

    QPointer<QComboBox> cb_test;
    QPointer<QTimer> timer;
    int cnt = 0;

    QScopedPointer<SimpleWidget> sw;

    void readJson(const QString &filename);

    void init(void);
    void createTestBar(void);

    void test_validator(void);
    int get_cnt(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
