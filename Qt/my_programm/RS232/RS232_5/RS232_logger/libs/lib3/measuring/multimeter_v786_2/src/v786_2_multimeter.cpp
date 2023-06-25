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
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#include "v786_2_serialport_test.hpp"

#include "v786_2_multimeter.hpp"
#include "v786_2_packets.hpp"
#include "v786_2_union.hpp"
#include "breloque.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "crc.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
Multimeter_V786_2::Multimeter_V786_2(QWidget *parent) :
    MyWidget(parent),
    is_silence(false)
{
    init();
}
//--------------------------------------------------------------------------------
Multimeter_V786_2::~Multimeter_V786_2()
{
#ifdef QT_DEBUG
    qDebug() << "~Multimeter_V786_2()";
#endif
    if(serial)
    {
        if(serial->isOpen())
        {
            serial->close();
        }
        delete serial;
    }
    MySettings *settings = new MySettings();
    if(sb_serno)
    {
        settings->save_int(P_SERNO, sb_serno->value());
        delete sb_serno;
    }
    if(sb_year)
    {
        settings->save_int(P_YEAR, sb_year->value());
        delete sb_year;
    }
    if(sb_address)
    {
        settings->save_int(P_ADDRESS, sb_address->value());
        delete sb_address;
    }
    if(sb_channel)
    {
        settings->save_int(P_CHANNEL, sb_channel->value());
        delete sb_channel;
    }
    delete settings;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::init(void)
{
    //emit trace(Q_FUNC_INFO);

    last_error = E_NO_ERROR;

    multimeter_address = 0;
    multimeter_serno   = 0;
    multimeter_year    = 0;
    multimeter_channel = 0;

#ifdef TEST
    serial = new SerialPort_V786_2_test();

    connect(serial, &SerialPort_V786_2_test::info,  this,   &Multimeter_V786_2::info);
    connect(serial, &SerialPort_V786_2_test::debug, this,   &Multimeter_V786_2::debug);
    connect(serial, &SerialPort_V786_2_test::error, this,   &Multimeter_V786_2::error);
    connect(serial, &SerialPort_V786_2_test::trace, this,   &Multimeter_V786_2::trace);

    connect(serial, &SerialPort_V786_2_test::output,    this,   &Multimeter_V786_2::read_data);
#else
    serial = new SerialBox5_wo_form();
    connect(serial, &SerialBox5_wo_form::output,
            this,   &Multimeter_V786_2::read_data);
#endif
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::read_data(QByteArray data)
{
    emit debug(QString("read_data [%1] [%2]")
               .arg(data.data())
               .arg(data.toHex().data()));
    data_radiomodule.append(data);
    is_ready = true;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::port_error(QSerialPort::SerialPortError serial_error)
{
    switch(serial_error)
    {
    case QSerialPort::NoError:  break;
    case QSerialPort::DeviceNotFoundError:  emit error("Error: DeviceNotFoundError"); break;
    case QSerialPort::PermissionError:      emit error("Error: PermissionError"); break;
    case QSerialPort::OpenError:            emit error("Error: OpenError"); break;
#ifdef Q_OS_LINUX
    case QSerialPort::ParityError:          emit error("Error: ParityError"); break;
    case QSerialPort::FramingError:         emit error("Error: FramingError"); break;
    case QSerialPort::BreakConditionError:  emit error("Error: BreakConditionError"); break;
#endif
    case QSerialPort::WriteError:           emit error("Error: WriteError"); break;
    case QSerialPort::ReadError:            emit error("Error: ReadError"); break;
    case QSerialPort::ResourceError:        emit error("Error: ResourceError"); break;
    case QSerialPort::UnsupportedOperationError: emit error("Error: UnsupportedOperationError"); break;
    case QSerialPort::UnknownError:         emit error("Error: UnknownError"); break;
    case QSerialPort::TimeoutError:         emit error("Error: TimeoutError"); break;
    case QSerialPort::NotOpenError:         emit error("Error: NotOpenError"); break;

    default:
        emit error(QString("Multimeter_V786_2::port_error Unknown error %1")
                   .arg(serial_error));
        break;
    }
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::close_device(void)
{
    if(serial->isOpen())
    {
        emit info("Порт мультиметра закрыт");
        serial->serial_close();
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::find_device(void)
{
    Q_ASSERT(multimeter_timeout >= 1000);

    emit trace(Q_FUNC_INFO);

    int speed = 57600;
    if(serial->isOpen())
    {
        serial->serial_close();
    }

    // используем последний порт
    QString last_portname = load_string(P_MULTIMETER_PORTNAME);
    if(!last_portname.isEmpty())
    {
        serial->setPortName(last_portname);
        bool ok = serial->setBaudRate(speed);
        if(ok)
        {
            if(serial->serial_open())
            {
                bool ok = check_breloque();
                if(ok)
                {
                    emit info(QString("USB приемо-передатчик В7-86/2 найден на порту %1")
                              .arg(last_portname));
                    return true;
                }
                serial->close();
            }
        }
    }
    //---

    QStringList sl_ports = serial->get_port_names();
    foreach (QString p_name, sl_ports)
    {
        emit info(QString("Порт %1").arg(p_name));
        serial->setPortName(p_name);
        bool ok = serial->setBaudRate(speed);
        if(ok)
        {
            if(serial->serial_open())
            {
                bool ok = check_breloque();
                if(ok)
                {
                    emit info(QString("USB приемо-передатчик В7-86/2 найден на порту %1")
                              .arg(p_name));
                    save_string(P_MULTIMETER_PORTNAME, p_name);
                    return true;
                }
                serial->serial_close();
            }
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::open_device(const QString &port_name)
{
    Q_ASSERT(multimeter_timeout >= 1000);

    emit trace(Q_FUNC_INFO);

    int speed = 57600;
    if(serial->isOpen())
    {
        serial->serial_close();
    }
    serial->setPortName(port_name);
    bool ok = serial->setBaudRate(speed);
    if(ok)
    {
        if(serial->serial_open())
        {
            bool ok2 = check_breloque();
            if(ok2)
            {
                emit info(QString("USB приемо-передатчик В7-86/2 найден на порту %1")
                          .arg(port_name));
                return true;
            }
            serial->serial_close();
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::check_set_new_addres(void)
{
    //TODO проверяем возможность установки нового адреса
    bool ok;

    // проверяем версию брелка
    ok = check_version();
    if(!ok)
    {
        return;
    }

    // пробуем установить новый адрес брелку
    RF_set_param param;
    param.magic = 0xAABB;
    param.address = multimeter_address + MAGIC_NUMBER;
    param.channel = multimeter_channel;

    emit info("set_new_param");
    emit info(QString("magic: 0x%1").arg(param.magic, 2, 16, QChar('0')));
    emit info(QString("address: 0x%1").arg(param.address, 2, 16, QChar('0')));
    emit info(QString("channel: %1").arg(param.channel));
    ok = set_new_param(param);
    if(!ok)
    {
        emit error("Не удалось установить новый адрес USB приёмо-передатчику");
        return;
    }

    wait(3000);

    // пробуем сообщить свой новый адрес мультиметру
    print_cmd_text(V764_2_RF_SET_NEW_ADDRESS);
    emit info(QString("Новый адрес: 0x%1").arg(param.address, 2, 16, QChar('0')));
    set_data_len(2);
    ok = set_new_address(param.address);
    if(!ok)
    {
        print_last_error();
    }
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::check_breloque(void)
{
    Q_ASSERT(multimeter_timeout >= 1000);
    emit trace(Q_FUNC_INFO);

    if(serial->isOpen() == false)
    {
        emit error("Порт USB приемо-передатчика не открыт!");
        return false;
    }

    emit info("Тест USB приемо-передатчика");

    QByteArray temp;
    temp.clear();

    temp.append(":");
    temp.append(BRELOGUE_FIND);
    temp.append(static_cast<char>(0x0D));

    data_radiomodule.clear();
    is_ready = false;

    serial->write(temp);

    QElapsedTimer time;
    time.start();
#ifndef FAST_COMMAND
    wait(multimeter_timeout);
#else
    wait(sizeof(breloque_answer), 100, multimeter_timeout);
#endif
    emit info(QString("Прошло времени %1 ms").arg(time.elapsed()));

    if(data_radiomodule.isEmpty())
    {
        emit error("USB приемо-передатчик не отвечает!");
        return false;
    }

    if(data_radiomodule.data() != breloque_answer)
    {
        emit error("Это не USB приемо-передатчик!");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::check_version(void)
{
    emit trace(Q_FUNC_INFO);

    if(serial->isOpen() == false)
    {
        emit error("Порт USB приемо-передатчика не открыт!");
        return false;
    }

    emit info("Получение версии USB приемо-передатчика");

    QByteArray temp;
    temp.clear();

    temp.append(":");
    temp.append(BRELOGUE_VER);
    temp.append(static_cast<char>(0x0D));

    data_radiomodule.clear();
    is_ready = false;

    serial->write(temp);

    QElapsedTimer time;
    time.start();
#ifndef FAST_COMMAND
    wait(multimeter_timeout);
#else
    wait(2, 100, multimeter_timeout);
#endif
    emit info(QString("Прошло времени %1 ms").arg(time.elapsed()));

    if(data_radiomodule.isEmpty())
    {
        emit error("USB приемо-передатчик не отвечает!");
        return false;
    }

    bool ok;
    int version = data_radiomodule.toInt(&ok);
    if(!ok)
    {
        emit error("Это не USB приемо-передатчик!");
        return false;
    }
    if(version < 30 || version > 100)
    {
        emit error(QString("bad version: %1`")
                   .arg(version));
        return false;
    }

    emit info(QString("Version: %1")
              .arg(version));

    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::get_version(int *version)
{
    emit trace(Q_FUNC_INFO);

    if(serial->isOpen() == false)
    {
        emit error("Порт USB приемо-передатчика не открыт!");
        return false;
    }

    emit info("Получение версии USB приемо-передатчика");

    QByteArray temp;
    temp.clear();

    temp.append(":");
    temp.append(BRELOGUE_VER);
    temp.append(static_cast<char>(0x0D));

    data_radiomodule.clear();
    is_ready = false;

    serial->write(temp);

    QElapsedTimer time;
    time.start();
#ifndef FAST_COMMAND
    wait(multimeter_timeout);
#else
    wait(2, 100, multimeter_timeout);
#endif
    emit info(QString("Прошло времени %1 ms").arg(time.elapsed()));

    if(data_radiomodule.isEmpty())
    {
        emit error("USB приемо-передатчик не отвечает!");
        return false;
    }

    bool ok;
    int t_version = data_radiomodule.toInt(&ok);
    if(ok)
    {
        *version = t_version;
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::set_breloque_address(uint16_t address)
{
    emit trace(Q_FUNC_INFO);

    if(serial->isOpen() == false)
    {
        emit error("Порт USB приемо-передатчика не открыт!");
        return false;
    }

    emit info("Установка адреса USB приемо-передатчика");

    RF_set_param_2 param;
    param.magic = 0xAABB;
    param.command = 0;  // set
    param.address = address;

    emit debug("set_address_transiever");

    QByteArray ba_res;
    if(!set_new_param(param, &ba_res))
    {
        print_last_error();
        return false;
    }

    if(ba_res.length() != sizeof (RF_set_param_2))
    {
        emit error("Bad len answer");
        return false;
    }

    emit debug(QString("ba_res: %1").arg(ba_res.toHex().toUpper().data()));
    RF_set_param_2 *packet = reinterpret_cast<RF_set_param_2 *>(ba_res.data());

    emit debug(QString("magic: 0x%1").arg(packet->magic, 2, 16, QChar('0')));
    emit debug(QString("command: %1").arg(packet->command));
    emit debug(QString("address: %1").arg(packet->address));

    emit info(QString("new address: %1")
              .arg(packet->address));

    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::get_breloque_address(int *address)
{
    emit trace(Q_FUNC_INFO);

    if(serial->isOpen() == false)
    {
        emit error("Порт USB приемо-передатчика не открыт!");
        return false;
    }

    emit info("Получение адреса USB приемо-передатчика");

    set_data_len(0);

    RF_set_param_2 param;
    param.magic = 0xAABB;
    param.command = 1;  // get
    param.address = 0;

    emit debug("get_address_transiever");

    QByteArray ba_res;
    if(!set_new_param(param, &ba_res))
    {
        print_last_error();
        return false;
    }

    if(ba_res.length() != sizeof(RF_set_param_2))
    {
        emit error("Bad len answer");
        emit error(QString("ba_res.length() = %1").arg(ba_res.length()));
        emit error(QString("sizeof(RF_set_param_2) = %1").arg(sizeof(RF_set_param_2)));
        return false;
    }
    RF_set_param_2 *packet = reinterpret_cast<RF_set_param_2 *>(ba_res.data());
    *address = packet->address;
    return true;
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::check_packet_answer(uint8_t cmd,
                                           int packet_len)
{
    emit trace(Q_FUNC_INFO);

    if(data_radiomodule.isEmpty())
    {
        last_error = E_ERROR_NO_ANSWER;
        return last_error;
    }

    if(data_radiomodule.length() < 2)
    {
        emit debug(data_radiomodule.toHex());
        last_error = E_ERROR_SMALL_ANSWER;
        return last_error;
    }
    if(data_radiomodule.at(1) != cmd)
    {
        emit debug(QString("HEX: %1").arg(data_radiomodule.toHex().toUpper().data()));
        //emit debug(QString("TEXT: %1").arg(data_radiomodule.data());
        uint8_t bad_cmd = data_radiomodule.at(1);
        uint8_t good_cmd = bad_cmd & ~0x80;
        emit debug(QString("Bad cmd: 0x%1 [0x%2]")
                   .arg(bad_cmd,  2, 16, QChar('0'))
                   .arg(good_cmd, 2, 16, QChar('0')));
        last_error = E_ERROR_BAD_CMD;
        return last_error;
    }

    if(data_radiomodule.length() > packet_len)
    {
        emit error(data_radiomodule.toHex());
        emit error(QString("check_packet_answer: Длина пакета (пришло) %1 > (ожидалось) %2")
                   .arg(data_radiomodule.length())
                   .arg(packet_len));
        last_error = E_ERROR_BIG_ANSWER;
        return last_error;
    }
    if(data_radiomodule.length() < packet_len)
    {
        emit error(data_radiomodule.toHex());
        emit error(QString("check_packet_answer: Длина пакета (пришло) %1 < (ожидалось) %2")
                   .arg(data_radiomodule.length())
                   .arg(packet_len));
        last_error = E_ERROR_SMALL_ANSWER;
        return last_error;
    }

    B7_86_HEADER *header = (B7_86_HEADER *)data_radiomodule.data();
    int len = data_radiomodule.size() - sizeof(B7_86_HEADER) - 2;  //3 байта заголовок + 2 байта контрольной суммы
    if(header->count_data != len)
    {
        emit error(data_radiomodule.toHex());
        emit error(QString("data_radiomodule.size() = %1")
                   .arg(data_radiomodule.size()));
        emit error(QString("check_packet_answer: size (пришло) %1 < (ожидалось) %2")
                   .arg(header->count_data)
                   .arg(len));
        last_error = E_ERROR_SIZE_NOT_CORRECT;
        return last_error;
    }

    uint16_t calc_crc16 = CRC::multimeter_crc16((uint8_t *)data_radiomodule.data(),
                                                packet_len - 2); //TODO отнимаем контрольную сумму

    union U_SHORT temp_crc;
    temp_crc.bytes.lo = data_radiomodule.at(packet_len - 1);
    temp_crc.bytes.hi = data_radiomodule.at(packet_len - 2);
    uint16_t data_crc16 = temp_crc.value;

    emit debug(data_radiomodule.toHex().data());
    if(data_crc16 != calc_crc16)
    {
        emit error(data_radiomodule.toHex());
        emit error(QString("Ошибка контрольной суммы data_crc16 [%1] calc_crc16 [%2]")
                   .arg(data_crc16, 0, 16)
                   .arg(calc_crc16, 0, 16));
        last_error = E_ERROR_BAD_CRC;
        return last_error;
    }
    last_error = E_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::get_deviation(bool *ok)
{
    emit trace(Q_FUNC_INFO);

    int err = check_packet_answer(V764_2_RF_DC_VOLTAGE_MEASUREMENT,
                                  sizeof(V764_2_RF_DC_VOLTAGE_MEASUREMENT_A));
    if(err != E_NO_ERROR)
    {
        *ok = false;
        last_error = err;
        return last_error;
    }

    V764_2_RF_DC_VOLTAGE_MEASUREMENT_A *b786_measuring = (V764_2_RF_DC_VOLTAGE_MEASUREMENT_A *)data_radiomodule.constData();
    BYTE_DEVIATION_RETENTION byte_options;
    byte_options.value = b786_measuring->deviation_retention;
    int deviation = byte_options.data.deviation;

    *ok = true;
    return deviation;
}
//--------------------------------------------------------------------------------
uint8_t Multimeter_V786_2::get_deviation_retention(bool *ok)
{
    emit trace(Q_FUNC_INFO);

    int err = check_packet_answer(V764_2_RF_DC_VOLTAGE_MEASUREMENT,
                                  sizeof(V764_2_RF_DC_VOLTAGE_MEASUREMENT_A));
    if(err != E_NO_ERROR)
    {
        *ok = false;
        last_error = err;
        return last_error;
    }

    V764_2_RF_DC_VOLTAGE_MEASUREMENT_A *b786_measuring = (V764_2_RF_DC_VOLTAGE_MEASUREMENT_A *)data_radiomodule.constData();

    *ok = true;
    return b786_measuring->deviation_retention;
}
//--------------------------------------------------------------------------------
uint8_t Multimeter_V786_2::get_limit(uint8_t cmd, bool *ok)
{
    emit trace(Q_FUNC_INFO);

    int err = check_packet_answer(cmd,
                                  sizeof(V764_2_MEASURING_A));
    if(err != E_NO_ERROR)
    {
        *ok = false;
        last_error = err;
        return last_error;
    }

    V764_2_MEASURING_A *b786_measuring = (V764_2_MEASURING_A *)data_radiomodule.constData();
    BYTE_LIMITS byte_limits;
    //qDebug() << "b786_measuring->limits" << b786_measuring->limits;
    byte_limits.value = b786_measuring->limits;
    uint8_t limit = byte_limits.data.limit;

    *ok = true;
    return limit;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::get_value(int cmd, double *value)
{
    emit trace(Q_FUNC_INFO);

    int err = check_packet_answer(cmd,
                                  sizeof(V764_2_MEASURING_A));
    if(err != E_NO_ERROR)
    {
        return false;
    }

    bool temp_ok = false;
    V764_2_MEASURING_A *b786_measuring = (V764_2_MEASURING_A *)data_radiomodule.constData();
    Q_ASSERT(b786_measuring);

    QString temp;
    temp.clear();
    for(unsigned int n=0; n<sizeof(b786_measuring->DisplaySymbols); n++)
        temp.append(b786_measuring->DisplaySymbols[n]);
    float x = temp.toFloat(&temp_ok);
    if(temp_ok == false)
    {
        emit debug(QString("Что это? [%1]")
                   .arg(temp.toLocal8Bit().toHex().data()));
        return false;
    }

    *value = x;
    return true;
}
//--------------------------------------------------------------------------------
QString Multimeter_V786_2::get_string_value(bool *ok)
{
    emit trace(Q_FUNC_INFO);

    int err = check_packet_answer(V764_2_RF_DC_VOLTAGE_MEASUREMENT,
                                  sizeof(V764_2_RF_DC_VOLTAGE_MEASUREMENT_A));
    if(err != E_NO_ERROR)
    {
        *ok = false;
        last_error = err;
        return "";
    }

    V764_2_RF_DC_VOLTAGE_MEASUREMENT_A *b786_measuring = (V764_2_RF_DC_VOLTAGE_MEASUREMENT_A *)data_radiomodule.constData();

    QString temp;
    temp.clear();
    for(unsigned int n=0; n<sizeof(b786_measuring->DisplaySymbols); n++)
        temp.append(b786_measuring->DisplaySymbols[n]);

    *ok = true;
    return temp;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::service_on(void)
{
    Q_ASSERT(multimeter_channel);
    Q_ASSERT(multimeter_address);
    Q_ASSERT(multimeter_data_len == 6);

    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_SERVICE_ENABLE_TEXT);

    QByteArray packet = prepare_V764_2_RF_SERVICE_ENABLE();

    data_radiomodule.clear();
    is_ready = false;
#ifdef CONVERT_DATA_TO_ASCII
    serial->write(convert_data_to_ascii(packet));
    emit debug(QString("%1").arg(convert_data_to_ascii(packet).data()));
#else
    serial->write(packet);
    emit debug(QString("packet: %1").arg(packet.toHex().data()));
#endif

    QElapsedTimer time;
    time.start();
#ifndef FAST_COMMAND
    wait(multimeter_timeout);
#else
    wait(sizeof(V764_2_RF_SERVICE_ENABLE_A), 100, multimeter_timeout);
#endif
    emit info(QString("Прошло времени %1 ms").arg(time.elapsed()));

    if(data_radiomodule.isEmpty())
    {
        last_error = E_ERROR_NO_ANSWER;
        return false;
    }

    int err = check_packet_answer(V764_2_RF_SERVICE_ENABLE, sizeof(V764_2_RF_SERVICE_ENABLE_A));
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    // анализируем данные
    V764_2_RF_SERVICE_ENABLE_A *output_packet = (V764_2_RF_SERVICE_ENABLE_A *)packet.data();
    V764_2_RF_SERVICE_ENABLE_A *input_packet  = (V764_2_RF_SERVICE_ENABLE_A *)data_radiomodule.constData();

    uint8_t cmd = input_packet->header.cmd;

    if(output_packet->header.address != input_packet->header.address)
    {
        emit debug(QString("receive %1").arg(data_radiomodule.toHex().data()));
        emit error(QString("Неверный адрес (пришло) %1 != (должно было прийти) %2")
                   .arg(input_packet->header.address)
                   .arg(output_packet->header.address));
        last_error = E_ERROR_BAD_ADDRESS;
        return false;
    }

    if((input_packet->SerialNo != multimeter_serno) ||
            (input_packet->SerialYear != multimeter_year))
    {
        QString text = QString("На канале \"%1\" по адресу \"%2\" обнаружен другой мультиметр:\n")
                .arg(multimeter_channel)
                .arg(multimeter_address);
        text.append(QString("-заводской номер %1\n")
                    .arg(input_packet->SerialNo));
        text.append(QString("-год выпуска %1\n")
                    .arg(input_packet->SerialYear));
        text.append("Использовать новые данные о приборе?");

        QMessageBox *mb = new QMessageBox();
        mb->setWindowTitle("Внимание!");
        mb->setText(text);
        mb->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        mb->setDefaultButton(QMessageBox::Yes);
        int res = mb->exec();
        if(res == QMessageBox::Yes)
        {
            //FIXME вернуть значение в основное окно
            multimeter_serno = input_packet->SerialNo;
            multimeter_year  = input_packet->SerialYear;

            emit update_serno(multimeter_serno);
            emit update_year(multimeter_year);
        }
    }

    union U_BYTE acmd;
    acmd.value = cmd;
    if(acmd.bites.bit7 == 1)
    {
        // ошибка //???
        return last_error;
    }

    if(cmd == V764_2_RF_STATUS_REQUEST)
    {
        V764_2_RF_STATUS_REQUEST_A *b786_state;
        b786_state = (V764_2_RF_STATUS_REQUEST_A *)data_radiomodule.constData();

        emit info("Статус прибора:");
        emit info(QString("    адрес %1").arg(b786_state->header.address));
        emit info(QString("    серийный номер %1").arg(b786_state->SerialNo));
        emit info(QString("    год изготовления %1").arg(b786_state->SerialYear));
        QString temp;
        temp.clear();
        for(unsigned int n=0; n<sizeof(b786_state->DisplaySymbols); n++)
            temp.append(b786_state->DisplaySymbols[n]);
        emit info(QString("    отображаемое значение %1").arg(temp));

        emit info(QString("    предел %1").arg(b786_state->Range));
        emit info(QString("    режим %1").arg(b786_state->Mode));

        U_BYTE_DECADES temp_decades;
        temp_decades.value = b786_state->Math_Hold;
        emit info(QString("    math %1").arg(temp_decades.decades.lo));
        emit info(QString("    hold %1").arg(temp_decades.decades.hi));

        emit info(QString("    напряжение батареи %1").arg((qreal)b786_state->BattVoltage / 1000.0));

        emit update_battery(b786_state->BattVoltage);
    }

    last_error = E_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::convert_data_to_ascii(QByteArray data)
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
int Multimeter_V786_2::send_raw_data(const QByteArray &data)
{
    emit trace(Q_FUNC_INFO);
    Q_ASSERT(multimeter_timeout);

    if(serial->isOpen() == false)
    {
        last_error = E_ERROR_PORT_NOT_OPEN;
        return last_error;
    }

    emit debug(data.toHex().data());
    serial->write(data);
    last_error = E_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::send(int command,
                            QByteArray data,
                            unsigned int len_data,
                            unsigned int serialno,
                            unsigned int year)
{
    emit trace(Q_FUNC_INFO);

    Q_ASSERT(multimeter_channel);
    Q_ASSERT(multimeter_address);
    Q_ASSERT(multimeter_timeout > 100);

    //---
    QByteArray ba;
    ba.append((const char *)data, len_data);
    emit debug(QString("multimeter_address %1")
               .arg(multimeter_address));
    emit debug(QString("%1 %2")
               .arg(V764_2_RF_STATUS_REQUEST_TEXT)
               .arg(ba.toHex().data()));
    //---

    if(serial->isOpen() == false)
    {
        last_error = E_ERROR_PORT_NOT_OPEN;
        return last_error;
    }

    if(len_data < sizeof(B7_86_HEADER))
    {
        emit error(QString("Длина отправляемого пакета слишком мала %1 < %2")
                   .arg(len_data)
                   .arg(sizeof(B7_86_HEADER)));
        last_error = E_ERROR_SMALL_ANSWER;
        return last_error;
    }

    emit debug("send");

#ifdef QT_DEBUG
    qDebug() << "send" << data.toHex();
#endif

    data_radiomodule.clear();
    is_ready = false;

#ifdef CONVERT_DATA_TO_ASCII
    serial->write(convert_data_to_ascii(data));
    emit debug(QString("%1").arg(convert_data_to_ascii(data).data()));
#else
    serial->write(data);
#endif

    QElapsedTimer time;
    time.start();
#ifndef FAST_COMMAND
    wait(multimeter_timeout);
#else
    wait(get_size_answer(command), 100, multimeter_timeout);
#endif
    if(!is_silence)
    {
        emit info(QString("Прошло времени %1 ms").arg(time.elapsed()));
    }

    if(data_radiomodule.isEmpty())
    {
        last_error = E_ERROR_NO_ANSWER;
        return last_error;
    }

#ifdef QT_DEBUG
    qDebug() << "get" << data_radiomodule.toHex();
#endif
    switch(command)
    {
    case V764_2_RF_STATUS_REQUEST:              check_packet_answer(command, sizeof(V764_2_RF_STATUS_REQUEST_A));           break;
    case V764_2_RF_REMOTE_CONTROL_ENABLE:       check_packet_answer(command, sizeof(V764_2_RF_REMOTE_CONTROL_ENABLE_A));    break;
    case V764_2_RF_REMOTE_CONTROL_DISABLE:      check_packet_answer(command, sizeof(V764_2_RF_REMOTE_CONTROL_DISABLE_A));   break;
    case V764_2_RF_SERVICE_ENABLE:              check_packet_answer(command, sizeof(V764_2_RF_SERVICE_ENABLE_A));           break;
    case V764_2_RF_SERVICE_DISABLE:             check_packet_answer(command, sizeof(V764_2_RF_SERVICE_DISABLE_A));          break;
    case V764_2_RF_POWER_OFF:                   check_packet_answer(command, sizeof(V764_2_RF_POWER_OFF_A));                break;
    case V764_2_RF_DC_VOLTAGE_MEASUREMENT:      check_packet_answer(command, sizeof(V764_2_RF_DC_VOLTAGE_MEASUREMENT_A));   break;
    case V764_2_RF_AC_VOLTAGE_MEASUREMENT:      check_packet_answer(command, sizeof(V764_2_RF_AC_VOLTAGE_MEASUREMENT_A));   break;
    case V764_2_RF_DC_CURRENT_MEASUREMENT:      check_packet_answer(command, sizeof(V764_2_RF_DC_CURRENT_MEASUREMENT_A));   break;
    case V764_2_RF_AC_CURRENT_MEASUREMENT:      check_packet_answer(command, sizeof(V764_2_RF_AC_CURRENT_MEASUREMENT_A));   break;
    case V764_2_RF_RESISTANCE_MEASUREMENT:      check_packet_answer(command, sizeof(V764_2_RF_RESISTANCE_MEASUREMENT_A));   break;
    case V764_2_RF_CAPACITANCE_MEASUREMENT:     check_packet_answer(command, sizeof(V764_2_RF_CAPACITANCE_MEASUREMENT_A));  break;
    case V764_2_RF_INDUCTANCE_MEASUREMENT:      check_packet_answer(command, sizeof(V764_2_RF_INDUCTANCE_MEASUREMENT_A));   break;
    case V764_2_RF_BUZZER_TEST:                 check_packet_answer(command, sizeof(V764_2_RF_BUZZER_TEST_A));              break;
    case V764_2_RF_FREQUENCY_MEASUREMENT:       check_packet_answer(command, sizeof(V764_2_RF_FREQUENCY_MEASUREMENT_A));    break;
    case V764_2_RF_ACTIVE_POWER_MEASUREMENT:    check_packet_answer(command, sizeof(V764_2_RF_ACTIVE_POWER_MEASUREMENT_A)); break;
    case V764_2_RF_REACTIVE_POWER_MEASUREMENT:  check_packet_answer(command, sizeof(V764_2_RF_REACTIVE_POWER_MEASUREMENT_A)); break;
    case V764_2_RF_FULL_POWER_MEASUREMENT:      check_packet_answer(command, sizeof(V764_2_RF_FULL_POWER_MEASUREMENT_A));   break;
    case V764_2_RF_COS_FI_MEASUREMENT:          check_packet_answer(command, sizeof(V764_2_RF_COS_FI_MEASUREMENT_A));       break;
    case V764_2_RF_UDC_CAL:                     check_packet_answer(command, sizeof(V764_2_RF_UDC_CAL_A));                  break;
    case V764_2_RF_UAC_CAL:                     check_packet_answer(command, sizeof(V764_2_RF_UAC_CAL_A));                  break;
    case V764_2_RF_IDC_CAL:                     check_packet_answer(command, sizeof(V764_2_RF_IDC_CAL_A));                  break;
    case V764_2_RF_IAC_CAL:                     check_packet_answer(command, sizeof(V764_2_RF_IAC_CAL_A));                  break;
    case V764_2_RF_RES_CAL:                     check_packet_answer(command, sizeof(V764_2_RF_RES_CAL_A));                  break;
    case V764_2_RF_FLASH_WRITE:                 check_packet_answer(command, sizeof(V764_2_RF_FLASH_WRITE_A));              break;
    case V764_2_RF_DIODE_TEST:                  check_packet_answer(command, sizeof(V764_2_RF_DIODE_TEST_A));               break;
    case V764_2_RF_UDC_CAL_ZEROES:              check_packet_answer(command, sizeof(V764_2_RF_UDC_CAL_ZEROES_A));           break;
    case V764_2_RF_IDC_CAL_ZEROES:              check_packet_answer(command, sizeof(V764_2_RF_IDC_CAL_ZEROES_A));           break;
    case V764_2_RF_RES_CAL_ZEROES:              check_packet_answer(command, sizeof(V764_2_RF_RES_CAL_ZEROES_A));           break;
    case V764_2_RF_CAP_CAL_ZEROES:              check_packet_answer(command, sizeof(V764_2_RF_CAP_CAL_ZEROES_A));           break;
    case V764_2_RF_IND_CAL_ZEROES:              check_packet_answer(command, sizeof(V764_2_RF_IND_CAL_ZEROES_A));           break;
    case V764_2_RF_FLASH_WRITE_ZEROES:          check_packet_answer(command, sizeof(V764_2_RF_FLASH_WRITE_ZEROES_A));       break;

        //TODO added 28.07.22
    case V764_2_RF_READ_POINT_FROM_MEMORY:      check_packet_answer(command, sizeof(V764_2_RF_READ_POINT_FROM_MEMORY_A));   break;
    case V764_2_RF_WRITE_POINT_TO_MEMORY:       check_packet_answer(command, sizeof(V764_2_RF_WRITE_POINT_TO_MEMORY_A));    break;
    case V764_2_RF_READ_FIRMWARE_VERSION:       check_packet_answer(command, sizeof(V764_2_RF_READ_FIRMWARE_VERSION_A));    break;
    case V764_2_RF_SET_NEW_ADDRESS:             check_packet_answer(command, sizeof(V764_2_RF_SET_NEW_ADDRESS_A));          break;
    case V764_2_RF_GET_RSSI:                    check_packet_answer(command, sizeof(V764_2_RF_GET_RSSI_A));                 break;

        //TODO added 27.09.22
    case V764_2_RF_TEST:
        check_packet_answer(command, sizeof(V764_2_RF_TEST_A));
        break;

    case V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES:
        check_packet_answer(command, sizeof(V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES_A));
        break;

    case V764_2_RF_GET_CNT_E21:
        check_packet_answer(command, sizeof(V764_2_RF_GET_CNT_E21_A));
        break;

    default:
        emit error(QString("unknown cmd (2) 0x%1").arg(command, 0, 16));
        last_error = E_ERROR_UNKNOWN_CMD;
        return last_error;
    }

    if(last_error == E_ERROR_BAD_CMD)
    {
        V764_2_ERROR_ANSWER *error_answer = (V764_2_ERROR_ANSWER *)data_radiomodule.constData();
        last_error = error_answer->error;
    }
    if(last_error != E_NO_ERROR)
    {
        return last_error;
    }

    // анализируем данные
    V764_2_HEADER_ANSWER *output_packet = (V764_2_HEADER_ANSWER *)data.constData();
    V764_2_HEADER_ANSWER *input_packet  = (V764_2_HEADER_ANSWER *)data_radiomodule.constData();

    emit debug(QString("multimeter send %1").arg(data_radiomodule.toHex().data()));
    if(output_packet->header.address != input_packet->header.address)
    {
        emit debug(QString("receive %1").arg(data_radiomodule.toHex().data()));
        emit error(QString("Неверный адрес (пришло) %1 != (должно было прийти) %2")
                   .arg(input_packet->header.address)
                   .arg(output_packet->header.address));
        last_error = E_ERROR_BAD_ADDRESS;
        return last_error;
    }

    if((input_packet->SerialNo != serialno) || (input_packet->SerialYear != year))
    {
        QString text = QString("На канале \"%1\" по адресу \"%2\" обнаружен другой мультиметр:\n")
                .arg(multimeter_channel)
                .arg(multimeter_address);
        text.append(QString("-заводской номер %1\n")
                    .arg(input_packet->SerialNo));
        text.append(QString("-год выпуска %1\n")
                    .arg(input_packet->SerialYear));
        text.append("Использовать новые данные о приборе?");

        QMessageBox *mb = new QMessageBox();
        mb->setWindowTitle("Внимание!");
        mb->setText(text);
        mb->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        mb->setDefaultButton(QMessageBox::Yes);
        int res = mb->exec();
        if(res == QMessageBox::Yes)
        {
            //FIXME вернуть значение в основное окно
            multimeter_serno = input_packet->SerialNo;
            multimeter_year  = input_packet->SerialYear;

            emit update_serno(multimeter_serno);
            emit update_year(multimeter_year);
        }
    }

    last_error = E_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::get_size_answer(int command)
{
    emit trace(Q_FUNC_INFO);

    int res = 0;
    switch(command)
    {
    case V764_2_RF_STATUS_REQUEST:              res = sizeof(V764_2_RF_STATUS_REQUEST_A); break;
    case V764_2_RF_REMOTE_CONTROL_ENABLE:       res = sizeof(V764_2_RF_REMOTE_CONTROL_ENABLE_A); break;
    case V764_2_RF_REMOTE_CONTROL_DISABLE:      res = sizeof(V764_2_RF_REMOTE_CONTROL_DISABLE_A); break;
    case V764_2_RF_SERVICE_ENABLE:              res = sizeof(V764_2_RF_SERVICE_ENABLE_A); break;
    case V764_2_RF_POWER_OFF:                   res = sizeof(V764_2_RF_POWER_OFF_A); break;
    case V764_2_RF_DC_VOLTAGE_MEASUREMENT:      res = sizeof(V764_2_RF_DC_VOLTAGE_MEASUREMENT_A); break;
    case V764_2_RF_AC_VOLTAGE_MEASUREMENT:      res = sizeof(V764_2_RF_AC_VOLTAGE_MEASUREMENT_A); break;
    case V764_2_RF_DC_CURRENT_MEASUREMENT:      res = sizeof(V764_2_RF_DC_CURRENT_MEASUREMENT_A); break;
    case V764_2_RF_AC_CURRENT_MEASUREMENT:      res = sizeof(V764_2_RF_AC_CURRENT_MEASUREMENT_A); break;
    case V764_2_RF_RESISTANCE_MEASUREMENT:      res = sizeof(V764_2_RF_RESISTANCE_MEASUREMENT_A); break;
    case V764_2_RF_CAPACITANCE_MEASUREMENT:     res = sizeof(V764_2_RF_CAPACITANCE_MEASUREMENT_A); break;
    case V764_2_RF_INDUCTANCE_MEASUREMENT:      res = sizeof(V764_2_RF_INDUCTANCE_MEASUREMENT_A); break;
    case V764_2_RF_BUZZER_TEST:                 res = sizeof(V764_2_RF_BUZZER_TEST_A); break;
    case V764_2_RF_FREQUENCY_MEASUREMENT:       res = sizeof(V764_2_RF_FREQUENCY_MEASUREMENT_A); break;
    case V764_2_RF_ACTIVE_POWER_MEASUREMENT:    res = sizeof(V764_2_RF_ACTIVE_POWER_MEASUREMENT_A); break;
    case V764_2_RF_REACTIVE_POWER_MEASUREMENT:  res = sizeof(V764_2_RF_REACTIVE_POWER_MEASUREMENT_A); break;
    case V764_2_RF_FULL_POWER_MEASUREMENT:      res = sizeof(V764_2_RF_FULL_POWER_MEASUREMENT_A); break;
    case V764_2_RF_COS_FI_MEASUREMENT:          res = sizeof(V764_2_RF_COS_FI_MEASUREMENT_A); break;
    case V764_2_RF_UDC_CAL:                     res = sizeof(V764_2_RF_UDC_CAL_A); break;
    case V764_2_RF_UAC_CAL:                     res = sizeof(V764_2_RF_UAC_CAL_A); break;
    case V764_2_RF_IDC_CAL:                     res = sizeof(V764_2_RF_IDC_CAL_A); break;
    case V764_2_RF_IAC_CAL:                     res = sizeof(V764_2_RF_IAC_CAL_A); break;
    case V764_2_RF_RES_CAL:                     res = sizeof(V764_2_RF_RES_CAL_A); break;
    case V764_2_RF_FLASH_WRITE:                 res = sizeof(V764_2_RF_FLASH_WRITE_A); break;
    case V764_2_RF_SERVICE_DISABLE:             res = sizeof(V764_2_RF_SERVICE_DISABLE_A); break;
    case V764_2_RF_DIODE_TEST:                  res = sizeof(V764_2_RF_DIODE_TEST_A); break;
    case V764_2_RF_UDC_CAL_ZEROES:              res = sizeof(V764_2_RF_UDC_CAL_ZEROES_A); break;
    case V764_2_RF_IDC_CAL_ZEROES:              res = sizeof(V764_2_RF_IDC_CAL_ZEROES_A); break;
    case V764_2_RF_RES_CAL_ZEROES:              res = sizeof(V764_2_RF_RES_CAL_ZEROES_A); break;
    case V764_2_RF_CAP_CAL_ZEROES:              res = sizeof(V764_2_RF_CAP_CAL_ZEROES_A); break;
    case V764_2_RF_IND_CAL_ZEROES:              res = sizeof(V764_2_RF_IND_CAL_ZEROES_A); break;
    case V764_2_RF_FLASH_WRITE_ZEROES:          res = sizeof(V764_2_RF_FLASH_WRITE_ZEROES_A); break;

        //TODO added 28.07.22
    case V764_2_RF_READ_POINT_FROM_MEMORY:      res = sizeof(V764_2_RF_READ_POINT_FROM_MEMORY_A); break;
    case V764_2_RF_WRITE_POINT_TO_MEMORY:       res = sizeof(V764_2_RF_WRITE_POINT_TO_MEMORY_A); break;
    case V764_2_RF_READ_FIRMWARE_VERSION:       res = sizeof(V764_2_RF_READ_FIRMWARE_VERSION_A); break;
    case V764_2_RF_SET_NEW_ADDRESS:             res = sizeof(V764_2_RF_SET_NEW_ADDRESS_A); break;
    case V764_2_RF_GET_RSSI:                    res = sizeof(V764_2_RF_GET_RSSI_A); break;

        //TODO added 27.09.22
    case V764_2_RF_TEST:
        res = sizeof(V764_2_RF_TEST_A);
        break;

    case V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES:
        res = sizeof(V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES_A);
        break;

    case V764_2_RF_GET_CNT_E21:
        res = sizeof(V764_2_RF_GET_CNT_E21_A);
        break;

    default:
        emit error(QString("unknown COMMAND %1").arg(command, 0, 16));
        break;
    }
    return res;
}
//--------------------------------------------------------------------------------
QString Multimeter_V786_2::get_limit_string(unsigned int cmd,
                                            uint8_t byte_options,
                                            bool is_russian)
{
    emit trace(Q_FUNC_INFO);

    QString temp;
    BYTE_LIMITS byte_limits;
    byte_limits.value = byte_options;

    //qDebug() << "byte_limits.data.reserved" << byte_limits.data.reserved;
    //qDebug() << "byte_limits.data.auto_limit" << byte_limits.data.auto_limit;
    //qDebug() << "byte_limits.data.limit" << byte_limits.data.limit;
    temp.clear();
    switch(cmd)
    {
    case V764_2_RF_DC_VOLTAGE_MEASUREMENT:
    case V764_2_RF_AC_VOLTAGE_MEASUREMENT:
        switch(byte_limits.data.limit)
        {
        case 0:
            is_russian ? temp.append("мВ") : temp.append("mV");
            break;

        case 1:
        case 2:
        case 3:
        case 4:
            is_russian ? temp.append("В") : temp.append("V");
            break;

        default:
            //temp.append(QString("%1").arg(limit);
            temp.append("---");
            break;
        }
        break;

    case V764_2_RF_DC_CURRENT_MEASUREMENT:
    case V764_2_RF_AC_CURRENT_MEASUREMENT:
        switch(byte_limits.data.limit)
        {
        case 0:
            is_russian ? temp.append("мкА") : temp.append("mkA");
            break;

        case 1:
        case 2:
        case 3:
        case 4:
            is_russian ? temp.append("мА") : temp.append("mA");
            break;

        case 5:
            is_russian ? temp.append("А") : temp.append("A");
            break;

        default:
            //temp.append(QString("%1").arg(limit);
            temp.append("---");
            break;
        }
        break;

    case V764_2_RF_BUZZER_TEST:
    case V764_2_RF_RESISTANCE_MEASUREMENT:
        switch(byte_limits.data.limit)
        {
        case 0:
            is_russian ? temp.append("Ом") : temp.append("Om");
            break;

        case 1:
        case 2:
        case 3:
            is_russian ? temp.append("кОм") : temp.append("kOm");
            break;

        case 4:
            is_russian ? temp.append("МОм") : temp.append("MOm");
            break;

        default:
            //temp.append(QString("%1").arg(limit);
            temp.append("---");
            break;
        }
        break;

    case V764_2_RF_CAPACITANCE_MEASUREMENT:
        switch(byte_limits.data.limit)
        {
        case 0:
            is_russian ? temp.append("пФ") : temp.append("pF");
            break;

        case 1:
            is_russian ? temp.append("мкФ") : temp.append("mkF");
            break;

        default:
            //temp.append(QString("%1").arg(limit);
            temp.append("---");
            break;
        }

        break;

    case V764_2_RF_INDUCTANCE_MEASUREMENT:
        is_russian ? temp.append("мГ") : temp.append("mH");
        break;

    case V764_2_RF_FREQUENCY_MEASUREMENT:
        switch(byte_limits.data.limit)
        {
        case 0:
            is_russian ? temp.append("Гц") : temp.append("Hz");
            break;

        case 1:
            is_russian ? temp.append("кГц") : temp.append("kHz");
            break;

        case 2:
            is_russian ? temp.append("МГц") : temp.append("MHz");
            break;

        default:
            break;
        }
        break;

    case V764_2_RF_ACTIVE_POWER_MEASUREMENT:
        is_russian ? temp.append("кВА") : temp.append("kWA");
        break;

    case V764_2_RF_REACTIVE_POWER_MEASUREMENT:
        is_russian ? temp.append("кВАр") : temp.append("kWAr");
        break;

    case V764_2_RF_FULL_POWER_MEASUREMENT:
        is_russian ? temp.append("кВт") : temp.append("kW");
        break;

    case V764_2_RF_COS_FI_MEASUREMENT:
        break;

    default:
        break;
    }

    //qDebug() << temp;
    return temp;
}
//--------------------------------------------------------------------------------
QString Multimeter_V786_2::get_limit_string(bool is_russian)
{
    emit trace(Q_FUNC_INFO);

    QString temp;

    V764_2_RF_DC_VOLTAGE_MEASUREMENT_A *b786_measuring;
    b786_measuring = (V764_2_RF_DC_VOLTAGE_MEASUREMENT_A *)data_radiomodule.constData();
    unsigned int cmd = b786_measuring->header.cmd;
    BYTE_LIMITS byte_limits;
    byte_limits.value = b786_measuring->limits;

    temp.clear();
    switch(cmd)
    {
    case V764_2_RF_DC_VOLTAGE_MEASUREMENT:
    case V764_2_RF_AC_VOLTAGE_MEASUREMENT:
        switch(byte_limits.data.limit)
        {
        case 0:
            is_russian ? temp.append("мВ") : temp.append("mV");
            break;

        case 1:
        case 2:
        case 3:
        case 4:
            is_russian ? temp.append("В") : temp.append("V");
            break;

        default:
            //temp.append(QString("%1").arg(limit);
            temp.append("---");
            break;
        }
        break;

    case V764_2_RF_DC_CURRENT_MEASUREMENT:
    case V764_2_RF_AC_CURRENT_MEASUREMENT:
        switch(byte_limits.data.limit)
        {
        case 0:
            is_russian ? temp.append("мкА") : temp.append("mkA");
            break;

        case 1:
        case 2:
        case 3:
        case 4:
            is_russian ? temp.append("мА") : temp.append("mA");
            break;

        case 5:
            is_russian ? temp.append("А") : temp.append("A");
            break;

        default:
            //temp.append(QString("%1").arg(limit);
            temp.append("---");
            break;
        }
        break;

    case V764_2_RF_BUZZER_TEST:
    case V764_2_RF_RESISTANCE_MEASUREMENT:
        switch(byte_limits.data.limit)
        {
        case 0:
            is_russian ? temp.append("Ом") : temp.append("Om");
            break;

        case 1:
        case 2:
        case 3:
            is_russian ? temp.append("кОм") : temp.append("kOm");
            break;

        case 4:
            is_russian ? temp.append("МОм") : temp.append("MOm");
            break;

        default:
            //temp.append(QString("%1").arg(limit);
            temp.append("---");
            break;
        }
        break;

    case V764_2_RF_CAPACITANCE_MEASUREMENT:
        switch(byte_limits.data.limit)
        {
        case 0:
            is_russian ? temp.append("пФ") : temp.append("pF");
            break;

        case 1:
            is_russian ? temp.append("мкФ") : temp.append("mkF");
            break;

        default:
            //temp.append(QString("%1").arg(limit);
            temp.append("---");
            break;
        }

        break;

    case V764_2_RF_INDUCTANCE_MEASUREMENT:
        is_russian ? temp.append("мГ") : temp.append("mH");
        break;

    case V764_2_RF_FREQUENCY_MEASUREMENT:
        switch(byte_limits.data.limit)
        {
        case 0:
            is_russian ? temp.append("Гц") : temp.append("Hz");
            break;

        case 1:
            is_russian ? temp.append("кГц") : temp.append("kHz");
            break;

        case 2:
            is_russian ? temp.append("МГц") : temp.append("MHz");
            break;

        default:
            break;
        }
        break;

    case V764_2_RF_ACTIVE_POWER_MEASUREMENT:
        is_russian ? temp.append("кВА") : temp.append("kWA");
        break;

    case V764_2_RF_REACTIVE_POWER_MEASUREMENT:
        is_russian ? temp.append("кВАр") : temp.append("kWAr");
        break;

    case V764_2_RF_FULL_POWER_MEASUREMENT:
        is_russian ? temp.append("кВт") : temp.append("kW");
        break;

    case V764_2_RF_COS_FI_MEASUREMENT:
        break;

    default:
        break;
    }

    //qDebug() << temp;
    return temp;
}
//--------------------------------------------------------------------------------
QString Multimeter_V786_2::get_deviation_string(void)
{
    emit trace(Q_FUNC_INFO);

    QString temp;
    temp.clear();

    if(data_radiomodule.length() == sizeof(V764_2_RF_DC_VOLTAGE_MEASUREMENT_A))
    {
        V764_2_RF_DC_VOLTAGE_MEASUREMENT_A *b786_measuring;
        b786_measuring = (V764_2_RF_DC_VOLTAGE_MEASUREMENT_A *)data_radiomodule.constData();

        BYTE_DEVIATION_RETENTION byte_deviation_retention;
        byte_deviation_retention.value = b786_measuring->deviation_retention;
        //qDebug() << "byte_deviation_retention.data.deviation" << byte_deviation_retention.data.data.deviation;
        switch(byte_deviation_retention.data.deviation)
        {
        case 1:
            temp.append("Δ");
            break;

        case 2:
            temp.append("%");
            break;

        default:
            break;
        }
    }
    //qDebug() << "deviation temp" << temp;
    return temp;
}
//--------------------------------------------------------------------------------
QString Multimeter_V786_2::get_retention_string(void)
{
    emit trace(Q_FUNC_INFO);

    QString temp;
    temp.clear();

    if(data_radiomodule.length() == sizeof(V764_2_RF_DC_VOLTAGE_MEASUREMENT_A))
    {
        V764_2_RF_DC_VOLTAGE_MEASUREMENT_A *b786_measuring;
        b786_measuring = (V764_2_RF_DC_VOLTAGE_MEASUREMENT_A *)data_radiomodule.constData();

        BYTE_DEVIATION_RETENTION byte_deviation_retention;
        byte_deviation_retention.value = b786_measuring->deviation_retention;
        //qDebug() << "byte_deviation_retention.data.retention" << byte_deviation_retention.data.retention;
        switch(byte_deviation_retention.data.retention)
        {
        case 3:
            temp.append("max");
            break;

        case 2:
            temp.append("min");
            break;

        case 1:
            temp.append("hold");
            break;

        case 0:
            break;

        default:
            temp.append(QString("%1").arg(byte_deviation_retention.data.retention));
            break;
        }
    }
    return temp;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::convert_ascii_to_data(QString input, bool *ok)
{
    emit trace(Q_FUNC_INFO);

    QByteArray temp_ba;

    int begin = input.indexOf(':');
    if(begin < 0)
    {
        temp_ba.clear();
        temp_ba.append(input.toLatin1());

        *ok = false;
        return temp_ba;
    }

    int end = input.indexOf('\r');
    if(end < 0)
    {
        temp_ba.clear();
        temp_ba.append(input.toLatin1());

        *ok = false;
        return temp_ba;
    }

    if(end < begin)
    {
        temp_ba.clear();
        temp_ba.append(input.toLatin1());

        *ok = false;
        return temp_ba;
    }

    temp_ba = QByteArray::fromHex(input.toLocal8Bit());

    *ok = true;
    return temp_ba;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::wait(int timeout_msec)
{
#ifdef TEST
    timeout_msec = TEST_WAIT_MS;
#endif

    emit trace(Q_FUNC_INFO);

    QElapsedTimer time;

    if(timeout_msec < 1)
        return;

    bool need_time = false;
    time.start();
    while(time.elapsed() < timeout_msec)
    {
        QCoreApplication::processEvents();
        if(serial->isOpen() == false)
        {
            return;
        }
        if(is_ready == true)
        {
            if(need_time == false)
            {
                timeout_msec = time.elapsed() + 500;
                need_time = true;
            }
        }
    }
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::wait(int max_len,
                             int min_time_msec,
                             int max_time_msec)
{
    emit trace(Q_FUNC_INFO);

#ifdef TEST
    min_time_msec = 0;
    max_time_msec = 100;
#endif

    QElapsedTimer time;

    time.start();
    while(true)
    {
        QCoreApplication::processEvents();
        if(serial->isOpen() == false)
        {
            return;
        }
        if(time.elapsed() > min_time_msec)
            break;
    }

    time.start();
    while(true)
    {
        QCoreApplication::processEvents();
        if(serial->isOpen() == false)
        {
            return;
        }
        if(time.elapsed() > max_time_msec)
            break;

        if(data_radiomodule.size() >= max_len)
            break;
    }
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::get_raw_data(void)
{
    return data_radiomodule;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::port_is_open(void)
{
    return serial->isOpen();
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::measuring(uint8_t cmd,
                                 double *value,
                                 bool *ok)
{
    emit trace(Q_FUNC_INFO);

    BYTE_MEASURING byte_measuring;

    byte_measuring.data.retention = multimeter_retention;
    byte_measuring.data.deviation = multimeter_deviation;

    byte_measuring.data.auto_limit = multimeter_auto_limit;
    if(multimeter_auto_limit)
        byte_measuring.data.limit = 0;
    else
        byte_measuring.data.limit = multimeter_limit;

    set_serno(multimeter_serno);
    set_year(multimeter_year);
    set_address(multimeter_address);
    set_channel(multimeter_channel);

    V764_2_MEASURING_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = cmd;
    question.header.count_data = multimeter_data_len;
    question.data_0 = byte_measuring.value;
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    int err = send(cmd,
                   (char *)&question,
                   sizeof(question),
                   multimeter_serno,
                   multimeter_year);
    if(err == ERROR_13)
    {
        emit change_retention("");
        emit change_deviation("");
        emit get_measuring_value("OL");
        emit change_limit("---");
        *ok = false;
        last_error = err;
        return last_error;
    }
    if(err == ERROR_14)
    {
        emit change_retention("");
        emit change_deviation("");
        emit get_measuring_value("- OL");
        emit change_limit("---");
        *ok = false;
        last_error = err;
        return last_error;
    }
    if(err == ERROR_15)
    {
        emit change_retention("");
        emit change_deviation("");
        emit get_measuring_value("LF");
        emit change_limit("---");
        *ok = false;
        last_error = err;
        return last_error;
    }
    if(err == E_NO_ERROR)
    {
        emit change_retention(get_retention_string());
        emit change_deviation(get_deviation_string());
        emit change_limit(get_limit_string(cmd, get_limit()));

        bool temp_ok = false;
        double measuring_value = 0;
        temp_ok = get_value(cmd, &measuring_value);
        if(temp_ok)
        {
            emit get_measuring_value(QString("%1").arg(measuring_value));
            *ok = true;
            *value = measuring_value;
            last_error = E_NO_ERROR;
            return last_error;
        }
        else
        {
            *ok = false;
            last_error = E_UNKNOWN_ERROR;
            return last_error;
        }
    }
    else
    {
        *ok = false;
        last_error = err;
        return last_error;
    }
    *ok = true;
    last_error = E_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::measuring_battery(bool *ok)
{
    emit trace(Q_FUNC_INFO);

    emit trace(V764_2_RF_STATUS_REQUEST_TEXT);
    set_serno(multimeter_serno);
    set_year(multimeter_year);
    set_address(multimeter_address);
    set_channel(multimeter_channel);

    QByteArray packet = prepare_V764_2_RF_STATUS_REQUEST();
    int err = send(V764_2_RF_STATUS_REQUEST,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err == E_NO_ERROR)
    {
        QByteArray ba;
        ba.clear();
        ba.append(get_raw_data());
        if(ba.length() == sizeof(V764_2_RF_STATUS_REQUEST_A))
        {
            V764_2_RF_STATUS_REQUEST_A *b786_state;
            b786_state = (V764_2_RF_STATUS_REQUEST_A *)ba.data();
            emit get_battery_value(b786_state->BattVoltage);
            *ok = true;
        }
        else
        {
            emit error("Bad len");
        }
    }
    else
    {
        emit error("Error");
        *ok = false;
        last_error = err;
        return last_error;
    }
    *ok = true;
    last_error = E_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_retention(char value)
{
    emit trace(Q_FUNC_INFO);

    if(value < 0) value = 0;
    emit debug(QString("set retention %1").arg((int)value));
    multimeter_retention = value;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_deviation(char value)
{
    emit trace(Q_FUNC_INFO);

    if(value < 0) value = 0;
    emit debug(QString("set deviation %1").arg((int)value));
    multimeter_deviation = value;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_limit(uint8_t value)
{
    emit trace(Q_FUNC_INFO);
    emit debug(QString("set limit %1").arg((int)value));
    multimeter_limit = value;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_auto_limit(bool state)
{
    emit trace(Q_FUNC_INFO);

    emit debug(QString("set auto_limit %1").arg(state));
    multimeter_auto_limit = state;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_timeout(unsigned int value)
{
    emit trace(Q_FUNC_INFO);

    emit debug(QString("set timeout %1").arg(value));
    multimeter_timeout = value;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_address(unsigned int value)
{
    emit trace(Q_FUNC_INFO);

    emit debug(QString("set address %1").arg(value));
    multimeter_address = value;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_channel(unsigned int value)
{
    emit trace(Q_FUNC_INFO);

    emit debug(QString("set channel %1").arg(value));
    multimeter_channel = value;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_serno(unsigned int value)
{
    emit trace(Q_FUNC_INFO);

    emit debug(QString("set serno %1").arg(value));
    multimeter_serno = value;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_year(unsigned int value)
{
    emit trace(Q_FUNC_INFO);

    emit debug(QString("set year %1").arg(value));
    multimeter_year = value;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_remote_address(unsigned int value)
{
    emit trace(Q_FUNC_INFO);

    emit debug(QString("set remote address %1").arg(value));
    remote_address = value;
}
//--------------------------------------------------------------------------------
#ifdef TEST
void Multimeter_V786_2::set_test_address(uint8_t value)
{
    emit trace(Q_FUNC_INFO);

    serial->setAddress(value);
}
#endif
//--------------------------------------------------------------------------------
#ifdef TEST
void Multimeter_V786_2::set_test_channel(uint8_t value)
{
    emit trace(Q_FUNC_INFO);

    serial->setChannel(value);
}
#endif
//--------------------------------------------------------------------------------
#ifdef TEST
void Multimeter_V786_2::set_test_serno(uint16_t value)
{
    emit trace(Q_FUNC_INFO);

    serial->setSerNo(value);
}
#endif
//--------------------------------------------------------------------------------
#ifdef TEST
void Multimeter_V786_2::set_test_year(uint16_t value)
{
    emit trace(Q_FUNC_INFO);

    serial->setYear(value);
}
#endif
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_silence(bool state)
{
    is_silence = state;
}
//--------------------------------------------------------------------------------
uint8_t Multimeter_V786_2::get_retention(void)
{
    emit trace(Q_FUNC_INFO);

    emit debug(QString("get retention %1").arg((int)multimeter_retention));
    return multimeter_retention;
}
//--------------------------------------------------------------------------------
uint8_t Multimeter_V786_2::get_deviation(void)
{
    emit trace(Q_FUNC_INFO);

    emit debug(QString("get deviation %1").arg((int)multimeter_deviation));
    return multimeter_deviation;
}
//--------------------------------------------------------------------------------
uint8_t Multimeter_V786_2::get_limit(void)
{
    emit trace(Q_FUNC_INFO);

    emit debug(QString("get limit %1").arg((int)multimeter_limit));
    return multimeter_limit;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::get_auto_limit(void)
{
    emit trace(Q_FUNC_INFO);

    emit debug(QString("get auto_limit %1").arg((int)multimeter_auto_limit));
    return multimeter_auto_limit;
}
//--------------------------------------------------------------------------------
unsigned int Multimeter_V786_2::get_timeout(void)
{
    emit trace(Q_FUNC_INFO);

    emit debug(QString("get timeout %1").arg((int)multimeter_timeout));
    return multimeter_timeout;
}
//--------------------------------------------------------------------------------
unsigned int Multimeter_V786_2::get_address(void)
{
    emit trace(Q_FUNC_INFO);

    emit debug(QString("get address %1").arg((int)multimeter_address));
    return multimeter_address;
}
//--------------------------------------------------------------------------------
unsigned int Multimeter_V786_2::get_channel(void)
{
    emit trace(Q_FUNC_INFO);

    emit debug(QString("get channel %1").arg((int)multimeter_channel));
    return multimeter_channel;
}
//--------------------------------------------------------------------------------
unsigned int Multimeter_V786_2::get_serno(void)
{
    emit trace(Q_FUNC_INFO);

    emit debug(QString("get serno %1").arg((int)multimeter_serno));
    return multimeter_serno;
}
//--------------------------------------------------------------------------------
unsigned int Multimeter_V786_2::get_year(void)
{
    emit trace(Q_FUNC_INFO);

    emit debug(QString("get year %1").arg((int)multimeter_year));
    return multimeter_year;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_step_a(uint8_t value)
{
    multimeter_step_a = value;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_step_b(uint8_t value)
{
    multimeter_step_b = value;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_step_c(uint8_t value)
{
    multimeter_step_c = value;
}
//--------------------------------------------------------------------------------
uint8_t Multimeter_V786_2::get_step_a(void)
{
    return multimeter_step_a;
}
//--------------------------------------------------------------------------------
uint8_t Multimeter_V786_2::get_step_b(void)
{
    return multimeter_step_b;
}
//--------------------------------------------------------------------------------
uint8_t Multimeter_V786_2::get_step_c(void)
{
    return multimeter_step_c;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_data_len(uint8_t value)
{
    multimeter_data_len = value;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_data_0(uint8_t value)
{
    multimeter_data_0 = value;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_data_1(uint8_t value)
{
    multimeter_data_1 = value;
}
//--------------------------------------------------------------------------------
uint8_t Multimeter_V786_2::get_data_len(void)
{
    return multimeter_data_len;
}
//--------------------------------------------------------------------------------
uint8_t Multimeter_V786_2::get_data_0(void)
{
    return multimeter_data_0;
}
//--------------------------------------------------------------------------------
uint8_t Multimeter_V786_2::get_data_1(void)
{
    return multimeter_data_1;
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::print_last_error(void)
{
    switch (last_error)
    {
    case E_NO_ERROR:                break;
    case E_ERROR_MULTIMETR:         emit error(E_ERROR_MULTIMETR_TEXT);         break;
    case E_ERROR_INDEX_NOT_CORRECT: emit error(E_ERROR_INDEX_NOT_CORRECT_TEXT); break;
    case E_ERROR_PORT_NOT_OPEN:     emit error(E_ERROR_PORT_NOT_OPEN_TEXT);     break;
    case E_ERROR_BREAK:             emit error(E_ERROR_BREAK_TEXT);             break;
    case E_ERROR_MAX_REPEAT:        emit error(E_ERROR_MAX_REPEAT_TEXT);        break;
    case E_ERROR_NO_ANSWER:         emit error(E_ERROR_NO_ANSWER_TEXT);         break;
    case E_ERROR_BIG_ANSWER:        emit error(E_ERROR_BIG_ANSWER_TEXT);        break;
    case E_ERROR_SMALL_ANSWER:      emit error(E_ERROR_SMALL_ANSWER_TEXT);      break;
    case E_ERROR_SMALL_PACKET:      emit error(E_ERROR_SMALL_PACKET_TEXT);      break;
    case E_ERROR_BAD_SERNO:         emit error(E_ERROR_BAD_SERNO_TEXT);         break;
    case E_ERROR_BAD_ADDRESS:       emit error(E_ERROR_BAD_ADDRESS_TEXT);       break;
    case E_ERROR_BAD_CRC:           emit error(E_ERROR_BAD_CRC_TEXT);           break;
    case E_ERROR_BAD_CMD:           emit error(E_ERROR_BAD_CMD_TEXT);           break;
    case E_ERROR_BAD_LEN:           emit error(E_ERROR_BAD_LEN_TEXT);           break;
    case E_ERROR_UNKNOWN_CMD:       emit error(E_ERROR_UNKNOWN_CMD_TEXT);       break;
    case E_ERROR_SIZE_NOT_CORRECT:  emit error(E_ERROR_SIZE_NOT_CORRECT_TEXT);  break;
    case E_UNKNOWN_ERROR:           emit error(E_UNKNOWN_ERROR_TEXT);           break;
    case E_INVALID_FLOAT:           emit error(E_INVALID_FLOAT_TEXT);           break;
    case E_INVALID_LIMIT:           emit error(E_INVALID_LIMIT_TEXT);           break;
    case E_INVALID_DELTA:           emit error(E_INVALID_DELTA_TEXT);           break;
    case E_BREAK_MEASURING:         emit error(E_BREAK_MEASURING_TEXT);         break;

    case ERROR_01: emit error(ERROR_01_TEXT); break;
    case ERROR_02: emit error(ERROR_02_TEXT); break;
    case ERROR_03: emit error(ERROR_03_TEXT); break;
    case ERROR_04: emit error(ERROR_04_TEXT); break;
    case ERROR_05: emit error(ERROR_05_TEXT); break;
    case ERROR_06: emit error(ERROR_06_TEXT); break;
    case ERROR_07: emit error(ERROR_07_TEXT); break;
    case ERROR_08: emit error(ERROR_08_TEXT); break;
    case ERROR_09: emit error(ERROR_09_TEXT); break;
    case ERROR_0A: emit error(ERROR_0A_TEXT); break;
    case ERROR_0B: emit error(ERROR_0B_TEXT); break;
    case ERROR_0C: emit error(ERROR_0C_TEXT); break;
    case ERROR_0D: emit error(ERROR_0D_TEXT); break;
    case ERROR_0E: emit error(ERROR_0E_TEXT); break;
    case ERROR_0F: emit error(ERROR_0F_TEXT); break;
    case ERROR_10: emit error(ERROR_10_TEXT); break;
    case ERROR_11: emit error(ERROR_11_TEXT); break;
    case ERROR_12: emit error(ERROR_12_TEXT); break;
    case ERROR_13: emit error(ERROR_13_TEXT); break;
    case ERROR_14: emit error(ERROR_14_TEXT); break;
    case ERROR_15: emit error(ERROR_15_TEXT); break;
    case ERROR_16: emit error(ERROR_16_TEXT); break;
    case ERROR_17: emit error(ERROR_17_TEXT); break;
    case ERROR_18: emit error(ERROR_18_TEXT); break;
    case ERROR_19: emit error(ERROR_19_TEXT); break;
    case ERROR_1A: emit error(ERROR_1A_TEXT); break;
    case ERROR_1B: emit error(ERROR_1B_TEXT); break;
    case ERROR_1C: emit error(ERROR_1C_TEXT); break;
    default:
        emit error(QString("Multimeter_V786_2::print_last_error Unknown error %1 0x%2")
                   .arg(last_error)
                   .arg(last_error, 2, 16, QChar('0')));
        break;
    }
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::get_last_error(void)
{
    return last_error;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::test(void)
{
    QByteArray ba;

    ba.clear();
    ba.append((char)0x34);
    ba.append((char)0x12);
    for(int n=0; n<10; n++)
    {
        ba.append((char)n);
    }
    ba.append((char)26);

    serial->write(convert_data_to_ascii(ba));
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::remote_control_on(void)
{
    Q_ASSERT(multimeter_timeout > 100);

    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_REMOTE_CONTROL_ENABLE_TEXT);

    QByteArray packet = prepare_V764_2_RF_REMOTE_CONTROL_ENABLE();
    int err = send(V764_2_RF_REMOTE_CONTROL_ENABLE,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::remote_control_off(void)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_REMOTE_CONTROL_DISABLE_TEXT);

    QByteArray packet = prepare_V764_2_RF_REMOTE_CONTROL_DISABLE();
    int err = send(V764_2_RF_REMOTE_CONTROL_DISABLE,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::power_off(void)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_POWER_OFF_TEXT);

    QByteArray packet = prepare_V764_2_RF_POWER_OFF();
    int err = send(V764_2_RF_POWER_OFF,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::buzzer_test(void)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_BUZZER_TEST_TEXT);

    QByteArray packet = prepare_V764_2_RF_BUZZER_TEST();
    int err = send(V764_2_RF_BUZZER_TEST,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::calibration_UDC(void)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_UDC_CAL_TEXT);

    QByteArray packet = prepare_V764_2_RF_UDC_CAL();
    int err = send(V764_2_RF_UDC_CAL,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::calibration_UAC(void)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_UAC_CAL_TEXT);

    QByteArray packet = prepare_V764_2_RF_UAC_CAL();
    int err = send(V764_2_RF_UAC_CAL,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::calibration_IDC(void)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_IDC_CAL_TEXT);

    QByteArray packet = prepare_V764_2_RF_IDC_CAL();
    int err = send(V764_2_RF_IDC_CAL,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::calibration_IAC(void)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_IAC_CAL_TEXT);

    QByteArray packet = prepare_V764_2_RF_IAC_CAL();
    int err = send(V764_2_RF_IAC_CAL,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::calibration_R(void)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_RES_CAL_TEXT);

    QByteArray packet = prepare_V764_2_RF_RES_CAL();
    int err = send(V764_2_RF_RES_CAL,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::service_off(void)
{
    Q_ASSERT(multimeter_data_len == 0);

    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_SERVICE_DISABLE_TEXT);

    QByteArray packet = prepare_V764_2_RF_SERVICE_DISABLE();
    int err = send(V764_2_RF_SERVICE_DISABLE,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::get_state(void)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_STATUS_REQUEST_TEXT);

    emit debug(QString("multimeter_serno: %1").arg(multimeter_serno));
    emit debug(QString("multimeter_year: %1").arg(multimeter_year));
    emit debug(QString("multimeter_address: %1").arg(multimeter_address));
    emit debug(QString("multimeter_channel: %1").arg(multimeter_channel));

    emit debug(QString("multimeter_data_len: %1").arg(multimeter_data_len));
    emit debug(QString("len %1").arg(sizeof(V764_2_RF_STATUS_REQUEST_Q)));

    QByteArray packet = prepare_V764_2_RF_STATUS_REQUEST();
    int err = send(V764_2_RF_STATUS_REQUEST,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    //---
    if(!is_silence)
    {
        V764_2_RF_STATUS_REQUEST_A *b786_state = (V764_2_RF_STATUS_REQUEST_A *)data_radiomodule.data();

        emit info("Статус прибора:");
        emit info(QString("    адрес %1").arg(b786_state->header.address));
        emit info(QString("    серийный номер %1").arg(b786_state->SerialNo));
        emit info(QString("    год изготовления %1").arg(b786_state->SerialYear));
        QString temp;
        temp.clear();
        for(unsigned int n=0; n<sizeof(b786_state->DisplaySymbols); n++)
            temp.append(b786_state->DisplaySymbols[n]);
        emit info(QString("    отображаемое значение %1").arg(temp));
        emit info(QString("    предел %1").arg(b786_state->Range));
        emit info(QString("    режим %1").arg(b786_state->Mode));

        U_BYTE_DECADES temp_decades;
        temp_decades.value = b786_state->Math_Hold;
        emit info(QString("    math %1").arg(temp_decades.decades.lo));
        emit info(QString("    hold %1").arg(temp_decades.decades.hi));
        emit info(QString("    напряжение батареи %1").arg((qreal)b786_state->BattVoltage / 1000.0));

        emit update_battery(b786_state->BattVoltage);
    }
    //---
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::set_new_param(RF_set_param param)
{
    QByteArray ba;
    ba.clear();
    ba.append((char *)&param, sizeof(param));
    int res = send_raw_data(convert_data_to_ascii(ba));
    if(res != E_NO_ERROR)
        return false;

    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::set_new_param(RF_set_param_2 param, QByteArray *ba_res)
{
    QByteArray ba;
    ba.clear();
    ba.append((char *)&param, sizeof(param));

    data_radiomodule.clear();
    int res = send_raw_data(convert_data_to_ascii(ba));
    if(res != E_NO_ERROR)
        return false;

    wait(multimeter_timeout);
    if(data_radiomodule.isEmpty())
    {
        emit error("No data");
        return false;
    }
    else
    {
        //emit info(data_radiomodule);
        *ba_res = data_radiomodule;
    }

    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::write_to_flash(void)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_FLASH_WRITE_TEXT);

    QByteArray packet = prepare_V764_2_RF_FLASH_WRITE();
    int err = send(V764_2_RF_FLASH_WRITE,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::test_diod(void)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_DIODE_TEST_TEXT);

    QByteArray packet = prepare_V764_2_RF_DIODE_TEST();
    int err = send(V764_2_RF_DIODE_TEST,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;

}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::udc_cal_zeroes(void)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_UDC_CAL_ZEROES_TEXT);

    QByteArray packet = prepare_V764_2_RF_UDC_CAL_ZEROES();
    int err = send(V764_2_RF_UDC_CAL_ZEROES,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;

}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::idc_cal_zeroes(void)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_IDC_CAL_ZEROES_TEXT);

    QByteArray packet = prepare_V764_2_RF_IDC_CAL_ZEROES();
    int err = send(V764_2_RF_IDC_CAL_ZEROES,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;

}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::res_cal_zeroes(void)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_RES_CAL_ZEROES_TEXT);

    QByteArray packet = prepare_V764_2_RF_RES_CAL_ZEROES();
    int err = send(V764_2_RF_RES_CAL_ZEROES,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::cap_cal_zeroes(void)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_CAP_CAL_ZEROES_TEXT);

    QByteArray packet = prepare_V764_2_RF_CAP_CAL_ZEROES();
    int err = send(V764_2_RF_CAP_CAL_ZEROES,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::ind_cal_zeroes(void)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_IND_CAL_ZEROES_TEXT);

    QByteArray packet = prepare_V764_2_RF_IND_CAL_ZEROES();
    int err = send(V764_2_RF_IND_CAL_ZEROES,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::flash_write_zeroes(void)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_FLASH_WRITE_ZEROES_TEXT);

    QByteArray packet = prepare_V764_2_RF_FLASH_WRITE_ZEROES();
    int err = send(V764_2_RF_FLASH_WRITE_ZEROES,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::read_point_from_memory(uint8_t table,
                                               uint8_t index_0,
                                               uint8_t index_1,
                                               float *value)
{
    emit trace(Q_FUNC_INFO);

    //FIXME новая команда
    point_table = table;
    point_index_0 = index_0;
    point_index_1 = index_1;

    QByteArray packet = prepare_V764_2_RF_READ_POINT_FROM_MEMORY();
    int err = send(V764_2_RF_READ_POINT_FROM_MEMORY,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    //TODO прочитаем данные
    V764_2_RF_READ_POINT_FROM_MEMORY_A *answer = (V764_2_RF_READ_POINT_FROM_MEMORY_A *)data_radiomodule.constData();
    float point_value = answer->point;
    *value = point_value;
    emit debug(QString("point_value: 0x%1")
               .arg(point_value));
    //---

    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::write_point_to_memory(uint8_t table,
                                              uint8_t index_0,
                                              uint8_t index_1,
                                              float value)
{
    emit trace(Q_FUNC_INFO);

    //FIXME новая команда
    point_table = table;
    point_index_0 = index_0;
    point_index_1 = index_1;
    point_value = value;

    QByteArray packet = prepare_V764_2_RF_WRITE_POINT_TO_MEMORY();
    int err = send(V764_2_RF_WRITE_POINT_TO_MEMORY,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::read_firmware_version(QString *f_version)
{
    emit trace(Q_FUNC_INFO);

    //FIXME новая команда
    QByteArray packet = prepare_V764_2_RF_READ_FIRMWARE_VERSION();
    int err = send(V764_2_RF_READ_FIRMWARE_VERSION,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    //TODO прочитаем данные
    V764_2_RF_READ_FIRMWARE_VERSION_A *answer = (V764_2_RF_READ_FIRMWARE_VERSION_A *)data_radiomodule.constData();
    *f_version = QString("firmware: %1 %2 %3")
            .arg(answer->Major, 2, 16)
            .arg(answer->Minor, 2, 16)
            .arg(answer->Release, 2, 16);
    //---

    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::get_rssi(int8_t *rssi)
{
    emit trace(Q_FUNC_INFO);

    //FIXME новая команда
    QByteArray packet = prepare_V764_2_RF_GET_RSSI();
    int err = send(V764_2_RF_GET_RSSI,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    //TODO прочитаем данные
    V764_2_RF_GET_RSSI_A *answer = (V764_2_RF_GET_RSSI_A *)data_radiomodule.constData();
    *rssi = answer->rssi;

    emit update_rssi(*rssi);
    //---

    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::set_new_address(void)
{
    emit trace(Q_FUNC_INFO);

    QDialog *dlg = new QDialog();
    QGridLayout *grid = new QGridLayout();
    HexSpinBox *sb_address = new HexSpinBox();
    sb_address->setRange(0, 0xFFFF);
    QPushButton *btn_ok = new QPushButton("OK");
    connect(btn_ok, &QPushButton::clicked, dlg, &QDialog::accept);
    grid->addWidget(new QLabel("Address:"), 0, 0);
    grid->addWidget(sb_address, 0, 1);
    grid->addWidget(btn_ok, 1, 0);
    dlg->setLayout(grid);
    int btn = dlg->exec();
    if(btn != QDialog::Accepted)
    {
        return false;
    }

    set_remote_address(sb_address->value());

    //FIXME новая команда
    QByteArray packet = prepare_V764_2_RF_SET_NEW_ADDRESS();
    int err = send(V764_2_RF_SET_NEW_ADDRESS,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::set_new_address(unsigned int new_address)
{
    emit trace(Q_FUNC_INFO);

    set_remote_address(new_address);

    //FIXME новая команда
    QByteArray packet = prepare_V764_2_RF_SET_NEW_ADDRESS();
    int err = send(V764_2_RF_SET_NEW_ADDRESS,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::fw_set_default_calibration_tables(void)
{
    emit trace(Q_FUNC_INFO);

    //TODO новая команда
    QByteArray packet = prepare_V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES();
    int err = send(V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    Q_ASSERT(data_radiomodule.count() == sizeof(V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES_A));

    //TODO прочитаем данные
    V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES_A *answer = (V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES_A *)data_radiomodule.constData();
    Q_ASSERT(answer);

    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::fw_get_cnt_e21(uint16_t *cnt_e21)
{
    emit trace(Q_FUNC_INFO);

    //TODO новая команда
    QByteArray packet = prepare_V764_2_RF_GET_CNT_E21();
    int err = send(V764_2_RF_GET_CNT_E21,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    Q_ASSERT(data_radiomodule.count() == sizeof(V764_2_RF_GET_CNT_E21_A));

    //TODO прочитаем данные
    V764_2_RF_GET_CNT_E21_A *answer = (V764_2_RF_GET_CNT_E21_A *)data_radiomodule.constData();
    Q_ASSERT(answer);
    *cnt_e21 = answer->cnt_e_21;

    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::fw_test(void)
{
    emit trace(Q_FUNC_INFO);

    //TODO новая команда
    QByteArray packet = prepare_V764_2_RF_TEST();
    int err = send(V764_2_RF_TEST,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    Q_ASSERT(data_radiomodule.count() == sizeof(V764_2_RF_TEST_A));

    //TODO прочитаем данные
    V764_2_RF_TEST_A *answer = (V764_2_RF_TEST_A *)data_radiomodule.constData();
    Q_ASSERT(answer);

    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::send_values(double measuring_value)
{
    emit trace(Q_FUNC_INFO);

    emit get_measuring_value(QString("%1").arg(measuring_value));
    emit change_retention(get_retention_string());
    emit change_deviation(get_deviation_string());
    emit change_limit(get_limit_string());
}
//--------------------------------------------------------------------------------
uint8_t Multimeter_V786_2::prepare_byte_measuring(void)
{
    emit trace(Q_FUNC_INFO);

    BYTE_MEASURING byte_measuring;

    byte_measuring.data.retention = multimeter_retention;
    byte_measuring.data.deviation = multimeter_deviation;

    byte_measuring.data.auto_limit = multimeter_auto_limit;
    byte_measuring.data.limit = multimeter_limit;

    return byte_measuring.value;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::measuring_UDC(double *value)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_DC_VOLTAGE_MEASUREMENT_TEXT);

    QByteArray packet = prepare_V764_2_RF_DC_VOLTAGE_MEASUREMENT();
    int err = send(V764_2_RF_DC_VOLTAGE_MEASUREMENT,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    //---
    bool ok = false;
    double measuring_value = 0;
    ok = get_value(V764_2_RF_DC_VOLTAGE_MEASUREMENT, &measuring_value);
    if(ok)
    {
        *value = measuring_value;
        send_values(measuring_value);
        last_error = E_NO_ERROR;
        return true;
    }
    last_error = E_INVALID_FLOAT;
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::measuring_UAC(double *value)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_AC_VOLTAGE_MEASUREMENT_TEXT);

    QByteArray packet = prepare_V764_2_RF_AC_VOLTAGE_MEASUREMENT();
    int err = send(V764_2_RF_AC_VOLTAGE_MEASUREMENT,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    //---
    bool ok = false;
    double measuring_value = 0;
    ok = get_value(V764_2_RF_AC_VOLTAGE_MEASUREMENT, &measuring_value);
    if(ok)
    {
        *value = measuring_value;
        send_values(measuring_value);
        last_error = E_NO_ERROR;
        return true;
    }
    last_error = E_INVALID_FLOAT;
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::measuring_IDC(double *value)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_DC_CURRENT_MEASUREMENT_TEXT);

    QByteArray packet = prepare_V764_2_RF_DC_CURRENT_MEASUREMENT();
    int err = send(V764_2_RF_DC_CURRENT_MEASUREMENT,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    //---
    bool ok = false;
    double measuring_value = 0;
    ok = get_value(V764_2_RF_DC_CURRENT_MEASUREMENT, &measuring_value);
    if(ok)
    {
        *value = measuring_value;
        send_values(measuring_value);
        last_error = E_NO_ERROR;
        return true;
    }
    last_error = E_INVALID_FLOAT;
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::measuring_IAC(double *value)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_AC_CURRENT_MEASUREMENT_TEXT);

    QByteArray packet = prepare_V764_2_RF_AC_CURRENT_MEASUREMENT();
    int err = send(V764_2_RF_AC_CURRENT_MEASUREMENT,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    //---
    bool ok = false;
    double measuring_value = 0;
    ok = get_value(V764_2_RF_AC_CURRENT_MEASUREMENT, &measuring_value);
    if(ok)
    {
        *value = measuring_value;
        send_values(measuring_value);
        last_error = E_NO_ERROR;
        return true;
    }
    last_error = E_INVALID_FLOAT;
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::measuring_R(double *value)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_RESISTANCE_MEASUREMENT_TEXT);

    QByteArray packet = prepare_V764_2_RF_RESISTANCE_MEASUREMENT();
    int err = send(V764_2_RF_RESISTANCE_MEASUREMENT,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    //---
    bool ok = false;
    double measuring_value = 0;
    ok = get_value(V764_2_RF_RESISTANCE_MEASUREMENT, &measuring_value);
    if(ok)
    {
        *value = measuring_value;
        send_values(measuring_value);
        last_error = E_NO_ERROR;
        return true;
    }
    last_error = E_INVALID_FLOAT;
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::measuring_L(double *value)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_INDUCTANCE_MEASUREMENT_TEXT);

    QByteArray packet = prepare_V764_2_RF_INDUCTANCE_MEASUREMENT();
    int err = send(V764_2_RF_INDUCTANCE_MEASUREMENT,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    //---
    bool ok = false;
    double measuring_value = 0;
    ok = get_value(V764_2_RF_INDUCTANCE_MEASUREMENT, &measuring_value);
    if(ok)
    {
        *value = measuring_value;
        send_values(measuring_value);
        last_error = E_NO_ERROR;
        return true;
    }
    last_error = E_INVALID_FLOAT;
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::measuring_C(double *value)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_CAPACITANCE_MEASUREMENT_TEXT);

    QByteArray packet = prepare_V764_2_RF_CAPACITANCE_MEASUREMENT();
    int err = send(V764_2_RF_CAPACITANCE_MEASUREMENT,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    //---
    bool ok = false;
    double measuring_value = 0;
    ok = get_value(V764_2_RF_CAPACITANCE_MEASUREMENT, &measuring_value);
    if(ok)
    {
        *value = measuring_value;
        send_values(measuring_value);
        last_error = E_NO_ERROR;
        return true;
    }
    last_error = E_INVALID_FLOAT;
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::measuring_F(double *value)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_FREQUENCY_MEASUREMENT_TEXT);

    QByteArray packet = prepare_V764_2_RF_FREQUENCY_MEASUREMENT();
    int err = send(V764_2_RF_FREQUENCY_MEASUREMENT,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    //---
    bool ok = false;
    double measuring_value = 0;
    ok = get_value(V764_2_RF_FREQUENCY_MEASUREMENT, &measuring_value);
    if(ok)
    {
        *value = measuring_value;
        emit get_measuring_value(QString("%1").arg(measuring_value));
        emit change_retention(get_retention_string());
        emit change_deviation(get_deviation_string());
        emit change_limit(get_limit_string());
        last_error = E_NO_ERROR;
        return true;
    }
    last_error = E_INVALID_FLOAT;
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::measuring_P(double *value)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_ACTIVE_POWER_MEASUREMENT_TEXT);

    //активная
    QByteArray packet = prepare_V764_2_RF_ACTIVE_POWER_MEASUREMENT();
    int err = send(V764_2_RF_ACTIVE_POWER_MEASUREMENT,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    //---
    bool ok = false;
    double measuring_value = 0;
    ok = get_value(V764_2_RF_ACTIVE_POWER_MEASUREMENT, &measuring_value);
    if(ok)
    {
        *value = measuring_value;
        send_values(measuring_value);
        last_error = E_NO_ERROR;
        return true;
    }
    last_error = E_INVALID_FLOAT;
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::measuring_Q(double *value)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_REACTIVE_POWER_MEASUREMENT_TEXT);

    //реактивная
    QByteArray packet = prepare_V764_2_RF_REACTIVE_POWER_MEASUREMENT();
    int err = send(V764_2_RF_REACTIVE_POWER_MEASUREMENT,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    //---
    bool ok = false;
    double measuring_value = 0;
    ok = get_value(V764_2_RF_REACTIVE_POWER_MEASUREMENT, &measuring_value);
    if(ok)
    {
        *value = measuring_value;
        send_values(measuring_value);
        last_error = E_NO_ERROR;
        return true;
    }
    last_error = E_INVALID_FLOAT;
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::measuring_S(double *value)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_FULL_POWER_MEASUREMENT_TEXT);

    //полная
    QByteArray packet = prepare_V764_2_RF_FULL_POWER_MEASUREMENT();
    int err = send(V764_2_RF_FULL_POWER_MEASUREMENT,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    //---
    bool ok = false;
    double measuring_value = 0;
    ok = get_value(V764_2_RF_FULL_POWER_MEASUREMENT, &measuring_value);
    if(ok)
    {
        *value = measuring_value;
        send_values(measuring_value);
        last_error = E_NO_ERROR;
        return true;
    }
    last_error = E_INVALID_FLOAT;
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::measuring_PF(double *value)
{
    emit trace(Q_FUNC_INFO);
    emit trace(V764_2_RF_COS_FI_MEASUREMENT_TEXT);

    //коэффициент мощности
    QByteArray packet = prepare_V764_2_RF_COS_FI_MEASUREMENT();
    int err = send(V764_2_RF_COS_FI_MEASUREMENT,
                   packet,
                   sizeof(packet),
                   multimeter_serno,
                   multimeter_year);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    //---
    bool ok = false;
    double measuring_value = 0;
    ok = get_value(V764_2_RF_COS_FI_MEASUREMENT, &measuring_value);
    if(ok)
    {
        *value = measuring_value;
        send_values(measuring_value);
        last_error = E_NO_ERROR;
        return true;
    }
    last_error = E_INVALID_FLOAT;
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::find_cmd(int cmd)
{
    bool ok = false;
    double value = 0;
    int8_t rssi = 0;
    uint16_t cnt_e21 = 0;
    QString f_version;
    switch(cmd)
    {
    case V764_2_RF_STATUS_REQUEST:                              set_data_len(0);    ok = get_state();                   break;
    case V764_2_RF_REMOTE_CONTROL_ENABLE:                       set_data_len(0);    ok = remote_control_on();           break;
    case V764_2_RF_REMOTE_CONTROL_DISABLE:                      set_data_len(0);    ok = remote_control_off();          break;
    case V764_2_RF_SERVICE_ENABLE:                              set_data_len(6);    ok = service_on();                  break;
    case V764_2_RF_SERVICE_DISABLE:                             set_data_len(0);    ok = service_off();                 break;
    case V764_2_RF_POWER_OFF:                                   set_data_len(0);    ok = power_off();                   break;
    case V764_2_RF_DC_VOLTAGE_MEASUREMENT:      set_data_0(0);  set_data_len(1);    ok = measuring_UDC(&value);         break;
    case V764_2_RF_AC_VOLTAGE_MEASUREMENT:      set_data_0(0);  set_data_len(1);    ok = measuring_UAC(&value);         break;
    case V764_2_RF_DC_CURRENT_MEASUREMENT:      set_data_0(0);  set_data_len(1);    ok = measuring_IDC(&value);         break;
    case V764_2_RF_AC_CURRENT_MEASUREMENT:      set_data_0(0);  set_data_len(1);    ok = measuring_IAC(&value);         break;
    case V764_2_RF_RESISTANCE_MEASUREMENT:      set_data_0(0);  set_data_len(1);    ok = measuring_R(&value);           break;
    case V764_2_RF_CAPACITANCE_MEASUREMENT:     set_data_0(0);  set_data_len(1);    ok = measuring_C(&value);           break;
    case V764_2_RF_INDUCTANCE_MEASUREMENT:      set_data_0(0);  set_data_len(1);    ok = measuring_L(&value);           break;
    case V764_2_RF_BUZZER_TEST:                 set_data_0(0);  set_data_len(1);    ok = buzzer_test();                 break;
    case V764_2_RF_FREQUENCY_MEASUREMENT:       set_data_0(0);  set_data_len(1);    ok = measuring_F(&value);           break;
    case V764_2_RF_ACTIVE_POWER_MEASUREMENT:    set_data_0(0);  set_data_len(1);    ok = measuring_P(&value);           break;
    case V764_2_RF_REACTIVE_POWER_MEASUREMENT:  set_data_0(0);  set_data_len(1);    ok = measuring_Q(&value);           break;
    case V764_2_RF_FULL_POWER_MEASUREMENT:      set_data_0(0);  set_data_len(1);    ok = measuring_S(&value);           break;
    case V764_2_RF_COS_FI_MEASUREMENT:          set_data_0(0);  set_data_len(1);    ok = measuring_PF(&value);          break;
    case V764_2_RF_UDC_CAL:                     set_data_0(0);  set_data_len(1);    ok = calibration_UDC();             break;
    case V764_2_RF_UAC_CAL:                     set_data_0(0);  set_data_1(0);  set_data_len(2);    ok = calibration_UAC(); break;
    case V764_2_RF_IDC_CAL:                     set_data_0(0);  set_data_len(1);                    ok = calibration_IDC(); break;
    case V764_2_RF_IAC_CAL:                     set_data_0(0);  set_data_1(0);  set_data_len(2);    ok = calibration_IAC(); break;
    case V764_2_RF_RES_CAL:                     set_data_0(0);  set_data_len(1);    ok = calibration_R();               break;
    case V764_2_RF_FLASH_WRITE:                                 set_data_len(0);    ok = write_to_flash();              break;
    case V764_2_RF_DIODE_TEST:                                  set_data_len(0);    ok = test_diod();                   break;
    case V764_2_RF_UDC_CAL_ZEROES:                              set_data_len(0);    ok = udc_cal_zeroes();              break;
    case V764_2_RF_IDC_CAL_ZEROES:                              set_data_len(0);    ok = idc_cal_zeroes();              break;
    case V764_2_RF_RES_CAL_ZEROES:                              set_data_len(0);    ok = res_cal_zeroes();              break;
    case V764_2_RF_CAP_CAL_ZEROES:                              set_data_len(1);    ok = cap_cal_zeroes();              break;
    case V764_2_RF_IND_CAL_ZEROES:                              set_data_len(0);    ok = ind_cal_zeroes();              break;
    case V764_2_RF_FLASH_WRITE_ZEROES:                          set_data_len(0);    ok = flash_write_zeroes();          break;

        //TODO added 28.07.22
    case V764_2_RF_READ_POINT_FROM_MEMORY:                      set_data_len(3);    ok = read_point_from_memory(point_table,
                                                                                                                point_index_0,
                                                                                                                point_index_1,
                                                                                                                &point_value);   break;
    case V764_2_RF_WRITE_POINT_TO_MEMORY:                       set_data_len(7);    ok = write_point_to_memory(point_table,
                                                                                                               point_index_0,
                                                                                                               point_index_1,
                                                                                                               point_value);     break;

    case V764_2_RF_READ_FIRMWARE_VERSION:
        set_data_len(0);
        ok = read_firmware_version(&f_version);
        if(ok)
        {
            emit info(QString("Firmware ver: %1").arg(f_version));
        }
        break;
    case V764_2_RF_SET_NEW_ADDRESS:                             set_data_len(2);    ok = set_new_address();             break;
    case V764_2_RF_GET_RSSI:
        set_data_len(0);
        ok = get_rssi(&rssi);
        if(ok)
        {
            emit info(QString("RSSI:  %1").arg(rssi));
        }
        break;

    case V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES:
        set_data_len(0);
        ok = fw_set_default_calibration_tables();
        if(ok)
        {
            emit info("OK");
        }
        break;

    case V764_2_RF_GET_CNT_E21:
        set_data_len(0);
        ok = fw_get_cnt_e21(&cnt_e21);
        if(ok)
        {
            emit info(QString("find_cmd: Cnt E21:  %1").arg(cnt_e21));
        }
        break;

    case V764_2_RF_TEST:
        set_data_len(0);
        ok = fw_test();
        if(ok)
        {
            emit info("OK");
        }
        break;

    default:
        ok = false;
        last_error = E_ERROR_UNKNOWN_CMD;
        break;
    }
    return ok;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::get_test_state_answer(int cmd,
                                                    int count_data,
                                                    int serial_no,
                                                    int serial_year,
                                                    int range,
                                                    int mode,
                                                    int math_hold,
                                                    int batt_voltage)
{
    emit trace(Q_FUNC_INFO);

    V764_2_RF_STATUS_REQUEST_A answer;
    answer.header.address = multimeter_address;
    answer.header.cmd = cmd;
    answer.header.count_data = count_data;
    answer.SerialNo = serial_no;
    answer.SerialYear = serial_year;
    for(int n=0; n<7; n++)
        answer.DisplaySymbols[n] = '0';
    answer.Range = range;
    answer.Mode = mode;
    answer.Math_Hold = math_hold;
    answer.BattVoltage = batt_voltage;
    answer.crc16 = CRC::multimeter_crc16((uint8_t *)&answer,
                                         sizeof(answer) - SIZE_CRC);

    QByteArray ba;
    ba.clear();
    for(unsigned int n=0; n<sizeof(V764_2_RF_STATUS_REQUEST_Q); n++)
        ba.append((char *)&answer + n);

    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::get_test_service_on_answer(int cmd,
                                                         int count_data,
                                                         int serial_no,
                                                         int serial_year)
{
    emit trace(Q_FUNC_INFO);

    V764_2_RF_SERVICE_ENABLE_A answer;
    answer.header.address = multimeter_address;
    answer.header.cmd = cmd;
    answer.header.count_data = count_data;
    answer.SerialNo = serial_no;
    answer.SerialYear = serial_year;
    answer.crc16 = CRC::multimeter_crc16((uint8_t *)&answer,
                                         sizeof(answer) - SIZE_CRC);

    QByteArray ba;
    ba.clear();
    for(unsigned int n=0; n<sizeof(V764_2_RF_SERVICE_ENABLE_A); n++)
        ba.append((char *)&answer + n);

#ifdef QT_DEBUG
    qDebug() << ba.toHex();
#endif
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::get_test_power_off_answer(int cmd,
                                                        int count_data,
                                                        int serial_no,
                                                        int serial_year)
{
    emit trace(Q_FUNC_INFO);

    V764_2_RF_POWER_OFF_A answer;
    answer.header.address = multimeter_address;
    answer.header.cmd = cmd;
    answer.header.count_data = count_data;
    answer.SerialNo = serial_no;
    answer.SerialYear = serial_year;
    answer.crc16 = CRC::multimeter_crc16((uint8_t *)&answer,
                                         sizeof(answer) - SIZE_CRC);

    QByteArray ba;
    ba.clear();
    for(unsigned int n=0; n<sizeof(V764_2_RF_POWER_OFF_A); n++)
        ba.append((char *)&answer + n);

    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::get_get_id_answer(int id,
                                                int count_data)
{
    emit trace(Q_FUNC_INFO);

    V764_2_RF_SERVICE_DISABLE_A answer;
    answer.header.address = multimeter_address;
    answer.header.cmd = id;
    answer.header.count_data = count_data;
    answer.crc16 = CRC::multimeter_crc16((uint8_t *)&answer,
                                         sizeof(answer) - SIZE_CRC);

    QByteArray ba;
    ba.clear();
    for(unsigned int n=0; n<sizeof(V764_2_RF_SERVICE_DISABLE_A); n++)
        ba.append((char *)&answer + n);

    return ba;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::print_cmd_text(int cmd)
{
    switch(cmd)
    {
    case V764_2_RF_STATUS_REQUEST:              emit info(V764_2_RF_STATUS_REQUEST_TEXT);   break;
    case V764_2_RF_REMOTE_CONTROL_ENABLE:       emit info(V764_2_RF_REMOTE_CONTROL_ENABLE_TEXT);   break;
    case V764_2_RF_REMOTE_CONTROL_DISABLE:      emit info(V764_2_RF_REMOTE_CONTROL_DISABLE_TEXT);   break;
    case V764_2_RF_SERVICE_ENABLE:              emit info(V764_2_RF_SERVICE_ENABLE_TEXT);   break;
    case V764_2_RF_POWER_OFF:                   emit info(V764_2_RF_POWER_OFF_TEXT);   break;
    case V764_2_RF_DC_VOLTAGE_MEASUREMENT:      emit info(V764_2_RF_DC_VOLTAGE_MEASUREMENT_TEXT);   break;
    case V764_2_RF_AC_VOLTAGE_MEASUREMENT:      emit info(V764_2_RF_AC_VOLTAGE_MEASUREMENT_TEXT);   break;
    case V764_2_RF_DC_CURRENT_MEASUREMENT:      emit info(V764_2_RF_DC_CURRENT_MEASUREMENT_TEXT);   break;
    case V764_2_RF_AC_CURRENT_MEASUREMENT:      emit info(V764_2_RF_AC_CURRENT_MEASUREMENT_TEXT);   break;
    case V764_2_RF_RESISTANCE_MEASUREMENT:      emit info(V764_2_RF_RESISTANCE_MEASUREMENT_TEXT);   break;
    case V764_2_RF_CAPACITANCE_MEASUREMENT:     emit info(V764_2_RF_CAPACITANCE_MEASUREMENT_TEXT);   break;
    case V764_2_RF_INDUCTANCE_MEASUREMENT:      emit info(V764_2_RF_INDUCTANCE_MEASUREMENT_TEXT);   break;
    case V764_2_RF_BUZZER_TEST:                 emit info(V764_2_RF_BUZZER_TEST_TEXT);   break; //FIXME надо проверить
    case V764_2_RF_FREQUENCY_MEASUREMENT:       emit info(V764_2_RF_FREQUENCY_MEASUREMENT_TEXT);   break;
    case V764_2_RF_ACTIVE_POWER_MEASUREMENT:    emit info(V764_2_RF_ACTIVE_POWER_MEASUREMENT_TEXT);   break;
    case V764_2_RF_REACTIVE_POWER_MEASUREMENT:  emit info(V764_2_RF_REACTIVE_POWER_MEASUREMENT_TEXT);   break;
    case V764_2_RF_FULL_POWER_MEASUREMENT:      emit info(V764_2_RF_FULL_POWER_MEASUREMENT_TEXT);   break;
    case V764_2_RF_COS_FI_MEASUREMENT:          emit info(V764_2_RF_COS_FI_MEASUREMENT_TEXT);   break;
    case V764_2_RF_UDC_CAL:                     emit info(V764_2_RF_UDC_CAL_TEXT);   break;
    case V764_2_RF_UAC_CAL:                     emit info(V764_2_RF_UAC_CAL_TEXT);   break;
    case V764_2_RF_IDC_CAL:                     emit info(V764_2_RF_IDC_CAL_TEXT);   break;
    case V764_2_RF_IAC_CAL:                     emit info(V764_2_RF_IAC_CAL_TEXT);   break;
    case V764_2_RF_RES_CAL:                     emit info(V764_2_RF_RES_CAL_TEXT);   break;
    case V764_2_RF_FLASH_WRITE:                 emit info(V764_2_RF_FLASH_WRITE_TEXT);   break;
    case V764_2_RF_SERVICE_DISABLE:             emit info(V764_2_RF_SERVICE_DISABLE_TEXT);   break;
    case V764_2_RF_DIODE_TEST:                  emit info(V764_2_RF_DIODE_TEST_TEXT);   break;
    case V764_2_RF_UDC_CAL_ZEROES:              emit info(V764_2_RF_UDC_CAL_ZEROES_TEXT);   break;
    case V764_2_RF_IDC_CAL_ZEROES:              emit info(V764_2_RF_IDC_CAL_ZEROES_TEXT);   break;
    case V764_2_RF_RES_CAL_ZEROES:              emit info(V764_2_RF_RES_CAL_ZEROES_TEXT);   break;
    case V764_2_RF_CAP_CAL_ZEROES:              emit info(V764_2_RF_CAP_CAL_ZEROES_TEXT);   break;
    case V764_2_RF_IND_CAL_ZEROES:              emit info(V764_2_RF_IND_CAL_ZEROES_TEXT);   break;
    case V764_2_RF_FLASH_WRITE_ZEROES:          emit info(V764_2_RF_FLASH_WRITE_ZEROES_TEXT);   break;

        //TODO added 28.07.22
    case V764_2_RF_READ_POINT_FROM_MEMORY:      emit info(V764_2_RF_READ_POINT_FROM_MEMORY_TEXT);   break;
    case V764_2_RF_WRITE_POINT_TO_MEMORY:       emit info(V764_2_RF_WRITE_POINT_TO_MEMORY_TEXT);    break;
    case V764_2_RF_READ_FIRMWARE_VERSION:       emit info(V764_2_RF_READ_FIRMWARE_VERSION_TEXT);    break;
    case V764_2_RF_SET_NEW_ADDRESS:             emit info(V764_2_RF_SET_NEW_ADDRESS_TEXT);          break;
    case V764_2_RF_GET_RSSI:                    emit info(V764_2_RF_GET_RSSI_TEXT);                 break;
        //---

    case V764_2_RF_TEST:                        emit info(V764_2_RF_TEST_TEXT);                 break;

    case V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES:  emit info(V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES_TEXT);                 break;
    case V764_2_RF_GET_CNT_E21:                     emit info(V764_2_RF_GET_CNT_E21_TEXT);                 break;

    default:
        emit error(QString("unknown cmd: %1")
                   .arg(cmd, 2, 16, QChar('0')));
        break;
    }
}
//--------------------------------------------------------------------------------
QString Multimeter_V786_2::get_cmd_text(int cmd)
{
    QString temp;
    switch(cmd)
    {
    case V764_2_RF_STATUS_REQUEST:              temp = V764_2_RF_STATUS_REQUEST_TEXT;   break;
    case V764_2_RF_REMOTE_CONTROL_ENABLE:       temp = V764_2_RF_REMOTE_CONTROL_ENABLE_TEXT;   break;
    case V764_2_RF_REMOTE_CONTROL_DISABLE:      temp = V764_2_RF_REMOTE_CONTROL_DISABLE_TEXT;   break;
    case V764_2_RF_SERVICE_ENABLE:              temp = V764_2_RF_SERVICE_ENABLE_TEXT;   break;
    case V764_2_RF_POWER_OFF:                   temp = V764_2_RF_POWER_OFF_TEXT;   break;
    case V764_2_RF_DC_VOLTAGE_MEASUREMENT:      temp = V764_2_RF_DC_VOLTAGE_MEASUREMENT_TEXT;   break;
    case V764_2_RF_AC_VOLTAGE_MEASUREMENT:      temp = V764_2_RF_AC_VOLTAGE_MEASUREMENT_TEXT;   break;
    case V764_2_RF_DC_CURRENT_MEASUREMENT:      temp = V764_2_RF_DC_CURRENT_MEASUREMENT_TEXT;   break;
    case V764_2_RF_AC_CURRENT_MEASUREMENT:      temp = V764_2_RF_AC_CURRENT_MEASUREMENT_TEXT;   break;
    case V764_2_RF_RESISTANCE_MEASUREMENT:      temp = V764_2_RF_RESISTANCE_MEASUREMENT_TEXT;   break;
    case V764_2_RF_CAPACITANCE_MEASUREMENT:     temp = V764_2_RF_CAPACITANCE_MEASUREMENT_TEXT;   break;
    case V764_2_RF_INDUCTANCE_MEASUREMENT:      temp = V764_2_RF_INDUCTANCE_MEASUREMENT_TEXT;   break;
    case V764_2_RF_BUZZER_TEST:                 temp = V764_2_RF_BUZZER_TEST_TEXT;   break;
    case V764_2_RF_FREQUENCY_MEASUREMENT:       temp = V764_2_RF_FREQUENCY_MEASUREMENT_TEXT;   break;
    case V764_2_RF_ACTIVE_POWER_MEASUREMENT:    temp = V764_2_RF_ACTIVE_POWER_MEASUREMENT_TEXT;   break;
    case V764_2_RF_REACTIVE_POWER_MEASUREMENT:  temp = V764_2_RF_REACTIVE_POWER_MEASUREMENT_TEXT;   break;
    case V764_2_RF_FULL_POWER_MEASUREMENT:      temp = V764_2_RF_FULL_POWER_MEASUREMENT_TEXT;   break;
    case V764_2_RF_COS_FI_MEASUREMENT:          temp = V764_2_RF_COS_FI_MEASUREMENT_TEXT;   break;
    case V764_2_RF_UDC_CAL:                     temp = V764_2_RF_UDC_CAL_TEXT;   break;
    case V764_2_RF_UAC_CAL:                     temp = V764_2_RF_UAC_CAL_TEXT;   break;
    case V764_2_RF_IDC_CAL:                     temp = V764_2_RF_IDC_CAL_TEXT;   break;
    case V764_2_RF_IAC_CAL:                     temp = V764_2_RF_IAC_CAL_TEXT;   break;
    case V764_2_RF_RES_CAL:                     temp = V764_2_RF_RES_CAL_TEXT;   break;
    case V764_2_RF_FLASH_WRITE:                 temp = V764_2_RF_FLASH_WRITE_TEXT;   break;
    case V764_2_RF_SERVICE_DISABLE:             temp = V764_2_RF_SERVICE_DISABLE_TEXT;   break;
    case V764_2_RF_DIODE_TEST:                  temp = V764_2_RF_DIODE_TEST_TEXT;   break;

        //TODO added 28.07.22
    case V764_2_RF_READ_POINT_FROM_MEMORY:      temp = V764_2_RF_READ_POINT_FROM_MEMORY_TEXT;   break;
    case V764_2_RF_WRITE_POINT_TO_MEMORY:       temp = V764_2_RF_WRITE_POINT_TO_MEMORY_TEXT;    break;
    case V764_2_RF_READ_FIRMWARE_VERSION:       temp = V764_2_RF_READ_FIRMWARE_VERSION_TEXT;    break;
    case V764_2_RF_SET_NEW_ADDRESS:             temp = V764_2_RF_SET_NEW_ADDRESS_TEXT;          break;
    case V764_2_RF_GET_RSSI:                    temp = V764_2_RF_GET_RSSI_TEXT;                 break;
        //---

    case V764_2_RF_TEST:                        temp = V764_2_RF_TEST_TEXT;   break;

    case V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES:  temp = V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES_TEXT;   break;
    case V764_2_RF_GET_CNT_E21:                     temp = V764_2_RF_GET_CNT_E21_TEXT;                      break;

    default:
        temp = QString("unknown cmd %1").arg(cmd);
        break;
    }
    return temp;
}
//--------------------------------------------------------------------------------
#ifdef TEST
void Multimeter_V786_2::receive(const QVariant &data)
{
    serial->receive(data);
}
#endif
//--------------------------------------------------------------------------------
QMenu *Multimeter_V786_2::add_menu(void)
{
    QMenu *menu = new QMenu("Мультиметр В786/2");

    typedef struct
    {
        QString caption;
        int (Multimeter_V786_2::*func)(void);
    } ACTION;
    QList<ACTION> actions;

    actions.clear();
    actions.append({ V764_2_RF_STATUS_REQUEST_TEXT,             &Multimeter_V786_2::run_dialog_0x01 });
    actions.append({ V764_2_RF_REMOTE_CONTROL_ENABLE_TEXT,      &Multimeter_V786_2::run_dialog_0x02 });
    actions.append({ V764_2_RF_REMOTE_CONTROL_DISABLE_TEXT,     &Multimeter_V786_2::run_dialog_0x03 });
    actions.append({ V764_2_RF_SERVICE_ENABLE_TEXT,             &Multimeter_V786_2::run_dialog_0x04 });
    actions.append({ V764_2_RF_SERVICE_DISABLE_TEXT,            &Multimeter_V786_2::run_dialog_0x1C });
    actions.append({ V764_2_RF_POWER_OFF_TEXT,                  &Multimeter_V786_2::run_dialog_0x05 });
    actions.append({ V764_2_RF_DC_VOLTAGE_MEASUREMENT_TEXT,     &Multimeter_V786_2::run_dialog_0x06 });
    actions.append({ V764_2_RF_AC_VOLTAGE_MEASUREMENT_TEXT,     &Multimeter_V786_2::run_dialog_0x07 });
    actions.append({ V764_2_RF_DC_CURRENT_MEASUREMENT_TEXT,     &Multimeter_V786_2::run_dialog_0x08 });
    actions.append({ V764_2_RF_AC_CURRENT_MEASUREMENT_TEXT,     &Multimeter_V786_2::run_dialog_0x09 });
    actions.append({ V764_2_RF_RESISTANCE_MEASUREMENT_TEXT,     &Multimeter_V786_2::run_dialog_0x0A });
    actions.append({ V764_2_RF_CAPACITANCE_MEASUREMENT_TEXT,    &Multimeter_V786_2::run_dialog_0x0B });
    actions.append({ V764_2_RF_INDUCTANCE_MEASUREMENT_TEXT,     &Multimeter_V786_2::run_dialog_0x0C });
    actions.append({ V764_2_RF_BUZZER_TEST_TEXT,                &Multimeter_V786_2::run_dialog_0x0D });
    actions.append({ V764_2_RF_FREQUENCY_MEASUREMENT_TEXT,      &Multimeter_V786_2::run_dialog_0x0E });
    actions.append({ V764_2_RF_ACTIVE_POWER_MEASUREMENT_TEXT,   &Multimeter_V786_2::run_dialog_0x0F });
    actions.append({ V764_2_RF_REACTIVE_POWER_MEASUREMENT_TEXT, &Multimeter_V786_2::run_dialog_0x10 });
    actions.append({ V764_2_RF_FULL_POWER_MEASUREMENT_TEXT,     &Multimeter_V786_2::run_dialog_0x11 });
    actions.append({ V764_2_RF_COS_FI_MEASUREMENT_TEXT,         &Multimeter_V786_2::run_dialog_0x12 });
    actions.append({ V764_2_RF_UDC_CAL_TEXT,                    &Multimeter_V786_2::run_dialog_0x13 });
    actions.append({ V764_2_RF_UAC_CAL_TEXT,                    &Multimeter_V786_2::run_dialog_0x14 });
    actions.append({ V764_2_RF_IDC_CAL_TEXT,                    &Multimeter_V786_2::run_dialog_0x15 });
    actions.append({ V764_2_RF_IAC_CAL_TEXT,                    &Multimeter_V786_2::run_dialog_0x16 });
    actions.append({ V764_2_RF_RES_CAL_TEXT,                    &Multimeter_V786_2::run_dialog_0x17 });
    actions.append({ V764_2_RF_FLASH_WRITE_TEXT,                &Multimeter_V786_2::run_dialog_0x1B });
    actions.append({ V764_2_RF_DIODE_TEST_TEXT,                 &Multimeter_V786_2::run_dialog_0x1D });
    actions.append({ V764_2_RF_UDC_CAL_ZEROES_TEXT,             &Multimeter_V786_2::run_dialog_0x1E });
    actions.append({ V764_2_RF_IDC_CAL_ZEROES_TEXT,             &Multimeter_V786_2::run_dialog_0x1F });
    actions.append({ V764_2_RF_RES_CAL_ZEROES_TEXT,             &Multimeter_V786_2::run_dialog_0x20 });
    actions.append({ V764_2_RF_CAP_CAL_ZEROES_TEXT,             &Multimeter_V786_2::run_dialog_0x21 });
    actions.append({ V764_2_RF_IND_CAL_ZEROES_TEXT,             &Multimeter_V786_2::run_dialog_0x22 });
    actions.append({ V764_2_RF_FLASH_WRITE_ZEROES_TEXT,         &Multimeter_V786_2::run_dialog_0x23 });

    //TODO added 28.07.22
    actions.append({ V764_2_RF_READ_POINT_FROM_MEMORY_TEXT,     &Multimeter_V786_2::run_dialog_0x50 });
    actions.append({ V764_2_RF_WRITE_POINT_TO_MEMORY_TEXT,      &Multimeter_V786_2::run_dialog_0x51 });
    actions.append({ V764_2_RF_READ_FIRMWARE_VERSION_TEXT, &Multimeter_V786_2::run_dialog_0x52 });
    //actions.append({ V764_2_RF_SET_NEW_ADDRESS_TEXT,            &Multimeter_V786_2::run_dialog_0x53 });
    actions.append({ V764_2_RF_GET_RSSI_TEXT,                   &Multimeter_V786_2::run_dialog_0x54 });

    //TODO added 27.09.22
    actions.append({ V764_2_RF_TEST_TEXT,                       &Multimeter_V786_2::run_dialog_0x57 });

    actions.append({ V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES_TEXT, &Multimeter_V786_2::run_dialog_0x56 });
    actions.append({ V764_2_RF_GET_CNT_E21_TEXT,                    &Multimeter_V786_2::run_dialog_0x55 });
    //---

    foreach (ACTION action, actions)
    {
        QAction *act = new QAction(action.caption, this);
        connect(act, &QAction::triggered, this, action.func);

        menu->addAction(act);
    }
    return menu;
}
//--------------------------------------------------------------------------------
QToolBar *Multimeter_V786_2::add_test_toolbar(void)
{
    QToolBar *tb = new QToolBar();

    typedef struct
    {
        QString caption;
        int (Multimeter_V786_2::*func)(void);
    } BUTTON;
    QList<BUTTON> buttons;

    buttons.clear();
    buttons.append({ V764_2_RF_STATUS_REQUEST_TEXT,             &Multimeter_V786_2::run_dialog_0x01 });
    buttons.append({ V764_2_RF_REMOTE_CONTROL_ENABLE_TEXT,      &Multimeter_V786_2::run_dialog_0x02 });
    buttons.append({ V764_2_RF_REMOTE_CONTROL_DISABLE_TEXT,     &Multimeter_V786_2::run_dialog_0x03 });
    buttons.append({ V764_2_RF_SERVICE_ENABLE_TEXT,             &Multimeter_V786_2::run_dialog_0x04 });
    buttons.append({ V764_2_RF_SERVICE_DISABLE_TEXT,            &Multimeter_V786_2::run_dialog_0x1C });
    buttons.append({ V764_2_RF_POWER_OFF_TEXT,                  &Multimeter_V786_2::run_dialog_0x05 });
    buttons.append({ V764_2_RF_DC_VOLTAGE_MEASUREMENT_TEXT,     &Multimeter_V786_2::run_dialog_0x06 });
    buttons.append({ V764_2_RF_AC_VOLTAGE_MEASUREMENT_TEXT,     &Multimeter_V786_2::run_dialog_0x07 });
    buttons.append({ V764_2_RF_DC_CURRENT_MEASUREMENT_TEXT,     &Multimeter_V786_2::run_dialog_0x08 });
    buttons.append({ V764_2_RF_AC_CURRENT_MEASUREMENT_TEXT,     &Multimeter_V786_2::run_dialog_0x09 });
    buttons.append({ V764_2_RF_RESISTANCE_MEASUREMENT_TEXT,     &Multimeter_V786_2::run_dialog_0x0A });
    buttons.append({ V764_2_RF_CAPACITANCE_MEASUREMENT_TEXT,    &Multimeter_V786_2::run_dialog_0x0B });
    buttons.append({ V764_2_RF_INDUCTANCE_MEASUREMENT_TEXT,     &Multimeter_V786_2::run_dialog_0x0C });
    buttons.append({ V764_2_RF_BUZZER_TEST_TEXT,                &Multimeter_V786_2::run_dialog_0x0D });
    buttons.append({ V764_2_RF_FREQUENCY_MEASUREMENT_TEXT,      &Multimeter_V786_2::run_dialog_0x0E });
    buttons.append({ V764_2_RF_ACTIVE_POWER_MEASUREMENT_TEXT,   &Multimeter_V786_2::run_dialog_0x0F });
    buttons.append({ V764_2_RF_REACTIVE_POWER_MEASUREMENT_TEXT, &Multimeter_V786_2::run_dialog_0x10 });
    buttons.append({ V764_2_RF_FULL_POWER_MEASUREMENT_TEXT,     &Multimeter_V786_2::run_dialog_0x11 });
    buttons.append({ V764_2_RF_COS_FI_MEASUREMENT_TEXT,         &Multimeter_V786_2::run_dialog_0x12 });
    buttons.append({ V764_2_RF_UDC_CAL_TEXT,                    &Multimeter_V786_2::run_dialog_0x13 });
    buttons.append({ V764_2_RF_UAC_CAL_TEXT,                    &Multimeter_V786_2::run_dialog_0x14 });
    buttons.append({ V764_2_RF_IDC_CAL_TEXT,                    &Multimeter_V786_2::run_dialog_0x15 });
    buttons.append({ V764_2_RF_IAC_CAL_TEXT,                    &Multimeter_V786_2::run_dialog_0x16 });
    buttons.append({ V764_2_RF_RES_CAL_TEXT,                    &Multimeter_V786_2::run_dialog_0x17 });
    buttons.append({ V764_2_RF_FLASH_WRITE_TEXT,                &Multimeter_V786_2::run_dialog_0x1B });
    buttons.append({ V764_2_RF_DIODE_TEST_TEXT,                 &Multimeter_V786_2::run_dialog_0x1D });
    buttons.append({ V764_2_RF_UDC_CAL_ZEROES_TEXT,             &Multimeter_V786_2::run_dialog_0x1E });
    buttons.append({ V764_2_RF_IDC_CAL_ZEROES_TEXT,             &Multimeter_V786_2::run_dialog_0x1F });
    buttons.append({ V764_2_RF_CAP_CAL_ZEROES_TEXT,             &Multimeter_V786_2::run_dialog_0x21 });
    buttons.append({ V764_2_RF_IND_CAL_ZEROES_TEXT,             &Multimeter_V786_2::run_dialog_0x22 });
    buttons.append({ V764_2_RF_FLASH_WRITE_ZEROES_TEXT,         &Multimeter_V786_2::run_dialog_0x23 });

    //TODO added 28.07.22
    buttons.append({ V764_2_RF_READ_POINT_FROM_MEMORY_TEXT,     &Multimeter_V786_2::run_dialog_0x50 });
    buttons.append({ V764_2_RF_WRITE_POINT_TO_MEMORY_TEXT,      &Multimeter_V786_2::run_dialog_0x51 });
    buttons.append({ V764_2_RF_READ_FIRMWARE_VERSION_TEXT, &Multimeter_V786_2::run_dialog_0x52 });
    //buttons.append({ V764_2_RF_SET_NEW_ADDRESS_TEXT,            &Multimeter_V786_2::run_dialog_0x53 });
    buttons.append({ V764_2_RF_GET_RSSI_TEXT,                   &Multimeter_V786_2::run_dialog_0x54 });

    //TODO added 27.09.22
    buttons.append({ V764_2_RF_TEST_TEXT,                       &Multimeter_V786_2::run_dialog_0x57 });

    buttons.append({ V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES_TEXT, &Multimeter_V786_2::run_dialog_0x56 });
    buttons.append({ V764_2_RF_GET_CNT_E21_TEXT,                    &Multimeter_V786_2::run_dialog_0x55 });

    QVBoxLayout *vbox = new QVBoxLayout();

    sb_serno   = new QSpinBox();
    sb_year    = new QSpinBox();
    sb_address = new QSpinBox();
    sb_channel = new QSpinBox();
    connect(sb_serno,   static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,       &Multimeter_V786_2::set_serno);
    connect(sb_year,    static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,       &Multimeter_V786_2::set_year);
    connect(sb_address, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,       &Multimeter_V786_2::set_address);
    connect(sb_channel, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,       &Multimeter_V786_2::set_channel);

    connect(this,       &Multimeter_V786_2::update_serno,
            sb_serno,   &QSpinBox::setValue);
    connect(this,       &Multimeter_V786_2::update_year,
            sb_year,    &QSpinBox::setValue);

    sb_address->setMinimum(0);
    sb_address->setMaximum(255);

    sb_serno->setMinimum(0);
    sb_serno->setMaximum(9999);

    sb_year->setMinimum(2010);
    sb_year->setMaximum(2100);

    sb_channel->setMinimum(1);
    sb_channel->setMaximum(15);

    int t_address = 0;
    int t_serno = 0;
    int t_year = 0;
    int t_channel = 0;
    MySettings *settings = new MySettings();
    if(settings->load_int(P_ADDRESS, &t_address))
        sb_address->setValue(t_address);
    if(settings->load_int(P_SERNO, &t_serno))
        sb_serno->setValue(t_serno);
    if(settings->load_int(P_YEAR, &t_year))
        sb_year->setValue(t_year);
    if(settings->load_int(P_CHANNEL, &t_channel))
        sb_channel->setValue(t_channel);
    delete settings;

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel("serno"),   0, 0);
    grid->addWidget(sb_serno,              0, 1);
    grid->addWidget(new QLabel("year"),    1, 0);
    grid->addWidget(sb_year,               1, 1);
    grid->addWidget(new QLabel("address"), 2, 0);
    grid->addWidget(sb_address,            2, 1);
    grid->addWidget(new QLabel("channel"), 3, 0);
    grid->addWidget(sb_channel,            3, 1);

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addLayout(grid);
    hbox->addStretch(1);

    vbox->addLayout(hbox);

    QGridLayout *buttons_grid = new QGridLayout();
    buttons_grid->setSpacing(0);
    buttons_grid->setMargin(0);
    int row = 0;
    int col = 0;
    foreach (BUTTON button, buttons)
    {
        QPushButton *btn = new QPushButton(button.caption);
        btn->setSizePolicy(QSizePolicy::Expanding,  QSizePolicy::Fixed);

        toolbar_buttons.append(btn);
        connect(btn, &QPushButton::clicked, this, button.func);
        connect(btn, &QPushButton::clicked, this, &Multimeter_V786_2::toolbar_btn_click);

        buttons_grid->addWidget(btn, row, col);
        row++;
        if(row>18)
        {
            row=0;
            col++;
        }
        //vbox->addWidget(btn);
    }
    vbox->addLayout(buttons_grid);

    QWidget *w = new QWidget();
    w->setLayout(vbox);

    tb->addWidget(w);
    return tb;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::toolbar_btn_click(void)
{
    QPushButton *btn = reinterpret_cast<QPushButton *>(sender());
    if(!btn)
        return;

    foreach (QPushButton *t_btn, toolbar_buttons)
    {
        if(t_btn == btn)
        {
            t_btn->setStyleSheet("background:red; color:white;");
        }
        else
        {
            t_btn->setStyleSheet("");
        }
    }
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::choice_options(int cmd,
                                       int *serno_value,
                                       int *year_value,
                                       int *address_value,
                                       int *channel_value)
{
    emit trace(Q_FUNC_INFO);

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

    address->setValue(*address_value);
    serno->setValue(*serno_value);
    year->setValue(*year_value);
    channel->setValue(*channel_value);

    set_serno(static_cast<uint>(serno->value()));
    set_year(static_cast<uint>(year->value()));
    set_address(static_cast<uint>(address->value()));
    set_channel(static_cast<uint>(channel->value()));
    set_timeout(multimeter_timeout);

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
    vbox->addWidget(new QLabel(get_cmd_text(cmd)));
    vbox->addLayout(grid);

    QDialog *dlg = new QDialog();
    dlg->setLayout(vbox);
    //dlg->setFixedSize(dlg->sizeHint();

    connect(btn_ok,     &QPushButton::clicked,
            dlg,        &QDialog::accept);
    connect(btn_cancel, &QPushButton::clicked,
            dlg,        &QDialog::reject);

    int button = dlg->exec();
    if(button == QDialog::Accepted)
    {
        *serno_value = serno->value();
        *year_value  = year->value();
        *address_value = address->value();
        *channel_value = channel->value();
    }
    return (button == QDialog::Accepted);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::send_cmd(int cmd)
{
    emit trace(Q_FUNC_INFO);

    if(!serial->isOpen())
    {
        last_error = E_ERROR_PORT_NOT_OPEN;
        return last_error;
    }

    bool ok = false;

    int serno_value = get_serno();
    int year_value = get_year();
    int address_value = get_address();
    int channel_value = get_channel();

    set_serno(static_cast<uint>(serno_value));
    set_year(static_cast<uint>(year_value));
    set_address(static_cast<uint>(address_value));
    set_channel(static_cast<uint>(channel_value));

    print_cmd_text(cmd);
    ok = find_cmd(cmd);
    if(!ok)
    {
        //return last_error;
        return print_last_error();
    }
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_STATUS_REQUEST(void)
{
    Q_ASSERT(multimeter_data_len == 0);

    V764_2_RF_STATUS_REQUEST_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_STATUS_REQUEST;
    question.header.count_data = multimeter_data_len;
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_REMOTE_CONTROL_ENABLE(void)
{
    Q_ASSERT(multimeter_data_len == 0);

    V764_2_RF_REMOTE_CONTROL_ENABLE_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_REMOTE_CONTROL_ENABLE;
    question.header.count_data = multimeter_data_len;
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_REMOTE_CONTROL_DISABLE(void)
{
    Q_ASSERT(multimeter_data_len == 0);

    V764_2_RF_REMOTE_CONTROL_DISABLE_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_REMOTE_CONTROL_DISABLE;
    question.header.count_data = multimeter_data_len;
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_SERVICE_ENABLE(void)
{
    Q_ASSERT(multimeter_data_len == 6);

    V764_2_RF_SERVICE_ENABLE_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_SERVICE_ENABLE;
    question.header.count_data = 6;
    question.password[0] = '7';
    question.password[1] = 'e';
    question.password[2] = '2';
    question.password[3] = 'c';
    question.password[4] = 'd';
    question.password[5] = '5';
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_POWER_OFF(void)
{
    Q_ASSERT(multimeter_data_len == 0);

    V764_2_RF_POWER_OFF_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_POWER_OFF;
    question.header.count_data = multimeter_data_len;
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_DC_VOLTAGE_MEASUREMENT(void)
{
    Q_ASSERT(multimeter_data_len == 1);

    V764_2_RF_DC_VOLTAGE_MEASUREMENT_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_DC_VOLTAGE_MEASUREMENT;
    question.header.count_data = multimeter_data_len;
    question.data_0 = prepare_byte_measuring();
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_AC_VOLTAGE_MEASUREMENT(void)
{
    Q_ASSERT(multimeter_data_len == 1);

    V764_2_RF_AC_VOLTAGE_MEASUREMENT_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_AC_VOLTAGE_MEASUREMENT;
    question.header.count_data = multimeter_data_len;
    question.data_0 = prepare_byte_measuring();
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_DC_CURRENT_MEASUREMENT(void)
{
    Q_ASSERT(multimeter_data_len == 1);

    V764_2_RF_DC_CURRENT_MEASUREMENT_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_DC_CURRENT_MEASUREMENT;
    question.header.count_data = multimeter_data_len;
    question.data_0 = prepare_byte_measuring();
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_AC_CURRENT_MEASUREMENT(void)
{
    Q_ASSERT(multimeter_data_len == 1);

    V764_2_RF_AC_CURRENT_MEASUREMENT_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_AC_CURRENT_MEASUREMENT;
    question.header.count_data = multimeter_data_len;
    question.data_0 = prepare_byte_measuring();
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_RESISTANCE_MEASUREMENT(void)
{
    Q_ASSERT(multimeter_data_len == 1);

    V764_2_RF_RESISTANCE_MEASUREMENT_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_RESISTANCE_MEASUREMENT;
    question.header.count_data = multimeter_data_len;
    question.data_0 = prepare_byte_measuring();
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_CAPACITANCE_MEASUREMENT(void)
{
    Q_ASSERT(multimeter_data_len == 1);

    V764_2_RF_CAPACITANCE_MEASUREMENT_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_CAPACITANCE_MEASUREMENT;
    question.header.count_data = multimeter_data_len;
    question.data_0 = prepare_byte_measuring();
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_INDUCTANCE_MEASUREMENT(void)
{
    Q_ASSERT(multimeter_data_len == 1);

    V764_2_RF_INDUCTANCE_MEASUREMENT_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_INDUCTANCE_MEASUREMENT;
    question.header.count_data = multimeter_data_len;
    question.data_0 = prepare_byte_measuring();
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_BUZZER_TEST(void)
{
    Q_ASSERT(multimeter_data_len == 1);

    V764_2_RF_BUZZER_TEST_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;

    question.data_0 = 0;
    question.header.cmd = V764_2_RF_BUZZER_TEST;
    question.header.count_data = multimeter_data_len;
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_FREQUENCY_MEASUREMENT(void)
{
    Q_ASSERT(multimeter_data_len == 1);

    V764_2_RF_FREQUENCY_MEASUREMENT_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_FREQUENCY_MEASUREMENT;
    question.header.count_data = multimeter_data_len;
    question.data_0 = prepare_byte_measuring();
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_ACTIVE_POWER_MEASUREMENT(void)
{
    Q_ASSERT(multimeter_data_len == 1);

    V764_2_RF_ACTIVE_POWER_MEASUREMENT_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_ACTIVE_POWER_MEASUREMENT;
    question.header.count_data = multimeter_data_len;
    question.data_0 = prepare_byte_measuring();
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_REACTIVE_POWER_MEASUREMENT(void)
{
    Q_ASSERT(multimeter_data_len == 1);

    V764_2_RF_REACTIVE_POWER_MEASUREMENT_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_REACTIVE_POWER_MEASUREMENT;
    question.header.count_data = multimeter_data_len;
    question.data_0 = prepare_byte_measuring();
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_FULL_POWER_MEASUREMENT(void)
{
    Q_ASSERT(multimeter_data_len == 1);

    V764_2_RF_FULL_POWER_MEASUREMENT_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_FULL_POWER_MEASUREMENT;
    question.header.count_data = multimeter_data_len;
    question.data_0 = prepare_byte_measuring();
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_COS_FI_MEASUREMENT(void)
{
    Q_ASSERT(multimeter_data_len == 1);

    V764_2_RF_COS_FI_MEASUREMENT_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_COS_FI_MEASUREMENT;
    question.header.count_data = multimeter_data_len;
    question.data_0 = prepare_byte_measuring();
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_UDC_CAL(void)
{
    Q_ASSERT(multimeter_data_len == 1);

    V764_2_RF_UDC_CAL_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_UDC_CAL;
    question.header.count_data = multimeter_data_len;
    question.data_0 = multimeter_data_0;
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_UAC_CAL(void)
{
    Q_ASSERT(multimeter_data_len == 2);

    V764_2_RF_UAC_CAL_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_UAC_CAL;
    question.header.count_data = multimeter_data_len;
    question.data_0 = multimeter_data_0;
    question.data_1 = multimeter_data_1;
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_IDC_CAL(void)
{
    Q_ASSERT(multimeter_data_len == 1);

    V764_2_RF_IDC_CAL_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_IDC_CAL;
    question.header.count_data = multimeter_data_len;
    question.data_0 = multimeter_data_0;
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_IAC_CAL(void)
{
    Q_ASSERT(multimeter_data_len == 2);

    V764_2_RF_IAC_CAL_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_IAC_CAL;
    question.header.count_data = multimeter_data_len;
    question.data_0 = multimeter_data_0;
    question.data_1 = multimeter_data_1;
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_RES_CAL(void)
{
    Q_ASSERT(multimeter_data_len == 1);

    V764_2_RF_RES_CAL_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_RES_CAL;
    question.header.count_data = multimeter_data_len;
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_FLASH_WRITE(void)
{
    Q_ASSERT(multimeter_data_len == 0);

    V764_2_RF_FLASH_WRITE_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_FLASH_WRITE;
    question.header.count_data = multimeter_data_len;
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_SERVICE_DISABLE(void)
{
    Q_ASSERT(multimeter_data_len == 0);

    V764_2_RF_SERVICE_DISABLE_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_SERVICE_DISABLE;
    question.header.count_data = multimeter_data_len;
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_DIODE_TEST(void)
{
    Q_ASSERT(multimeter_data_len == 0);

    V764_2_RF_DIODE_TEST_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_DIODE_TEST;
    question.header.count_data = multimeter_data_len;
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_UDC_CAL_ZEROES(void)
{
    Q_ASSERT(multimeter_data_len == 1);

    V764_2_RF_UDC_CAL_ZEROES_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_UDC_CAL_ZEROES;
    question.header.count_data = multimeter_data_len;
    question.data_0 = multimeter_data_0;
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_IDC_CAL_ZEROES(void)
{
    Q_ASSERT(multimeter_data_len == 1);

    V764_2_RF_IDC_CAL_ZEROES_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_IDC_CAL_ZEROES;
    question.header.count_data = multimeter_data_len;
    question.data_0 = multimeter_data_0;
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_RES_CAL_ZEROES(void)
{
    Q_ASSERT(multimeter_data_len == 1);

    V764_2_RF_RES_CAL_ZEROES_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_RES_CAL_ZEROES;
    question.header.count_data = multimeter_data_len;
    question.data_0 = multimeter_data_0;
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_CAP_CAL_ZEROES(void)
{
    V764_2_RF_CAP_CAL_ZEROES_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_CAP_CAL_ZEROES;
    question.header.count_data = multimeter_data_len;
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_IND_CAL_ZEROES(void)
{
    V764_2_RF_IND_CAL_ZEROES_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_IND_CAL_ZEROES;
    question.header.count_data = multimeter_data_len;
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_FLASH_WRITE_ZEROES(void)
{
    Q_ASSERT(multimeter_data_len == 0);

    V764_2_RF_FLASH_WRITE_ZEROES_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_FLASH_WRITE_ZEROES;
    question.header.count_data = multimeter_data_len;
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_READ_POINT_FROM_MEMORY(void)
{
    Q_ASSERT(multimeter_data_len == 3);

    V764_2_RF_READ_POINT_FROM_MEMORY_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_READ_POINT_FROM_MEMORY;
    question.header.count_data = multimeter_data_len;

    question.point_table = point_table;
    question.point_index_0 = point_index_0;
    question.point_index_1 = point_index_1;

    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_WRITE_POINT_TO_MEMORY(void)
{
    Q_ASSERT(multimeter_data_len == 7);

    V764_2_RF_WRITE_POINT_TO_MEMORY_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_WRITE_POINT_TO_MEMORY;
    question.header.count_data = multimeter_data_len;

    question.point_table = point_table;
    question.point_index_0 = point_index_0;
    question.point_index_1 = point_index_1;
    question.point = point_value;

    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_READ_FIRMWARE_VERSION(void)
{
    Q_ASSERT(multimeter_data_len == 0);

    V764_2_RF_READ_FIRMWARE_VERSION_Q question;
    question.header.address = multimeter_address;
    question.channel = multimeter_channel;
    question.header.cmd = V764_2_RF_READ_FIRMWARE_VERSION;
    question.header.count_data = multimeter_data_len;
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_SET_NEW_ADDRESS(void)
{
    Q_ASSERT(multimeter_data_len == 2);

    V764_2_RF_SET_NEW_ADDRESS_Q question;
    question.header.address = multimeter_address;
    question.header.cmd = V764_2_RF_SET_NEW_ADDRESS;
    question.header.count_data = multimeter_data_len;
    question.remote_address = remote_address;
    question.channel = multimeter_channel;
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_GET_RSSI(void)
{
    Q_ASSERT(multimeter_data_len == 0);

    V764_2_RF_GET_RSSI_Q question;
    question.header.address = multimeter_address;
    question.header.cmd = V764_2_RF_GET_RSSI;
    question.header.count_data = multimeter_data_len;
    question.channel = multimeter_channel;
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_TEST(void)
{
    Q_ASSERT(multimeter_data_len == 0);

    V764_2_RF_TEST_Q question;
    question.header.address = multimeter_address;
    question.header.cmd = V764_2_RF_TEST;
    question.header.count_data = multimeter_data_len;
    question.channel = multimeter_channel;
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_GET_CNT_E21(void)
{
    Q_ASSERT(multimeter_data_len == 0);

    V764_2_RF_GET_CNT_E21_Q question;
    question.header.address = multimeter_address;
    question.header.cmd = V764_2_RF_GET_CNT_E21;
    question.header.count_data = multimeter_data_len;
    question.channel = multimeter_channel;
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::prepare_V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES(void)
{
    Q_ASSERT(multimeter_data_len == 0);

    V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES_Q question;
    question.header.address = multimeter_address;
    question.header.cmd = V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES;
    question.header.count_data = multimeter_data_len;
    question.channel = multimeter_channel;
    question.crc16 = CRC::multimeter_crc16((uint8_t *)&question,
                                           sizeof(question) - SIZE_CRC);
    question.channel = multimeter_channel;

    QByteArray ba;
    ba.clear();
    ba.append((char *)&question, sizeof(question));
    return ba;
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x01(void)
{
    return send_cmd(V764_2_RF_STATUS_REQUEST);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x02(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_REMOTE_CONTROL_ENABLE);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x03(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_REMOTE_CONTROL_DISABLE);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x04(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_SERVICE_ENABLE);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x05(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_POWER_OFF);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x06(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_DC_VOLTAGE_MEASUREMENT);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x07(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_AC_VOLTAGE_MEASUREMENT);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x08(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_DC_CURRENT_MEASUREMENT);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x09(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_AC_CURRENT_MEASUREMENT);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x0A(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_RESISTANCE_MEASUREMENT);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x0B(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_CAPACITANCE_MEASUREMENT);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x0C(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_INDUCTANCE_MEASUREMENT);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x0D(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_BUZZER_TEST);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x0E(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_FREQUENCY_MEASUREMENT);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x0F(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_ACTIVE_POWER_MEASUREMENT);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x10(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_REACTIVE_POWER_MEASUREMENT);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x11(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_FULL_POWER_MEASUREMENT);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x12(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_COS_FI_MEASUREMENT);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x13(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_UDC_CAL);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x14(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_UAC_CAL);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x15(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_IDC_CAL);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x16(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_IAC_CAL);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x17(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_RES_CAL);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x1B(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_FLASH_WRITE);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x1C(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_SERVICE_DISABLE);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x1D(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_DIODE_TEST);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x1E(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_UDC_CAL_ZEROES);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x1F(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_IDC_CAL_ZEROES);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x20(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_RES_CAL_ZEROES);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x21(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_CAP_CAL_ZEROES);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x22(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_IND_CAL_ZEROES);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x23(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_FLASH_WRITE_ZEROES);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x50(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_READ_POINT_FROM_MEMORY);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x51(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_WRITE_POINT_TO_MEMORY);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x52(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_READ_FIRMWARE_VERSION);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x53(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_SET_NEW_ADDRESS);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x54(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_GET_RSSI);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x55(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_GET_CNT_E21);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x56(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x57(void)
{
    emit trace(Q_FUNC_INFO);
    return send_cmd(V764_2_RF_TEST);
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::save_setting(void)
{

}
//--------------------------------------------------------------------------------
