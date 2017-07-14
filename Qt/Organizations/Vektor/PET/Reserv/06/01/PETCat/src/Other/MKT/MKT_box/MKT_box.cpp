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
#include <QPushButton>
#include <QToolButton>
#include <QSpinBox>
#include <QComboBox>
#include <QToolBar>
#include <QAction>
#include <QTimer>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_MKT_box.h"
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "MKT_box.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "grapherbox.hpp"
//--------------------------------------------------------------------------------
MKT_box::MKT_box(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MKT_box)
{
    init();
    load_widgets("MKT");
}
//--------------------------------------------------------------------------------
MKT_box::~MKT_box()
{
    save_widgets("MKT");
    delete ui;
}
//--------------------------------------------------------------------------------
void MKT_box::init(void)
{
    ui->setupUi(this);

    init_timer();
    //---
    ui->sb_slave_addr->setRange(MIN_MODBUS_ADDRESS, MAX_MODBUS_ADDRESS);
    //---
    ui->sb_width_window->setRange(MIN_WIDTH_WINDOW, MAX_WIDTH_WINDOW);
    //---
    ui->sb_cnt_sensors->setRange(MIN_SENSORS, MAX_SENSORS);
    //---
    ui->sb_interval->setRange(MIN_INTERVAL, MAX_INTERVAL);
    //---
    ui->dsb_min_temp->setRange(MIN_TEMPERATURE, MAX_TEMPERATURE);
    ui->dsb_max_temp->setRange(MIN_TEMPERATURE, MAX_TEMPERATURE);
    //---
    connect(ui->btn_start,  SIGNAL(clicked(bool)),  this,   SLOT(btn_start_click()));
    connect(ui->btn_stop,   SIGNAL(clicked(bool)),  this,   SLOT(btn_stop_click()));
    connect(ui->btn_reset,  SIGNAL(clicked(bool)),  this,   SLOT(btn_reset_click()));
    connect(ui->btn_refresh,    SIGNAL(clicked(bool)),  this,   SLOT(btn_refresh_click()));
    connect(ui->btn_set_min_max,    SIGNAL(clicked(bool)),  this,   SLOT(btn_set_min_max_click()));
    //---
    ui->btn_stop->setDisabled(true);
    //---
    ui->btn_refresh->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));
    f_refresh_ports();
    //---

#ifdef TEST
    createTestBar();
