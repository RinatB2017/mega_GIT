/*********************************************************************************
**                                                                              **
**     Copyright (C) 2023                                                       **
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
#include "mywidget.hpp"
#include "dac32.hpp"
//--------------------------------------------------------------------------------
DAC32::DAC32(QWidget *parent) :
    MyWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
DAC32::~DAC32()
{
    if(serial)
    {
        serial->serial_close();
        delete serial;
    }
}
//--------------------------------------------------------------------------------
void DAC32::init(void)
{
    serial = new SerialBox5_wo_form(this);
    connect(serial, &SerialBox5_wo_form::port_is_active,
            this,   &DAC32::port_is_active);

    connect(serial, &SerialBox5_wo_form::output,
            this,   &DAC32::read_data);
}
//--------------------------------------------------------------------------------
void DAC32::read_data(QByteArray ba)
{
    if(ba.isEmpty())
    {
        return;
    }

    emit debug(QString("DAC: read %1 bytes").arg(ba.length()));
    emit debug(ba.toHex());

    dac_data = ba;
    data_is_ready = true;
}
//--------------------------------------------------------------------------------
bool DAC32::port_is_open(void)
{
    return serial->isOpen();
}
//--------------------------------------------------------------------------------
void DAC32::f_dac_send(QByteArray data)
{
    if(data.isEmpty())
    {
        emit error("No data to send!");
        return;
    }
    emit debug(QString("send to dac: %1")
               .arg(data.toHex().toUpper().data()));

    if(serial)
    {
        dac_data.clear();
        data_is_ready = false;
        serial->write(data);
    }
}
//--------------------------------------------------------------------------------
uint8_t DAC32::crc8(uint8_t *buf,uint8_t offset, uint8_t length)
{
    uint8_t l;
    if(offset<length)
    {
        buf+=offset;
        l= length - offset;
    }
    else l= length;
    uint8_t crc = 0xff;
    int i=0;
    while (l--)
    {
        crc ^= *buf++;
        for (i = 0; i < 8; i++)
            crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
    }
    return crc;
}
//--------------------------------------------------------------------------------
template<typename T>
bool DAC32::check_answer(int cmd)
{
    if(dac_data.isEmpty())
    {
        emit error("Data is empty");
        return false;
    }
    if(dac_data.length() != sizeof(T))
    {
        emit error("Bad len answer");
        return false;
    }

    T *answer = reinterpret_cast<T *>(dac_data.data());

    uint16_t crc = crc8(reinterpret_cast<uint8_t *>(answer),
                        0,
                        sizeof(*answer) - sizeof(answer->crc8));
    uint16_t crc8 = answer->crc8;

    if(crc != crc8)
    {
        emit error("bad crc");
        emit error(QString("crc:  0x%1").arg(QString::number(crc, 16)));
        emit error(QString("crc8: 0x%1").arg(QString::number(crc8, 16)));

        return false;
    }

    if(answer->header.address != MY_ADDR)
    {
        emit error(QString("Bad address: %1").arg(answer->header.address));
        return false;
    }
    if(answer->header.cmd != cmd)
    {
        emit error(QString("Bad cmd: %1").arg(answer->header.cmd));
        return false;
    }

    return true;
}
//--------------------------------------------------------------------------------
bool DAC32::f_GET_ID(void)
{
    GET_ID_Q question;
    question.header.address = MY_ADDR;
    question.header.cmd = GET_ID;
    question.header.count_data = 0;
    question.crc8 = crc8((uint8_t *)&question,
                         0,
                         sizeof(question) - sizeof(question.crc8));

    QByteArray packet;
    packet.append((char *)&question, sizeof(question));
    f_dac_send(packet);

    // анализируем ответ от DAC
#ifdef FAST_DAC_COMMAND
    wait_msec(DAC_WAIT_MS);
#else
    wait_msec(DAC_WAIT_MS, true);
#endif

    bool ok = check_answer<GET_ID_A>(GET_ID);
    if(!ok)
    {
        return false;
    }

    GET_ID_A *answer = reinterpret_cast<GET_ID_A *>(dac_data.data());
    Q_ASSERT(answer);

    if(answer->data.id != MY_ID)
    {
        emit error(QString("BAD ID: %1").arg(answer->data.id));
        return false;
    }

    emit debug(QString("addr %1").arg(answer->header.address));
    emit debug(QString("cmd  %1").arg(answer->header.cmd));
    emit debug(QString("cnt  %1").arg(answer->header.count_data));
    emit info(QString("id %1").arg(answer->data.id));
    emit debug(QString("crc8 0x%1").arg(answer->crc8, 2, 16, QChar('0')));

    if(is_silence == false)
    {
        emit info("OK");
    }
    return true;
}
//--------------------------------------------------------------------------------
bool DAC32::f_GET_SWITCH(void)
{
    GET_SWITCH_Q question;
    question.header.address = MY_ADDR;
    question.header.cmd = GET_SWITCH;
    question.header.count_data = sizeof(question.data);

    question.crc8 = crc8((uint8_t *)&question,
                         0,
                         sizeof(question) - sizeof(question.crc8));

    QByteArray packet;
    packet.append((char *)&question, sizeof(question));
    f_dac_send(packet);

    // анализируем ответ от DAC
#ifdef FAST_DAC_COMMAND
    wait_msec(DAC_WAIT_MS);
#else
    wait_msec(DAC_WAIT_MS, true);
#endif

    bool ok = check_answer<GET_SWITCH_A>(GET_SWITCH);
    if(!ok)
    {
        return false;
    }

    GET_SWITCH_A *answer = reinterpret_cast<GET_SWITCH_A *>(dac_data.data());
    Q_ASSERT(answer);

    emit debug(QString("addr %1").arg(answer->header.address));
    emit debug(QString("cmd  %1").arg(answer->header.cmd));
    emit debug(QString("cnt  %1").arg(answer->header.count_data));
    emit info(QString("sw   %1").arg(answer->data.sw));
    emit debug(QString("val  %1").arg(answer->data.value));
    emit debug(QString("crc8 0x%1").arg(answer->crc8, 2, 16, QChar('0')));

    if(is_silence == false)
    {
        emit info("OK");
    }
    return true;
}
//--------------------------------------------------------------------------------
bool DAC32::f_SET_SWITCH(uint8_t sw, uint8_t value)
{
    SET_SWITCH_Q question;
    question.header.address = MY_ADDR;
    question.header.cmd = SET_SWITCH;
    question.header.count_data = sizeof(question.data);

    question.data.sw = sw;
    question.data.value = value;

    question.crc8 = crc8((uint8_t *)&question,
                         0,
                         sizeof(question) - sizeof(question.crc8));

    QByteArray packet;
    packet.append((char *)&question, sizeof(question));
    f_dac_send(packet);

    // анализируем ответ от DAC
#ifdef FAST_DAC_COMMAND
    wait_msec(DAC_WAIT_MS);
#else
    wait_msec(DAC_WAIT_MS, true);
#endif

    bool ok = check_answer<SET_SWITCH_A>(SET_SWITCH);
    if(!ok)
    {
        return false;
    }

    SET_SWITCH_A *answer = reinterpret_cast<SET_SWITCH_A *>(dac_data.data());
    Q_ASSERT(answer);

    emit debug(QString("addr %1").arg(answer->header.address));
    emit debug(QString("cmd  %1").arg(answer->header.cmd));
    emit debug(QString("cnt  %1").arg(answer->header.count_data));
    emit debug(QString("sw   %1").arg(answer->data.sw));
    emit debug(QString("val  %1").arg(answer->data.value));
    emit debug(QString("crc8 0x%1").arg(answer->crc8, 2, 16, QChar('0')));

    if(is_silence == false)
    {
        emit info("OK");
    }
    return true;
}
//--------------------------------------------------------------------------------
bool DAC32::f_GET_SWITCHES(void)
{
    GET_SWITCHES_Q question;
    question.header.address = MY_ADDR;
    question.header.cmd = GET_SWITCHES;
    question.header.count_data = 0;

    question.crc8 = crc8((uint8_t *)&question,
                         0,
                         sizeof(question) - sizeof(question.crc8));

    QByteArray packet;
    packet.append((char *)&question, sizeof(question));
    f_dac_send(packet);

    // анализируем ответ от DAC
#ifdef FAST_DAC_COMMAND
    wait_msec(DAC_WAIT_MS);
#else
    wait_msec(DAC_WAIT_MS, true);
#endif

    bool ok = check_answer<GET_SWITCHES_A>(GET_SWITCHES);
    if(!ok)
    {
        return false;
    }

    GET_SWITCHES_A *answer = reinterpret_cast<GET_SWITCHES_A *>(dac_data.data());
    Q_ASSERT(answer);

    emit debug(QString("addr %1").arg(answer->header.address));
    emit debug(QString("cmd  %1").arg(answer->header.cmd));
    emit debug(QString("cnt  %1").arg(answer->header.count_data));
    emit debug(QString("val  0x%1").arg(answer->data.values.value, 4, 16, QChar('0')));
    emit debug(QString("crc8 0x%1").arg(answer->crc8, 2, 16, QChar('0')));

    emit update_ui(answer->data.values);

    if(is_silence == false)
    {
        emit info("OK");
    }
    return true;
}
//--------------------------------------------------------------------------------
bool DAC32::f_get_switches(uint32_t *value)
{
    GET_SWITCHES_Q question;
    question.header.address = MY_ADDR;
    question.header.cmd = GET_SWITCHES;
    question.header.count_data = 0;

    question.crc8 = crc8((uint8_t *)&question,
                         0,
                         sizeof(question) - sizeof(question.crc8));

    QByteArray packet;
    packet.append((char *)&question, sizeof(question));
    f_dac_send(packet);

    // анализируем ответ от DAC
#ifdef FAST_DAC_COMMAND
    wait_msec(DAC_WAIT_MS);
#else
    wait_msec(DAC_WAIT_MS, true);
#endif

    bool ok = check_answer<GET_SWITCHES_A>(GET_SWITCHES);
    if(!ok)
    {
        return false;
    }

    GET_SWITCHES_A *answer = reinterpret_cast<GET_SWITCHES_A *>(dac_data.data());
    Q_ASSERT(answer);

    *value = answer->data.values.value;
    return true;
}
//--------------------------------------------------------------------------------
bool DAC32::f_set_switches(uint32_t value)
{
    SWITCH sw;
    sw.value = value;

    SET_SWITCHES_Q question;
    question.header.address = MY_ADDR;
    question.header.cmd = SET_SWITCHES;
    question.header.count_data = sizeof(question.data);
    question.data.values = sw;
    question.crc8 = crc8((uint8_t *)&question,
                         0,
                         sizeof(question) - sizeof(question.crc8));

    QByteArray packet;
    packet.append((char *)&question, sizeof(question));
    f_dac_send(packet);

    // анализируем ответ от DAC
#ifdef FAST_DAC_COMMAND
    wait_msec(DAC_WAIT_MS);
#else
    wait_msec(DAC_WAIT_MS, true);
#endif

    return check_answer<SET_SWITCHES_A>(SET_SWITCHES);
}
//--------------------------------------------------------------------------------
bool DAC32::f_SET_SWITCHES(void)
{
    SET_SWITCHES_Q question;
    question.header.address = MY_ADDR;
    question.header.cmd = SET_SWITCHES;
    question.header.count_data = sizeof(question.data);

    SWITCH sw;
    sw.value = 0;

    Switches_box *sb = new Switches_box();
    sb->set(sw);
    int btn = sb->exec();
    if(btn != QDialog::Accepted)
    {
        return false;
    }
    question.data.values = sb->get();

    question.crc8 = crc8((uint8_t *)&question,
                         0,
                         sizeof(question) - sizeof(question.crc8));

    emit debug(QString("sw: 0x%1").arg(sw.value, 4, 16, QChar('0')));

    QByteArray packet;
    packet.append((char *)&question, sizeof(question));
    f_dac_send(packet);

    // анализируем ответ от DAC
#ifdef FAST_DAC_COMMAND
    wait_msec(DAC_WAIT_MS);
#else
    wait_msec(DAC_WAIT_MS, true);
#endif

    bool ok = check_answer<SET_SWITCHES_A>(SET_SWITCHES);
    if(!ok)
    {
        return false;
    }

    SET_SWITCHES_A *answer = reinterpret_cast<SET_SWITCHES_A *>(dac_data.data());
    Q_ASSERT(answer);

    emit debug("debug:");
    emit debug(QString("addr %1").arg(answer->header.address));
    emit debug(QString("cmd  %1").arg(answer->header.cmd));
    emit debug(QString("cnt  %1").arg(answer->header.count_data));
    emit debug(QString("val  0x%1").arg(answer->data.values.value, 4, 16, QChar('0')));
    emit debug(QString("crc8 0x%1").arg(answer->crc8, 2, 16, QChar('0')));

    emit update_ui(answer->data.values);

    if(is_silence == false)
    {
        emit info("OK");
    }
    return true;
}
//--------------------------------------------------------------------------------
bool DAC32::f_SET_SWITCHES(SWITCH values)
{
    SET_SWITCHES_Q question;
    question.header.address = MY_ADDR;
    question.header.cmd = SET_SWITCHES;
    question.header.count_data = sizeof(question.data);
    question.data.values = values;
    question.crc8 = crc8((uint8_t *)&question,
                         0,
                         sizeof(question) - sizeof(question.crc8));

    emit debug(QString("sw: 0x%1").arg(values.value, 4, 16, QChar('0')));

    QByteArray packet;
    packet.append((char *)&question, sizeof(question));
    f_dac_send(packet);

    // анализируем ответ от DAC
#ifdef FAST_DAC_COMMAND
    wait_msec(DAC_WAIT_MS);
#else
    wait_msec(DAC_WAIT_MS, true);
#endif

    bool ok = check_answer<SET_SWITCHES_A>(SET_SWITCHES);
    if(!ok)
    {
        return false;
    }

    SET_SWITCHES_A *answer = reinterpret_cast<SET_SWITCHES_A *>(dac_data.data());
    Q_ASSERT(answer);

    emit debug("answer:");
    emit debug(QString("addr %1").arg(answer->header.address));
    emit debug(QString("cmd  %1").arg(answer->header.cmd));
    emit debug(QString("cnt  %1").arg(answer->header.count_data));
    emit debug(QString("val  0x%1").arg(answer->data.values.value, 4, 16, QChar('0')));
    emit debug(QString("crc8 0x%1").arg(answer->crc8, 2, 16, QChar('0')));

    emit update_ui(answer->data.values);

    if(is_silence == false)
    {
        emit info("OK");
    }
    return true;
}
//--------------------------------------------------------------------------------
bool DAC32::f_GET_DAC_ZERO(uint16_t *value)
{
    GET_DAC_ZERO_Q question;
    question.header.address = MY_ADDR;
    question.header.cmd = GET_DAC_ZERO;
    question.header.count_data = 0;

    question.crc8 = crc8((uint8_t *)&question,
                         0,
                         sizeof(question) - sizeof(question.crc8));

    QByteArray packet;
    packet.append((char *)&question, sizeof(question));
    f_dac_send(packet);

    // анализируем ответ от DAC
#ifdef FAST_DAC_COMMAND
    wait_msec(DAC_WAIT_MS);
#else
    wait_msec(DAC_WAIT_MS, true);
#endif

    bool ok = check_answer<GET_DAC_ZERO_A>(GET_DAC_ZERO);
    if(!ok)
    {
        return false;
    }

    GET_DAC_ZERO_A *answer = reinterpret_cast<GET_DAC_ZERO_A *>(dac_data.data());
    Q_ASSERT(answer);

    *value = answer->data.value;

    emit debug(QString("addr %1").arg(answer->header.address));
    emit debug(QString("cmd  %1").arg(answer->header.cmd));
    emit debug(QString("cnt  %1").arg(answer->header.count_data));
    emit debug(QString("val  %1").arg(answer->data.value));
    emit debug(QString("crc8 0x%1").arg(answer->crc8, 2, 16, QChar('0')));

    if(is_silence == false)
    {
        emit info("OK");
    }
    return true;
}
//--------------------------------------------------------------------------------
bool DAC32::f_SET_DAC_ZERO(uint16_t value)
{
    SET_DAC_ZERO_Q question;
    question.header.address = MY_ADDR;
    question.header.cmd = SET_DAC_ZERO;
    question.header.count_data = sizeof(question.data);
    question.data.value = value;

    question.crc8 = crc8((uint8_t *)&question,
                         0,
                         sizeof(question) - sizeof(question.crc8));

    QByteArray packet;
    packet.append((char *)&question, sizeof(question));
    f_dac_send(packet);

    // анализируем ответ от DAC
#ifdef FAST_DAC_COMMAND
    wait_msec(DAC_WAIT_MS);
#else
    wait_msec(DAC_WAIT_MS, true);
#endif

    bool ok = check_answer<SET_DAC_ZERO_A>(SET_DAC_ZERO);
    if(!ok)
    {
        return false;
    }

    SET_DAC_ZERO_A *answer = reinterpret_cast<SET_DAC_ZERO_A *>(dac_data.data());
    Q_ASSERT(answer);

    emit debug(QString("addr %1").arg(answer->header.address));
    emit debug(QString("cmd  %1").arg(answer->header.cmd));
    emit debug(QString("cnt  %1").arg(answer->header.count_data));
    emit debug(QString("val  %1").arg(answer->data.value));
    emit debug(QString("crc8 0x%1").arg(answer->crc8, 2, 16, QChar('0')));

    if(is_silence == false)
    {
        emit info("OK");
    }
    return true;
}
//--------------------------------------------------------------------------------
bool DAC32::f_GET_DAC_LOW(uint16_t *value)
{
    GET_DAC_LOW_Q question;
    question.header.address = MY_ADDR;
    question.header.cmd = GET_DAC_LOW;
    question.header.count_data = 0;

    question.crc8 = crc8((uint8_t *)&question,
                         0,
                         sizeof(question) - sizeof(question.crc8));

    QByteArray packet;
    packet.append((char *)&question, sizeof(question));
    f_dac_send(packet);

    // анализируем ответ от DAC
#ifdef FAST_DAC_COMMAND
    wait_msec(DAC_WAIT_MS);
#else
    wait_msec(DAC_WAIT_MS, true);
#endif

    bool ok = check_answer<GET_DAC_LOW_A>(GET_DAC_LOW);
    if(!ok)
    {
        return false;
    }

    GET_DAC_LOW_A *answer = reinterpret_cast<GET_DAC_LOW_A *>(dac_data.data());
    Q_ASSERT(answer);

    *value = answer->data.value;

    emit debug(QString("addr %1").arg(answer->header.address));
    emit debug(QString("cmd  %1").arg(answer->header.cmd));
    emit debug(QString("cnt  %1").arg(answer->header.count_data));
    emit debug(QString("val  %1").arg(answer->data.value));
    emit debug(QString("crc8 0x%1").arg(answer->crc8, 2, 16, QChar('0')));

    if(is_silence == false)
    {
        emit info("OK");
    }
    return true;
}
//--------------------------------------------------------------------------------
bool DAC32::f_SET_DAC_LOW(uint16_t value)
{
    SET_DAC_LOW_Q question;
    question.header.address = MY_ADDR;
    question.header.cmd = SET_DAC_LOW;
    question.header.count_data = sizeof(question.data);
    question.data.value = value;

    question.crc8 = crc8((uint8_t *)&question,
                         0,
                         sizeof(question) - sizeof(question.crc8));

    QByteArray packet;
    packet.append((char *)&question, sizeof(question));
    f_dac_send(packet);

    // анализируем ответ от DAC
#ifdef FAST_DAC_COMMAND
    wait_msec(DAC_WAIT_MS);
#else
    wait_msec(DAC_WAIT_MS, true);
#endif

    bool ok = check_answer<SET_DAC_LOW_A>(SET_DAC_LOW);
    if(!ok)
    {
        return false;
    }

    SET_DAC_LOW_A *answer = reinterpret_cast<SET_DAC_LOW_A *>(dac_data.data());
    Q_ASSERT(answer);

    emit debug(QString("addr %1").arg(answer->header.address));
    emit debug(QString("cmd  %1").arg(answer->header.cmd));
    emit debug(QString("cnt  %1").arg(answer->header.count_data));
    emit debug(QString("val  %1").arg(answer->data.value));
    emit debug(QString("crc8 0x%1").arg(answer->crc8, 2, 16, QChar('0')));

    if(is_silence == false)
    {
        emit info("OK");
    }
    return true;
}
//--------------------------------------------------------------------------------
bool DAC32::find_device(void)
{
    QList<int> speeds;
    bool ok;

    speeds.clear();
    speeds.append(9600);

    QStringList sl_ports = serial->get_port_names();

    // вдруг раньше уже выбирали
    QString port_name = load_string(DAC_PORTNAME);
    if(port_name.isEmpty() == false)
    {
        QStringList t_sl;
        t_sl.append(port_name);
        foreach (QString p_name, sl_ports)
        {
            if(p_name != port_name)
            {
                t_sl.append(p_name);
            }
        }
        sl_ports.clear();
        sl_ports.append(t_sl);
    }
    //---

    foreach (QString port_name, sl_ports)
    {
        if(serial->isOpen()) serial->serial_close();

        emit info(QString("Ищем на порту %1").arg(port_name));
        serial->set_port_name(port_name);
        serial->set_baudRate(9600);
        if(serial->serial_open())
        {
            ok = f_GET_ID();
            if(ok)
            {
                emit info(QString("DAC найден на порту %1").arg(port_name));
                save_string(DAC_PORTNAME, port_name);
                return true;
            }
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
void DAC32::close_connect(void)
{
    if(serial->isOpen())
    {
        serial->serial_close();
        emit info("Порт закрыт");
    }
}
//--------------------------------------------------------------------------------
void DAC32::set_state_silence(bool state)
{
    is_silence = state;
}
//--------------------------------------------------------------------------------
void DAC32::wait_msec(int timeout_msec, bool waiting)
{
    QElapsedTimer time;

#ifdef TEST
    timeout_msec = TEST_WAIT_MS;
#endif

    emit debug(QString("Пауза %1 ms").arg(timeout_msec));
    if(timeout_msec < 1)
    {
        emit debug("timeout_msec < 1");
        return;
    }
    time.start();
    while(time.elapsed() < timeout_msec)
    {
        QCoreApplication::processEvents();
        if(!waiting)
        {
            if(data_is_ready)
            {
                emit info(QString("Данные готовы. Прошло %1 msec").arg(time.elapsed()));
                return;
            }
        }
    }
}
//--------------------------------------------------------------------------------
void DAC32::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool DAC32::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void DAC32::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void DAC32::save_setting(void)
{

}
//--------------------------------------------------------------------------------
