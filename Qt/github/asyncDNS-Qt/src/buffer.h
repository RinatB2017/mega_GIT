#ifndef BUFFER_H
#define BUFFER_H

#include <QtEndian>
#include <QByteArray>
#include <cstdarg>
#include <QString>
#include <iostream>
#include <QList>
#include <QObject>
class Buffer : public QByteArray
{
public:
    Buffer(const char * buf, unsigned int length);
    Buffer(unsigned int length);
    Buffer(const Buffer &buf);
    Buffer();

    /* toQSring
     * WARNING: Since QString uses utf-8 encoding, this converation is NOT bidirectional!
     */
    QString toQString();

    static Buffer pack(const char * fmt, ...);
    static unsigned int unpack(const char * fmt, const Buffer &data, ...);
    static unsigned int unpack(const char * fmt, char * data, ...);
};

#endif // BUFFER_H
