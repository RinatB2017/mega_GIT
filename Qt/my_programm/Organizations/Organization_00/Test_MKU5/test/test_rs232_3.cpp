//--------------------------------------------------------------------------------
#include "test_rs232_3.hpp"
//--------------------------------------------------------------------------------
const uchar Send_info_message = 0x0D;
const uchar Address_RS232_3 = 0x03;
//--------------------------------------------------------------------------------
Test_RS232_3::Test_RS232_3(QObject* parent,
                           const QString address,
                           int port) :
    TestThread(parent)
{
    this->address.setAddress(address);
    this->port = port;
    sleep_interval = 1000;
}
//--------------------------------------------------------------------------------
void Test_RS232_3::run()
{
    print("<br />Проверка последовательного канала обмена данными по интерфейсу RS-232 N3.<br />", 0, true);
    if (!init_proto(address.toString(),
                    static_cast<unsigned int>(port)))
    {
        delete_proto();
        return;
    }
    QByteArray array = get_filled_array( 0xFF );
    QByteArray answer;
    if (proto->sendTestCmd(Send_info_message, Address_RS232_3, array, answer, 60000) != Proto::State_Ok)
    {
        print(QString("Ошибка (%1)").arg(proto->errstr()), -1);
    }
    else
    {
        // --- контролируем ответ
        // --- --- обрезаем заголовок
        answer = answer.mid(proto->headerSize());
        if (answer.length() == 0xFF)
        {
            for( int i = 0; i < 0xFF; i++ )
            {
                if( array[i] != answer[i] )
                {
                    print( "Неисправно", -1, true );
                    return;
                }
            }
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
QByteArray Test_RS232_3::get_filled_array(int size)
{
    QByteArray array;
    char byte = 1;
    for (int i = 0; i < size; i++)
        array.append(byte++);
    return array;
}
//--------------------------------------------------------------------------------
