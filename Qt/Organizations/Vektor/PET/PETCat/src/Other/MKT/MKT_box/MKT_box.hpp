/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#ifndef MKT_BOX_HPP
#define MKT_BOX_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#ifdef Q_OS_WIN
    #include <modbus-rtu.h>
#else
    #include <modbus/modbus-rtu.h>
#endif
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "MKT_defines.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
struct DATA {
    QString curve_name;
    int curve_ID;
    QList<float> curve_data;
};
//--------------------------------------------------------------------------------
namespace Ui {
    class MKT_box;
}
//--------------------------------------------------------------------------------
class QToolButton;
class QToolBar;
class QComboBox;
class GrapherBox;
class QTreeWidgetItem;
//--------------------------------------------------------------------------------
class MKT_box : public MyWidget
{
    Q_OBJECT

public:
    explicit MKT_box(QWidget *parent);
    ~MKT_box();

signals:
    void block_widget(bool);

private slots:
    void choice_test(void);
    void test_0(void);
    void test_1(void);
    void test_2(void);
    void test_3(void);
    void test_4(void);
    void test_5(void);

    void save_log(void);

    void f_start(void);
    void f_stop(void);
    bool f_reset(void);
    void f_refresh_ports(void);

    void f_set_min_max(void);

    void grapher_add_data(int index_sensor, float value);
    void set_max_sensors(int max_sensors);

    void update(void);

    //---
    void btn_start_click(void);
    void btn_stop_click(void);
    void btn_reset_click(void);
    void btn_refresh_click(void);
    void btn_set_min_max_click(void);
    //---

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
    struct CMD
    {
        int cmd;
        QString cmd_text;
        void (MKT_box::*func)(void);
    };
    Ui::MKT_box *ui = 0;

    QComboBox *cb_test = 0;
    QList<CMD> commands;

    GrapherBox *grapher = 0;

    QList<DATA> grapher_data;

    QTimer *timer = 0;

    void init(void);
    void init_timer(void);

    QToolButton *add_button(QToolBar *tool_bar,
                            QToolButton *tool_button,
                            QIcon icon,
                            const QString &text,
                            const QString &tool_tip);

    void createTestBar(void);
    void create_grapher(void);

    modbus_t *ctx = 0;
    uint16_t tab_reg[MAX_SENSORS];
    int cmd_address = 0;
    bool is_open = false;

    bool open(void);
    bool read(void);
    bool write(void);
    void close(void);

    bool get_num_sensors(void);
    bool set_max_sensors(unsigned int max_sensors);
    bool set_num_sensors(void);
    void get_width_window(void);
    bool set_width_window(void);
    bool get_status(void);
    bool get_ver_firmvare(void);

protected:
    void changeEvent(QEvent *event);
};
//--------------------------------------------------------------------------------
#endif // MKT_box_HPP
