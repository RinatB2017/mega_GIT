/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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
#include <QCoreApplication>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QLCDNumber>
#include <QCheckBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QFrame>
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include <QTime>
#include <limits.h>
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#include "grapherbox.hpp"
#include "controlbox.hpp"
#include "defines.hpp"
#include "sleeper.h"
#include "crc.h"

#include "ui_controlbox.h"
//--------------------------------------------------------------------------------
#define BUTTON_ON   "background:gray"
#define BUTTON_OFF  ""
//--------------------------------------------------------------------------------
ControlBox::ControlBox(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ControlBox),
    display(0),
    timer(0)
{
    init();
}
//--------------------------------------------------------------------------------
ControlBox::~ControlBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void ControlBox::connect_log(void)
{
    connect(this, SIGNAL(info(QString)),  topLevelWidget(), SIGNAL(info(QString)));
    connect(this, SIGNAL(debug(QString)), topLevelWidget(), SIGNAL(debug(QString)));
    connect(this, SIGNAL(error(QString)), topLevelWidget(), SIGNAL(error(QString)));
}
//--------------------------------------------------------------------------------
void ControlBox::init(void)
{
    ui->setupUi(this);

    connect_log();

    grapher = new GrapherBox(this);
    grapher->set_axis_scale_x(0, 1000);
    grapher->set_axis_scale_y(-3, 3);

    grapher->add_curve(tr("value"));

    create_timer();
    create_serial_widgets();

    QHBoxLayout *d_hbox = new QHBoxLayout();
    d_hbox->addLayout(add_display_layout());
    d_hbox->addWidget(add_measure_group());
    d_hbox->addStretch(1);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(d_hbox, 1);
    vbox->addWidget(grapher, 100);

    QFrame *frame = new QFrame(this);
    frame->setLayout(vbox);
    frame->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);

    QHBoxLayout *main = new QHBoxLayout();
    main->addWidget(frame);

    setLayout(main);

    reset();
}
//--------------------------------------------------------------------------------
void ControlBox::create_timer(void)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}
//--------------------------------------------------------------------------------
void ControlBox::create_serial_widgets(void)
{
    connect(&serial, SIGNAL(readyRead()), this, SLOT(port_read()));
    //connect(&serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(port_error(QSerialPort::SerialPortError)));
}
//--------------------------------------------------------------------------------
bool ControlBox::check_stm(void)
{
    emit debug("CMD_GET_ID");

    if(!serial.isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    qint64 id = 0;
    int err = send_check_cmd(CMD_GET_ID, &id);
    if(err != ERR_PASS)
    {
        print_last_error();
        return false;
    }
    if(id != ID)
    {
        emit error(QString("ID %1 != id %2")
                   .arg(ID)
                   .arg(id));
        return false;
    }
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::find_device(void)
{
    QList<int> speeds;

    speeds.clear();
    //speeds.append(4800);
    speeds.append(9600);
    speeds.append(19200);
    speeds.append(28800);
    speeds.append(38400);
    speeds.append(57600);
    speeds.append(76800);
    speeds.append(115200);

    if(serial.isOpen()) serial.close();

    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
        emit info(QString(tr("ищем ADC на порту %1")).arg(port.portName()));
        foreach (int speed, speeds)
        {
            serial.setPort(port);
            bool ok = serial.setBaudRate(speed);
            if(ok)
            {
                if(serial.open(QIODevice::ReadWrite))
                {
                    ok = check_stm();
                    if(ok)
                    {
                        emit info(QString("stm found on %1").arg(port.portName()));
                        return true;
                    }
                    serial.close();
                }
                else
                {
                    //emit error(QString("error open port %1").arg(port.portName()));
                }
            }
            else
            {
                //emit error(QString("setBaudRate(%1) is failed!").arg(speed));
            }
        }
    }
    emit error("not found!");
    return false;
}
//--------------------------------------------------------------------------------
void ControlBox::port_read(void)
{
    data_serial.append(serial.readAll());
    //emit debug(QString("port_read [%1]").arg(data_serial.data()));
}
//--------------------------------------------------------------------------------
void ControlBox::port_error(QSerialPort::SerialPortError serial_error)
{
    switch(serial_error)
    {
    case QSerialPort::NoError:  break;
    case QSerialPort::DeviceNotFoundError:  emit error("Error: DeviceNotFoundError"); break;
    case QSerialPort::PermissionError:      emit error("Error: PermissionError"); break;
    case QSerialPort::OpenError:            emit error("Error: OpenError"); break;
    case QSerialPort::ParityError:          emit error("Error: ParityError"); break;
    case QSerialPort::FramingError:         emit error("Error: FramingError"); break;
    case QSerialPort::BreakConditionError:  emit error("Error: BreakConditionError"); break;
    case QSerialPort::WriteError:           emit error("Error: WriteError"); break;
    case QSerialPort::ReadError:            emit error("Error: ReadError"); break;
    case QSerialPort::ResourceError:        emit error("Error: ResourceError"); break;
    case QSerialPort::UnsupportedOperationError: emit error("Error: UnsupportedOperationError"); break;
    case QSerialPort::UnknownError:         emit error("Error: UnknownError"); break;
    case QSerialPort::TimeoutError:         emit error("Error: TimeoutError"); break;
    case QSerialPort::NotOpenError:         emit error("Error: NotOpenError"); break;
    default:
        emit error(QString("Unknown error %1").arg(serial_error));
        break;
    }
}
//--------------------------------------------------------------------------------
QGroupBox *ControlBox::add_measure_group(void)
{
    btnStart = new QPushButton("start");
    btnStop  = new QPushButton("stop");

    btnStart->setFixedWidth(50);
    btnStop->setFixedWidth(50);

    sbTimer = new QSpinBox();
    sbTimer->setMinimum(1);
    sbTimer->setMaximum(60);
    sbTimer->setObjectName("sbTimer");

    sb_timeout = new QSpinBox();
    sb_timeout->setMinimum(100);
    sb_timeout->setMaximum(0xFFFF);
    sb_timeout->setObjectName("sb_timeout");

    sbMaxTime = new QSpinBox();
    sbMaxTime->setMinimum(1);
    sbMaxTime->setMaximum(0xFFFF);
    sbMaxTime->setValue(60);
    sbMaxTime->setObjectName("sbMaxTime");

    cbUnlimTimer = new QCheckBox();
    connect(cbUnlimTimer, SIGNAL(toggled(bool)), this, SLOT(timer_unlimited(bool)));

    connect(btnStart, SIGNAL(clicked()), this, SLOT(measure_start()));
    connect(btnStop,  SIGNAL(clicked()), this, SLOT(measure_stop()));

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(new QLabel(tr("изм.")));
    hbox->addWidget(btnStart);
    hbox->addWidget(btnStop);
    hbox->addWidget(new QLabel(tr("сек.")));
    hbox->addWidget(sbTimer);
    hbox->addWidget(new QLabel(tr("max.")));
    hbox->addWidget(sbMaxTime);
    hbox->addWidget(new QLabel(tr("unl.")));
    hbox->addWidget(cbUnlimTimer);
    hbox->addWidget(new QLabel(tr("timeout")));
    hbox->addWidget(sb_timeout);
    hbox->addStretch(1);

    QPushButton *btn_0 = new QPushButton("0");
    QPushButton *btn_1 = new QPushButton("1");
    QPushButton *btn_2 = new QPushButton("2");
    QPushButton *btn_3 = new QPushButton("3");
    QPushButton *btn_4 = new QPushButton("4");
    QPushButton *btn_5 = new QPushButton("5");

    connect(btn_0, SIGNAL(clicked()), this, SLOT(cmd_calibration_0()));
    connect(btn_1, SIGNAL(clicked()), this, SLOT(cmd_calibration_1()));
    connect(btn_2, SIGNAL(clicked()), this, SLOT(cmd_calibration_2()));
    connect(btn_3, SIGNAL(clicked()), this, SLOT(cmd_calibration_3()));
    connect(btn_4, SIGNAL(clicked()), this, SLOT(cmd_calibration_4()));
    connect(btn_5, SIGNAL(clicked()), this, SLOT(cmd_calibration_5()));

    QPushButton *btn_find_device = new QPushButton();
    btn_find_device->setText(tr("Поиск"));
    btn_find_device->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));

    connect(btn_find_device, SIGNAL(clicked()), this, SLOT(find_device()));

    QHBoxLayout *button_box = new QHBoxLayout();
    button_box->addWidget(btn_0);
    button_box->addWidget(btn_1);
    button_box->addWidget(btn_2);
    button_box->addWidget(btn_3);
    button_box->addWidget(btn_4);
    button_box->addWidget(btn_5);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(btn_find_device);
    vbox->addLayout(hbox);
    vbox->addLayout(button_box);
    vbox->addStretch(1);

    QGroupBox *group = new QGroupBox(this);
    group->setLayout(vbox);

    btnStop->setStyleSheet(BUTTON_ON);
    return group;
}
//--------------------------------------------------------------------------------
void ControlBox::timer_unlimited(bool state)
{
    sbMaxTime->setEnabled(!state);
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_calibration_0(void)
{
    emit info("CMD_CALIBRATION_0");

    if(!serial.isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    qint64 value = 0;
    int err = send_check_cmd(CMD_CALIBRATION_0, &value);
    if(err != ERR_PASS)
    {
        print_last_error();
        return false;
    }

    CMD_ANSWER *answer = (CMD_ANSWER *)data_serial.data();
    emit info(QString("state %1").arg(answer->data.state));

    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_calibration_1(void)
{
    emit info("CMD_CALIBRATION_1");

    if(!serial.isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    qint64 value = 0;
    int err = send_check_cmd(CMD_CALIBRATION_1, &value);
    if(err != ERR_PASS)
    {
        print_last_error();
        return false;
    }

    CMD_ANSWER *answer = (CMD_ANSWER *)data_serial.data();
    emit info(QString("state %1").arg(answer->data.state));

    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_calibration_2(void)
{
    emit info("CMD_CALIBRATION_2");

    if(!serial.isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    qint64 value = 0;
    int err = send_check_cmd(CMD_CALIBRATION_2, &value);
    if(err != ERR_PASS)
    {
        print_last_error();
        return false;
    }

    CMD_ANSWER *answer = (CMD_ANSWER *)data_serial.data();
    emit info(QString("state %1").arg(answer->data.state));

    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_calibration_3(void)
{
    emit info("CMD_CALIBRATION_3");

    if(!serial.isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    qint64 value = 0;
    int err = send_check_cmd(CMD_CALIBRATION_3, &value);
    if(err != ERR_PASS)
    {
        print_last_error();
        return false;
    }

    CMD_ANSWER *answer = (CMD_ANSWER *)data_serial.data();
    emit info(QString("state %1").arg(answer->data.state));

    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_calibration_4(void)
{
    emit info("CMD_CALIBRATION_4");

    if(!serial.isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    qint64 value = 0;
    int err = send_check_cmd(CMD_CALIBRATION_4, &value);
    if(err != ERR_PASS)
    {
        print_last_error();
        return false;
    }

    CMD_ANSWER *answer = (CMD_ANSWER *)data_serial.data();
    emit info(QString("state %1").arg(answer->data.state));

    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_calibration_5(void)
{
    emit info("CMD_CALIBRATION_5");

    if(!serial.isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    qint64 value = 0;
    int err = send_check_cmd(CMD_CALIBRATION_5, &value);
    if(err != ERR_PASS)
    {
        print_last_error();
        return false;
    }

    CMD_ANSWER *answer = (CMD_ANSWER *)data_serial.data();
    emit info(QString("state %1").arg(answer->data.state));

    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
QHBoxLayout *ControlBox::add_display_layout(void)
{
    display = new QLCDNumber(this);
    display->setMinimumWidth(350);
    display->setMinimumHeight(48);
    display->setDigitCount(12);
    display->setSegmentStyle(QLCDNumber::Flat);
    display->setStyleSheet("background:black; color:lightgreen;");

    QLabel *label = new QLabel(this);
    label->setText("nV");
    label->setFont(QFont("Times", 22));

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addStretch();
    hbox->addWidget(display);
    hbox->addWidget(label);
    hbox->addStretch();

    return hbox;
}
//--------------------------------------------------------------------------------
void ControlBox::update(void)
{
    //emit info("update");

    if(serial.isOpen() == false)
    {
        measure_stop();
        QMessageBox::critical(this, tr("Ошибка"), tr("Порт STM32F1 не открыт"));
        return;
    }

    bool ok = false;
    qint64 value = 0;

    Sleeper::msleep(1000);
    ok = get_data(&value);
    if(!ok)
    {
        print_last_error();
        return;
    }

    send_data_to_grapherBox(value);

    if(cbUnlimTimer->isChecked()) return;

    cnt_max_time++;
    if(cnt_max_time >= sbMaxTime->value())
    {
        measure_stop();
    }
}
//--------------------------------------------------------------------------------
void ControlBox::measure_start(void)
{
    btnStart->setStyleSheet(BUTTON_ON);
    btnStop->setStyleSheet(BUTTON_OFF);
    cnt_max_time = 0;
    timer->start(sbTimer->value() * 1000);
}
//--------------------------------------------------------------------------------
void ControlBox::measure_stop(void)
{
    btnStart->setStyleSheet(BUTTON_OFF);
    btnStop->setStyleSheet(BUTTON_ON);
    timer->stop();
}
//--------------------------------------------------------------------------------
int ControlBox::send_check_cmd(unsigned char cmd,
                               qint64 *value)
{
    CMD_QUESTION question;

    question.data.cmd = cmd;
    question.data.param = 0;
    question.data.crc8 = CRC::crc8((unsigned char *)&question.buf, sizeof(CMD_QUESTION) - 1);

    QByteArray data;
    data.clear();
    data.append((const char *)question.buf, sizeof(CMD_QUESTION));

    data_serial.clear();
    serial.write(convert_data_to_ascii(data));
    wait_msec(sb_timeout->value());
    //---

#ifdef QT_DEBUG
    qDebug() << "data_serial" << data_serial.toHex() << "data_serial.length()" << data_serial.length();
#endif

    QByteArray ta;
    bool ok = false;
    ta.clear();
    ta.append(convert_ascii_to_data(data_serial.data(), &ok));

    data_serial.clear();
    data_serial.append(ta);

    int res = check_len_and_crc8(sizeof(CMD_ANSWER));
    if(res != ERR_PASS)
    {
        last_error = res;
        return last_error;
    }

    CMD_ANSWER *answer = (CMD_ANSWER *)data_serial.data();
    if(answer->data.error != ERR_PASS)
    {
        last_error = answer->data.error;
        return last_error;
    }

    *value = answer->data.value;
    last_error = ERR_PASS;
    return last_error;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_TEST(void)
{
    emit info("cmd_TEST");

    if(!serial.isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    qint64 value = 0;
    bool ok = send_check_cmd(CMD_TEST, &value);
    if(!ok)
    {
        emit info("OK");
        return true;
    }
    else
    {
        emit error("Failed");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_INIT(void)
{
    emit info("cmd_INIT");

    if(!serial.isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    qint64 value = 0;
    bool ok = send_check_cmd(CMD_INIT, &value);
    if(!ok)
    {
        emit info("OK");
        return true;
    }
    else
    {
        emit error("Failed");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_GET_DATA(void)
{
    emit info("cmd_GET_DATA");

    if(!serial.isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }

    bool ok = false;
    qint64 value = 0;

    ok = get_data(&value);
    if(ok)
    {
        draw_value(value);
    }
    else
    {
        emit error("Failed");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::get_data(qint64 *data)
{
    //emit info("get_data");

    if(!serial.isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    qint64 value = 0;
    int err = send_check_cmd(CMD_GET_DATA, &value);
    if(err != ERR_PASS)
    {
        print_last_error();
        return false;
    }

    //emit info("OK");
    CMD_ANSWER *answer = (CMD_ANSWER *)data_serial.data();

    draw_value(answer->data.value);
    *data = answer->data.value;
    return true;
}
//--------------------------------------------------------------------------------
void ControlBox::reset(void)
{
    emit info(tr("сброс"));

    min_value = ULONG_MAX;
    max_value = -ULONG_MAX;
    delta_value = 0.0f;

    value_index = 0;
    old_value = 0;
}
//--------------------------------------------------------------------------------
void ControlBox::wait(int max_time_ms)
{
    QTime time;
    time.start();
    while(time.elapsed() < max_time_ms)
    {
        QCoreApplication::processEvents();
        //if(is_ready) break;
    }
}
//--------------------------------------------------------------------------------
void ControlBox::set_enabled(bool state)
{
    btnStart->setEnabled(state);
    btnStop->setEnabled(state);

    btn_CMD_TEST->setEnabled(state);
    btn_CMD_INIT->setEnabled(state);
    btn_CMD_GET_DATA->setEnabled(state);

    QCoreApplication::processEvents();
}
//--------------------------------------------------------------------------------
void ControlBox::send_data_to_grapherBox(double value)
{
    value_index++;
    grapher->add_curve_data(0, value_index, value);
    draw_value(value);

    //qDebug() << min_value << value << max_value;

    if(value > max_value) max_value = value;
    if(value < min_value) min_value = value;
    delta_value = max_value - min_value;

    emit info(QString("Value %1 \t\t min_value %2 \t\t max_value %3 \t\t delta_value %4")
              .arg(value,       0, 'g', 9)
              .arg(min_value,   0, 'g', 9)
              .arg(max_value,   0, 'g', 9)
              .arg(delta_value, 0, 'g', 9));
    old_value = value;
}
//--------------------------------------------------------------------------------
void ControlBox::wait_msec(int timeout_msec)
{
    QTime time;

    if(timeout_msec < 1)
        return;

    time.start();
    while(true)
    {
        QCoreApplication::processEvents();
        if(time.elapsed() > timeout_msec)
            break;
    }
}
//--------------------------------------------------------------------------------
void ControlBox::draw_value(double value)
{
    display->display(QString("%1").arg(value));
}
//--------------------------------------------------------------------------------
QByteArray ControlBox::convert_ascii_to_data(QString input, bool *ok)
{
    QByteArray temp_ba;

    int begin = input.indexOf(':');
    if(begin < 0)
    {
        temp_ba.clear();
        temp_ba.append(input);

        *ok = false;
        return temp_ba;
    }

    int end = input.indexOf('\r');
    if(end < 0)
    {
        temp_ba.clear();
        temp_ba.append(input);

        *ok = false;
        return temp_ba;
    }

    if(end < begin)
    {
        temp_ba.clear();
        temp_ba.append(input);

        *ok = false;
        return temp_ba;
    }

    temp_ba = QByteArray::fromHex(input.toLocal8Bit());

    *ok = true;
    return temp_ba;
}
//--------------------------------------------------------------------------------
QByteArray ControlBox::convert_data_to_ascii(QByteArray data)
{
    QByteArray temp;

    temp.clear();
    temp.append(':');
    temp.append(data.toHex());
    temp.append((char)0x0D);

    emit debug(QString("convert_data_to_ascii %1").arg(temp.data()));
    return temp;
}
//--------------------------------------------------------------------------------
int ControlBox::check_len_and_crc8(int len)
{
    if(data_serial.isEmpty())
    {
        last_error = ERR_NO_ANSWER;
        return last_error;
    }

    emit debug(data_serial.toHex());
    if(data_serial.length() != len)
    {
        last_error = ERR_BAD_LEN;
        return last_error;
    }

    unsigned short calc_crc8 = CRC::crc8((unsigned char *)data_serial.constData(), len - 1);

    unsigned char data_crc8 = data_serial.at(len - 1);

    if(data_crc8 != calc_crc8)
    {
        data_crc = data_crc8;
        calc_crc = calc_crc8;
        last_error = ERR_FAIL_CRC;
        return last_error;
    }
    last_error = ERR_PASS;
    return last_error;
}
//--------------------------------------------------------------------------------
void ControlBox::print_last_error(void)
{
    switch(last_error)
    {
    case ERR_PASS:              emit error("ERR_PASS"); break;

    case ERR_FAIL_CRC:
        emit error(QString("ERR_FAIL_CRC data_crc8 0x%1 != calc_crc8 0x%2")
                   .arg(data_crc, 0, 16)
                   .arg(calc_crc, 0, 16));
        break;

    case ERR_WRONG_PACKET:      emit error("ERR_WRONG_PACKET");     break;
    case ERR_ADC_DOWN:          emit error("ERR_ADC_DOWN");         break;
    case ERR_ADC_EMPTY:         emit error("ERR_ADC_EMPTY");        break;
    case ERR_CAL_BIGZERO:       emit error("ERR_CAL_BIGZERO");      break;
    case ERR_CAL_BIGKOEFF:      emit error("ERR_CAL_BIGKOEFF");     break;
    case ERR_CAL_NOZERO:        emit error("ERR_CAL_NOZERO");       break;
    case ERR_OVERLOAD:          emit error("ERR_OVERLOAD");         break;
    case ERR_WILL_TAKE_TIME:    emit error("ERR_WILL_TAKE_TIME");   break;
    case ERR_BUSY:              emit error("ERR_BUSY");             break;
    case ERR_BAD_LEN:           emit error("ERR_BAD_LEN");          break;
    case ERR_NO_ANSWER:         emit error("ERR_NO_ANSWER");        break;

    default:
        emit error(QString("unknown error %1").arg(last_error));
        break;
    }
}
//--------------------------------------------------------------------------------