#endif
    create_grapher();
}
//--------------------------------------------------------------------------------
void MKT_box::btn_start_click(void)
{
    f_start();
}
//--------------------------------------------------------------------------------
void MKT_box::btn_stop_click(void)
{
    f_stop();
}
//--------------------------------------------------------------------------------
void MKT_box::btn_reset_click(void)
{
    f_reset();
}
//--------------------------------------------------------------------------------
void MKT_box::btn_refresh_click(void)
{
    f_refresh_ports();
}
//--------------------------------------------------------------------------------
void MKT_box::btn_set_min_max_click(void)
{
    f_set_min_max();
}
//--------------------------------------------------------------------------------
void MKT_box::init_timer(void)
{
    timer = new QTimer();
    if(timer)
    {
        connect(timer,  SIGNAL(timeout()),  this,   SLOT(update()));
    }
}
//--------------------------------------------------------------------------------
void MKT_box::set_max_sensors(int max_sensors)
{
    if(max_sensors < 0) return;
    if(max_sensors > MAX_SENSORS)   return;

    grapher->remove_all_curve();
    for(int n=0; n<max_sensors; n++)
    {
        grapher->add_curve(QString("Датчик %1").arg(n));
    }
}
//--------------------------------------------------------------------------------
void MKT_box::update(void)
{
    bool ok = read();
    if(ok == false)
    {

    }
}
//--------------------------------------------------------------------------------
void MKT_box::f_start(void)
{
    QString s_port = ui->cb_port->currentText();
    if(s_port.isEmpty())
    {
        QString title = "Ошибка";
        QString text  = "Порт не выбран!";
        messagebox_info(title, text);
        return;
    }
    bool ok = false;

    //---
    ok = open();
    if(ok == false)
    {
        return;
    }
    ok = set_num_sensors();
    if(ok == false)
    {
        return;
    }
    ok = set_width_window();
    if(ok == false)
    {
        return;
    }
    ok = get_status();
    if(ok == false)
    {
        return;
    }
    ok = get_ver_firmvare();
    if(ok == false)
    {
        return;
    }
    //---
    timer->start(ui->sb_interval->value() * 1000);
    //---
    ui->dsb_min_temp->setEnabled(false);
    ui->dsb_max_temp->setEnabled(false);
    ui->btn_set_min_max->setEnabled(false);
    ui->cb_port->setEnabled(false);
    ui->btn_refresh->setEnabled(false);
    ui->btn_start->setEnabled(false);
    ui->btn_stop->setEnabled(true);
    ui->btn_reset->setEnabled(false);
    ui->sb_slave_addr->setEnabled(false);
    ui->sb_width_window->setEnabled(false);
    ui->sb_cnt_sensors->setEnabled(false);
    ui->sb_interval->setEnabled(false);
    //---

    emit info("Старт: OK");
}
//--------------------------------------------------------------------------------
void MKT_box::f_stop(void)
{
    timer->stop();
    //---
    ui->dsb_min_temp->setEnabled(true);
    ui->dsb_max_temp->setEnabled(true);
    ui->btn_set_min_max->setEnabled(true);
    ui->cb_port->setEnabled(true);
    ui->btn_refresh->setEnabled(true);
    ui->btn_start->setEnabled(true);
    ui->btn_stop->setEnabled(false);
    ui->btn_reset->setEnabled(true);
    ui->sb_slave_addr->setEnabled(true);
    ui->sb_width_window->setEnabled(true);
    ui->sb_cnt_sensors->setEnabled(true);
    ui->sb_interval->setEnabled(true);
    //---
    close();
    emit info("Стоп: OK");
}
//--------------------------------------------------------------------------------
bool MKT_box::f_reset(void)
{
    bool ok = false;
    int res = 0;
    uint16_t value = 0xEE77;

    ok = open();
    if(ok == false)
    {
        return false;
    }

    res = modbus_write_registers(ctx, ADDRESS_RESET, 1, &value);
    if(res < 0)
    {
        emit error("Не могу записать регистры");
        return false;
    }
    close();

    emit info("Сброс: OK");
    return true;
}
//--------------------------------------------------------------------------------
void MKT_box::f_refresh_ports(void)
{
    ui->cb_port->clear();
    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
#ifdef Q_OS_LINUX
        ui->cb_port->addItem(port.systemLocation());
#endif
#ifdef Q_OS_WIN
        ui->cb_port->addItem(port.portName());
#endif
    }
}
//--------------------------------------------------------------------------------
void MKT_box::f_set_min_max(void)
{
    double min = ui->dsb_min_temp->value();
    double max = ui->dsb_max_temp->value();

    if(max <= min)
    {
        messagebox_critical("Ошибка", "Не корректный интервал температур");
        return;
    }
    grapher->set_axis_scale_y(min, max);
}
//--------------------------------------------------------------------------------
void MKT_box::create_grapher(void)
{
    grapher = new GrapherBox(this);
    grapher->set_title_axis_Y("Температура");
    grapher->set_axis_scale_x(0, 1000);
    grapher->set_axis_scale_y(MIN_TEMPERATURE, MAX_TEMPERATURE);

    grapher->set_visible_btn_Vertical(false);
    grapher->set_visible_btn_Horizontal(false);

    grapher->push_btn_Horizontal(true);

    grapher->set_visible_btn_Options(false);
    grapher->set_visible_btn_Load(false);
    grapher->set_visible_btn_Save(false);
    grapher->set_visible_btn_Statistic(false);

    ui->grapher_layout->addWidget(grapher);

    grapher_data.clear();
}
//--------------------------------------------------------------------------------
QToolButton *MKT_box::add_button(QToolBar *tool_bar,
                                 QToolButton *tool_button,
                                 QIcon icon,
                                 const QString &text,
                                 const QString &tool_tip)
{
    if(!tool_bar) return NULL;
    if(!tool_button) return NULL;

    tool_button->setIcon(icon);
    tool_button->setText(text);
    tool_button->setToolTip(tool_tip);
    tool_button->setObjectName(text);
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void MKT_box::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(mw == nullptr)
    {
        return;
    }

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &MKT_box::test_0 });
    commands.append({ ID_TEST_1, "test 1", &MKT_box::test_1 });
    commands.append({ ID_TEST_2, "test 2", &MKT_box::test_2 });
    commands.append({ ID_TEST_3, "test 3", &MKT_box::test_3 });
    commands.append({ ID_TEST_4, "test 4", &MKT_box::test_4 });
    commands.append({ ID_TEST_5, "test 5", &MKT_box::test_5 });

    QToolBar *toolBar = new QToolBar(tr("testbar"));
    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(Qt::UserRole + command.cmd));
    }

    toolBar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(toolBar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));
}
//--------------------------------------------------------------------------------
void MKT_box::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef void (MKT_box::*function)(void);
            function x;
            x = command.func;
            if(x)
            {
                (this->*x)();
            }
            else
            {
                emit error("no func");
            }

            return;
        }
    }
}
//--------------------------------------------------------------------------------
void MKT_box::test_0(void)
{
    emit info("Test_0()");
}
//--------------------------------------------------------------------------------
void MKT_box::test_1(void)
{
    emit info("Test_1()");
}
//--------------------------------------------------------------------------------
void MKT_box::test_2(void)
{
    emit info("Test_2()");
}
//--------------------------------------------------------------------------------
void MKT_box::test_3(void)
{
    emit info("Test_3()");
}
//--------------------------------------------------------------------------------
void MKT_box::test_4(void)
{
    emit info("Test_4()");
}
//--------------------------------------------------------------------------------
void MKT_box::test_5(void)
{
    emit info("Test_5()");
}

