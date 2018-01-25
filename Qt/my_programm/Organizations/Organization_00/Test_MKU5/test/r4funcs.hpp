//--------------------------------------------------------------------------------
#ifndef _R4FUNCS_H
#define _R4FUNCS_H
//--------------------------------------------------------------------------------
#include <QString>
#include <QLabel>
#include <QFile>
#include <QBitArray>
#include <QByteArray>
//--------------------------------------------------------------------------------
#define htons(n) (unsigned short)((((unsigned short) (n)) << 8) | (((unsigned short) (n)) >> 8))
#define htonl(n) (unsigned int)((((unsigned int) (n)) << 16) | (((unsigned int) (n)) >> 16))
//--------------------------------------------------------------------------------
/*
============== ОПЕРАЦИИ СО СТРОКАМИ И СПИСКАМИ СТРОК ===============
*/
// Преобразование text из текущей локали в UNICODE 
extern QString fr( QString text );

// Преобразуем list из текущей локали в UNICODE
extern void frList( QStringList& list );

extern QString rf(const QString& str);

// Реверсируем список строк
extern void reverseStringList( QStringList& );
// Преобразуем объект класса QBitArray в строку
// порядок следования битов определяется флагом
extern QString bitArrayToString( const QBitArray&, bool = true );
// Преобразование слова в строке в слово  и запись по указателю
// --- указатель адресует uchar (предполагается в массиве)
extern int stringToUShort( uchar*, const QString& );
// --- указатель адресует слово
extern int stringToUShort( ushort*, const QString& );
// Преобразование слова в строке в слово с сетевым порядком байт и запись по указателю
// --- указатель адресует uchar (предполагается в массиве)
extern int stringToUShortNet( uchar*, const QString&, int = 10);
// --- указатель адресует слово
extern int stringToUShortNet( ushort*, const QString&, int = 10);
// Преобразование двойного слова в строке в двойное слово с сетевым порядком байт и запись по указателю
// --- указатель адресует uchar (предполагается в массиве)
extern int stringToULongNet( uchar*, const QString& );
// --- указатель адресует двойное слово
extern int stringToULongNet( ulong*, const QString& );
// --- преобразование строки в беззнаковое целое (без изменения порядка байт)
extern int stringToULong(uchar*, const QString&);
// Преобразование беззнакового байта в строке в uchar (последний параметр - база)
extern int stringToByte( uchar*, const QString&, int = 10 );
// Получение строки слов, разделенных пробелами, из массива buf длинной length
extern QString getUShortString( ushort* buf, int length );
extern QString getUCharString( uchar* buf, int length );
// Получение массива uchar из строки значений, разделенных пробелами (по указанной базе)
extern int getUCharArray( const QString& line, uchar* buf, int base=10 );

extern int find_in_buf(uchar* buf, int length, uchar data, int from = 0);

extern QStringList::const_iterator get_iterator_by_index(int, const QStringList&);

/*
===================== ОПЕРАЦИИ С ЧИСЛАМИ =========================
*/
// Проверка попадания значения value в диапазон [v1:v2]
extern bool isInRange( int value, int v1, int v2 );
// Проверка попадания числа в строке в диапазон
// преобразованное из строки число возвращается по указателю
extern bool isInRange( const QString&, int, int, int* );
// Перестановка байт значения float
extern float reverse_float( float value );
// Операция копирования слов (из src в dst)
extern int wordcpy( ushort* dst, ushort* src, int count );

/*
==================== ОПЕРАЦИИ С ВИДЖЕТАМИ ========================
*/
// Создание и возврат указателя на объект класса QLabel с текстом
// и предустановленным стилем
extern QLabel* get_new_header_label( QString text, QWidget* parent );
// Создание и возврат указателя на объект класса QLabel с текстом
// и предустановленным стилем
extern QLabel* get_new_status_label( QString text, QWidget* parent );

/*
====================== БИТОВЫЕ ОПЕРАЦИИ =========================
*/
// Поиск первого единичного бита в слове и возврат его индекса
// если не найден - возвращаем -1
extern int find_first_1( unsigned short value );
// Установка в байте mask бита с индексом index в 1, остальные в 0
extern int get_byte_mask( unsigned char* mask, unsigned short index );
// Инициализация объекта класса QBitArray размера size
// и содержащего значение val
extern QBitArray getBitArray( ushort size, unsigned long val );
// Подсчет элементов со значением val в объекте QBitArray
extern int getCountInBitArray( int val, const QBitArray& );
// Преобразование объекта класса QBitArray в слово
extern ushort bitArrayToUShort( const QBitArray& );
/*
=================== ОПЕРАЦИИ С ПАРАМЕТРАМИ В ФАЙЛАХ ==============
*/
// Ищем в файле file параметр param и записываем в val его значение
// возвращаем длину строки найденного параметра, или 0 - если не найден
extern int getParametr( QFile& file, QString param, QString& val );
// Специализированный метод для модуля управления изделием М-498-НН
// поиск  секции в файле
extern int getParam( QFile&, QString, QString& );
extern int getSection(QFile&, const QString&, QStringList&, bool = false);

extern int close_descriptor(int);

/*
=================== Массивы символов QByteArray ========================
*/
extern QByteArray create_byte_array(uchar*, int);
extern QByteArray create_byte_array(const QString&, int base = 10);
extern QString byte_array_to_string(const QByteArray&, bool = false, bool = true);
extern int byte_array_to_uchar(const QByteArray&, uchar*);
extern void append_to_array(QByteArray&, ushort, bool = false);
extern ushort array_to_ushort(const QByteArray&, int, bool = false, bool* = 0);
//--------------------------------------------------------------------------------
#endif
