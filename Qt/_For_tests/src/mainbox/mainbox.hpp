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
#include <QApplication>
#include <QWidget>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class QToolButton;
class QComboBox;
class QToolBar;
//--------------------------------------------------------------------------------
class MyThread : public QObject
{
    Q_OBJECT

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void finished(void);

public slots:
    void process(void)
    {
        for(int n=0; n<1000; n++)
        {
            emit info(QString("info: n=%1").arg(n));
            emit debug(QString("debug: n=%1").arg(n));
            emit error(QString("error: n=%1").arg(n));
            emit trace(QString("trace: n=%1").arg(n));
        }
        emit finished();
    }
};
//--------------------------------------------------------------------------------
//Синглтон Меерса
class SingletonClass
{
public:
    int getSomeInfo()
    {
        return 10;
    }

    static SingletonClass &instance()
    {
        static SingletonClass instance;
        return instance;
    }

private:
    SingletonClass(){}
};
//--------------------------------------------------------------------------------
class MyDebug
{
public:
    QString &operator << (QString &data)
    {
        qDebug() << data;
        return data;
    }
};
//--------------------------------------------------------------------------------
class class_C : public QObject
{
    Q_OBJECT

public:
    void xxx(void)
    {
        qDebug() << "yes";
    }

    void send(QByteArray input, QByteArray *output)
    {
        emit info("class_C");
        (*output).append("class_C ");
        Q_UNUSED(input);
    }

signals:
    void info(const QString &);

};
class class_B : public class_C
{
public:
    void send(QByteArray input, QByteArray *output)
    {
        emit info("class_B");
        (*output).append("class_B ");
        class_C::send(input, output);
        Q_UNUSED(input);
    }
};
class class_A : public class_B
{
public:
    void send(QByteArray input, QByteArray *output)
    {
        emit info("class_A");
        (*output).append("class_A ");
        class_B::send(input, output);
        Q_UNUSED(input);
    }
};
//--------------------------------------------------------------------------------
#include <iostream>
#include <memory>

class IComponent
{
public:
    virtual void operation() = 0;
    virtual ~IComponent(){}
};

class Component : public IComponent
{
public:
    virtual void operation()
    {
        std::cout<<"World!"<<std::endl;
    }
};

class DecoratorOne : public IComponent
{
    std::shared_ptr<IComponent> m_component;

public:
    DecoratorOne(IComponent* component): m_component(component)
    {

    }

    virtual void operation()
    {
        std::cout << ", ";
        m_component->operation();
    }
};

class DecoratorTwo : public IComponent
{
    std::shared_ptr<IComponent> m_component;

public:
    DecoratorTwo(IComponent* component): m_component(component)
    {

    }

    virtual void operation()
    {
        std::cout << "Hello";
        m_component->operation();
    }
};
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    ~MainBox();

public slots:
    void choice_test(void);
    bool test_0(void);
    bool test_1(void);
    bool test_2(void);
    bool test_3(void);
    bool test_4(void);
    bool test_5(void);

    void thread_is_finished(void);

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

    QToolButton *add_button(QToolBar *tool_bar,
                            QToolButton *tool_button,
                            QIcon icon,
                            const QString &text,
                            const QString &tool_tip);

    void createTestBar(void);

    void create_thread(void);

protected:
    void changeEvent(QEvent *event);
    bool eventFilter(QObject *, QEvent *e);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
