/*********************************************************************************
**                                                                              **
**     Copyright (C) 2021                                                       **
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
#ifndef ADB_WIDGET_HPP
#define ADB_WIDGET_HPP
//--------------------------------------------------------------------------------
#include <QElapsedTimer>
#include <QProcess>
#include <QPointer>
//--------------------------------------------------------------------------------
#include <QTcpServer>
//--------------------------------------------------------------------------------
#include "tcp_local_server.hpp"

#include "mywidget.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class ADB_widget;
}
//--------------------------------------------------------------------------------
class ADB_widget : public MyWidget
{
    Q_OBJECT

public:
    explicit ADB_widget(QWidget *parent = nullptr);
    virtual ~ADB_widget();

signals:
    void r_programm(QString program, QStringList arguments);
    void set_pixmap(QPixmap pixmap);

public slots:
    bool f_tap(int pos_x, int pos_y);

private slots:
    void started(void);
    void finished(int result);
    void process_error(QProcess::ProcessError p_error);

    void readData(void);

private:
    Ui::ADB_widget *ui;

    QPointer<QProcess> myProcess;

    int s_width = 0;
    int s_height = 0;

    bool f_busy = false;
    bool binary_data = false;
    int process_result = 0;
    QStringList sl_data;

    QStringList sl_commands;

    QPointer <QTimer> timer_autoshot;

    QPointer<TCP_Server> server;

    void init(void);
    void create_process(void);

    void run_program(const QString &program,
                     const QStringList &arguments);
    bool test_run(const QStringList &arguments);

    bool f_devices(void);
    void f_create_screenshot(void);
    bool f_get_screeshot(void);
    bool f_get_file_screeshot(void);
    void f_screen_tap(void);

    bool f_swipe(int x1, int y1, int x2, int y2, int delay);
    bool f_test_swipe_LR(void);
    bool f_test_swipe_RL(void);
    bool f_test_swipe_UD(void);
    bool f_test_swipe_DU(void);

    bool f_adb(void);
    bool f_test(void);

    void f_start(void);
    void f_stop(void);

    void f_1(void);
    void f_2(void);
    void f_3(void);
    void f_4(void);

    void f_get_data(const QByteArray &data);

    void run_cmd(void);

    void f_auto_shot(bool state);

    void f_show_screeshot(QPixmap pixmap);

    bool copy_file(const QString &src_filename,
                   const QString &dst_filename);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // ADB_WIDGET_HPP
