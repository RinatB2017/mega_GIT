//--------------------------------------------------------------------------------
#include <QAbstractSocket>
//--------------------------------------------------------------------------------
#include "proto.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#if 0
typedef struct
{
    ushort marker;  // маркер начала 0x55a9
    ushort length;  // длина данных
    uchar address;  // адрес модуля
    uchar use;      // назначение сообщения (0-отладочная информация,
                    // 1 - сообщение ТПО, 2 - сообщение СПО)
    uchar nb;       // null byte const = 0
    uchar code;     // код команды
} MessageHeader;
#endif
//--------------------------------------------------------------------------------
const int Marker_Index  = 0;
const int Length_Index  = 2;
const int Address_Index = 4;
const int Use_Index     = 5;
const int NB_Index      = 6;
const int Code_Index    = 7;

const short Marker      = 0x55a9;
const uchar null_byte   = 0x00;
const uchar Debug_Cmd   = 0x00;
const uchar Test_Cmd    = 0x01;
const uchar Spec_Cmd    = 0x02;

const int Write_Interval = 2000;
const int Header_Size = 8;
//--------------------------------------------------------------------------------
Proto::Proto(QTcpSocket* s) : socket(s)
{  
    error_code = 0;
    append_to_array(marker_array, Marker, true);
}
//--------------------------------------------------------------------------------
Proto::~Proto()
{
    socket->disconnectFromHost();
    if (socket->state() != QAbstractSocket::UnconnectedState)
    {
        if (!socket->waitForDisconnected(3000))
            socket->abort();
    }
}
//--------------------------------------------------------------------------------
int Proto::headerSize()
{
    return Header_Size;
}
//--------------------------------------------------------------------------------
Proto::State Proto::sendTestCmd(uchar code,
                                const QByteArray& array)
{
    return request(0, Test_Cmd, code, array);
}
//--------------------------------------------------------------------------------
Proto::State Proto::sendTestCmd(uchar code,
                                const QByteArray&
                                array,
                                QByteArray& answer,
                                int interval)
{
    return request(0, Test_Cmd, code, array, answer, interval);
}
//--------------------------------------------------------------------------------
Proto::State Proto::sendTestCmd(uchar code,
                                uchar addr,
                                const QByteArray& array,
                                QByteArray& answer,
                                int interval)
{
    return request(addr, Test_Cmd, code, array, answer, interval);
}
//--------------------------------------------------------------------------------
Proto::State Proto::sendSpecCmd(uchar code,
                                uchar addr,
                                const QByteArray& array)
{
    return request(addr, Spec_Cmd, code, array);
}
//--------------------------------------------------------------------------------
Proto::State Proto::sendSpecCmd(uchar code,
                                uchar addr,
                                const QByteArray& array,
                                QByteArray& answer,
                                int interval)
{
    return request(addr, Spec_Cmd, code, array, answer, interval);
}
//--------------------------------------------------------------------------------
Proto::State Proto::request(uchar addr,
                            uchar use,
                            uchar code,
                            const QByteArray& data)
{
    QByteArray array;
    append_to_array(array, Marker, true);
    append_to_array(array, data.length(), true);
    array[Address_Index] = addr;
    array[Use_Index] = use;
    array[NB_Index] = null_byte;
    array[Code_Index] = code;
    array.append(data);
    int written = socket->write(array);
    //if (!socket->waitForBytesWritten(Write_Interval))
    if (written != array.length())
    {
        error_code = -8;
        return State_Write_Error;
    }
    return State_Ok;
}
//--------------------------------------------------------------------------------
Proto::State Proto::request(uchar addr,
                            uchar use,
                            uchar code,
                            const QByteArray& data,
                            QByteArray& inbuf,
                            int interval)
{
    // --- выполняем запрос
    State state = request(addr, use, code, data);
    if (state != State_Ok)
        return state;
    // --- принимаем ответное сообщение
    QByteArray answer;
    state = read(interval, answer);
    if ( state != State_Ok ) return state;
    // --- сохраняем полученное сообщение
    inbuf = answer;
    // --- контроль ответного сообщения
    // --- --- контроль адреса
    if (answer.at(Address_Index) != addr)
    {
        error_code = -5;
        return State_Read_Error;
    }
    // --- --- контроль байта назначения
    if (answer.at(Use_Index) != use)
    {
        error_code = -6;
        return State_Read_Error;
    }
    uchar answer_code = answer.at(Code_Index);
    // --- --- контроль кода команды
    if (answer_code != code)
    {
        qDebug() << answer.toHex();
        error_code = -7;
        return State_Read_Error;
    }
    // --- all ok
    return State_Ok;
}
//--------------------------------------------------------------------------------
Proto::State Proto::read(int interval,
                         QByteArray& answer)
{
    // --- выполняем ожидание ответных данных (1 раз)
    if (wait(interval) != State_Ok)
    {
        //qDebug() << socket->state();
        return State_Read_Error;
    }
    // --- Читаем данные из сокета
    QByteArray array = socket->readAll();
    // --- проверяем наличие маркера
    if (!array.contains(marker_array))
    {
        error_code = -3;
        return State_Read_Error;
    }
    // --- убираем лишние данные перед маркером (при их наличии)
    int marker_pos = array.indexOf(marker_array);
    array.remove(0, marker_pos);
    // --- проверяем минимальную длину
    if (array.length() < Header_Size)
    {
        // --- если размер полученных данных меньше минимальных - ждем (2 раза)
        if (wait(interval) != State_Ok) return State_Read_Error;
        // --- если данные подошли - выполняем повторную проверку
        if (array.length() < Header_Size)
        {
            error_code = -2;
            return State_Read_Error;
        }
    }
    // --- проверяем заявленную длину информационной части сообщения
    ushort length = array_to_ushort(array, 2, true);
    if (array.length() != Header_Size + length)
    {
        error_code = -4;
        return State_Read_Error;
    }
    // ---
    error_code = 0;
    answer = array;
    return State_Ok;
}
//--------------------------------------------------------------------------------
Proto::State Proto::wait(int interval)
{
    if (!socket->waitForReadyRead(interval))
    {
        error_code = -1;
        return State_Read_Error;
    }
    error_code = 0;
    return State_Ok;
}
//--------------------------------------------------------------------------------
ushort Proto::change_order(ushort val)
{
    uchar hi = val >> 8;
    uchar low = val;
    ushort res = low << 8;
    res |= hi;
    return res;
}
//--------------------------------------------------------------------------------
QString Proto::errstr()
{
    return errstr(error_code);
}
//--------------------------------------------------------------------------------
QString Proto::errstr(int code)
{
    QString str;
    switch (code)
    {
    case -1: str = "Время ожидания ответа истекло"; break;
    case -2: str = "Недостаточная длина для анализа"; break;
    case -3: str = "Маркер не найден"; break;
    case -4: str = "Ошибка длины сообщения"; break;
    case -5: str = "Ошибка адреса в ответе"; break;
    case -6: str = "Ошибка кода назначение в ответе"; break;
    case -7: str = "Ошибка кода команды в ответе"; break;
    case -8: str = "Ошибка записи"; break;
    case -9: str = "Соединение разорвано"; break;
    default: str = "Неизвестная ошибка"; break;
    }
    return str;
}
//--------------------------------------------------------------------------------
