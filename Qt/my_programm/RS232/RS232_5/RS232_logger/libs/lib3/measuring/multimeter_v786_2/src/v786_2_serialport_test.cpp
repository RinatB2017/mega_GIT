/*********************************************************************************
**                                                                              **
**     Copyright (C) 2022                                                       **
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
#include <QVariant>
#include <QtMath>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "v786_2_multimeter.hpp"
#include "v786_2_serialport_test.hpp"
#include "v786_2_packets.hpp"
#include "defines.hpp"
#include "crc.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "connection.hpp"
//--------------------------------------------------------------------------------
SerialPort_V786_2_test::SerialPort_V786_2_test(QObject *parent) :
    QObject(parent)
{
    init();
}
//--------------------------------------------------------------------------------
SerialPort_V786_2_test::~SerialPort_V786_2_test()
{
    if(test_tb)
        mw->removeToolBar(test_tb);

    if(tp)
        delete tp;
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::init(void)
{
    ba_input.clear();
    ba_output.clear();

    data_0 = 0;

    is_open = false;
    measuring_value = 0;

    init_functions();
    add_toolbar();

    tp->set_serno(SerialNo);
    tp->set_year(SerialYear);
    tp->set_address(Address);
    tp->set_channel(Channel);

    connect(tp,     &SerialPort_V786_2_test_param::s_get,
            this,   &SerialPort_V786_2_test::f_get);
    connect(tp,     &SerialPort_V786_2_test_param::s_set,
            this,   &SerialPort_V786_2_test::f_set);
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::f_get(void)
{
    tp->set_serno(SerialNo);
    tp->set_year(SerialYear);
    tp->set_address(Address);
    tp->set_channel(Channel);
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::f_set(void)
{
    SerialNo    = tp->get_serno();
    SerialYear  = tp->get_year();
    Address     = tp->get_address();
    Channel     = tp->get_channel();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::add_toolbar(void)
{
    QWidgetList widgets = qApp->allWidgets();
    foreach (QWidget *widget, widgets)
    {
        mw = dynamic_cast<MainWindow *>(widget);
        if(mw)
        {
#if 0
            test_tb = new QToolBar();
            test_tb->setObjectName("test_toolbar");

            QPushButton *btn = new QPushButton("set param");
            connect(btn,    &QPushButton::clicked,
                    this,   &SerialPort_V786_2_test::set_param);

            test_tb->addWidget(btn);

            mw->addToolBar(test_tb);
#else
            tp = new SerialPort_V786_2_test_param();
            mw->add_dock_widget("test_param", "test_param", Qt::RightDockWidgetArea, tp);
#endif
            return;
        }
    }
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::init_functions(void)
{
    functions.append({ V764_2_RF_STATUS_REQUEST,                V764_2_RF_STATUS_REQUEST_TEXT,              &SerialPort_V786_2_test::prepare_answer_V764_2_RF_STATUS_REQUEST });
    functions.append({ V764_2_RF_REMOTE_CONTROL_ENABLE,         V764_2_RF_REMOTE_CONTROL_ENABLE_TEXT,       &SerialPort_V786_2_test::prepare_answer_V764_2_RF_REMOTE_CONTROL_ENABLE });
    functions.append({ V764_2_RF_REMOTE_CONTROL_DISABLE,        V764_2_RF_REMOTE_CONTROL_DISABLE_TEXT,      &SerialPort_V786_2_test::prepare_answer_V764_2_RF_REMOTE_CONTROL_DISABLE });
    functions.append({ V764_2_RF_SERVICE_ENABLE,                V764_2_RF_SERVICE_ENABLE_TEXT,              &SerialPort_V786_2_test::prepare_answer_V764_2_RF_SERVICE_ENABLE });
    functions.append({ V764_2_RF_SERVICE_DISABLE,               V764_2_RF_SERVICE_DISABLE_TEXT,             &SerialPort_V786_2_test::prepare_answer_V764_2_RF_SERVICE_DISABLE });
    functions.append({ V764_2_RF_POWER_OFF,                     V764_2_RF_POWER_OFF_TEXT,                   &SerialPort_V786_2_test::prepare_answer_V764_2_RF_POWER_OFF });
    functions.append({ V764_2_RF_DC_VOLTAGE_MEASUREMENT,        V764_2_RF_DC_VOLTAGE_MEASUREMENT_TEXT,      &SerialPort_V786_2_test::prepare_answer_V764_2_RF_DC_VOLTAGE_MEASUREMENT });
    functions.append({ V764_2_RF_AC_VOLTAGE_MEASUREMENT,        V764_2_RF_AC_VOLTAGE_MEASUREMENT_TEXT,      &SerialPort_V786_2_test::prepare_answer_V764_2_RF_AC_VOLTAGE_MEASUREMENT });
    functions.append({ V764_2_RF_DC_CURRENT_MEASUREMENT,        V764_2_RF_DC_CURRENT_MEASUREMENT_TEXT,      &SerialPort_V786_2_test::prepare_answer_V764_2_RF_DC_CURRENT_MEASUREMENT });
    functions.append({ V764_2_RF_AC_CURRENT_MEASUREMENT,        V764_2_RF_AC_CURRENT_MEASUREMENT_TEXT,      &SerialPort_V786_2_test::prepare_answer_V764_2_RF_AC_CURRENT_MEASUREMENT });
    functions.append({ V764_2_RF_RESISTANCE_MEASUREMENT,        V764_2_RF_RESISTANCE_MEASUREMENT_TEXT,      &SerialPort_V786_2_test::prepare_answer_V764_2_RF_RESISTANCE_MEASUREMENT });
    functions.append({ V764_2_RF_CAPACITANCE_MEASUREMENT,       V764_2_RF_CAPACITANCE_MEASUREMENT_TEXT,     &SerialPort_V786_2_test::prepare_answer_V764_2_RF_CAPACITANCE_MEASUREMENT });
    functions.append({ V764_2_RF_INDUCTANCE_MEASUREMENT,        V764_2_RF_INDUCTANCE_MEASUREMENT_TEXT,      &SerialPort_V786_2_test::prepare_answer_V764_2_RF_INDUCTANCE_MEASUREMENT });
    functions.append({ V764_2_RF_BUZZER_TEST,                   V764_2_RF_BUZZER_TEST_TEXT,                 &SerialPort_V786_2_test::prepare_answer_V764_2_RF_BUZZER_TEST });
    functions.append({ V764_2_RF_FREQUENCY_MEASUREMENT,         V764_2_RF_FREQUENCY_MEASUREMENT_TEXT,       &SerialPort_V786_2_test::prepare_answer_V764_2_RF_FREQUENCY_MEASUREMENT });
    functions.append({ V764_2_RF_ACTIVE_POWER_MEASUREMENT,      V764_2_RF_ACTIVE_POWER_MEASUREMENT_TEXT,    &SerialPort_V786_2_test::prepare_answer_V764_2_RF_ACTIVE_POWER_MEASUREMENT });
    functions.append({ V764_2_RF_REACTIVE_POWER_MEASUREMENT,    V764_2_RF_REACTIVE_POWER_MEASUREMENT_TEXT,  &SerialPort_V786_2_test::prepare_answer_V764_2_RF_REACTIVE_POWER_MEASUREMENT });
    functions.append({ V764_2_RF_FULL_POWER_MEASUREMENT,        V764_2_RF_FULL_POWER_MEASUREMENT_TEXT,      &SerialPort_V786_2_test::prepare_answer_V764_2_RF_FULL_POWER_MEASUREMENT });
    functions.append({ V764_2_RF_COS_FI_MEASUREMENT,            V764_2_RF_COS_FI_MEASUREMENT_TEXT,          &SerialPort_V786_2_test::prepare_answer_V764_2_RF_COS_FI_MEASUREMENT });
    functions.append({ V764_2_RF_UDC_CAL,                       V764_2_RF_UDC_CAL_TEXT,                     &SerialPort_V786_2_test::prepare_answer_V764_2_RF_UDC_CAL });
    functions.append({ V764_2_RF_UAC_CAL,                       V764_2_RF_UAC_CAL_TEXT,                     &SerialPort_V786_2_test::prepare_answer_V764_2_RF_UAC_CAL });
    functions.append({ V764_2_RF_IDC_CAL,                       V764_2_RF_IDC_CAL_TEXT,                     &SerialPort_V786_2_test::prepare_answer_V764_2_RF_IDC_CAL });
    functions.append({ V764_2_RF_IAC_CAL,                       V764_2_RF_IAC_CAL_TEXT,                     &SerialPort_V786_2_test::prepare_answer_V764_2_RF_IAC_CAL });
    functions.append({ V764_2_RF_RES_CAL,                       V764_2_RF_RES_CAL_TEXT,                     &SerialPort_V786_2_test::prepare_answer_V764_2_RF_RES_CAL });
    functions.append({ V764_2_RF_FLASH_WRITE,                   V764_2_RF_FLASH_WRITE_TEXT,                 &SerialPort_V786_2_test::prepare_answer_V764_2_RF_FLASH_WRITE });
    functions.append({ V764_2_RF_DIODE_TEST,                    V764_2_RF_DIODE_TEST_TEXT,                  &SerialPort_V786_2_test::prepare_answer_V764_2_RF_DIODE_TEST });
    functions.append({ V764_2_RF_UDC_CAL_ZEROES,                V764_2_RF_UDC_CAL_ZEROES_TEXT,              &SerialPort_V786_2_test::prepare_answer_V764_2_RF_UDC_CAL_ZEROES });
    functions.append({ V764_2_RF_IDC_CAL_ZEROES,                V764_2_RF_IDC_CAL_ZEROES_TEXT,              &SerialPort_V786_2_test::prepare_answer_V764_2_RF_IDC_CAL_ZEROES });
    functions.append({ V764_2_RF_RES_CAL_ZEROES,                V764_2_RF_RES_CAL_ZEROES_TEXT,              &SerialPort_V786_2_test::prepare_answer_V764_2_RF_RES_CAL_ZEROES });
    functions.append({ V764_2_RF_CAP_CAL_ZEROES,                V764_2_RF_CAP_CAL_ZEROES_TEXT,              &SerialPort_V786_2_test::prepare_answer_V764_2_RF_CAP_CAL_ZEROES });
    functions.append({ V764_2_RF_IND_CAL_ZEROES,                V764_2_RF_IND_CAL_ZEROES_TEXT,              &SerialPort_V786_2_test::prepare_answer_V764_2_RF_IND_CAL_ZEROES });
    functions.append({ V764_2_RF_FLASH_WRITE_ZEROES,            V764_2_RF_FLASH_WRITE_ZEROES_TEXT,          &SerialPort_V786_2_test::prepare_answer_V764_2_RF_FLASH_WRITE_ZEROES });
    functions.append({ V764_2_RF_READ_POINT_FROM_MEMORY,        V764_2_RF_READ_POINT_FROM_MEMORY_TEXT,      &SerialPort_V786_2_test::prepare_answer_V764_2_RF_READ_POINT_FROM_MEMORY });
    functions.append({ V764_2_RF_WRITE_POINT_TO_MEMORY,         V764_2_RF_WRITE_POINT_TO_MEMORY_TEXT,       &SerialPort_V786_2_test::prepare_answer_V764_2_RF_WRITE_POINT_TO_MEMORY });
    functions.append({ V764_2_RF_READ_FIRMWARE_VERSION,         V764_2_RF_READ_FIRMWARE_VERSION_TEXT,       &SerialPort_V786_2_test::prepare_answer_V764_2_RF_READ_FIRMWARE_VERSION });
    functions.append({ V764_2_RF_SET_NEW_ADDRESS,               V764_2_RF_SET_NEW_ADDRESS_TEXT,             &SerialPort_V786_2_test::prepare_answer_V764_2_RF_SET_NEW_ADDRESS });
    functions.append({ V764_2_RF_GET_RSSI,                      V764_2_RF_GET_RSSI_TEXT,                    &SerialPort_V786_2_test::prepare_answer_V764_2_RF_GET_RSSI });

    functions.append({ V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES,
                       V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES_TEXT,
                       &SerialPort_V786_2_test::prepare_answer_V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES });
    functions.append({ V764_2_RF_GET_CNT_E21,
                       V764_2_RF_GET_CNT_E21_TEXT,
                       &SerialPort_V786_2_test::prepare_answer_V764_2_RF_GET_CNT_E21 });
    functions.append({ V764_2_RF_TEST,
                       V764_2_RF_TEST_TEXT,
                       &SerialPort_V786_2_test::prepare_answer_V764_2_RF_TEST });
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::set_param(void)
{
    QSpinBox *serno   = new QSpinBox();
    QSpinBox *year    = new QSpinBox();
    QSpinBox *address = new QSpinBox();
    QSpinBox *channel = new QSpinBox();

    address->setMinimum(0);
    address->setMaximum(255);

    serno->setMinimum(0);
    serno->setMaximum(9999);

    year->setMinimum(2010);
    year->setMaximum(2100);

    channel->setMinimum(1);
    channel->setMaximum(15);

    address->setValue(Address);
    serno->setValue(SerialNo);
    year->setValue(SerialYear);
    channel->setValue(Channel);

    QPushButton *btn_ok = new QPushButton("OK");
    QPushButton *btn_cancel = new QPushButton("Cancel");

    btn_ok->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogOkButton));
    btn_cancel->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogCancelButton));

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel("serno"),   0, 0);
    grid->addWidget(serno,                 0, 1);
    grid->addWidget(new QLabel("year"),    1, 0);
    grid->addWidget(year,                  1, 1);
    grid->addWidget(new QLabel("address"), 2, 0);
    grid->addWidget(address,               2, 1);
    grid->addWidget(new QLabel("channel"), 3, 0);
    grid->addWidget(channel,               3, 1);
    grid->addWidget(btn_ok,                4, 0);
    grid->addWidget(btn_cancel,            4, 1);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid);

    QDialog *dlg = new QDialog();
    dlg->setLayout(vbox);
    dlg->setFixedSize(dlg->sizeHint());

    connect(btn_ok,     &QPushButton::clicked,
            dlg,        &QDialog::accept);
    connect(btn_cancel, &QPushButton::clicked,
            dlg,        &QDialog::reject);

    int button = dlg->exec();
    if(button == QDialog::Accepted)
    {
        SerialNo = serno->value();
        SerialYear  = year->value();
        Address = address->value();
        Channel = channel->value();
    }
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::print_log(const QString &text)
{
    QString temp;
    temp.append("### Б7-86/2: ");
    temp.append(text);

    emit debug(temp);
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::print_err_log(const QString &text)
{
    QString temp;
    temp.append("### Б7-86/2: ");
    temp.append(text);

    emit debug(temp);
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::isOpen(void)
{
    return is_open;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::open(int mode)
{
#ifdef QT_DEBUG
    qDebug() << "open" << mode;
#endif
    Q_UNUSED(mode)
    is_open = true;
    return is_open;
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::close(void)
{
#ifdef QT_DEBUG
    qDebug() << "close";
#endif
    is_open = false;
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::port_close(void)
{
    is_open = false;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::serial_open(void)
{
    is_open = true;
    return is_open;
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::serial_close(void)
{
    is_open = false;
}
//--------------------------------------------------------------------------------
QStringList SerialPort_V786_2_test::get_port_names(void)
{
    QStringList sl;
    sl.append("TEST");

    return sl;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::setPortName(const QString &name)
{
    SerialName = name;
    return true;
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::setPort(QSerialPortInfo info)
{
#ifdef QT_DEBUG
    qDebug() << "setPort" << info.portName();
#endif
    Q_UNUSED(info)
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::setBaudRate(int speed)
{
#ifdef QT_DEBUG
    qDebug() << "setBaudRate" << speed;
#endif
    Q_UNUSED(speed)
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::setSerNo(uint16_t serno)
{
    emit trace(Q_FUNC_INFO);
    SerialNo = serno;
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::setYear(uint16_t year)
{
    emit trace(Q_FUNC_INFO);
    SerialYear = year;
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::setAddress(uint8_t address)
{
    Address = address;
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::setChannel(uint8_t channel)
{
    Channel = channel;
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::set_data_0(uint8_t value)
{
    data_0 = value;
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::set_data_1(uint8_t value)
{
    data_1 = value;
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::set_timeout(uint16_t value)
{
    timeout_ms = value;
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::set_Range(int value)
{
    Range = value;
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::set_Mode(int value)
{
    Mode = value;
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::set_Math_Hold(int value)
{
    Math_Hold = value;
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::set_limits(uint8_t value)
{
    limits = value;
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::set_deviation_retention(int value)
{
    deviation_retention = value;
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::set_Major(int value)
{
    Major = value;
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::set_Minor(int value)
{
    Minor = value;
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::set_Release(int value)
{
    Release = value;
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::set_measuring_value(int value)
{
    measuring_value = value;
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::set_BattVoltage(qreal value)
{
    BattVoltage = value;
    return true;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
uint16_t SerialPort_V786_2_test::getSerNo(void)
{
    return SerialNo;
}
//--------------------------------------------------------------------------------
uint16_t SerialPort_V786_2_test::getYear(void)
{
    return SerialYear;
}
//--------------------------------------------------------------------------------
uint8_t SerialPort_V786_2_test::getAddress(void)
{
    return Address;
}
//--------------------------------------------------------------------------------
uint8_t SerialPort_V786_2_test::getChannel(void)
{
    return Channel;
}
//--------------------------------------------------------------------------------
QByteArray SerialPort_V786_2_test::readAll(void)
{
#ifdef QT_DEBUG
    qDebug() << "output" << ba_output.toHex();
#endif
    return ba_output;
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::input(QByteArray data)
{
#ifdef QT_DEBUG
    qDebug() << QString("SerialPort_V786_2_test: отправлены данные: %1`")
                .arg(data.data());
#endif
    write(data);
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::write(const QByteArray &data)
{
    if(data.isEmpty()) return;

    print_log(data.toHex());

#ifndef FAST_TEST
    wait(100);
#endif

    QByteArray ba = data;
    ba.replace("\r", "").replace("\n", "").replace(":", "");
    if(ba == BRELOGUE_FIND)
    {
        print_log(QString("Возвращаем ответ от брелка %1").arg(breloque_answer));
        //ba_output.clear();
        //ba_output.append(breloque_answer);
        //emit s_readyRead();
        delay();
        //emit output(ba_output);
        emit output(breloque_answer.toLocal8Bit());
        return;
    }
    if(ba == BRELOGUE_VER)
    {
        print_log(QString("Возвращаем версию брелка %1").arg(30));
        ba_output.clear();
        ba_output.append("30");
        //emit s_readyRead();
        delay();
        emit output(ba_output);
        return;
    }

    QByteArray ba_hex = QByteArray::fromHex(ba);
    if(ba_hex.length() == sizeof(RF_set_param_2))
    {
        RF_set_param_2 *packet = reinterpret_cast<RF_set_param_2 *>(ba_hex.data());
        if(packet->magic == 0xAABB)
        {
            RF_set_param_2 param;
            param.magic = 0xAABB;
            param.address = 1234;
            param.command = 0;

            //emit s_readyRead();
            delay();

            QByteArray ba;
            ba.append((char *)&param, sizeof(param));

            emit output(ba);
            return;
        }
    }

    bool ok = false;
    ba_input.clear();
    ba_input.append(convert_ascii_to_data(data, &ok));
    if(!ok)
    {
#ifdef QT_DEBUG
        qDebug() << "error data" << data.toHex();
#endif
        return;
    }

    int id = ba_input.at(1);
    auto cmd_it = std::find_if(
                functions.begin(),
                functions.end(),
                [id](FUNC func){ return func.id == id; }
            );
    if (cmd_it != functions.end())
    {
        typedef void (SerialPort_V786_2_test::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (this->*x)();
        }
    }
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::write(const char *data, int len)
{
#ifdef QT_DEBUG
    qDebug() << "write";
#endif

    QByteArray temp;
    temp.append(data, len);
    print_log(temp.toHex());

    QByteArray ba;
    ba.clear();
    ba.append(data, len);
    write(ba);
}
//--------------------------------------------------------------------------------
template<typename T1>
bool SerialPort_V786_2_test::check_packet(void)
{
    T1 *question = reinterpret_cast<T1 *>(ba_input.data());
    Q_ASSERT(question);

    uint16_t crc = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(question),
                                         sizeof(*question) - SIZE_CRC);
    uint16_t data_crc = question->crc16;

#ifdef USE_HTONS2
    crc = htons(crc);   //FIXME надо сделать правимльно
#endif

    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_packet: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        print_err_log(QString("  crc: %1").arg(crc, 2, 16, QChar('0')));
        print_err_log(QString("crc16: %1").arg(data_crc, 2, 16, QChar('0')));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_STATUS_REQUEST(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_STATUS_REQUEST_TEXT));
    bool ok = check_packet<V764_2_RF_STATUS_REQUEST_Q>();
    if(ok)
    {
        V764_2_RF_STATUS_REQUEST_Q *question = reinterpret_cast<V764_2_RF_STATUS_REQUEST_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_REMOTE_CONTROL_ENABLE(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_REMOTE_CONTROL_ENABLE_TEXT));
    bool ok = check_packet<V764_2_RF_REMOTE_CONTROL_ENABLE_Q>();
    if(ok)
    {
        V764_2_RF_REMOTE_CONTROL_ENABLE_Q *question = reinterpret_cast<V764_2_RF_REMOTE_CONTROL_ENABLE_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_REMOTE_CONTROL_DISABLE(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_REMOTE_CONTROL_DISABLE_TEXT));
    bool ok = check_packet<V764_2_RF_REMOTE_CONTROL_DISABLE_Q>();
    if(ok)
    {
        V764_2_RF_REMOTE_CONTROL_DISABLE_Q *question = reinterpret_cast<V764_2_RF_REMOTE_CONTROL_DISABLE_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_SERVICE_ENABLE(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_SERVICE_ENABLE_TEXT));
    bool ok = check_packet<V764_2_RF_SERVICE_ENABLE_Q>();
    if(ok)
    {
        V764_2_RF_SERVICE_ENABLE_Q *question = reinterpret_cast<V764_2_RF_SERVICE_ENABLE_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_POWER_OFF(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_POWER_OFF_TEXT));
    bool ok = check_packet<V764_2_RF_POWER_OFF_Q>();
    if(ok)
    {
        V764_2_RF_POWER_OFF_Q *question = reinterpret_cast<V764_2_RF_POWER_OFF_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_DC_VOLTAGE_MEASUREMENT(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_DC_VOLTAGE_MEASUREMENT_TEXT));
    bool ok = check_packet<V764_2_RF_DC_VOLTAGE_MEASUREMENT_Q>();
    if(ok)
    {
        V764_2_RF_DC_VOLTAGE_MEASUREMENT_Q *question = reinterpret_cast<V764_2_RF_DC_VOLTAGE_MEASUREMENT_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   data_0: %1").arg(question->data_0));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));

        limits = question->data_0;  //TODO надо проверить
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_AC_VOLTAGE_MEASUREMENT(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_AC_VOLTAGE_MEASUREMENT_TEXT));
    bool ok = check_packet<V764_2_RF_AC_VOLTAGE_MEASUREMENT_Q>();
    if(ok)
    {
        V764_2_RF_AC_VOLTAGE_MEASUREMENT_Q *question = reinterpret_cast<V764_2_RF_AC_VOLTAGE_MEASUREMENT_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   data_0: %1").arg(question->data_0));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));

        limits = question->data_0;  //TODO надо проверить
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_DC_CURRENT_MEASUREMENT(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_DC_CURRENT_MEASUREMENT_TEXT));
    bool ok = check_packet<V764_2_RF_DC_CURRENT_MEASUREMENT_Q>();
    if(ok)
    {
        V764_2_RF_DC_CURRENT_MEASUREMENT_Q *question = reinterpret_cast<V764_2_RF_DC_CURRENT_MEASUREMENT_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   data_0: %1").arg(question->data_0));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));

        limits = question->data_0;  //TODO надо проверить
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_AC_CURRENT_MEASUREMENT(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_AC_CURRENT_MEASUREMENT_TEXT));
    bool ok = check_packet<V764_2_RF_AC_CURRENT_MEASUREMENT_Q>();
    if(ok)
    {
        V764_2_RF_AC_CURRENT_MEASUREMENT_Q *question = reinterpret_cast<V764_2_RF_AC_CURRENT_MEASUREMENT_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   data_0: %1").arg(question->data_0));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));

        limits = question->data_0;  //TODO надо проверить
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_RESISTANCE_MEASUREMENT(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_RESISTANCE_MEASUREMENT_TEXT));
    bool ok = check_packet<V764_2_RF_RESISTANCE_MEASUREMENT_Q>();
    if(ok)
    {
        V764_2_RF_RESISTANCE_MEASUREMENT_Q *question = reinterpret_cast<V764_2_RF_RESISTANCE_MEASUREMENT_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   data_0: %1").arg(question->data_0));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));

        limits = question->data_0;  //TODO надо проверить
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_CAPACITANCE_MEASUREMENT(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_CAPACITANCE_MEASUREMENT_TEXT));
    bool ok = check_packet<V764_2_RF_CAPACITANCE_MEASUREMENT_Q>();
    if(ok)
    {
        V764_2_RF_CAPACITANCE_MEASUREMENT_Q *question = reinterpret_cast<V764_2_RF_CAPACITANCE_MEASUREMENT_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   data_0: %1").arg(question->data_0));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));

        limits = question->data_0;  //TODO надо проверить
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_INDUCTANCE_MEASUREMENT(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_INDUCTANCE_MEASUREMENT_TEXT));
    bool ok = check_packet<V764_2_RF_INDUCTANCE_MEASUREMENT_Q>();
    if(ok)
    {
        V764_2_RF_INDUCTANCE_MEASUREMENT_Q *question = reinterpret_cast<V764_2_RF_INDUCTANCE_MEASUREMENT_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   data_0: %1").arg(question->data_0));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));

        limits = question->data_0;  //TODO надо проверить
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_BUZZER_TEST(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_BUZZER_TEST_TEXT));
    bool ok = check_packet<V764_2_RF_BUZZER_TEST_Q>();
    if(ok)
    {
        V764_2_RF_BUZZER_TEST_Q *question = reinterpret_cast<V764_2_RF_BUZZER_TEST_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   data_0: %1").arg(question->data_0));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_FREQUENCY_MEASUREMENT(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_FREQUENCY_MEASUREMENT_TEXT));
    bool ok = check_packet<V764_2_RF_FREQUENCY_MEASUREMENT_Q>();
    if(ok)
    {
        V764_2_RF_FREQUENCY_MEASUREMENT_Q *question = reinterpret_cast<V764_2_RF_FREQUENCY_MEASUREMENT_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   data_0: %1").arg(question->data_0));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_ACTIVE_POWER_MEASUREMENT(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_ACTIVE_POWER_MEASUREMENT_TEXT));
    bool ok = check_packet<V764_2_RF_ACTIVE_POWER_MEASUREMENT_Q>();
    if(ok)
    {
        V764_2_RF_ACTIVE_POWER_MEASUREMENT_Q *question = reinterpret_cast<V764_2_RF_ACTIVE_POWER_MEASUREMENT_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   data_0: %1").arg(question->data_0));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_REACTIVE_POWER_MEASUREMENT(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_REACTIVE_POWER_MEASUREMENT_TEXT));
    bool ok = check_packet<V764_2_RF_REACTIVE_POWER_MEASUREMENT_Q>();
    if(ok)
    {
        V764_2_RF_REACTIVE_POWER_MEASUREMENT_Q *question = reinterpret_cast<V764_2_RF_REACTIVE_POWER_MEASUREMENT_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   data_0: %1").arg(question->data_0));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_FULL_POWER_MEASUREMENT(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_FULL_POWER_MEASUREMENT_TEXT));
    bool ok = check_packet<V764_2_RF_FULL_POWER_MEASUREMENT_Q>();
    if(ok)
    {
        V764_2_RF_FULL_POWER_MEASUREMENT_Q *question = reinterpret_cast<V764_2_RF_FULL_POWER_MEASUREMENT_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   data_0: %1").arg(question->data_0));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_COS_FI_MEASUREMENT(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_COS_FI_MEASUREMENT_TEXT));
    bool ok = check_packet<V764_2_RF_COS_FI_MEASUREMENT_Q>();
    if(ok)
    {
        V764_2_RF_COS_FI_MEASUREMENT_Q *question = reinterpret_cast<V764_2_RF_COS_FI_MEASUREMENT_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   data_0: %1").arg(question->data_0));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_UDC_CAL(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_UDC_CAL_TEXT));
    bool ok = check_packet<V764_2_RF_UDC_CAL_Q>();
    if(ok)
    {
        V764_2_RF_UDC_CAL_Q *question = reinterpret_cast<V764_2_RF_UDC_CAL_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   data_0: %1").arg(question->data_0));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_UAC_CAL(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_UAC_CAL_TEXT));
    bool ok = check_packet<V764_2_RF_UAC_CAL_Q>();
    if(ok)
    {
        V764_2_RF_UAC_CAL_Q *question = reinterpret_cast<V764_2_RF_UAC_CAL_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   data_0: %1").arg(question->data_0));
        print_log(QString("   data_1: %1").arg(question->data_1));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_IDC_CAL(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_IDC_CAL_TEXT));
    bool ok = check_packet<V764_2_RF_IDC_CAL_Q>();
    if(ok)
    {
        V764_2_RF_IDC_CAL_Q *question = reinterpret_cast<V764_2_RF_IDC_CAL_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   data_0: %1").arg(question->data_0));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_IAC_CAL(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_IAC_CAL_TEXT));
    bool ok = check_packet<V764_2_RF_IAC_CAL_Q>();
    if(ok)
    {
        V764_2_RF_IAC_CAL_Q *question = reinterpret_cast<V764_2_RF_IAC_CAL_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   data_0: %1").arg(question->data_0));
        print_log(QString("   data_1: %1").arg(question->data_1));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_RES_CAL(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_RES_CAL_TEXT));
    bool ok = check_packet<V764_2_RF_RES_CAL_Q>();
    if(ok)
    {
        V764_2_RF_RES_CAL_Q *question = reinterpret_cast<V764_2_RF_RES_CAL_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   data_0: %1").arg(question->data_0));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_FLASH_WRITE(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_FLASH_WRITE_TEXT));
    bool ok = check_packet<V764_2_RF_FLASH_WRITE_Q>();
    if(ok)
    {
        V764_2_RF_FLASH_WRITE_Q *question = reinterpret_cast<V764_2_RF_FLASH_WRITE_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_SERVICE_DISABLE(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_SERVICE_DISABLE_TEXT));
    bool ok = check_packet<V764_2_RF_SERVICE_DISABLE_Q>();
    if(ok)
    {
        V764_2_RF_SERVICE_DISABLE_Q *question = reinterpret_cast<V764_2_RF_SERVICE_DISABLE_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_DIODE_TEST(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_DIODE_TEST_TEXT));
    bool ok = check_packet<V764_2_RF_DIODE_TEST_Q>();
    if(ok)
    {
        V764_2_RF_DIODE_TEST_Q *question = reinterpret_cast<V764_2_RF_DIODE_TEST_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_UDC_CAL_ZEROES(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_UDC_CAL_ZEROES_TEXT));
    bool ok = check_packet<V764_2_RF_UDC_CAL_ZEROES_Q>();
    if(ok)
    {
        V764_2_RF_UDC_CAL_ZEROES_Q *question = reinterpret_cast<V764_2_RF_UDC_CAL_ZEROES_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_IDC_CAL_ZEROES(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_IDC_CAL_ZEROES_TEXT));
    bool ok = check_packet<V764_2_RF_IDC_CAL_ZEROES_Q>();
    if(ok)
    {
        V764_2_RF_IDC_CAL_ZEROES_Q *question = reinterpret_cast<V764_2_RF_IDC_CAL_ZEROES_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_RES_CAL_ZEROES(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_RES_CAL_ZEROES_TEXT));
    bool ok = check_packet<V764_2_RF_RES_CAL_ZEROES_Q>();
    if(ok)
    {
        V764_2_RF_RES_CAL_ZEROES_Q *question = reinterpret_cast<V764_2_RF_RES_CAL_ZEROES_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_CAP_CAL_ZEROES(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_CAP_CAL_ZEROES_TEXT));
    bool ok = check_packet<V764_2_RF_CAP_CAL_ZEROES_Q>();
    if(ok)
    {
        V764_2_RF_CAP_CAL_ZEROES_Q *question = reinterpret_cast<V764_2_RF_CAP_CAL_ZEROES_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_IND_CAL_ZEROES(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_IND_CAL_ZEROES_TEXT));
    bool ok = check_packet<V764_2_RF_IND_CAL_ZEROES_Q>();
    if(ok)
    {
        V764_2_RF_IND_CAL_ZEROES_Q *question = reinterpret_cast<V764_2_RF_IND_CAL_ZEROES_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_FLASH_WRITE_ZEROES(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_FLASH_WRITE_ZEROES_TEXT));
    bool ok = check_packet<V764_2_RF_FLASH_WRITE_ZEROES_Q>();
    if(ok)
    {
        V764_2_RF_FLASH_WRITE_ZEROES_Q *question = reinterpret_cast<V764_2_RF_FLASH_WRITE_ZEROES_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_READ_POINT_FROM_MEMORY(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_READ_POINT_FROM_MEMORY_TEXT));
    bool ok = check_packet<V764_2_RF_READ_POINT_FROM_MEMORY_Q>();
    if(ok)
    {
        V764_2_RF_READ_POINT_FROM_MEMORY_Q *question = reinterpret_cast<V764_2_RF_READ_POINT_FROM_MEMORY_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   point_table: %1").arg(question->point_table));
        print_log(QString("   point_index_0: %1").arg(question->point_index_0));
        print_log(QString("   point_index_1: %1").arg(question->point_index_1));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_WRITE_POINT_TO_MEMORY(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_WRITE_POINT_TO_MEMORY_TEXT));
    bool ok = check_packet<V764_2_RF_WRITE_POINT_TO_MEMORY_Q>();
    if(ok)
    {
        V764_2_RF_WRITE_POINT_TO_MEMORY_Q *question = reinterpret_cast<V764_2_RF_WRITE_POINT_TO_MEMORY_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   point: %1").arg(question->point));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_READ_FIRMWARE_VERSION(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_READ_FIRMWARE_VERSION_TEXT));
    bool ok = check_packet<V764_2_RF_READ_FIRMWARE_VERSION_Q>();
    if(ok)
    {
        V764_2_RF_READ_FIRMWARE_VERSION_Q *question = reinterpret_cast<V764_2_RF_READ_FIRMWARE_VERSION_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_SET_NEW_ADDRESS(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_SET_NEW_ADDRESS_TEXT));
    bool ok = check_packet<V764_2_RF_SET_NEW_ADDRESS_Q>();
    if(ok)
    {
        V764_2_RF_SET_NEW_ADDRESS_Q *question = reinterpret_cast<V764_2_RF_SET_NEW_ADDRESS_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   remote_address: %1").arg(question->remote_address, 4, 16, QChar('0')));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_GET_RSSI(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_GET_RSSI_TEXT));
    bool ok = check_packet<V764_2_RF_GET_RSSI_Q>();
    if(ok)
    {
        V764_2_RF_GET_RSSI_Q *question = reinterpret_cast<V764_2_RF_GET_RSSI_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES_TEXT));
    bool ok = check_packet<V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES_Q>();
    if(ok)
    {
        V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES_Q *question = reinterpret_cast<V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_GET_CNT_E21(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_GET_CNT_E21_TEXT));
    bool ok = check_packet<V764_2_RF_GET_CNT_E21_Q>();
    if(ok)
    {
        V764_2_RF_GET_CNT_E21_Q *question = reinterpret_cast<V764_2_RF_GET_CNT_E21_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_V764_2_RF_TEST(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_2_RF_TEST_TEXT));
    bool ok = check_packet<V764_2_RF_TEST_Q>();
    if(ok)
    {
        V764_2_RF_TEST_Q *question = reinterpret_cast<V764_2_RF_TEST_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_STATUS_REQUEST(void)
{
    if(!check_V764_2_RF_STATUS_REQUEST()) return;

    V764_2_RF_STATUS_REQUEST_Q *question = reinterpret_cast<V764_2_RF_STATUS_REQUEST_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_STATUS_REQUEST_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);

    for(int n=0; n<7; n++) answer.DisplaySymbols[n] = ' ';
    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp;
    tmp.clear();
    tmp.append(temp.toLatin1());
    //if(measuring_value < 0) answer.DisplaySymbols[0] = '-';
    memcpy(&answer.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());

    answer.Range       = Range; // предел
    answer.Mode        = Mode; // режим
    answer.Math_Hold   = Math_Hold;
    answer.BattVoltage = BattVoltage;
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   DisplaySymbols: %1").arg(tmp.data()));
    print_log(QString("   Range: %1").arg(answer.Range));
    print_log(QString("   Mode: %1").arg(answer.Mode));
    print_log(QString("   Math_Hold: %1").arg(answer.Math_Hold));
    print_log(QString("   BattVoltage: %1").arg(answer.BattVoltage));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_REMOTE_CONTROL_ENABLE(void)
{
    if(!check_V764_2_RF_REMOTE_CONTROL_ENABLE()) return;

    V764_2_RF_REMOTE_CONTROL_ENABLE_Q *question = reinterpret_cast<V764_2_RF_REMOTE_CONTROL_ENABLE_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_REMOTE_CONTROL_ENABLE_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_REMOTE_CONTROL_DISABLE(void)
{
    if(!check_V764_2_RF_REMOTE_CONTROL_DISABLE()) return;

    V764_2_RF_REMOTE_CONTROL_DISABLE_Q *question = reinterpret_cast<V764_2_RF_REMOTE_CONTROL_DISABLE_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_REMOTE_CONTROL_DISABLE_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_SERVICE_ENABLE(void)
{
    if(!check_V764_2_RF_SERVICE_ENABLE()) return;

    V764_2_RF_SERVICE_ENABLE_Q *question = reinterpret_cast<V764_2_RF_SERVICE_ENABLE_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_SERVICE_ENABLE_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_POWER_OFF(void)
{
    if(!check_V764_2_RF_POWER_OFF()) return;

    V764_2_RF_POWER_OFF_Q *question = reinterpret_cast<V764_2_RF_POWER_OFF_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_POWER_OFF_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_DC_VOLTAGE_MEASUREMENT(void)
{
    if(!check_V764_2_RF_DC_VOLTAGE_MEASUREMENT()) return;

    V764_2_RF_DC_VOLTAGE_MEASUREMENT_Q *question = reinterpret_cast<V764_2_RF_DC_VOLTAGE_MEASUREMENT_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_DC_VOLTAGE_MEASUREMENT_A answer;

    //TODO measuring_value = Connection::get_uint16_value();
    measuring_value = Connection::get_uint16_value();

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);

    answer.limits = calc_limit();
    answer.deviation_retention = deviation_retention;
    for(int n=0; n<7; n++) answer.DisplaySymbols[n] = ' ';

    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp;
    tmp.clear();
    tmp.append(temp.toLatin1());
    memcpy(&answer.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());

#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   DisplaySymbols: %1").arg(tmp.data()));
    print_log(QString("   limits: %1").arg(answer.limits));
    print_log(QString("   deviation_retention: %1").arg(answer.deviation_retention));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_AC_VOLTAGE_MEASUREMENT(void)
{
    if(!check_V764_2_RF_AC_VOLTAGE_MEASUREMENT()) return;

    V764_2_RF_AC_VOLTAGE_MEASUREMENT_Q *question = reinterpret_cast<V764_2_RF_AC_VOLTAGE_MEASUREMENT_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_AC_VOLTAGE_MEASUREMENT_A answer;

    data_0 = question->data_0;

    //TODO measuring_value = Connection::get_uint16_value();

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    //answer.limits = data_0 & 0x7;
    //answer.deviation_retention = data_0 & 0xC0;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);

    answer.limits = calc_limit();
    answer.deviation_retention = deviation_retention;
    for(int n=0; n<7; n++) answer.DisplaySymbols[n] = ' ';

    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp;
    tmp.clear();
    tmp.append(temp.toLatin1());
    memcpy(&answer.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());

#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   DisplaySymbols: %1").arg(tmp.data()));
    print_log(QString("   limits: %1").arg(answer.limits));
    print_log(QString("   deviation_retention: %1").arg(answer.deviation_retention));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((const char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_DC_CURRENT_MEASUREMENT(void)
{
    if(!check_V764_2_RF_DC_CURRENT_MEASUREMENT()) return;

    V764_2_RF_DC_CURRENT_MEASUREMENT_Q *question = reinterpret_cast<V764_2_RF_DC_CURRENT_MEASUREMENT_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_DC_CURRENT_MEASUREMENT_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);

    answer.limits = calc_limit();
    answer.deviation_retention = deviation_retention;
    for(int n=0; n<7; n++) answer.DisplaySymbols[n] = ' ';

    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp;
    tmp.clear();
    tmp.append(temp.toLatin1());
    //if(measuring_value < 0) answer.DisplaySymbols[0] = '-';
    memcpy(&answer.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());

#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   DisplaySymbols: %1").arg(tmp.data()));
    print_log(QString("   limits: %1").arg(answer.limits));
    print_log(QString("   deviation_retention: %1").arg(answer.deviation_retention));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_AC_CURRENT_MEASUREMENT(void)
{
    if(!check_V764_2_RF_AC_CURRENT_MEASUREMENT()) return;

    V764_2_RF_AC_CURRENT_MEASUREMENT_Q *question = reinterpret_cast<V764_2_RF_AC_CURRENT_MEASUREMENT_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_AC_CURRENT_MEASUREMENT_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);

    answer.limits = calc_limit();
    answer.deviation_retention = deviation_retention;
    for(int n=0; n<7; n++) answer.DisplaySymbols[n] = ' ';

    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp;
    tmp.clear();
    tmp.append(temp.toLatin1());
    //if(measuring_value < 0) answer.DisplaySymbols[0] = '-';
    memcpy(&answer.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());

#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   DisplaySymbols: %1").arg(tmp.data()));
    print_log(QString("   limits: %1").arg(answer.limits));
    print_log(QString("   deviation_retention: %1").arg(answer.deviation_retention));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_RESISTANCE_MEASUREMENT(void)
{
    if(!check_V764_2_RF_RESISTANCE_MEASUREMENT()) return;

    V764_2_RF_RESISTANCE_MEASUREMENT_Q *question = reinterpret_cast<V764_2_RF_RESISTANCE_MEASUREMENT_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_RESISTANCE_MEASUREMENT_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);

    answer.limits = calc_limit();
    answer.deviation_retention = deviation_retention;
    for(int n=0; n<7; n++) answer.DisplaySymbols[n] = ' ';

    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp;
    tmp.clear();
    tmp.append(temp.toLatin1());
    //if(measuring_value < 0) answer.DisplaySymbols[0] = '-';
    memcpy(&answer.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());

#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   DisplaySymbols: %1").arg(tmp.data()));
    print_log(QString("   limits: %1").arg(answer.limits));
    print_log(QString("   deviation_retention: %1").arg(answer.deviation_retention));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_CAPACITANCE_MEASUREMENT(void)
{
    if(!check_V764_2_RF_CAPACITANCE_MEASUREMENT()) return;

    V764_2_RF_CAPACITANCE_MEASUREMENT_Q *question = reinterpret_cast<V764_2_RF_CAPACITANCE_MEASUREMENT_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_CAPACITANCE_MEASUREMENT_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);

    answer.limits = calc_limit();
    answer.deviation_retention = deviation_retention;
    for(int n=0; n<7; n++) answer.DisplaySymbols[n] = ' ';

    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp;
    tmp.clear();
    tmp.append(temp.toLatin1());
    //if(measuring_value < 0) answer.DisplaySymbols[0] = '-';
    memcpy(&answer.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());

#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   DisplaySymbols: %1").arg(tmp.data()));
    print_log(QString("   limits: %1").arg(answer.limits));
    print_log(QString("   deviation_retention: %1").arg(answer.deviation_retention));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_INDUCTANCE_MEASUREMENT(void)
{
    if(!check_V764_2_RF_INDUCTANCE_MEASUREMENT()) return;

    V764_2_RF_INDUCTANCE_MEASUREMENT_Q *question = reinterpret_cast<V764_2_RF_INDUCTANCE_MEASUREMENT_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_INDUCTANCE_MEASUREMENT_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);

    answer.limits = calc_limit();
    answer.deviation_retention = deviation_retention;
    for(int n=0; n<7; n++) answer.DisplaySymbols[n] = ' ';

    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp;
    tmp.clear();
    tmp.append(temp.toLatin1());
    memcpy(&answer.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());

#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   DisplaySymbols: %1").arg(tmp.data()));
    print_log(QString("   limits: %1").arg(answer.limits));
    print_log(QString("   deviation_retention: %1").arg(answer.deviation_retention));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_BUZZER_TEST(void)
{
    if(!check_V764_2_RF_BUZZER_TEST()) return;

    V764_2_RF_BUZZER_TEST_Q *question = reinterpret_cast<V764_2_RF_BUZZER_TEST_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_BUZZER_TEST_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   xxx: ???"));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_FREQUENCY_MEASUREMENT(void)
{
    if(!check_V764_2_RF_FREQUENCY_MEASUREMENT()) return;

    V764_2_RF_FREQUENCY_MEASUREMENT_Q *question = reinterpret_cast<V764_2_RF_FREQUENCY_MEASUREMENT_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_FREQUENCY_MEASUREMENT_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);

    answer.limits = calc_limit();
    answer.deviation_retention = deviation_retention;
    for(int n=0; n<7; n++) answer.DisplaySymbols[n] = ' ';

    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp;
    tmp.clear();
    tmp.append(temp.toLatin1());
    memcpy(&answer.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());

#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   DisplaySymbols: %1").arg(tmp.data()));
    print_log(QString("   limits: %1").arg(answer.limits));
    print_log(QString("   deviation_retention: %1").arg(answer.deviation_retention));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_ACTIVE_POWER_MEASUREMENT(void)
{
    if(!check_V764_2_RF_ACTIVE_POWER_MEASUREMENT()) return;

    V764_2_RF_ACTIVE_POWER_MEASUREMENT_Q *question = reinterpret_cast<V764_2_RF_ACTIVE_POWER_MEASUREMENT_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_ACTIVE_POWER_MEASUREMENT_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);

    answer.limits = calc_limit();
    answer.deviation_retention = deviation_retention;
    for(int n=0; n<7; n++) answer.DisplaySymbols[n] = ' ';

    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp;
    tmp.clear();
    tmp.append(temp.toLatin1());
    memcpy(&answer.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());

#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   DisplaySymbols: %1").arg(tmp.data()));
    print_log(QString("   limits: %1").arg(answer.limits));
    print_log(QString("   deviation_retention: %1").arg(answer.deviation_retention));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_REACTIVE_POWER_MEASUREMENT(void)
{
    if(!check_V764_2_RF_REACTIVE_POWER_MEASUREMENT()) return;

    V764_2_RF_REACTIVE_POWER_MEASUREMENT_Q *question = reinterpret_cast<V764_2_RF_REACTIVE_POWER_MEASUREMENT_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_REACTIVE_POWER_MEASUREMENT_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);

    answer.limits = calc_limit();
    answer.deviation_retention = deviation_retention;
    for(int n=0; n<7; n++) answer.DisplaySymbols[n] = ' ';

    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp;
    tmp.clear();
    tmp.append(temp.toLatin1());
    memcpy(&answer.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());

#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   DisplaySymbols: %1").arg(tmp.data()));
    print_log(QString("   limits: %1").arg(answer.limits));
    print_log(QString("   deviation_retention: %1").arg(answer.deviation_retention));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_FULL_POWER_MEASUREMENT(void)
{
    if(!check_V764_2_RF_FULL_POWER_MEASUREMENT()) return;

    V764_2_RF_FULL_POWER_MEASUREMENT_Q *question = reinterpret_cast<V764_2_RF_FULL_POWER_MEASUREMENT_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_FULL_POWER_MEASUREMENT_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);

    answer.limits = calc_limit();
    answer.deviation_retention = deviation_retention;
    for(int n=0; n<7; n++) answer.DisplaySymbols[n] = ' ';

    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp;
    tmp.clear();
    tmp.append(temp.toLatin1());
    memcpy(&answer.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());

#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   DisplaySymbols: %1").arg(tmp.data()));
    print_log(QString("   limits: %1").arg(answer.limits));
    print_log(QString("   deviation_retention: %1").arg(answer.deviation_retention));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_COS_FI_MEASUREMENT(void)
{
    if(!check_V764_2_RF_COS_FI_MEASUREMENT()) return;

    V764_2_RF_COS_FI_MEASUREMENT_Q *question = reinterpret_cast<V764_2_RF_COS_FI_MEASUREMENT_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_COS_FI_MEASUREMENT_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);

    answer.limits = calc_limit();
    answer.deviation_retention = deviation_retention;
    for(int n=0; n<7; n++) answer.DisplaySymbols[n] = ' ';

    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp;
    tmp.clear();
    tmp.append(temp.toLatin1());
    memcpy(&answer.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());

#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   DisplaySymbols: %1").arg(tmp.data()));
    print_log(QString("   limits: %1").arg(answer.limits));
    print_log(QString("   deviation_retention: %1").arg(answer.deviation_retention));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_UDC_CAL(void)
{
    if(!check_V764_2_RF_UDC_CAL()) return;

    V764_2_RF_UDC_CAL_Q *question = reinterpret_cast<V764_2_RF_UDC_CAL_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_UDC_CAL_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_UAC_CAL(void)
{
    if(!check_V764_2_RF_UAC_CAL()) return;

    V764_2_RF_UAC_CAL_Q *question = reinterpret_cast<V764_2_RF_UAC_CAL_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_UAC_CAL_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_IDC_CAL(void)
{
    if(!check_V764_2_RF_IDC_CAL()) return;

    V764_2_RF_IDC_CAL_Q *question = reinterpret_cast<V764_2_RF_IDC_CAL_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_IDC_CAL_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_IAC_CAL(void)
{
    if(!check_V764_2_RF_IAC_CAL()) return;

    V764_2_RF_IAC_CAL_Q *question = reinterpret_cast<V764_2_RF_IAC_CAL_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_IAC_CAL_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_RES_CAL(void)
{
    if(!check_V764_2_RF_RES_CAL()) return;

    V764_2_RF_RES_CAL_Q *question = reinterpret_cast<V764_2_RF_RES_CAL_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_RES_CAL_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_FLASH_WRITE(void)
{
    if(!check_V764_2_RF_FLASH_WRITE()) return;

    V764_2_RF_FLASH_WRITE_Q *question = reinterpret_cast<V764_2_RF_FLASH_WRITE_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_FLASH_WRITE_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_SERVICE_DISABLE(void)
{
    if(!check_V764_2_RF_SERVICE_DISABLE()) return;

    V764_2_RF_SERVICE_DISABLE_Q *question = reinterpret_cast<V764_2_RF_SERVICE_DISABLE_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_SERVICE_DISABLE_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_DIODE_TEST(void)
{
    if(!check_V764_2_RF_DIODE_TEST()) return;

    V764_2_RF_DIODE_TEST_Q *question = reinterpret_cast<V764_2_RF_DIODE_TEST_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_DIODE_TEST_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_UDC_CAL_ZEROES(void)
{
    if(!check_V764_2_RF_UDC_CAL_ZEROES()) return;

    V764_2_RF_UDC_CAL_ZEROES_Q *question = reinterpret_cast<V764_2_RF_UDC_CAL_ZEROES_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_UDC_CAL_ZEROES_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_IDC_CAL_ZEROES(void)
{
    if(!check_V764_2_RF_IDC_CAL_ZEROES()) return;

    V764_2_RF_IDC_CAL_ZEROES_Q *question = reinterpret_cast<V764_2_RF_IDC_CAL_ZEROES_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_IDC_CAL_ZEROES_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_RES_CAL_ZEROES(void)
{
    if(!check_V764_2_RF_RES_CAL_ZEROES()) return;

    V764_2_RF_RES_CAL_ZEROES_Q *question = reinterpret_cast<V764_2_RF_RES_CAL_ZEROES_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_IDC_CAL_ZEROES_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_CAP_CAL_ZEROES(void)
{
    if(!check_V764_2_RF_CAP_CAL_ZEROES()) return;

    V764_2_RF_CAP_CAL_ZEROES_Q *question = reinterpret_cast<V764_2_RF_CAP_CAL_ZEROES_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_CAP_CAL_ZEROES_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_IND_CAL_ZEROES(void)
{
    if(!check_V764_2_RF_IND_CAL_ZEROES()) return;

    V764_2_RF_IND_CAL_ZEROES_Q *question = reinterpret_cast<V764_2_RF_IND_CAL_ZEROES_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_IND_CAL_ZEROES_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_FLASH_WRITE_ZEROES(void)
{
    if(!check_V764_2_RF_FLASH_WRITE_ZEROES()) return;

    V764_2_RF_FLASH_WRITE_ZEROES_Q *question = reinterpret_cast<V764_2_RF_FLASH_WRITE_ZEROES_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_FLASH_WRITE_ZEROES_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_READ_POINT_FROM_MEMORY(void)
{
    //FIXME new cmd
    if(!check_V764_2_RF_READ_POINT_FROM_MEMORY()) return;

    V764_2_RF_READ_POINT_FROM_MEMORY_Q *question = reinterpret_cast<V764_2_RF_READ_POINT_FROM_MEMORY_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_READ_POINT_FROM_MEMORY_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);

    answer.point = 123.456f;

#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   mem_data: ???"));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_WRITE_POINT_TO_MEMORY(void)
{
    //FIXME new cmd
    if(!check_V764_2_RF_WRITE_POINT_TO_MEMORY()) return;

    V764_2_RF_WRITE_POINT_TO_MEMORY_Q *question = reinterpret_cast<V764_2_RF_WRITE_POINT_TO_MEMORY_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_WRITE_POINT_TO_MEMORY_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_READ_FIRMWARE_VERSION(void)
{
    //FIXME new cmd
    if(!check_V764_2_RF_READ_FIRMWARE_VERSION()) return;

    V764_2_RF_READ_FIRMWARE_VERSION_Q *question = reinterpret_cast<V764_2_RF_READ_FIRMWARE_VERSION_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_READ_FIRMWARE_VERSION_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   Major: %1").arg(answer.Major));
    print_log(QString("   Minor: %1").arg(answer.Minor));
    print_log(QString("   Release: %1").arg(answer.Release));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_SET_NEW_ADDRESS(void)
{
    //FIXME new cmd
    if(!check_V764_2_RF_SET_NEW_ADDRESS()) return;

    V764_2_RF_SET_NEW_ADDRESS_Q *question = reinterpret_cast<V764_2_RF_SET_NEW_ADDRESS_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_SET_NEW_ADDRESS_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_GET_RSSI(void)
{
    //FIXME new cmd
    if(!check_V764_2_RF_GET_RSSI()) return;

    V764_2_RF_GET_RSSI_Q *question = reinterpret_cast<V764_2_RF_GET_RSSI_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_GET_RSSI_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.rssi = 10 - rand() % 20;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES(void)
{
    //FIXME new cmd
    if(!check_V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES()) return;

    V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES_Q *question = reinterpret_cast<V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_GET_CNT_E21(void)
{
    //FIXME new cmd
    if(!check_V764_2_RF_GET_CNT_E21()) return;

    V764_2_RF_GET_CNT_E21_Q *question = reinterpret_cast<V764_2_RF_GET_CNT_E21_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_GET_CNT_E21_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_V764_2_RF_TEST(void)
{
    //FIXME new cmd
    if(!check_V764_2_RF_TEST()) return;

    V764_2_RF_TEST_Q *question = reinterpret_cast<V764_2_RF_TEST_Q *>(ba_input.data());
    Q_ASSERT(question);
    V764_2_RF_TEST_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.SerialNo    = SerialNo;
    answer.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer) - sizeof(B7_86_HEADER) - sizeof(answer.crc16);
#ifdef USE_HTONS
    answer.crc16 = htons(CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                               sizeof(answer) - sizeof(answer.crc16)));
#else
    answer.crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(&answer),
                                         sizeof(answer) - sizeof(answer.crc16));
#endif

    print_log("Подготавливаем ответ");
    print_log(QString("   address: %1").arg(answer.header.address));
    print_log(QString("   cmd: %1").arg(answer.header.cmd));
    print_log(QString("   SerialNo: %1").arg(answer.SerialNo));
    print_log(QString("   SerialYear: %1").arg(answer.SerialYear));
    print_log(QString("   cnt: %1").arg(answer.header.count_data));
    print_log(QString("   crc16: %1").arg(answer.crc16, 2, 16, QChar('0')));

    ba_output.clear();
    ba_output.append((char *)&answer,
                     sizeof(answer));
    delay();
    emit output(ba_output);

    emit s_readyRead();
}
//--------------------------------------------------------------------------------
QByteArray SerialPort_V786_2_test::convert_ascii_to_data(QString input, bool *ok)
{
    int begin = input.indexOf(':');
    if(begin < 0)
    {
        *ok = false;
        return input.toLatin1();
    }

    int end = input.indexOf('\r');
    if(end < 0)
    {
        *ok = false;
        return input.toLatin1();
    }

    if(end < begin)
    {
        *ok = false;
        return input.toLatin1();
    }

    QByteArray temp_ba = QByteArray::fromHex(input.toLatin1());

    *ok = true;
    return temp_ba;
}
//--------------------------------------------------------------------------------
QByteArray SerialPort_V786_2_test::convert_data_to_ascii(QByteArray data)
{
    QByteArray temp;

    temp.clear();
    temp.append(':');
    temp.append(data.toHex());
    temp.append((char)0x0D);

#ifdef QT_DEBUG
    qDebug() << "convert_data_to_ascii" << temp;
#endif
    return temp;
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::receive(const QVariant &data)
{
    bool ok = false;
    float temp = data.toFloat(&ok);
    if(ok)
    {
        measuring_value = temp; //TODO нужно уточнить
#ifdef QT_DEBUG
        qDebug() << "measuring_value" << measuring_value;
#endif
    }
}
//--------------------------------------------------------------------------------
uint8_t SerialPort_V786_2_test::calc_limit(void)
{
    return limits;
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::set_answer_delay(int delay_ms)
{
    answer_delay = delay_ms;
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::delay(void)
{
    if(answer_delay > 0)
    {
        wait(answer_delay);
    }
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::wait(int timeout_msec)
{
#ifdef TEST
    timeout_msec = TEST_WAIT_MS;
#endif

    QElapsedTimer time;

    if(timeout_msec < 1)
        return;

    time.start();
    while(time.elapsed() < timeout_msec)
    {
        QCoreApplication::processEvents();
    }
}
//--------------------------------------------------------------------------------
