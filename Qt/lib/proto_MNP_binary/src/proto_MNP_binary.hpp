//--------------------------------------------------------------------------------
#ifndef PROTO_MNP_BINARY_HPP
#define PROTO_MNP_BINARY_HPP
//--------------------------------------------------------------------------------
#include <QObject>
#include <QString>
#include <QWidget>
//--------------------------------------------------------------------------------
enum {
    NO_ERROR    = 0,
    ERROR_FORMAT_MESSAGE,
    ERROR_UNKNOWN_MESSAGE,
    ERROR_UNKNOWN_PORT,
    ERROR_UNKNOWN_BAULDRATE,
    ERROR_TYPE_PROTO,
    ERROR_SYSTEM_COORD,
    ERROR_HOUR,
    ERROR_MIN,
    ERROR_SEC,
    ERROR_CHECKSUM
};
//---
typedef struct {
    short sync;
    short id;
    short size;
    short reserv;
    short checksum;
} DATA_BINARY;
//---
union SHORT
{
    short value;
    struct
    {
        unsigned bit0:1;
        unsigned bit1:1;
        unsigned bit2:1;
        unsigned bit3:1;
        unsigned bit4:1;
        unsigned bit5:1;
        unsigned bit6:1;
        unsigned bit7:1;
        unsigned bit8:1;
        unsigned bit9:1;
        unsigned bitA:1;
        unsigned bitB:1;
        unsigned bitC:1;
        unsigned bitD:1;
        unsigned bitE:1;
        unsigned bitF:1;
    } bits;
};
//--------------------------------------------------------------------------------
class Proto_MNP_binary : public QWidget
{
    Q_OBJECT
public:
    explicit Proto_MNP_binary(QWidget *parent = 0);
    void test(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

private slots:
    void log(const QString &data);
    
private:
    QWidget *parent;
    DATA_BINARY data_binary;
    void init(void);

    void connect_log(void);
    bool check_id(void);
    bool check_checksum(void);
    void calc_checksum(void);
    void view_bytearray(const QByteArray &data);

};
//--------------------------------------------------------------------------------
#endif