//--------------------------------------------------------------------------------
void MKT_box::save_log(void)
{
    QString title = "Ошибка";
    QString text  = TODODO;
    messagebox_critical(title, text);
}
//--------------------------------------------------------------------------------
void MKT_box::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        //grapher->retranslateUi();
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MKT_box::grapher_add_data(int index_sensor, float value)
{
    grapher->add_curve_data(index_sensor, value);
}
//--------------------------------------------------------------------------------
bool MKT_box::open(void)
{
    int res = 0;

    emit info(QString("Открываем порт %1").arg(ui->cb_port->currentText()));

    if(is_open == true)
    {
        return true;
    }

    if(ui->cb_port->currentText().isEmpty())
    {
        emit error("Устройство отсутствует или не выбрано");
        return false;
    }

    ctx = modbus_new_rtu(ui->cb_port->currentText().toLocal8Bit(),
                         115200,
                         'N', 8, 1);
    if(ctx == NULL)
    {
        emit error("Не могу открыть устройство");
        return false;
    }

#ifdef Q_OS_LINUX
    modbus_set_response_timeout(ctx, DEFAULT_MODBUS_TIMEOUT_SEC, 0);
#endif
#ifdef Q_OS_WIN
    struct timeval tv;
    tv.tv_sec = DEFAULT_MODBUS_TIMEOUT_SEC;
    tv.tv_usec = 0;
    modbus_set_response_timeout(ctx, &tv);
#endif

    res = modbus_set_slave(ctx, ui->sb_slave_addr->value()); // Устанавливаем slave адрес.
    if(res)
    {
        emit error(QString("Не могу открыть установить адрес устройства %1").arg(ui->sb_slave_addr->value()));
        return false;
    }

    res = modbus_connect(ctx);
    if(res)
    {
        emit error("Не могу соедениться с устройством");
        return false;
    }

    is_open = true;
    return true;
}
//--------------------------------------------------------------------------------
bool MKT_box::read(void)
{
    emit debug("Чтение");
    int res = 0;

    if(ctx == NULL)
    {
        emit error("error open modbus");
        is_open = false;
        return false;
    }

    bool ok = get_num_sensors();
    if(!ok)
    {
        return false;
    }

    res = modbus_read_input_registers(ctx, 0, ui->sb_cnt_sensors->value(), tab_reg);
    if(res < 0)
    {
        emit error("Не могу прочитать input регистры");
        return false;
    }

    //---
    for(int n=0; n<ui->sb_cnt_sensors->value(); n++)
    {
        grapher_add_data(n, tab_reg[n] / 10.0f);
    }
    //---

    return true;
}
//--------------------------------------------------------------------------------
bool MKT_box::write(void)
{
    emit info("Запись");
    int res = 0;
    uint16_t value = ui->sb_width_window->value();

    res = modbus_write_registers(ctx, cmd_address, 1, &value);
    if(res < 0)
    {
        emit error("Не могу записать регистр");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
void MKT_box::close(void)
{
    modbus_close(ctx);
    ctx = 0;
    is_open = false;
}
//--------------------------------------------------------------------------------
bool MKT_box::get_num_sensors(void)
{
    int res = 0;
    uint16_t value = 0;

    res = modbus_read_registers(ctx, ADDRESS_NUM_SENSORS, 1, &value);
    if(res < 0)
    {
        emit error("Не могу прочитать регистры");
        return false;
    }
    if(value < MIN_SENSORS)
    {
        emit error(ERROR_VALUE_SMALL);
        return false;
    }
    if(value > MAX_SENSORS)
    {
        emit error(ERROR_VALUE_LARGE);
        return false;
    }
    if(value != ui->sb_cnt_sensors->value())
    {
        set_max_sensors(value);
    }
    return true;
}
//--------------------------------------------------------------------------------
bool MKT_box::set_max_sensors(unsigned int max_sensors)
{
    if(max_sensors > MAX_SENSORS)
    {
        emit error(QString("выбрано слишком много датчиков %1. макс %2")
                   .arg(max_sensors)
                   .arg(MAX_SENSORS));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool MKT_box::set_num_sensors(void)
{
    int res = 0;
    uint16_t value = ui->sb_cnt_sensors->value();

    emit info(QString("Устанавливаем кол-во сенсоров %1").arg(value));

    res = modbus_write_registers(ctx, ADDRESS_NUM_SENSORS, 1, &value);
    if(res < 0)
    {
        emit error("Не могу записать регистры");
        return false;
    }
    if(value > MAX_SENSORS)
    {
        emit error(ERROR_VALUE_LARGE);
        return false;
    }
    set_max_sensors(value);
    emit info("Установка количества сенсоров: OK");
    return true;
}
//--------------------------------------------------------------------------------
void MKT_box::get_width_window(void)
{
    int res = 0;
    uint16_t value = 0;

    res = modbus_read_registers(ctx, ADDRESS_WIDTH_WINDOW, 1, &value);
    if(res < 0)
    {
        emit error("Не могу прочитать регистр");
        return;
    }
    if(value < MIN_WIDTH_WINDOW)
    {
        emit error(ERROR_VALUE_SMALL);
        return;
    }
    if(value > MAX_WIDTH_WINDOW)
    {
        emit error(ERROR_VALUE_LARGE);
        return;
    }
    emit info("Получение ширины окна: OK");
}
//--------------------------------------------------------------------------------
bool MKT_box::set_width_window(void)
{
    int res = 0;
    uint16_t value = ui->sb_width_window->value();

    emit info(QString("Устанавливаем ширину окна %1").arg(value));

    res = modbus_write_registers(ctx, ADDRESS_WIDTH_WINDOW, 1, &value);
    if(res < 0)
    {
        emit error("Не могу записать регистр");
        return false;
    }
    emit info("Установка ширины окна: OK");
    return true;
}
//--------------------------------------------------------------------------------
bool MKT_box::get_status(void)
{
    int res = 0;
    uint16_t value = 0;

    emit info("Читаем статус");

    res = modbus_read_registers(ctx, ADDRESS_STATUS, 1, &value);
    if(res < 0)
    {
        emit error("Не могу прочитать регистр");
        return false;
    }
    bool err_power = (value & 1);
    bool err_read  = (value & 2);

    emit info(QString("status 0x%1").arg(value, 0, 16));
    if(err_power)
    {
        emit error("Ошибка питания датчиков");
        return false;
    }
    if(err_read)
    {
        emit error("Ошибка чтения датчиков");
        return false;
    }
    emit info("Получение статуса: OK");
    return true;
}
//--------------------------------------------------------------------------------
bool MKT_box::get_ver_firmvare(void)
{
    int res = 0;
    uint16_t value = 0;

    emit info("Читаем версию прошивки");

    res = modbus_read_registers(ctx, ADDRESS_VER_FIRMVARE, 1, &value);
    if(res < 0)
    {
        emit error("Не могу прочитать регистр");
        return false;
    }

    int hi = value / 100;
    int lo = value - (hi * 100);
    emit debug(QString("value %1").arg(value));
    emit info(QString("firmvare %1.%2")
              .arg(hi)
              .arg(lo));
    return true;
}
//--------------------------------------------------------------------------------
