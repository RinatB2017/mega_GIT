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
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QStyle>
#include <QLabel>
#include <QTime>
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#include "oscilloscope_GDS_840C.hpp"
#include "grapherbox.hpp"
//--------------------------------------------------------------------------------
#include "ui_oscilloscope_gds_840c.h"
//--------------------------------------------------------------------------------
#include <qwt_round_scale_draw.h>
#include <qwt_scale_map.h>
//--------------------------------------------------------------------------------
#ifdef FAKE
    #include "fake_oscilloscope_GDS_840C.hpp"
#endif
//--------------------------------------------------------------------------------
Oscilloscope_GDS_840C::Oscilloscope_GDS_840C(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Oscilloscope_GDS_840C),
    timeout(1000)
{
    init();
}
//--------------------------------------------------------------------------------
void Oscilloscope_GDS_840C::init(void)
{
    ui->setupUi(this);

#ifdef FAKE
#endif

    connect(&serial, SIGNAL(readyRead()), this, SLOT(port_read()));
    //connect(&serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(port_error(QSerialPort::SerialPortError)));

    oscilloscope = new GrapherBox(this);
    channel_1 = oscilloscope->add_curve("1");
    channel_2 = oscilloscope->add_curve("2");

    ui->sb_timeout->setMinimum(1000);
    ui->sb_timeout->setMaximum(10000);

    ui->oscilloscope_layout->addWidget(oscilloscope);

    ui->btn_find_device->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));

    connect(ui->btn_find_device,    SIGNAL(clicked(bool)), this, SLOT(find_device()));
    connect(ui->btn_auto,           SIGNAL(clicked(bool)), this, SLOT(set_AUToset()));
    connect(ui->sb_timeout,         SIGNAL(valueChanged(int)), this, SLOT(set_timeout(int)));
    connect(ui->btn_run,            SIGNAL(clicked(bool)), this, SLOT(set_RUN()));
    connect(ui->btn_stop,           SIGNAL(clicked(bool)), this, SLOT(set_STOP()));
    connect(ui->btn_cursor_X,       SIGNAL(clicked(bool)), this, SLOT(set_CURSor_XDISplay(bool)));
    connect(ui->btn_cursor_Y,       SIGNAL(clicked(bool)), this, SLOT(set_CURSor_YDISplay(bool)));
}
//--------------------------------------------------------------------------------
void Oscilloscope_GDS_840C::port_read(void)
{
    //emit debug("port_read");
    data_oscilloscope.append(serial.readAll());
}
//--------------------------------------------------------------------------------
void Oscilloscope_GDS_840C::port_error(QSerialPort::SerialPortError serial_error)
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
bool Oscilloscope_GDS_840C::find_oscilloscope(void)
{
    if(serial.isOpen() == false)
    {
        emit error(tr("Порт осциллографа не открыт!"));
        return false;
    }

    QByteArray output;
    bool ok = get_IDN(&output);
    if(!ok)
    {
        emit debug("get_IDN return FALSE");
        return false;
    }

    emit debug(output);
    if(output != oscilloscope_answer)
    {
        emit error(tr("Это не осциллограф!"));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool Oscilloscope_GDS_840C::find_device(void)
{
    QList<int> speeds;

    speeds.clear();
    speeds.append(115200);
    speeds.append(57600);
    speeds.append(38400);
    speeds.append(28800);
    speeds.append(19200);
    speeds.append(9600);
    speeds.append(4800);

    if(serial.isOpen()) serial.close();

    flag_closed = false;
    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
        emit info(QString(tr("ищем осциллограф на порту %1")).arg(port.portName()));
        foreach (int speed, speeds)
        {
            if(flag_closed) return false;

            serial.setPort(port);
            emit debug(QString("setBaudRate %1").arg(speed));
            bool ok = serial.setBaudRate(speed);
            if(ok)
            {
                if(serial.open(QIODevice::ReadWrite))
                {
                    bool ok = find_oscilloscope();
                    if(ok)
                    {
                        emit info(QString(tr("Осциллограф GDS-840C найден на скорости %1")).arg(speed));
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
                emit error(QString("setBaudRate(%1) is failed!").arg(speed));
            }
        }
    }
    emit error(tr("Осциллограф не найден"));
    return false;
}
//--------------------------------------------------------------------------------
/*
Очищает все регистры данных статуса события.
Это включает в себя выходные очереди, регистр статуса операции события,
регистр статуса неопределенного события, и  регистр статуса стандартного события.
*/
bool Oscilloscope_GDS_840C::CLS(void)
{
    QByteArray output;
    send("*CLS\n", &output);
    return  true;
}
//--------------------------------------------------------------------------------
/*
Устанавливает биты в регистр статуса допустимого события (ESER).
ESER включает регистр статуса стандартного события (SESR),
чтобы суммироваться в 5 бите (ESB) регистра состояния байта (SBR).

value - число от 0 до 255
*/
bool Oscilloscope_GDS_840C::set_ESE(unsigned char value)
{
    QByteArray input;
    input.append(QString("*ESE %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Возвращает биты в регистр статуса допустимого события (ESER).
ESER включает регистр статуса стандартного события (SESR),
чтобы суммироваться в 5 бите (ESB) регистра состояния байта (SBR).

результат - число от 0 до 255
*/
bool Oscilloscope_GDS_840C::get_ESE(unsigned char *value)
{
    QByteArray output;
    bool ok = send("*ESE?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *value = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Возвращает и очищает содержимое регистра статуса стандартного события (SESR).
*/
bool Oscilloscope_GDS_840C::get_ESR(int *result)
{
    QByteArray output;
    bool ok = send("*ESR?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Возвращает уникальный код идентификации GDS-820/GDS-840.
*/
bool Oscilloscope_GDS_840C::get_IDN(QByteArray *output)
{
    return send("*IDN?\n", output);
}
//--------------------------------------------------------------------------------
/*
Возвращает строку, в которой будут перечислены параметры настройки GDS-820/GDS-840.
*/
bool Oscilloscope_GDS_840C::get_LRN(QByteArray *output)
{
    return send("*LRN?\n", output);
}
//--------------------------------------------------------------------------------
/*
Форма запроса (*OPC?) говорит осциллографу, чтобы он установил все ASCII 1 в выходную
очередь, когда завершаются все незаконченные операции.
*/
bool Oscilloscope_GDS_840C::get_OPC(QByteArray *output)
{
    return send("*OPC?\n", output);
}
//--------------------------------------------------------------------------------
/*
Командная форма (*OPC) устанавливает бит завершения операции (bit 0) в регистр статуса
стандартного события (SESR), когда завершаются все операции ожидающие обработки.
*/
bool Oscilloscope_GDS_840C::set_OPC(int value)
{
    QByteArray input;
    input.append(QString("*OPC %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Восстанавливает из памяти данные настроек, которые были сохранены в прошлый раз.
Параметры настроек RS-232 (или GPIB) могут быть сохранены в память M1~M15.
Тем не менее, если пользователь обратится к резервной памяти с настройками RS-232 (или GPIB),
отличными от текущих параметров настроек, то будут сохранены текущие настройки RS-232 (или GPIB).
RS-232 (или GPIB) настройки не будут влиять на запрошенный в данный момент RS-232 (или GPIB) параметр.

value - 1~15
*/
bool Oscilloscope_GDS_840C::set_RCL(int value)
{
    if((value < 1) || (value > 15))
    {
        return false;
    }
    QByteArray input;
    input.append(QString("*RCL %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Выставляет все контрольные настройки осциллографа в их значение по умолчанию,
но не очищает сохраненную установку.
*/
bool Oscilloscope_GDS_840C::set_RST(void)
{
    QByteArray output;
    send("*RST\n", &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Сохраняет данные настроек в память.

value - 1~15
*/
bool Oscilloscope_GDS_840C::set_SAV(unsigned int value)
{
    if((value < 1) || (value > 15))
    {
        return false;
    }
    QByteArray input;
    input.append(QString("*SAV %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
В форме запроса возвращает содержимое SRER. Бит 6 SRER – всегда нулевой.
Биты из SRER переписываются на биты в SBR.
*/
bool Oscilloscope_GDS_840C::get_SRE(unsigned char *value)
{
    QByteArray output;
    bool ok = send("*SRE?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *value = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Устанавливает содержимое регистра запроса обслуживания (SRER).
*/
bool Oscilloscope_GDS_840C::set_SRE(unsigned char value)
{
    QByteArray input;
    input.append(QString("*SRE %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Запрос регистра состояния байта (SBR) с помощью *STB? возвратит десятичное число,
представляющее собой биты, установленные (true) в регистр статуса.
*/
bool Oscilloscope_GDS_840C::get_STB(int *result)
{
    QByteArray output;
    bool ok = send("*STB?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
WAI препятствует дальнейшему программированию прибора, для последующего исполнения команд
или запросов, пока все незаконченные операции не будут завершены.
*/
bool Oscilloscope_GDS_840C::set_WAI(void)
{
    QByteArray output;
    send("*WAI\n", &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Устанавливает среднее число собранных данных о сигнале.
Диапазон для усреднения - от 2 до 256 с шагом 2.

Аргументы

1→Среднее значение 2                                     2→Среднее значение 4
3→Среднее значение 8                                     4→Среднее значение 16
5→Среднее значение 32                                    6→Среднее значение 64
7→Среднее значение 128                                   8→Среднее значение 256

Примечание: Перед работой с прибором, пожалуйста сделайте обращение к “:ACQuire:MODe 2”!
*/
bool Oscilloscope_GDS_840C::set_ACQuire_AVERage(Average value)
{
    if((value < 1) || (value > 8)) return false;
    QByteArray input;
    input.append(QString(":ACQuire:AVERage %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Возвращает среднее число собранных данных о сигнале.
*/
bool Oscilloscope_GDS_840C::get_ACQuire_AVERage(int *result)
{
    QByteArray output;
    bool ok = send(":ACQuire:AVERage?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает значение длины записи. GDS-820/GDS-840 обеспечивает
длину записи 500, 1250, 2500, 5000, 12500, 25000, 50000, и 125000.
*/
bool Oscilloscope_GDS_840C::set_ACQuire_LENGth(Len value)
{
    QByteArray input;
    input.append(QString(":ACQuire:LENGth %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Возвращает значение длины записи.
*/
bool Oscilloscope_GDS_840C::get_ACQuire_LENGth(int *value)
{
    QByteArray output;
    bool ok = send(":ACQuire:LENGth?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *value = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает режим сбора. Есть четыре способа сбора данных: по образцу, амплитудное детектирование,
сбор и усреднение.

Аргументы

0→Выбор типового режима
1→Выбор режима амплитудного детектирования
2→Выбор обыкновенного режима
*/
bool Oscilloscope_GDS_840C::set_ACQuire_MODe(unsigned char value)
{
    if(value > 2) return false;

    QByteArray input;
    input.append(QString(":ACQuire:MODe %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Возвращает режим сбора.
*/
bool Oscilloscope_GDS_840C::get_ACQuire_MODe(int *result)
{
    QByteArray output;
    bool ok = send(":ACQuire:MODe?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Передает данные о сигнале (всего 500 делений на все значения) от GDS-820/GDS-840.
Каждое деление сформировано двумя байтами (целое значение 16 бит).
Байт с более высоким приоритетом (MSD) будет передан первым.
*/
bool Oscilloscope_GDS_840C::get_ACQuire_POINt(Channel channel, QByteArray *output)
{
    QByteArray input;
    input.append(QString(":ACQuire%1:POINt\n").arg(channel));
    return send(input, output);
}
//--------------------------------------------------------------------------------
/*
Выполняет автоматическую установку для того, чтобы оптимизировать принимаемые параметры.
*/
bool Oscilloscope_GDS_840C::set_AUToset(void)
{
    QByteArray output;
    send(":AUToset\n", &output, true);
    return true;
}
//--------------------------------------------------------------------------------
/*
Включает или выключает функцию ограничения полосы пропускания.

0→Выключает функцию ограничения полосы пропускания
1→Включает функцию ограничения полосы пропускания
*/
bool Oscilloscope_GDS_840C::set_CHANnel_BWLimit(Channel channel, bool state)
{
    QByteArray input;
    input.append(QString(":CHANnel%1:BWLimit %2\n").arg(channel).arg(state));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Возвращает режим функции ограничения полосы пропускания.
*/
bool Oscilloscope_GDS_840C::get_CHANnel_BWLimit(Channel channel, int *result)
{
    QByteArray input;
    QByteArray output;
    input.append(QString(":CHANnel%1:BWLimit?\n").arg(channel));
    bool ok = send(input, &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Включает функцию ограничения полосы пропускания GDS-820/GDS-840.
*/
bool Oscilloscope_GDS_840C::set_CHANnel_COUPling(Channel channel, CState state)
{
    QByteArray input;
    input.append(QString(":CHANnel%1:COUPling %2\n").arg(channel).arg(state));
    QByteArray output;
    return send(input, &output);
}
//--------------------------------------------------------------------------------
/*
Возвращает режим функции ограничения полосы пропускания GDS-820/GDS-840.
*/
bool Oscilloscope_GDS_840C::get_CHANnel_COUPling(Channel channel, int *result)
{
    QByteArray input;
    QByteArray output;
    input.append(QString(":CHANnel%1:COUPling?\n").arg(channel));
    bool ok = send(input, &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Включает или выключает отображение канала.

0→Выключить канал <X> дисплея
1→Включить канал <X> дисплея
*/
bool Oscilloscope_GDS_840C::set_CHANnel_DISPlay(Channel channel, bool state)
{
    QByteArray input;
    input.append(QString(":CHANnel%1:DISPlay %2\n").arg(channel).arg(state));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Включает или выключает инверсную функцию сигнала.

0→ Выключает инверсную функцию
1→ Выключает инверсную функцию
*/
bool Oscilloscope_GDS_840C::set_CHANnel_INVert(Channel channel, bool state)
{
    QByteArray input;
    input.append(QString(":CHANnel%1:INVert %2\n").arg(channel).arg(state));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Возвращает состояние инверсии сигнала.
*/
bool Oscilloscope_GDS_840C::get_CHANnel_INVert(Channel channel, int *result)
{
    QByteArray input;
    QByteArray output;
    input.append(QString(":CHANnel%1:INVert?\n").arg(channel));
    bool ok = send(input, &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Устанавливает математическое выражение.
*/
bool Oscilloscope_GDS_840C::set_CHANnel_MATH(Channel channel, bool state)
{
    QByteArray input;
    input.append(QString(":CHANnel%1:MATH %2\n").arg(channel).arg(state));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Возвращает математическое выражение.
*/
bool Oscilloscope_GDS_840C::get_CHANnel_MATH(Channel channel, int *result)
{
    QByteArray input;
    QByteArray output;
    input.append(QString(":CHANnel%1:MATH?\n").arg(channel));
    bool ok = send(input, &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Устанавливает напряжение компенсации.
*/
bool Oscilloscope_GDS_840C::set_CHANnel_OFFSet(Channel channel, int voltage)
{
    QByteArray input;
    input.append(QString(":CHANnel%1:OFFSet %2\n").arg(channel).arg(voltage));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Запрашивает напряжение компенсации.
*/
bool Oscilloscope_GDS_840C::get_CHANnel_OFFSet(Channel channel, int *result)
{
    QByteArray input;
    QByteArray output;
    input.append(QString(":CHANnel%1:OFFSet?\n").arg(channel));
    bool ok = send(input, &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает различные показатели делителя сигнала.

0→1X
1→10X
2→100X
*/
bool Oscilloscope_GDS_840C::set_CHANnel_PROBe(Channel channel, Divider div)
{
    if(div > 2) return false;

    QByteArray input;
    input.append(QString(":CHANnel%1:PROBe %2\n").arg(channel).arg(div));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Возвращает показатели делителя сигнала.
*/
bool Oscilloscope_GDS_840C::get_CHANnel_PROBe(Channel channel, int *result)
{
    QByteArray input;
    QByteArray output;
    input.append(QString(":CHANnel%1:PROBe?\n").arg(channel));
    bool ok = send(input, &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Устанавливает вертикальную шкалу указанного канала.
*/
bool Oscilloscope_GDS_840C::set_CHANnel_SCALe(Channel channel, VScale scale)
{
    float temp = 0.0f;
    switch(scale)
    {
    case VScale_2mV: temp = 0.002; break;
    case VScale_5mV: temp = 0.005; break;
    case VScale_10mV: temp = 0.01; break;
    case VScale_20mV: temp = 0.02; break;
    case VScale_50mV: temp = 0.05; break;
    case VScale_100mV: temp = 0.1; break;
    case VScale_200mV: temp = 0.2; break;
    case VScale_500mV: temp = 0.5; break;
    case VScale_1V: temp = 1; break;
    case VScale_2V: temp = 2; break;
    case VScale_5V: temp = 5; break;
    }

    QByteArray input;
    input.append(QString(":CHANnel%1:SCALe %2\n").arg(channel).arg(temp));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Запрашивает вертикальную шкалу указанного канала.
*/
bool Oscilloscope_GDS_840C::get_CHANnel_SCALe(Channel channel, int *result)
{
    QByteArray input;
    QByteArray output;
    input.append(QString(":CHANnel%1:SCALe?\n").arg(channel));
    bool ok = send(input, &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает положение курсоров оси X.

по оси  x, диапазон от 0 до 250
*/
bool Oscilloscope_GDS_840C::set_CURSor_XPosition(Channel channel, unsigned int pos)
{
    if(pos > 250) return false;

    QByteArray input;
    input.append(QString(":CURSor%1:XPosition %2\n").arg(channel).arg(pos));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает положение курсоров оси Y.

по оси  y, диапазон от 0 до 200
*/
bool Oscilloscope_GDS_840C::set_CURSor_YPosition(Channel channel, unsigned int pos)
{
    if(pos > 200) return false;

    QByteArray input;
    input.append(QString(":CURSor%1:YPosition %2\n").arg(channel).arg(pos));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Возвращает разницу времени или напряжения между двумя вертикальными или горизонтальными курсорами.
*/
bool Oscilloscope_GDS_840C::get_CURSor_DELta(Channel channel, int *result)
{
    QByteArray input;
    QByteArray output;
    input.append(QString(":CURSor%1:DELta?\n").arg(channel));
    bool ok = send(input, &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Включает или выключает изображение курсоров по оси X.

0→ Выключает изображение курсоров
1→ Включает изображение курсоров
*/
bool Oscilloscope_GDS_840C::set_CURSor_XDISplay(bool state)
{
    QByteArray input;
    input.append(QString(":CURSor:XDISplay %1\n").arg(state));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Включает или выключает изображение курсоров по оси Y.

0→ Выключает изображение курсоров
1→ Включает изображение курсоров
*/
bool Oscilloscope_GDS_840C::set_CURSor_YDISplay(bool state)
{
    QByteArray input;
    input.append(QString(":CURSor:YDISplay %1\n").arg(state));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает, какие канальные курсоры активны для управления лицевой панелью.
1→ Выбирает канал 1 для измерения курсоров
2→ Выбирает канал 2 для измерения курсоров
3→ Выбирает мат. функцию для измерения курсоров
*/
bool Oscilloscope_GDS_840C::set_CURSor_SOURce(Source value)
{
    QByteArray input;
    input.append(QString(":CURSor:SOURce %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Возвращает номер активного курсора для управления лицевой панелью.
*/
bool Oscilloscope_GDS_840C::get_CURSor_SOURce(int *result)
{
    QByteArray output;
    bool ok = send(":CURSor:SOURce?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает аккумулирующий режим дисплея.

0→ Выключает аккумулирующий режим дисплея
1→ Включает аккумулирующий режим дисплея
*/
bool Oscilloscope_GDS_840C::set_DISPlay_ACCumulate(bool state)
{
    QByteArray input;
    input.append(QString(":DISPlay:ACCumulate %1\n").arg(state));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Возвращает аккумулирующий режим дисплея.
*/
bool Oscilloscope_GDS_840C::get_DISPlay_ACCumulate(int *result)
{
    QByteArray output;
    bool ok = send(":DISPlay:ACCumulate?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает уровень контраста LCD экрана для вывода данных и демонстрирования сигнала.

Диапазон от 0~20 (0% to 100%).
*/
bool Oscilloscope_GDS_840C::set_DISPlay_CONTrast(int value)
{
    QByteArray input;
    input.append(QString(":DISPlay:CONTrast %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Возвращает уровень контраста LCD экрана для вывода данных и демонстрирования сигнала.
*/
bool Oscilloscope_GDS_840C::get_DISPlay_CONTrast(int *value)
{
    QByteArray output;
    bool ok = send(":DISPlay:CONTrast?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug("bad data");
        return false;
    }
    *value = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает способ показа координатной сетки на экране LCD.

0→Полностью наносить координатную сетку
1→Выбирает приближенный способ нанесения.
2→Показывается только рамка.
*/
bool Oscilloscope_GDS_840C::set_DISPlay_GRATicule(Graticule value)
{
    QByteArray input;
    input.append(QString(":DISPlay:GRATicule %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Возвращает способ показа координатной сетки на экране LCD.
*/
bool Oscilloscope_GDS_840C::get_DISPlay_GRATicule(int *result)
{
    QByteArray output;
    bool ok = send(":DISPlay:GRATicule?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug("bad data");
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Возвращает способ показа результатов обработки в виде точек (или векторов).
*/
bool Oscilloscope_GDS_840C::get_DISPlay_WAVeform(int *result)
{
    QByteArray output;
    bool ok = send(":DISPlay:WAVeform?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug("bad data");
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает способ показа результатов обработки в виде точек (или векторов).

0→ Включает векторный метод
1→ Включает точечный метод
*/
bool Oscilloscope_GDS_840C::set_DISPlay_WAVeform(int value)
{
    QByteArray input;
    input.append(QString(":DISPlay:WAVeform %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Синхронизирует результаты измерения сигнала относительно заднего фронта импульса.

Примечание: Пожалуйста, выберите определенный канал перед осуществлением любого измерения.
См. пояснение для “:MEASure:SOURce”
*/
bool Oscilloscope_GDS_840C::get_MEASure_FALL(float *result)
{
    QByteArray output;
    bool ok = send(":MEASure:FALL?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }

    float temp = output.replace("\n", "").toFloat(&ok);
    if(ok)
    {
        *result = temp;
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
/*
Возвращает значение измерения частоты.
*/
bool Oscilloscope_GDS_840C::get_MEASure_FREQuency(float *result)
{
    QByteArray output;
    bool ok = send(":MEASure:FREQuency?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }

    float temp = output.replace("\n", "").toFloat(&ok);
    if(ok)
    {
        *result = temp;
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
/*
Возвращает значение измерения спада первого импульса.
*/
bool Oscilloscope_GDS_840C::get_MEASure_NWIDth(float *result)
{
    QByteArray output;
    bool ok = send(":MEASure:NWIDth?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }

    float temp = output.replace("\n", "").toFloat(&ok);
    if(ok)
    {
        *result = temp;
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
/*
Возвращает отношение реальной ширины импульса к периоду сигнала.
*/
bool Oscilloscope_GDS_840C::get_MEASure_PDUTy(float *result)
{
    QByteArray output;
    bool ok = send(":MEASure:PDUTy?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }

    float temp = output.replace("\n", "").toFloat(&ok);
    if(ok)
    {
        *result = temp;
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
/*
Возвращает значение измерения периода.
*/
bool Oscilloscope_GDS_840C::get_MEASure_PERiod(float *result)
{
    QByteArray output;
    bool ok = send(":MEASure:PERiod?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }

    float temp = output.replace("\n", "").toFloat(&ok);
    if(ok)
    {
        *result = temp;
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
/*
Возвращает значение измерения пика первого импульса.
*/
bool Oscilloscope_GDS_840C::get_MEASure_PWIDth(float *result)
{
    QByteArray output;
    bool ok = send(":MEASure:PWIDth?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }

    float temp = output.replace("\n", "").toFloat(&ok);
    if(ok)
    {
        *result = temp;
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
/*
Синхронизирует результаты измерения сигнала относительно переднего фронта импульса.

Примечание: Пожалуйста, выберите определенный канал перед осуществлением любого измерения.
См. пояснение для “:MEASure:SOURce”
*/
bool Oscilloscope_GDS_840C::get_MEASure_RISe(float *result)
{
    QByteArray output;
    bool ok = send(":MEASure:RISe?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }

    float temp = output.replace("\n", "").toFloat(&ok);
    if(ok)
    {
        *result = temp;
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
/*
Выбирает измерительный канал (канал 1 или 2).
В настройках по умолчанию измерительным каналом является канал один.

1→ Включает функцию измерения канала 1
2→ Включает функцию измерения канала 2
*/
bool Oscilloscope_GDS_840C::set_MEASure_SOURce(Measurement_channel value)
{
    if((value != 1) && (value !=2)) return false;

    QByteArray input;
    input.append(QString(":MEASure:SOURce %1").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Возвращает разницу высокого и низкого значения напряжения.

Примечание: Пожалуйста, выберите определенный канал перед осуществлением любого измерения.
См. пояснение для “:MEASure:SOURce”
*/
bool Oscilloscope_GDS_840C::get_MEASure_VAMPlitude(float *result)
{
    QByteArray output;
    bool ok = send(":MEASure:VAMPlitude?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }

    float temp = output.replace("\n", "").toFloat(&ok);
    if(ok)
    {
        *result = temp;
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
/*
Возвращает среднее значение напряжения.

Примечание: Пожалуйста, выберите определенный канал перед осуществлением любого измерения.
См. пояснение для “:MEASure:SOURce”
*/
bool Oscilloscope_GDS_840C::get_MEASure_VAVerage(float *result)
{
    QByteArray output;
    bool ok = send(":MEASure:VAVerage?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }

    float temp = output.replace("\n", "").toFloat(&ok);
    if(ok)
    {
        *result = temp;
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
/*
Возвращает величину общего высокого значения напряжения.

Примечание: Пожалуйста, выберите определенный канал перед осуществлением любого измерения.
См. пояснение для “:MEASure:SOURce”
*/
bool Oscilloscope_GDS_840C::get_MEASure_VHI(float *result)
{
    QByteArray output;
    bool ok = send(":MEASure:VHI?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }

    float temp = output.replace("\n", "").toFloat(&ok);
    if(ok)
    {
        *result = temp;
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
/*
Возвращает величину общего низкого значения напряжения.

Примечание: Пожалуйста, выберите определенный канал перед осуществлением любого измерения.
См. пояснение для “:MEASure:SOURce”
*/
bool Oscilloscope_GDS_840C::get_MEASure_VLO(float *result)
{
    QByteArray output;
    bool ok = send(":MEASure:VLO?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }

    float temp = output.replace("\n", "").toFloat(&ok);
    if(ok)
    {
        *result = temp;
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
/*
Возвращает значение максимума амплитуды.

Примечание: Пожалуйста, выберите определенный канал перед осуществлением любого измерения.
См. пояснение для “:MEASure:SOURce”
*/
bool Oscilloscope_GDS_840C::get_MEASure_VMAX(float *result)
{
    QByteArray output;
    bool ok = send(":MEASure:VMAX?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }

    float temp = output.replace("\n", "").toFloat(&ok);
    if(ok)
    {
        *result = temp;
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
/*
Возвращает значение минимума амплитуды.

Примечание: Пожалуйста, выберите определенный канал перед осуществлением любого измерения.
См. пояснение для “:MEASure:SOURce”
*/
bool Oscilloscope_GDS_840C::get_MEASure_VMIN(float *result)
{
    QByteArray output;
    bool ok =  send(":MEASure:VMIN?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }

    float temp = output.replace("\n", "").toFloat(&ok);
    if(ok)
    {
        *result = temp;
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
/*
Возвращает разницу максимального (Vmax) и минимального (Vmin) значения напряжения.

Примечание: Пожалуйста, выберите определенный канал перед осуществлением любого измерения.
См. пояснение для “:MEASure:SOURce”
*/
bool Oscilloscope_GDS_840C::get_MEASure_VPP(float *result)
{
    QByteArray output;
    bool ok = send(":MEASure:VPP?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }

    float temp = output.replace("\n", "").toFloat(&ok);
    if(ok)
    {
        *result = temp;
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
/*
Возвращает истинное значение среднеквадратичного напряжения.

Примечание: Пожалуйста, выберите определенный канал перед осуществлением любого измерения.
См. пояснение для “:MEASure:SOURce”
*/
bool Oscilloscope_GDS_840C::get_MEASure_VRMS(float *result)
{
    QByteArray output;
    bool ok = send(":MEASure:VRMS?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }

    float temp = output.replace("\n", "").toFloat(&ok);
    if(ok)
    {
        *result = temp;
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
/*
Печатает на указанном принтере.
*/
bool Oscilloscope_GDS_840C::set_PRINt(void)
{
    QByteArray output;
    send(":PRINt\n", &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Обновляет информацию о сигнале на LCD экране.
*/
bool Oscilloscope_GDS_840C::set_REFResh(void)
{
    QByteArray output;
    send(":REFResh\n", &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Управляет включением системы запуска.
*/
bool Oscilloscope_GDS_840C::set_RUN(void)
{
    QByteArray output;
    send(":RUN\n", &output, true);
    return true;
}
//--------------------------------------------------------------------------------
/*
Управляет выключением системы запуска.
*/
bool Oscilloscope_GDS_840C::set_STOP(void)
{
    QByteArray output;
    send(":STOP\n", &output, true);
    return true;
}
//--------------------------------------------------------------------------------
/*
Клавиатура и кнопки на лицевой панели GDS-820/GDS-840 отключаются после любой полученной
команды дистанционного управления. Используйте эту команду, чтобы реактивировать
клавиатуру и кнопки на лицевой панели.
*/
bool Oscilloscope_GDS_840C::set_SYSTem_UNLock(void)
{
    QByteArray output;
    send(":SYSTem:UNLock\n", &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Устанавливает параметр горизонтального положения (задержка временной развертки).
*/
bool Oscilloscope_GDS_840C::set_TIMebase_DELay(float delay)
{
    QByteArray input;
    input.append(QString(":TIMebase:DELay %1\n").arg(delay));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Возвращает параметр горизонтального положения (задержка временной развертки).
*/
bool Oscilloscope_GDS_840C::get_TIMebase_DELay(int *result)
{
    QByteArray output;
    bool ok = send(":TIMebase:DELay?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Устанавливает масштаб величины горизонтальной временной развертки на деление (Сек/Делен).
(см. документацию на прибор)
*/
bool Oscilloscope_GDS_840C::set_TIMebase_SCALe(HScale scale)
{
    QString temp;
    switch(scale)
    {
    case HScale_1ns: temp = "1e-9"; break;
    case HScale_2_5ns: temp = "2.5e-9"; break;
    case HScale_5ns: temp = "5e-9"; break;
    case HScale_10ns: temp = "10e-9"; break;
    case HScale_25ns: temp = "25e-9"; break;
    case HScale_50ns: temp = "50e-9"; break;
    case HScale_100ns: temp = "100e-9"; break;
    case HScale_250ns: temp = "250e-9"; break;
    case HScale_500ns: temp = "500e-9"; break;

    case HScale_1us: temp = "1e-6"; break;
    case HScale_2_5us: temp = "2.5e-6"; break;
    case HScale_5us: temp = "5e-6"; break;
    case HScale_10us: temp = "10e-6"; break;
    case HScale_25us: temp = "25e-6"; break;
    case HScale_50us: temp = "50e-6"; break;
    case HScale_100us: temp = "100e-6"; break;
    case HScale_250us: temp = "250e-6"; break;
    case HScale_500us: temp = "500e-6"; break;

    case HScale_1ms: temp = "1e-3"; break;
    case HScale_2_5ms: temp = "2.5e-3"; break;
    case HScale_5ms: temp = "5e-3"; break;
    case HScale_10ms: temp = "10e-3"; break;
    case HScale_25ms: temp = "25e-3"; break;
    case HScale_50ms: temp = "50e-3"; break;
    case HScale_100ms: temp = "100e-3"; break;
    case HScale_250ms: temp = "250e-3"; break;

    case HScale_1s: temp = "1"; break;
    case HScale_2_5s: temp = "2.5"; break;
    case HScale_5s: temp = "5"; break;
    case HScale_10s: temp = "10"; break;
    }

    QByteArray input;
    input.append(QString(":TIMebase:SCALe %1\n").arg(temp));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Возвращает масштаб величины горизонтальной временной развертки на деление (Сек/Делен).
(см. документацию на прибор)
*/
bool Oscilloscope_GDS_840C::get_TIMebase_SCALe(int *result)
{
    QByteArray output;
    bool ok = send(":TIMebase:SCALe?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Устанавливает режим очистки горизонтальной временной развертки.
Эта команда эквивалентна меню горизонтальных настроек.

0→ Основной режим временной развертки
1→ Оконный
2→ Оконный, с изменением масштаба
3→ Режим прокрутки
4→ По осям XY
*/
bool Oscilloscope_GDS_840C::set_TIMebase_SWEep(Sweep value)
{
    QByteArray input;
    input.append(QString(":TIMebase:SWEep %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Возвращает режим очистки горизонтальной временной развертки.
*/
bool Oscilloscope_GDS_840C::get_TIMebase_SWEep(int *result)
{
    QByteArray output;
    bool ok = send(":TIMebase:SWEep?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Устанавливает увеличение области (область серого цвета) для показа с измененным масштабом окна.
*/
bool Oscilloscope_GDS_840C::set_TIMebase_WINDow_DELay(int value)
{
    QByteArray input;
    input.append(QString(":TIMebase:WINDow:DELay %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Запрашивает увеличение области (область серого цвета) для показа с измененным масштабом окна.
*/
bool Oscilloscope_GDS_840C::get_TIMebase_WINDow_DELay(int *result)
{
    QByteArray output;
    bool ok = send(":TIMebase:WINDow:DELay?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Устанавливает масштаб (длину) окон для увеличения или уменьшения временной развертки.
*/
bool Oscilloscope_GDS_840C::set_TIMebase_WINDow_SCALe(int value)
{
    QString input=QString(":TIMebase:WINDow:SCALe %1\n").arg(value);
    QByteArray output;
    send("", &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Запрашивает масштаб (длину) окон для увеличения или уменьшения временной развертки.
*/
bool Oscilloscope_GDS_840C::get_TIMebase_WINDow_SCALe(int *result)
{
    QByteArray output;
    bool ok = send(":TIMebase:WINDow:SCALe?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает тип связи триггера.

0→ Переменный ток (AC)
1→ Постоянный ток (DC)
*/
bool Oscilloscope_GDS_840C::set_TRIGger_COUPle(Trigger_couple value)
{
    QByteArray input;
    input.append(QString(":TRIGger:COUPle %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Запрашивает тип связи триггера.
*/
bool Oscilloscope_GDS_840C::get_TRIGger_COUPle(int *result)
{
    QByteArray output;
    bool ok = send(":TRIGger:COUPle?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает определенную пользователем временную задержку вызова.

В диапазоне от 100нс~1.3мс.
*/
bool Oscilloscope_GDS_840C::set_TRIGger_DELay_TIMe(int value)
{
    QByteArray input;
    input.append(QString(":TIMebase:DELay:TIMe %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Запрашивает определенную пользователем временную задержку вызова.
*/
bool Oscilloscope_GDS_840C::get_TRIGger_DELay_TIMe(int *result)
{
    QByteArray output;
    bool ok = send(":TIMebase:DELay:TIMe?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает задержку вызова события определенную пользователем.

В диапазоне от 2~65000.

Примечание: Пожалуйста, выберите определенный тип задержки перед осуществлением любого измерения.
См. пояснение для “:TRIGger:DELay:TYPe”
*/
bool Oscilloscope_GDS_840C::set_TRIGger_DELay_EVENt(int value)
{
    QByteArray input;
    input.append(QString(":TIMebase:DELay:EVENt %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Запрашивает задержку вызова события определенную пользователем.
*/
bool Oscilloscope_GDS_840C::get_TRIGger_DELay_EVENt(int *result)
{
    QByteArray output;
    bool ok = send(":TIMebase:DELay:EVENt?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает стартовый триггер сигнального уровня определенного пользователем.

В диапазоне ±12.
*/
bool Oscilloscope_GDS_840C::set_TRIGger_DELay_LEVel(int value)
{
    QByteArray input;
    input.append(QString(":TIMebase:DELay:LEVel %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Запрашивает стартовый триггер сигнального уровня определенного пользователем.
*/
bool Oscilloscope_GDS_840C::get_TRIGger_DELay_LEVel(int *result)
{
    QByteArray output;
    bool ok = send(":TIMebase:DELay:LEVel?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает другой стартовый триггер (то есть внешний триггер) сигнального уровня.

0→ TTL
1→ ECL
2→ USR
*/
bool Oscilloscope_GDS_840C::set_TRIGger_DELay_MODe(Trigger value)
{
    QByteArray input;
    input.append(QString(":TIMebase:DELay:MODe %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Запрашивает другой стартовый триггер (то есть внешний триггер) сигнального уровня.
*/
bool Oscilloscope_GDS_840C::get_TRIGger_DELay_MODe(int *result)
{
    QByteArray output;
    bool ok = send(":TIMebase:DELay:MODe?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает различные задержки вызова настроек.

0→ временные настройки
1→ случайные настройки
*/
bool Oscilloscope_GDS_840C::set_TRIGger_DELay_TYPe(Type value)
{
    QByteArray input;
    input.append(QString(":TIMebase:DELay:TYPe %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Запрашивает различные задержки вызова настроек.
*/
bool Oscilloscope_GDS_840C::get_TRIGger_DELay_TYPe(int *result)
{
    QByteArray output;
    bool ok = send(":TIMebase:DELay:TYPe?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit error(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Возвращает величину показаний переключателя счетчика частоты.
*/
bool Oscilloscope_GDS_840C::get_TRIGger_FREQuency(int *result)
{
    QByteArray output;
    bool ok = send(":TRIGger:FREQuency?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает значение запуска системы.
*/
bool Oscilloscope_GDS_840C::set_TRIGger_LEVel(int value)
{
    QByteArray input;
    input.append(QString(":TRIGger:LEVel %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Запрашивает значение запуска системы.
*/
bool Oscilloscope_GDS_840C::get_TRIGger_LEVel(int *result)
{
    QByteArray output;
    bool ok = send(":TRIGger:LEVel?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает режим запуска системы.

0→ Устанавливается автоматически
1→ Автоматический
2→ Нормальный
3→ Одиночный
*/
bool Oscilloscope_GDS_840C::set_TRIGger_MODe(Mode value)
{
    QByteArray input;
    input.append(QString(":TRIGger:MODe %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Запрашивает режим запуска системы.
*/
bool Oscilloscope_GDS_840C::get_TRIGger_MODe(int *result)
{
    QByteArray output;
    bool ok = send(":TRIGger:MODe?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Переключает режим подавления шумов.

0→ Выкл
1→ Вкл
*/
bool Oscilloscope_GDS_840C::set_TRIGger_NREJ(bool state)
{
    QByteArray input;
    input.append(QString(":TRIGger:NREJ %1\n").arg(state));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Запрашивает режим подавления шумов.
*/
bool Oscilloscope_GDS_840C::get_TRIGger_NREJ(int *result)
{
    QByteArray output;
    bool ok = send(":TRIGger:NREJ?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Переключает другой режим переключения импульса.

0→ <
1→ >
2→ =
3→ ≠
*/
bool Oscilloscope_GDS_840C::set_TRIGger_PULSe_MODe(int value)
{
    QByteArray input;
    input.append(QString(":TRIGger:PULSe:MODe %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Запрашивает другой режим переключения импульса.
*/
bool Oscilloscope_GDS_840C::get_TRIGger_PULSe_MODe(int *result)
{
    QByteArray output;
    bool ok = send(":TRIGger:PULSe:MODe?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает временную величину для ширины импульса.

(единица в секунду)
*/
bool Oscilloscope_GDS_840C::set_TRIGger_PULSe_TIMe(int value)
{
    QByteArray input;
    input.append(QString(":TRIGger:PULSe:TIMe %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Запрашивает временную величину для ширины импульса.
*/
bool Oscilloscope_GDS_840C::get_TRIGger_PULSe_TIMe(int *result)
{
    QByteArray output;
    bool ok = send(":TRIGger:PULSe:TIMe?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает режим отклонения частот.

0→ Выкл
1→ Низкий уровень отклонения частот
2→ Высокий уровень отклонения частот
*/
bool Oscilloscope_GDS_840C::set_TRIGger_REJect(Reject_mode value)
{
    QByteArray input;
    input.append(QString(":TRIGger:REJect %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Запрашивает режим отклонения частот.
*/
bool Oscilloscope_GDS_840C::get_TRIGger_REJect(int *result)
{
    QByteArray output;
    bool ok = send(":TRIGger:REJect?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает нарастание или спад фронта.

0→ нарастающий фронт
1→ спадающий фронт
*/
bool Oscilloscope_GDS_840C::set_TRIGger_SLOP(Slope value)
{
    QByteArray input;
    input.append(QString(":TRIGger:SLOP %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Запрашивает нарастание или спад фронта.
*/
bool Oscilloscope_GDS_840C::get_TRIGger_SLOP(int *result)
{
    QByteArray output;
    bool ok = send(":TRIGger:SLOP?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает источник запуска.

0→ Канал 1
1→ Канал 2
2→ Внешний запуск
3→ Напряжение в линии переменного тока
*/
bool Oscilloscope_GDS_840C::set_TRIGger_SOURce(Trigger_source value)
{
    QByteArray input;
    input.append(QString(":TRIGger:SOURce %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Запрашивает источник запуска.
*/
bool Oscilloscope_GDS_840C::get_TRIGger_SOURce(int *result)
{
    QByteArray output;
    bool ok = send(":TRIGger:SOURce?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает тип запуска.

0→ Граница
1→ Видео
2→ Импульс
3→ Задержка
*/
bool Oscilloscope_GDS_840C::set_TRIGger_TYPe(Trigger_type value)
{
    QByteArray input;
    input.append(QString(":TRIGger:TYPe %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Запрашивает тип запуска.
*/
bool Oscilloscope_GDS_840C::get_TRIGger_TYPe(int *result)
{
    QByteArray output;
    bool ok = send(":TRIGger:TYPe?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает область, на которой будет инициирован запуск видеорежима.

0→ Линия
1→ Нечетный фрейм (Область 1)
2→ Четный фрейм (Область 2)
*/
bool Oscilloscope_GDS_840C::set_TRIGger_VIDeo_FIELd(Field value)
{
    QByteArray input;
    input.append(QString(":TRIGger:VIDeo:FIELd %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Запрашивает область, на которой будет инициирован запуск видеорежима.
*/
bool Oscilloscope_GDS_840C::get_TRIGger_VIDeo_FIELd(int *result)
{
    QByteArray output;
    bool ok = send(":TRIGger:VIDeo:FIELd?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает указанную линию для видео сигнала.

Для системы NTSC, диапазон линии от 1~263 для нечетного фрейма, 1-262 для четного фрейма.
Для системы PAL,  диапазон линии 1~313 для нечетного фрейма,    1-312 для четного фрейма.
*/
bool Oscilloscope_GDS_840C::set_TRIGger_VIDeo_LINe(int value)
{
    QByteArray input;
    input.append(QString(":TRIGger:VIDeo:LINe %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Запрашивает указанную линию для видео сигнала.
*/
bool Oscilloscope_GDS_840C::get_TRIGger_VIDeo_LINe(int *result)
{
    QByteArray output;
    bool ok = send(":TRIGger:VIDeo:LINe?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает полярность видеовхода.

0→ Положительный синхронизирующий импульс
1→ Отрицательный синхронизирующий импульс
*/
bool Oscilloscope_GDS_840C::set_TRIGger_VIDeo_POLarity(Polarity value)
{
    QByteArray input;
    input.append(QString(":TRIGger:VIDeo:POLarity %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Запрашивает полярность видеовхода.
*/
bool Oscilloscope_GDS_840C::get_TRIGger_VIDeo_POLarity(int *result)
{
    QByteArray output;
    bool ok = send(":TRIGger:VIDeo:POLarity?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает тип широковещания ТВ.

0→ PAL
1→ NTSC
2→ SECAM
*/
bool Oscilloscope_GDS_840C::set_TRIGger_VIDeo_TYPe(Video_type value)
{
    QByteArray input;
    input.append(QString(":TRIGger:VIDeo:TYPe %1\n").arg(value));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Запрашивает тип широковещания ТВ.
*/
bool Oscilloscope_GDS_840C::get_TRIGger_VIDeo_TYPe(int *result)
{
    QByteArray output;
    bool ok = send(":TRIGger:VIDeo:TYPe?\n", &output);
    if(!ok)
    {
        emit debug("error send command");
        return false;
    }
    int temp = output.replace("\n", "").toInt(&ok);
    if(!ok)
    {
        emit debug(QString("error [%1] toInt").arg(output.data()));
        return false;
    }
    *result = temp;
    return true;
}
//--------------------------------------------------------------------------------
/*
Выберите, будет ли удалено изображение сигнала, после сохранения.

0→ Выкл
1→ Вкл
*/
bool Oscilloscope_GDS_840C::set_WMEMory_DISPlay(Channel channel, bool state)
{
    QByteArray input;
    input.append(QString(":WMEMory%1:DISPlay %2\n").arg(channel).arg(state));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Выберите, будет ли удалена информация о сигнале после сохранения.
*/
bool Oscilloscope_GDS_840C::set_WMEMory_ERASe(Channel channel, QByteArray *output)
{
    QByteArray input;
    input.append(QString(":WMEMory%1:ERASe %1\n").arg(channel));
    send(input, output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Устанавливает расположение сохраненения сигнала.

В диапазоне от –200 до +200
*/
bool Oscilloscope_GDS_840C::set_WMEMory_LOCate(Channel channel, int pos, QByteArray *output)
{
    QByteArray input;
    input.append(QString(":WMEMory%1:LOCate %2\n")
                 .arg(channel)
                 .arg(pos));
    send(input, output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Вы можете установить верхнюю или нижнюю позицию с помощью команды “:WMEMory<X>:LOCate”.

В диапазоне от –100 до +100.
*/
bool Oscilloscope_GDS_840C::set_WMEMory_OFFSet(Channel channel, int pos, QByteArray *output)
{
    QByteArray input;
    input.append(QString(":WMEMory%1:OFFSet %2\n")
                 .arg(channel)
                 .arg(pos));
    send(input, output);
    return true;
}
//--------------------------------------------------------------------------------
/*
Выбирает, будут ли сохранены настройки памяти.

1→ Канал 1
2→ Канал 2
3→ Математическая функция
*/
bool Oscilloscope_GDS_840C::set_WMEMory_SAVe(Ref_memory channel, Data data)
{
    QByteArray input;
    input.append(QString(":WMEMory%1:SAVe %2\n").arg(channel).arg(data));
    QByteArray output;
    send(input, &output);
    return true;
}
//--------------------------------------------------------------------------------
void Oscilloscope_GDS_840C::set_timeout(int timeout)
{
    this->timeout = timeout;
}
//--------------------------------------------------------------------------------
bool Oscilloscope_GDS_840C::measuring_all_channels(void)
{
    oscilloscope->clear();
    measuring_channel_1();
    measuring_channel_2();
    return true;
}
//--------------------------------------------------------------------------------
bool Oscilloscope_GDS_840C::measuring_channel_1(void)
{
    QByteArray output;
    bool ok = get_ACQuire_POINt(A, &output);
    if(!ok) return ok;

    if(output.size() != sizeof(OSC_HEADER))
    {
        emit error(QString("output.size %1 != %2")
                   .arg(output.size())
                   .arg(sizeof(OSC_HEADER)));
        return false;
    }

    OSC_HEADER *packet = (OSC_HEADER *)output.data();
    for(int n=0; n<500; n++)
    {
        oscilloscope->add_curve_data(channel_1, packet->data[n]);
    }

    return true;
}
//--------------------------------------------------------------------------------
bool Oscilloscope_GDS_840C::measuring_channel_2(void)
{
    QByteArray output;
    bool ok = get_ACQuire_POINt(B, &output);
    if(!ok) return ok;

    if(output.size() != sizeof(OSC_HEADER))
    {
        emit error(QString("output.size %1 != %2")
                   .arg(output.size())
                   .arg(sizeof(OSC_HEADER)));
        return false;
    }

    OSC_HEADER *packet = (OSC_HEADER *)output.data();
    for(int n=0; n<500; n++)
    {
        oscilloscope->add_curve_data(channel_2, packet->data[n]);
    }

    return true;
}
//--------------------------------------------------------------------------------
bool Oscilloscope_GDS_840C::send(QByteArray input,
                                 QByteArray *output,
                                 bool no_answer)
{
    if(serial.isOpen() == false)
    {
        emit error(tr("Порт осциллографа не открыт!"));
        return false;
    }

    data_oscilloscope.clear();
    is_ready = false;
    serial.write(input);

    QTime time;
    time.start();
    wait_msec(timeout);
    //emit info(QString(tr("Прошло времени %1 ms")).arg(time.elapsed()));

    if(no_answer) return true;

    if(data_oscilloscope.isEmpty())
    {
        //emit error(tr("Осциллограф не отвечает!"));
        return false;
    }

#if 0
    emit debug(data_oscilloscope.toHex());
    emit debug(QString("len %1 bytes").arg(data_oscilloscope.size()));
    emit debug(QString("size %1").arg(sizeof(OSC_HEADER)));
#endif

    (*output).append(data_oscilloscope);
    return true;
}
//--------------------------------------------------------------------------------
#if 0
OSC_HEADER *data = (OSC_HEADER *)data_oscilloscope.data();
emit debug(QString("octotorp %1").arg(data->octotorp));
emit debug(QString("four %1").arg(data->four));        //Содержит символ '4' (количество цифр в следующем поле, размер данных)
emit debug(QString("len %1%2%3%4")                     //Содержит символы '1008' (размер данных в байтах, 1000 байт осциллограмма плюс оставшаяся часть заголовка)
           .arg(data->len[0])
.arg(data->len[1])
.arg(data->len[2])
.arg(data->len[3]));
emit debug(QString("tdiv %1").arg(data->tdiv));        //Число в формате float, содержащее масштаб по оси времени (?)
emit debug(QString("chan %1").arg(data->chan, 0, 16)); //Номер канала в двоичной форме, 1 - первый канал, 2 - второй канал

union U_INT_BYTES x;
x.value = 0;
x.bytes.byte_3 = 0;
x.bytes.byte_2 = data->cnt[0];
x.bytes.byte_1 = data->cnt[1];
x.bytes.byte_0 = data->cnt[2];
emit debug(QString("cnt %1").arg(x.value));            //Число отсчетов в осциллограмме, всегда 500 (0x0001F4)
#endif
//--------------------------------------------------------------------------------
void Oscilloscope_GDS_840C::wait_msec(int timeout_msec)
{
    QTime time;

    if(timeout_msec < 1)
        return;

    time.start();
    while(time.elapsed() < timeout_msec)
    {
        QCoreApplication::processEvents();
    }
}
//--------------------------------------------------------------------------------
bool Oscilloscope_GDS_840C::test(unsigned int test_number)
{
    bool ok = false;
    unsigned char value = 1;
    int int_value = 0;
    QByteArray output;
    Channel channel = A;
    bool state = true;
    int pos = 0;
    float result = 0;

    switch(test_number)
    {
    case 0: ok = CLS(); break; break;
    case 1: ok = set_ESE(value); break;
    case 2: ok = get_ESE(&value); break;
    case 3: ok = get_ESR(&int_value); break;
    case 4: ok = get_IDN(&output); break;
    case 5: ok = get_LRN(&output); break;
    case 6: ok = get_OPC(&output); break;
    case 7: ok = set_OPC(value); break;
    case 8: ok = set_RCL(value); break;
    case 9: ok = set_RST(); break;
    case 10: ok = set_SAV(value); break;
    case 11: ok = get_SRE(&value); break;
    case 12: ok = set_SRE(value); break;
    case 13: ok = get_STB(&int_value); break;
    case 14: ok = set_WAI(); break;
    case 15: ok = set_ACQuire_AVERage(a2); break;
    case 16: ok = get_ACQuire_AVERage(&int_value); break;
    case 17: ok = set_ACQuire_LENGth(Len_500); break;
    case 18: ok = get_ACQuire_LENGth(&int_value); break;
    case 19: ok = set_ACQuire_MODe(value); break;
    case 20: ok = get_ACQuire_MODe(&int_value); break;
    case 21: ok = get_ACQuire_POINt(channel, &output); break;
    case 22: ok = set_AUToset(); break;
    case 23: ok = set_CHANnel_BWLimit(channel, true); break;
    case 24: ok = get_CHANnel_BWLimit(channel, &int_value); break;
    case 25: ok = set_CHANnel_COUPling(channel, AC); break;
    case 26: ok = get_CHANnel_COUPling(channel, &int_value); break;
    case 27: ok = set_CHANnel_DISPlay(channel, state); break;
    case 28: ok = set_CHANnel_INVert(channel, state); break;
    case 29: ok = get_CHANnel_INVert(channel, &int_value); break;
    case 30: ok = set_CHANnel_MATH(channel, state); break;
    case 31: ok = get_CHANnel_MATH(channel, &int_value); break;
    case 32: ok = set_CHANnel_OFFSet(channel, state); break;
    case 33: ok = get_CHANnel_OFFSet(channel, &int_value); break;
    case 34: ok = set_CHANnel_PROBe(channel, X1); break;
    case 35: ok = get_CHANnel_PROBe(channel, &int_value); break;
    case 36: ok = set_CHANnel_SCALe(channel, VScale_1V); break;
    case 37: ok = get_CHANnel_SCALe(channel, &int_value); break;
    case 38: ok = set_CURSor_XPosition(channel, pos); break;
    case 39: ok = set_CURSor_YPosition(channel, pos); break;
    case 40: ok = get_CURSor_DELta(channel, &int_value); break;
    case 41: ok = set_CURSor_XDISplay(state); break;
    case 42: ok = set_CURSor_YDISplay(state); break;
    case 43: ok = set_CURSor_SOURce(s1); break;
    case 44: ok = get_CURSor_SOURce(&int_value); break;
    case 45: ok = set_DISPlay_ACCumulate(state); break;
    case 46: ok = get_DISPlay_ACCumulate(&int_value); break;
    case 47: ok = set_DISPlay_CONTrast(10); break;
    case 48: ok = get_DISPlay_CONTrast(&int_value); break;
    case 49: ok = set_DISPlay_GRATicule(Full_grids); break;
    case 50: ok = get_DISPlay_GRATicule(&int_value); break;
    case 51: ok = get_MEASure_FALL(&result); break;
    case 52: ok = get_MEASure_FREQuency(&result); break;
    case 53: ok = get_MEASure_NWIDth(&result); break;
    case 54: ok = get_MEASure_PDUTy(&result); break;
    case 55: ok = get_MEASure_PERiod(&result); break;
    case 56: ok = get_MEASure_PWIDth(&result); break;
    case 57: ok = get_MEASure_RISe(&result); break;
    case 58: ok = set_MEASure_SOURce(Channel_1); break;
    case 59: ok = get_MEASure_VAMPlitude(&result); break;
    case 60: ok = get_MEASure_VAVerage(&result); break;
    case 61: ok = get_MEASure_VHI(&result); break;
    case 62: ok = get_MEASure_VLO(&result); break;
    case 63: ok = get_MEASure_VMAX(&result); break;
    case 64: ok = get_MEASure_VMIN(&result); break;
    case 65: ok = get_MEASure_VPP(&result); break;
    case 66: ok = get_MEASure_VRMS(&result); break;
    case 67: ok = set_PRINt(); break;
    case 68: ok = set_REFResh(); break;
    case 69: ok = set_RUN(); break;
    case 70: ok = set_STOP(); break;
    case 71: ok = set_SYSTem_UNLock(); break;
    case 72: ok = set_TIMebase_DELay(HScale_100us); break;
    case 73: ok = get_TIMebase_DELay(&int_value); break;
    case 74: ok = set_TIMebase_SCALe(HScale_100us); break;
    case 75: ok = get_TIMebase_SCALe(&int_value); break;
    case 76: ok = set_TIMebase_SWEep(Main_timebase); break;
    case 77: ok = get_TIMebase_SWEep(&int_value); break;
    case 78: ok = set_TIMebase_WINDow_DELay(value); break;
    case 79: ok = get_TIMebase_WINDow_DELay(&int_value); break;
    case 80: ok = set_TIMebase_WINDow_SCALe(value); break;
    case 81: ok = get_TIMebase_WINDow_SCALe(&int_value); break;
    case 82: ok = set_TRIGger_COUPle(t_AC); break;
    case 83: ok = get_TRIGger_COUPle(&int_value); break;
    case 84: ok = set_TRIGger_DELay_TIMe(value); break;
    case 85: ok = get_TRIGger_DELay_TIMe(&int_value); break;
    case 86: ok = set_TRIGger_DELay_EVENt(value); break;
    case 87: ok = get_TRIGger_DELay_EVENt(&int_value); break;
    case 88: ok = set_TRIGger_DELay_LEVel(value); break;
    case 89: ok = get_TRIGger_DELay_LEVel(&int_value); break;
    case 90: ok = set_TRIGger_DELay_MODe(TTL); break;
    case 91: ok = get_TRIGger_DELay_MODe(&int_value); break;
    case 92: ok = set_TRIGger_DELay_TYPe(Time_setting); break;
    case 93: ok = get_TRIGger_DELay_TYPe(&int_value); break;
    case 94: ok = get_TRIGger_FREQuency(&int_value); break;
    case 95: ok = set_TRIGger_LEVel(value); break;
    case 96: ok = get_TRIGger_LEVel(&int_value); break;
    case 97: ok = set_TRIGger_MODe(Auto); break;
    case 98: ok = get_TRIGger_MODe(&int_value); break;
    case 99: ok = set_TRIGger_NREJ(value); break;
    case 100: ok = get_TRIGger_NREJ(&int_value); break;
    case 101: ok = set_TRIGger_PULSe_MODe(value); break;
    case 102: ok = get_TRIGger_PULSe_MODe(&int_value); break;
    case 103: ok = set_TRIGger_PULSe_TIMe(value); break;
    case 104: ok = get_TRIGger_PULSe_TIMe(&int_value); break;
    case 105: ok = set_TRIGger_REJect(OFF); break;
    case 106: ok = get_TRIGger_REJect(&int_value); break;
    case 107: ok = set_TRIGger_SLOP(Rising); break;
    case 108: ok = get_TRIGger_SLOP(&int_value); break;
    case 109: ok = set_TRIGger_SOURce(s_Channel_1); break;
    case 110: ok = get_TRIGger_SOURce(&int_value); break;
    case 111: ok = set_TRIGger_TYPe(Edge); break;
    case 112: ok = get_TRIGger_TYPe(&int_value); break;
    case 113: ok = set_TRIGger_VIDeo_FIELd(Line); break;
    case 114: ok = get_TRIGger_VIDeo_FIELd(&int_value); break;
    case 115: ok = set_TRIGger_VIDeo_LINe(value); break;
    case 116: ok = get_TRIGger_VIDeo_LINe(&int_value); break;
    case 117: ok = set_TRIGger_VIDeo_POLarity(Positive); break;
    case 118: ok = get_TRIGger_VIDeo_POLarity(&int_value); break;
    case 119: ok = set_TRIGger_VIDeo_TYPe(PAL); break;
    case 120: ok = get_TRIGger_VIDeo_TYPe(&int_value); break;
    case 121: ok = set_WMEMory_DISPlay(channel, value); break;
    case 122: ok = set_WMEMory_ERASe(channel, &output); break;
    case 123: ok = set_WMEMory_LOCate(channel, pos, &output); break;
    case 124: ok = set_WMEMory_OFFSet(channel, pos, &output); break;
    case 125: ok = set_WMEMory_SAVe(RefA, m_Channel_1); break;

    default: ok = false; break;
    }

    return ok;
}
//--------------------------------------------------------------------------------
bool Oscilloscope_GDS_840C::test_all(void)
{
    bool ok = false;
    for(int n=0; n<=125; n++)
    {
        if(n == 40) continue;
        if(n == 85) continue;
        if(n == 87) continue;
        if(n == 89) continue;
        if(n == 91) continue;
        if(n == 93) continue;
        if(n == 102) continue;
        if(n == 104) continue;
        emit info(QString("test %1 of 125").arg(n));
        ok = test(n);
        if(!ok)
        {
            emit error(QString("test %1 failed!").arg(n));
            return false;
        }
    }
    return true;
}
//--------------------------------------------------------------------------------
void Oscilloscope_GDS_840C::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
