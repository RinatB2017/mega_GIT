//--------------------------------------------------------------------------------
#include "test_rs232_1.hpp"
//--------------------------------------------------------------------------------
const int Max_Pack_Length = 1500;
const int Max_Pack_Count  = 1500;
//--------------------------------------------------------------------------------
const uchar Send_info_message   = 0x0D;
const uchar length_test_message = 0xFF;
//--------------------------------------------------------------------------------
const uchar Address_RS232_1 = 0x01;
//--------------------------------------------------------------------------------
Test_RS232_1::Test_RS232_1(QObject* parent,
                           const QString address,
                           unsigned int port) :
    TestThread(parent)
{
    this->address = address;
    this->port = port;
    sleep_interval = 1000;
}
//--------------------------------------------------------------------------------
void Test_RS232_1::run()
{
    print("<br />Проверка последовательного канала обмена данными по интерфейсу RS-232 N1.<br />", 0, true);
    if (!init_proto(address, port))
    {
        delete_proto();
        return;
    }
    QByteArray array = get_filled_array( length_test_message );
    QByteArray answer;
    if (proto->sendTestCmd(Send_info_message, Address_RS232_1, array, answer, 60000) != Proto::State_Ok)
    {
        print(QString("Ошибка (%1)").arg(proto->errstr()), -1);
    }
    else
    {
        // --- контролируем ответ
        // --- --- обрезаем заголовок
        answer = answer.mid(proto->headerSize());
        if (answer.length() == length_test_message )
        {
            if( answer != array )
                print("Неисправно");
            else
                print("Исправно", 1, true);
        }
        else
        {
            qDebug() << answer.toHex();
            print("Ответ неопределен");
        }
    }
    delete_proto();
}
//--------------------------------------------------------------------------------
QByteArray Test_RS232_1::get_filled_array(int size)
{
    QByteArray array;
    uchar byte = 0x01;
    for (int i = 0; i < size; i++)
        array.append(byte++);
    return array;
}
//--------------------------------------------------------------------------------
