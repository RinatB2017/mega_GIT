//--------------------------------------------------------------------------------
#include "test_flbi_3.hpp"
//--------------------------------------------------------------------------------
const int Max_Pack_Length = 4096;
const int Max_Pack_Count  = 4096;

const uchar Send_info_message = 0x0D;
const uchar Address_MCDATC_3  = 0x06;
//--------------------------------------------------------------------------------
Test_FLBI_3::Test_FLBI_3(QObject* parent,
                         const QString address,
                         unsigned int port) :
    TestThread(parent)
{
    this->address.setAddress(address);
    this->port = port;
    sleep_interval = 1000;
}
//--------------------------------------------------------------------------------
void Test_FLBI_3::run()
{
    print("<br />Проверка последовательного канала обмена данными С1-ФЛ-БИ N3.<br />", 0, true);
    if (!init_proto(address.toString(), port))
    {
        delete_proto();
        return;
    }
    QByteArray array, answer;
    for( int i = 0; i < 0xFF; i++ )
    {
        array[0] = i+1;
        if( proto->sendTestCmd( Send_info_message, Address_MCDATC_3, array, answer, 60000 ) != Proto::State_Ok )
        {
            print(QString("Ошибка (%1)").arg(proto->errstr()), -1);
        }
        else
        {
            answer = answer.mid(proto->headerSize());
            if( answer.length() == 1 )
                if( answer[0] != array[0] )
                {
                    print( QString("Неисправно"), -1, true );
                    delete_proto();
                    return;
                }
        }
    }
    print( "Исправно", 1, true );
    delete_proto();
}
//--------------------------------------------------------------------------------
QByteArray Test_FLBI_3::get_filled_array(int size)
{
    QByteArray array;
    uchar byte = 1;
    for (int i = 0; i < size; i++)
        array.append(byte++);
    return array;
}
//--------------------------------------------------------------------------------
