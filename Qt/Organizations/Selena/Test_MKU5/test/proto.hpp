//--------------------------------------------------------------------------------
#ifndef _PROTO_H
#define _PROTO_H
//--------------------------------------------------------------------------------
#include <QtNetwork>
#include <QByteArray>
//--------------------------------------------------------------------------------
#include "r4funcs.hpp"
//--------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------
class Proto : public QObject
{
    Q_OBJECT

public:
    Proto(QTcpSocket*);
    ~Proto();
    enum State { State_None = 0,
                 State_Ok = 1,
                 State_Write_Error = -1,
                 State_Read_Error = -2
               };
    State sendTestCmd(uchar, const QByteArray&);
    State sendTestCmd(uchar, const QByteArray&, QByteArray&, int = 3000);
    State sendTestCmd(uchar, uchar, const QByteArray&, QByteArray&, int = 3000);
    State sendSpecCmd(uchar, uchar, const QByteArray&);
    State sendSpecCmd(uchar, uchar, const QByteArray&, QByteArray&, int = 3000);
    State request(uchar, uchar, uchar, const QByteArray&);
    State request(uchar, uchar, uchar, const QByteArray&, QByteArray&, int);
    static QString errstr(int);
    QString errstr();
    int headerSize();

private:
    State read(int, QByteArray&);
    State wait(int);
    QTcpSocket* socket;
    ushort change_order(ushort);
    int error_code;
    QByteArray marker_array;
};
//--------------------------------------------------------------------------------
#endif
