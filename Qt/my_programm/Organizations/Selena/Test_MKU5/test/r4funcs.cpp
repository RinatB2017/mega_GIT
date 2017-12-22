//--------------------------------------------------------------------------------
#include <unistd.h>
//--------------------------------------------------------------------------------
#include "r4funcs.hpp"
#include "r4defs.hpp"
//--------------------------------------------------------------------------------
#define END_SECTION	"END\n"
//--------------------------------------------------------------------------------
#define htons(n) (unsigned short)((((unsigned short) (n)) << 8) | (((unsigned short) (n)) >> 8))
#define htonl(n) (unsigned int)((((unsigned int) (n)) << 16) | (((unsigned int) (n)) >> 16))
//--------------------------------------------------------------------------------
ushort array_to_ushort(const QByteArray& array,
                       int pos,
                       bool net_order,
                       bool* ptr_flag)
{
    if (array.length() < pos + 2)
    {
        if (ptr_flag) *ptr_flag = false;
        return 0;
    }
    QByteArray mid = array.mid(pos, 2);
    uchar hi, low;
    if (net_order)
    {
        hi = mid[0];
        low = mid[1];
    }
    else
    {
        hi = mid[1];
        low = mid[0];
    }
    ushort val;
    val = hi << 8;
    val |= low;
    if (ptr_flag) *ptr_flag = true;
    return val;
}
//--------------------------------------------------------------------------------
void append_to_array(QByteArray& array,
                     ushort val,
                     bool net_order /*=false*/)
{
    ushort value = val;
    if (net_order)
        value = htons(val);
    uchar low = value;
    uchar hi = value >> 8;
    array.append(low);
    array.append(hi);
}
//--------------------------------------------------------------------------------
QStringList::const_iterator get_iterator_by_index(int index,
                                                  const QStringList& list)
{
    QStringList::const_iterator it = list.begin();
    int i = 0;
    while ((it != list.end()) && (i != index))
    {
        it++;
        i++;
    }
    return it;
}
//--------------------------------------------------------------------------------
int byte_array_to_uchar(const QByteArray& array,
                        uchar* data)
{
    if ( !data ) return -1;
    int index = 0;
    for (int i = 0; i < array.size(); i++)
    {
        data[index++] = (uchar)array.at(i);
    }
    return index;
}
//--------------------------------------------------------------------------------
QString byte_array_to_string(const QByteArray& ba,
                             bool ascii /*=false*/, bool separate/*=true*/)
{
    QString string;
    for (int i = 0; i < ba.size(); i++)
    {
        QString s;
        if (ascii) s = ba.at(i);
        else s = s.sprintf("%.2x", (uchar)ba.at(i));
        string.append(s);
        if (separate && (i != ba.size()-1)) string.append(" ");
    }
    return string;
}
//--------------------------------------------------------------------------------
QByteArray create_byte_array(uchar* data,
                             int length)
{
    QByteArray array;
    if (!data || length < 0) return array;
    array.resize(length);
    for (int i = 0; i < length; i++)
        array[i] = data[i];
    return array;
}
//--------------------------------------------------------------------------------
QByteArray create_byte_array(const QString& string,
                             int base)
{
    QStringList list = string.split(" ");
    QByteArray ba;
    ba.resize(list.count());
    uchar byte;
    int index = 0;
    for (QStringList::ConstIterator it = list.begin(); it != list.end(); it++)
        if (stringToByte(&byte, *it, base) < 0)
        {
            ba.resize(0);
            break;
        }
        else
        {
            ba[index++] = byte;
        }
    return ba;
}
//--------------------------------------------------------------------------------
int find_in_buf(uchar* buf,
                int length,
                uchar data,
                int from)
{
    //qDebug("%d %d", length, from);
    if (!buf || (length < 0) || (from > length-1))
        return -2;
    for ( int i = from; i < length; i++ )
        if (buf[i] == data)
            return i;
    return -1;
}
//--------------------------------------------------------------------------------
int getUCharArray( const QString& line,
                   uchar* buf,
                   int base )
{
    QStringList list;
    //list = QStringList::split(" ", line);
    list = line.simplified().split(" ");
    int index = 0;
    for ( QStringList::ConstIterator it = list.begin(); it != list.end(); ++it )
    {
        bool isok;
        ushort val = (*it).toUShort( &isok, base );
        if ( !isok || !isInRange( val, 0, 255 ) ) return -1;
        buf[ index++ ] = val;
    }
    return index;
}
//--------------------------------------------------------------------------------
QString getUCharString( uchar* data,
                        int length )
{
    QString text="[empty]";
    if ( !data ) return text;
    if ( length <= 0 ) return text;
    text ="";
    QString str;
    for ( int  i = 0; i < length; i++ )
        str += text.sprintf( "%.2x ", data[i] );
    return str;
}
//--------------------------------------------------------------------------------
QString getUShortString( ushort* data,
                         int length )
{
    QString text="[empty]";
    if ( !data ) return text;
    if ( length <= 0 ) return text;
    text ="";
    QString str;
    for ( int  i = 0; i < length; i++ )
        str += text.sprintf( "%.4x ", data[i] );
    return str;
}
//--------------------------------------------------------------------------------
int wordcpy( ushort* dest,
             ushort* source,
             int length  )
{
    if ( !dest || !source ) return -1;
    if ( length <= 0 ) return -2;
    memcpy( dest, source, length*2 );
    return 0;
}
//--------------------------------------------------------------------------------
int stringToByte( uchar* ptr_byte,
                  const QString& string,
                  int base )
{
    if ( !ptr_byte ) return -1;
    bool isok;
    ushort val = string.toUShort( &isok, base );
    if ( !isok || ( val > 255 ) ) return -2;
    *ptr_byte = val;
    return 1;
}
//--------------------------------------------------------------------------------
int stringToUShortNet( uchar* buf,
                       const QString& string,
                       int base /*= 10*/ )
{
    if ( !buf ) return -1;
    bool isok;
    ushort val = string.toUShort( &isok, base );
    if ( !isok ) return -2;
    ushort* ptr_val = (ushort*)buf;
    *ptr_val = htons( val );
    return sizeof( ushort );
}
//--------------------------------------------------------------------------------
int stringToUShortNet( ushort* buf,
                       const QString& string,
                       int base /*= 10*/ )
{
    if ( !buf ) return -1;
    bool isok;
    ushort val = string.toUShort( &isok, base );
    if ( !isok ) return -2;
    *buf = htons( val );
    return sizeof( ushort );
}
//--------------------------------------------------------------------------------
int stringToUShort( uchar* buf,
                    const QString& string )
{
    if ( !buf ) return -1;
    bool isok;
    ushort val = string.toUShort( &isok );
    if ( !isok ) return -2;
    ushort* ptr_val = (ushort*)buf;
    *ptr_val = val;
    return sizeof( ushort );
}
//--------------------------------------------------------------------------------
int stringToUShort( ushort* buf,
                    const QString& string )
{
    if ( !buf ) return -1;
    bool isok;
    ushort val = string.toUShort( &isok );
    if ( !isok ) return -2;
    *buf = val;
    return sizeof( ushort );
}
//--------------------------------------------------------------------------------
int stringToULong( uchar* buf,
                   const QString& string )
{
    if (!buf) return -1;
    bool isok;
    ulong val = string.toULong(&isok);
    if (!isok) return -2;
    ulong* ptr = (ulong*)buf;
    *ptr = val;
    return sizeof(ulong);
}
//--------------------------------------------------------------------------------
int stringToULongNet( uchar* buf,
                      const QString& string )
{
    if ( !buf ) return -1;
    bool isok;
    ulong val = string.toULong( &isok );
    if ( !isok ) return -2;
    ulong* ptr_val = (ulong*)buf;
    *ptr_val = htonl( val );
    return sizeof( ulong );
}
//--------------------------------------------------------------------------------
int stringToULongNet( ulong* ptr,
                      const QString& string )
{
    if ( !ptr ) return -1;
    bool isok;
    ulong val = string.toULong( &isok );
    if ( !isok ) return -2;
    *ptr = htonl( val );
    return sizeof( ulong );
}
//--------------------------------------------------------------------------------
ushort bitArrayToUShort( const QBitArray& array )
{
    ushort val = 0;
    ushort mask = 1;
    for ( int i = 0; i < array.size(); i++ )
    {
        if ( array[i] == 1 ) val |= mask;
        mask <<= 1;
    }
    return val;
}
//--------------------------------------------------------------------------------
QString bitArrayToString( const QBitArray& array, bool order /*= true*/ ) 
{
    QString string;
    if ( order )
    {
        for ( int i = 0; i < array.size(); i++ )
            string += QString::number( array[i] );
    }
    else
    {
        for ( int i = array.size()-1; i >=0; i-- )
            string += QString::number( array[i] );
    }
    return string;
}
//--------------------------------------------------------------------------------
int getCountInBitArray( int val,
                        const QBitArray& array )
{
    int count = 0;
    for ( int i = 0; i < array.size(); i++ )
        if ( array[ i ] == val ) count++;
    return count;
}
//--------------------------------------------------------------------------------
QBitArray getBitArray( ushort size,
                       unsigned long val )
{
    QBitArray bit_array( size );
    ushort mask = 1;
    for ( ushort i = 0; i < size; i++ )
    {
        if ( mask & val ) bit_array[i] = 1;
        else bit_array[i] = 0;
        mask <<= 1;
    }
    return bit_array;
}
//--------------------------------------------------------------------------------
int getParametr( QFile& file,
                 QString param,
                 QString& val )
{
    // --- проверяем возможность открытия файла
    if (!file.isOpen())
    {
        if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
        {
            qError( "can't open file" );
            return -1;
        }
    }
    QString line;
    ulong size;
    const int max_length = 1024;
    char buf[max_length];
    while ( !file.atEnd() )
    {

        size = file.readLine( &buf[0], max_length );
        line = QString(&buf[0]);
        line = line.simplified();
        if ( size <  1 ) continue;
        QStringList list = line.split(" ");
        QStringList::iterator it = list.begin();
        if ( *it == param )
        {
            val = *( ++it );
            file.close();
            return val.length();
        }
    }
    file.close();
    return 0;
}
//--------------------------------------------------------------------------------
/*
тоже что и getParam
*/
int getSection(QFile& file,
               const QString& section_name,
               QStringList& output_list,
               bool with_comments /*=false*/)
{
    // --- проверяем возможность открытия файла
    if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        qError( "can't open file" );
        return -1;
    }
    QString line;
    ushort size;
    bool flag = false;	// флаг лексемы
    const int max_length = 1024;
    char buf[max_length];
    // --- перебираем все строки
    while ( !file.atEnd() )
    {
        // --- читаем строку
        size = file.readLine( &buf[0], max_length );
        line = QString(&buf[0]);
        // --- проверяем на минимальную длину строки
        if ( size <  1 ) continue;
        if ( !flag )
        {
            // --- очищаем от нежелательных пробелов
            line = line.simplified();
            // --- формируем список лексем строки
            QStringList list = line.split(" ");
            // --- получем первую лексему
            QStringList::Iterator it = list.begin();
            // --- сравнение на ключевое слово
            if ( *it == section_name ) flag = true;
            continue;
        }
        // --- пока не встретили КОНЕЦ СЕКЦИИ добавляем строки в выходной список
        if ( line != END_SECTION )
        {
            if (with_comments) output_list << line;
            else if (line[0] != '#') output_list << line;

        }
        else
        {
            file.close();
            return output_list.count();
        }

    }
    file.close();
    return 0;
}
//--------------------------------------------------------------------------------
/*
Метод для формирования текста (val) секции в файле file 
между ключевыми словами param и END
*/
int getParam( QFile& file,
              QString key,
              QString& text )
{
    // --- проверяем возможность открытия файла
    if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        qError( "can't open file" );
        return -1;
    }
    QString line;
    ushort size;
    bool flag = false;	// флаг лексемы
    QStringList listOut;  // список для вывода
    text = "[пусто]";
    const int max_length = 1024;
    char buf[max_length];
    // --- перебираем все строки
    while ( !file.atEnd() )
    {
        // --- читаем строку
        size = file.readLine( &buf[0], max_length );
        line = QString(&buf[0]);
        // --- проверяем на минимальную длину строки
        if ( size <  1 ) continue;
        if ( !flag )
        {
            // --- очищаем от нежелательных пробелов
            line = line.simplified();
            // --- формируем список лексем строки
            QStringList list = line.split(" ");
            // --- получем первую лексему
            QStringList::Iterator it = list.begin();
            // --- сравнение на ключевое слово
            if ( *it == key ) flag = true;
            continue;
        }
        // --- пока не встретили КОНЕЦ СЕКЦИИ добавляем строки в выходной список
        if ( line != END_SECTION ) listOut << line.append("<br />");
        else
        {
            text = listOut.join("");
            file.close();
            return text.length();
        }

    }
    file.close();
    return 0;
}
//--------------------------------------------------------------------------------
int get_byte_mask( unsigned char* mask,
                   unsigned short index )
{
    if ( index > 7 ) return -1;
    *mask = 1 << index;
    return 1;
}
//--------------------------------------------------------------------------------
int find_first_1( unsigned short value )
{
    unsigned short mask = 1;
    for ( int i = 0; i <=15; i ++ )
    {
        if ( mask & value ) return i;
        value >>= 1;
    }
    return -1;
}
//--------------------------------------------------------------------------------
float reverse_float( float value )
{
    unsigned char* ptr = ( unsigned char* )&value;
    float rev_value;
    unsigned char* rev_ptr = (unsigned char*)&rev_value;
    rev_ptr[0] = ptr[3];
    rev_ptr[1] = ptr[2];
    rev_ptr[2] = ptr[1];
    rev_ptr[3] = ptr[0];
    return rev_value;
}
//--------------------------------------------------------------------------------
QLabel *get_new_status_label( QString text,
                              QWidget* parent )
{
    QLabel* lbl = new QLabel( parent );
    lbl->setText( text );
    lbl->setFrameStyle( QFrame::StyledPanel | QFrame::Plain );
    return lbl;
}
//--------------------------------------------------------------------------------
QLabel* get_new_header_label( QString text,
                              QWidget* parent )
{
    QLabel* lbl = new QLabel( parent );
    lbl->setText( text );
    lbl->setAlignment( Qt::AlignVCenter | Qt::AlignHCenter );
    lbl->setFrameStyle( QFrame::StyledPanel | QFrame::Plain );
    return lbl;
}
//--------------------------------------------------------------------------------
QString fr( QString text )
{
    //return QString::fromLocal8Bit( text.ascii() );
    return text;
}
//--------------------------------------------------------------------------------
void frList( QStringList& list )
{
    QStringList cp_list = list;
    list.clear();
    for ( QStringList::Iterator it = cp_list.begin();
          it != cp_list.end(); ++it )
    {
        list << fr( *it );
    }
}
//--------------------------------------------------------------------------------
QString rf(const QString& str)
{
    //return str.local8Bit();
    return str;
}
//--------------------------------------------------------------------------------
void reverseStringList( QStringList& list )
{
    QStringList list2;
    QStringList::Iterator it;
    for ( it=list.begin(); it != list.end(); it++ )
        list2.prepend( *it );
    list = list2;
}
//--------------------------------------------------------------------------------
bool isInRange( int value,
                int v1,
                int v2 )
{
    int min, max;
    if ( v1 < v2 )
    {
        min = v1; max=v2;
    }
    else
    {
        min = v2; max=v1;
    }
    if ( ( value < min ) || ( value > max ) )
        return false;
    return true;
}
//--------------------------------------------------------------------------------
bool isInRange( const QString& string,
                int v1,
                int v2,
                int* ptr )
{
    if ( !ptr ) return false;
    bool isok;
    *ptr = string.toInt( &isok );
    if ( !isok ) return false;
    return isInRange( *ptr, v1, v2 );
}
//--------------------------------------------------------------------------------
int close_descriptor(int fd)
{
    return close(fd);
}
//--------------------------------------------------------------------------------
