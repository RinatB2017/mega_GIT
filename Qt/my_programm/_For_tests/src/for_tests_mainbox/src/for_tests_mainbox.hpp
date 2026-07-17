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
#include <QTextToSpeech>
#include <QElapsedTimer>

#include <QMetaType>
#include <QMetaObject>

#include <QThread>
//--------------------------------------------------------------------------------
#include "for_tests_mainbox_gui.hpp"
#include "mysplashscreen.hpp"
#include "mymainwindow.hpp"
#include "myfiledialog.hpp"
#include "test_classes.hpp"
#include "mywidget.hpp"
// #include "ogl_widget.hpp"
// #include "cubewidget.h"
#include "defines.hpp"

#include "worker.h"

#include "simpletcpserver.h"

#include "custom_test_style.hpp"
#include "custom_mfc_style.hpp"
#include "custom_aqua_style.hpp"
//--------------------------------------------------------------------------------
class CyberWidget : public QWidget {
    Q_OBJECT // Обязательно для работы мета-системы
public:
    // Q_INVOKABLE — это разрешение для метода newInstance() вызывать этот конструктор
    Q_INVOKABLE explicit CyberWidget(QWidget *parent = nullptr) : QWidget(parent) {

        // Просто добавим внутрь виджета какой-нибудь текст для наглядности
        QVBoxLayout *layout = new QVBoxLayout(this);
        QLabel *label = new QLabel("Привет! Я — CyberWidget.", this);
        layout->addWidget(label);

        // Сделаем ему рамку, чтобы его было видно на экране
        setStyleSheet("border: 2px solid blue; background-color: white;");
    }
};
//--------------------------------------------------------------------------------
class TestClass : public QWidget
{
    Q_OBJECT

public:
    explicit TestClass(QWidget *parent) : QWidget(parent)
    {

    }

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void colorLog(const QString &, const QColor &, const QColor &);
    void clear_log();
};
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
//--------------------------------------------------------------------------------
class MainBox : public MainBox_GUI
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent, MySplashScreen *splash);
    virtual ~MainBox();

public slots:
    void choice_test(void);

    bool test(void);
    bool test2(void);
    bool test_style(void);
    bool run_cube_widget(void);
    bool run_ogl_widget(void);

    void startThread(void);
    void stopThread(void);
    void handleProgress(int value);

private:
    typedef struct
    {
        int cmd;
        QString cmd_text;
        bool (MainBox::*func)(void);
    } CMD;
    QList<CMD> commands;

    QPointer<MySplashScreen> splash;

    QPointer<QComboBox> cb_test;
    QPointer<QSpinBox>  sb_test;
    QPointer<QCheckBox> cb_block;

    QTextToSpeech sp;

    QThread *m_thread; // Указатель на объект потока
    Worker  *m_worker; // <-- ДОБАВЬТЕ ЭТУ СТРОЧКУ!

    // SimpleSshServer *m_sshServer;
    SimpleRemoteConsole *m_remoteConsole;

    bool set_theme_windows(void);
    bool set_norton_commander(void);
    bool set_styles(void);

    void createWidgetByName(void);

    void init(void);
    void create_test_bar(void);

    void calc_line(qreal center_x,
                   qreal center_y,
                   qreal angle,
                   qreal radius,
                   qreal *end_x,
                   qreal *end_y);

    bool calc_norm(qreal x,
                   qreal y,
                   qreal w,
                   qreal h,
                   qreal *norm_x,
                   qreal *norm_y);

    QImage create_bone(int num);

    template<typename T1, typename T2>
    void test_template(void);

    void heavy_function(bool *result);

    template<class T>
    void test_function(int delay);
    void test_function2(bool (MainBox::*func)(void));

    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
