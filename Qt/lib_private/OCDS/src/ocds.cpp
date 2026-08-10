//---------------------------------------------------------------------------
#include <QCoreApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>
#include <QPixmap>
#include <QDebug>
//---------------------------------------------------------------------------
#include <QStringListModel>
#include <QTextStream>
#include <QListView>
#include <QtGlobal>
#include <QObject>
#include <QDialog>
#include <QFile>
#include <QTime>
//---------------------------------------------------------------------------
#include <math.h>
//---------------------------------------------------------------------------
#include "parport.hpp"
#include "ocds.hpp"
//---------------------------------------------------------------------------
struct mem
{
    char active;
    short data;
};
//#define MAX_SIZE 262144  // не правильно
//#define MAX_SIZE 0x50000 // правильно
#define MAX_SIZE 2000000
#define MOD %
QVector <mem> memory(MAX_SIZE);
//---------------------------------------------------------------------------
QTextStream logger(stdout);
//---------------------------------------------------------------------------
OCDS::OCDS(QObject* parent) :
    QObject(parent),
    parent((QWidget *)parent),
    max_write(0),
    magic_counter(256)
{
    connect_log();

    parport = new Parport(this);
    parport->open_parport();

    outData = 0xff;
    parport->setData(outData);
}
//--------------------------------------------------------------------------------
void OCDS::connect_log(void)
{
    if(parent)
    {
        connect(this, SIGNAL(info(QString)),  parent, SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parent, SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parent, SIGNAL(error(QString)));
    }
    else
    {
        connect(this, SIGNAL(info(QString)),  this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)), this, SLOT(log(QString)));
    }
}
//--------------------------------------------------------------------------------
void OCDS::log(const QString &data)
{
    qDebug() << data;
}
//---------------------------------------------------------------------------
void OCDS::ReadHEX (QString filename)
{
    int count = 0;

    emit progress(0);

#ifdef EMIT_LOG_ENABLED
    emit info(QString(tr("Читаем %1 файл")).arg(filename));
    QCoreApplication::processEvents();
#else
    logger << QString(tr("Читаем %1 файл\n")).arg(filename);
    QCoreApplication::processEvents();
#endif

    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
#ifdef EMIT_LOG_ENABLED
        emit error(QString("file %1 not open").arg(filename));
        QCoreApplication::processEvents();
#else
        logger << QString("file %1 not open\n").arg(filename);
        QCoreApplication::processEvents();
#endif
        return;
    }
    QTextStream stream(&file);
    int OffsetAddr=0; //адрес смещения
    //читаем весь файл до конца

    //    Memory.Clear();
    for(int i=0; i<MAX_SIZE; i++) memory[i].active = 0;
    max_write = 0;

    QStringList fields = stream.readAll().split('\n');
    for(int i=0; i<fields.count(); i++)
    {
        int strLength = fields.at(i).length();
        if(strLength != 0)
        {
            int NumberOfByte = HexToInt(fields.at(i).mid(1, 2)); //количество байт в строке
            int TypeOfStr = HexToInt(fields.at(i).mid(7, 2));
            QString Data = fields.at(i).mid(9, 2*NumberOfByte); //строка с данными
            if (TypeOfStr == 2)  //если тип строки "2" (смещение по сектору)
            {
                OffsetAddr = HexToInt(Data)*16; //???
                //                OffsetAddr = HexToInt(Data) % 65536;
            }
            if (TypeOfStr == 4)  //если тип строки "4" (смещение по адресу)
            {
                OffsetAddr = HexToInt(Data) << 16;
                //OffsetAddr = HexToInt(Data) * 65536;
                //                OffsetAddr = HexToInt(Data) % 65536;
            }
            int Addr = HexToInt(fields.at(i).mid(3, 4)); //определение адреса
            if (TypeOfStr == 0)  //если тип строки "0" (абсолютный адрес)
            {
                for (int k=0; k<NumberOfByte; k++)
                {
                    //                    Memory.Add(Addr+k+OffsetAddr,HexToInt(Data.mid(2*k, 2)));
                    int mega = Addr+k+OffsetAddr;
                    //                    if(((mega > 0x7FFF) && (mega < 0x18000)) || (mega > 0x04FFFF))
                    //                    {
                    //                        emit debug(QString("num str %1 mega %2").arg(i).arg(mega));
                    //                        emit debug(QString("  str %1").arg(Data));
                    //                    }
                    if(mega >= MAX_SIZE)
                    {
#ifdef EMIT_LOG_ENABLED
                        emit error(fields.at(i));
                        emit error(QString(tr("Слишком большой адрес %1")).arg(Addr+k+OffsetAddr));
                        emit error(QString(tr("Addr=%1 k=%2 OffsetAddr=%3")).arg(Addr).arg(k).arg(OffsetAddr));
                        QCoreApplication::processEvents();
#else
                        logger << QString(tr("Слишком большой адрес %1\n")).arg(Addr+k+OffsetAddr);
                        logger << QString(tr("Addr=%1 k=%2 OffsetAddr=%3\n")).arg(Addr).arg(k).arg(OffsetAddr);
                        QCoreApplication::processEvents();
#endif
                        max_write = 0;
                        file.close();
                        return;
                    }
#if 1
                    if((mega > 0x7FFF) && (mega < 0x18000))
                    {
#ifdef EMIT_LOG_ENABLED
                        emit error(QString("Address %1 is false").arg(IntToHex(mega,16)));
                        QCoreApplication::processEvents();
#else
                        logger << QString("Address %1 is false").arg(IntToHex(mega,16));
                        QCoreApplication::processEvents();
#endif
                        max_write = 0;
                        file.close();
                        return;
                    }
#endif
                    //                    memory[Addr+k+OffsetAddr].data = HexToInt(Data.mid(2*k, 2));
                    //                    memory[Addr+k+OffsetAddr].active = 1;
                    memory[mega].data = HexToInt(Data.mid(2*k, 2));
                    memory[mega].active = 1;
                    max_write++;
                    count++;
                }
            }
        }
    }
    file.close();

#ifdef EMIT_LOG_ENABLED
    //    emit info(QString("count = %1").arg(count));
    //    emit info(QString("fields.count()  = %1").arg(fields.count()));
    emit info(QString("size = %1").arg(max_write));
    //    if(Memory.SizeOf() > 262144)
    //    {
    //        emit error("Warning: size > 262144");
    //    }
    emit info(QString(tr("Данные прочитаны")));
    QCoreApplication::processEvents();
#else
    logger << QString("count = %1\n").arg(max_write);
    //    logger << QString("fields.count()  = %1\n").arg(fields.count());
    //    logger << QString("Memory.SizeOf() = %1\n").arg(Memory.SizeOf());
    logger << QString(tr("Данные прочитаны\n"));
    QCoreApplication::processEvents();
#endif
    //    Memory.Sort();
}
//---------------------------------------------------------------------------
void OCDS::ReadH86 (QString filename)
{
    int count = 0;
    unsigned char  RECLEN = 0;
    unsigned short LOAD_OFFSET = 0;
    unsigned char  RECTYP = 0;

    unsigned short ULBA = 0;
    unsigned short USBA = 0;
    //    unsigned int   EIP = 0;
    //    unsigned int   CS_IP = 0;

    int mega;
    int OffsetAddr;
    unsigned long module = 0xFFFFFFFF;

    //    unsigned char  CHKSUM = 0;

#ifdef EMIT_LOG_ENABLED
    emit info(QString(tr("Читаем %1 файл")).arg(filename));
    QCoreApplication::processEvents();
#else
    logger << QString(tr("Читаем %1 файл\n")).arg(filename);
    QCoreApplication::processEvents();
#endif

    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
#ifdef EMIT_LOG_ENABLED
        emit error(QString("file %1 not open").arg(filename));
        QCoreApplication::processEvents();
#else
        logger << QString("file %1 not open\n").arg(filename);
        QCoreApplication::processEvents();
#endif
        return;
    }
    QTextStream stream(&file);

    for(int i =0; i<MAX_SIZE; i++) memory[i].active = 0;
    max_write = 0;
    OffsetAddr = 0;

    QStringList fields = stream.readAll().split('\n');
    for(int i=0; i<fields.count(); i++)
    {
        int strLength = fields.at(i).length();
        if(strLength != 0)
        {
            RECLEN = HexToInt(fields.at(i).mid(1, 2));      //количество байт в строке
            RECTYP = HexToInt(fields.at(i).mid(7, 2));
            switch(RECTYP)
            {
            case 0:
                //emit debug("00 Data Record");
                for (int index=0; index<RECLEN; index++)
                {
                    LOAD_OFFSET = HexToInt(fields.at(i).mid(2, 4));
                    if(module > 0xFFFF) mega = LOAD_OFFSET+index+OffsetAddr;
                    else mega = OffsetAddr+((LOAD_OFFSET+index) % module);
                    if(mega >= MAX_SIZE)
                    {
#ifdef EMIT_LOG_ENABLED
                        emit error(QString("Address 0x%1 too large").arg(mega, 0, 16));
                        QCoreApplication::processEvents();
#else
                        logger << QString("Address 0x%1 too large").arg(mega, 0, 16);
                        QCoreApplication::processEvents();
#endif
                        max_write = 0;
                        file.close();
                        return;
                    }
#if 1
                    if((mega > 0x7FFF) && (mega < 0x18000))
                    {
#ifdef EMIT_LOG_ENABLED
                        emit error(QString("module = 0x%1").arg(module, 0, 16));
                        emit error(fields.at(i));
                        emit error(QString("Address 0x%1 is false").arg(mega, 0, 16));
                        QCoreApplication::processEvents();
#else
                        logger << QString("module = 0x%1").arg(module, 0, 16);
                        logger << fields.at(i);
                        logger << QString("Address 0x%1 is false").arg(mega, 0, 16);
                        QCoreApplication::processEvents();
#endif
                        max_write = 0;
                        file.close();
                        return;
                    }
#endif
                    QString Data = fields.at(i).mid(9, 2*RECLEN);   //строка с данными
                    memory[mega].data = HexToInt(Data.mid(2*index, 2));
                    memory[mega].active = 1;
                    max_write++;
                    count ++;
                }
                break;
            case 1:
                //emit info("01 End of File Record");
                break;
            case 2:
#ifdef EMIT_LOG_ENABLED
                emit info("02 Extended Segment Address Record");
                QCoreApplication::processEvents();
#else
                logger << "02 Extended Segment Address Record";
                QCoreApplication::processEvents();
#endif
                LOAD_OFFSET = HexToInt(fields.at(i).mid(2, 4));
                USBA = HexToInt(fields.at(i).mid(3, 4));
                OffsetAddr = USBA;
                module = 0xFFFF;
                break;
            case 3:
#ifdef EMIT_LOG_ENABLED
                emit info(fields.at(i));
                emit info("03 Start Segment Address Record");
                QCoreApplication::processEvents();
#else
                logger << fields.at(i);
                logger << "03 Start Segment Address Record";
                QCoreApplication::processEvents();
#endif
                LOAD_OFFSET = HexToInt(fields.at(i).mid(2, 4));
                //                CS_IP = HexToInt(fields.at(i).mid(3, 8));
                break;
            case 4:
#ifdef EMIT_LOG_ENABLED
                emit info("04 Extended Linear Address Record");
                QCoreApplication::processEvents();
#else
                logger << "04 Extended Linear Address Record";
                QCoreApplication::processEvents();
#endif
                LOAD_OFFSET = HexToInt(fields.at(i).mid(2, 4));
                ULBA = HexToInt(fields.at(i).mid(3, 4));
                OffsetAddr = ULBA;
                module = 0xFFFFFFFF;
                break;
            case 5:
#ifdef EMIT_LOG_ENABLED
                emit info("05 Start Linear Address Record");
                QCoreApplication::processEvents();
#else
                logger << "05 Start Linear Address Record";
                QCoreApplication::processEvents();
#endif
                LOAD_OFFSET = HexToInt(fields.at(i).mid(2, 4));
                //                EIP = HexToInt(fields.at(i).mid(5, 8));
                break;
            default:
                count = 0;
#ifdef EMIT_LOG_ENABLED
                emit error(tr("RECTYP is unknown"));
                QCoreApplication::processEvents();
#else
                logger << tr("RECTYP is unknown");
                QCoreApplication::processEvents();
#endif
                return;
            }
        }
    }
#ifdef EMIT_LOG_ENABLED
    emit info(QString("count=%1").arg(count));
    emit info(QString(tr("Данные прочитаны")));
    QCoreApplication::processEvents();
#else
    logger << QString("count=%1").arg(count);
    logger << QString(tr("Данные прочитаны"));
    QCoreApplication::processEvents();
#endif
    QCoreApplication::processEvents();
    file.close();
}
//---------------------------------------------------------------------------
void OCDS::hardReset(int n)
{
#if 0
    for (int i=0; i<n; i++)
    {
        //передача бита
        outData = outData & 0xDF; //RST = 0  1111 1111 & 1101 1111 = 1101 1111
        parport->setData(outData);
    }
    //передача бита
    outData = outData | 0x20;     //RST = 1   1101 1111 OR 0010 0000 = 1111 1111 (6 pin - reset)
#else
    for (int i=0; i<n; i++)
    {
        //передача бита
        outData = outData & 0xEF;   // не работает Rst на LPT
        parport->setData(outData);
    }
    //передача бита
    outData = outData | 0x10;       // не работает Rst на LPT
#endif
    parport->setData(outData);
}
//---------------------------------------------------------------------------
void OCDS::trst0(int n)
{
    for (int i=0; i<n; i++)
    {
        //передача бита
        outData = outData & 0xBD; //TRST = 0 TCK = 0   1111 1111 & 1011 1101 = 1011 1101 (7 pin - trst dwn, 2 pin - clk dwn)
        parport->setData(outData);
        outData = outData | 0x02; //TCK = 1   1011 1101 OR 0000 0010 = 1011 1111 (7 pin - trst dwn, 2 pin - clk up)
        parport->setData(outData);
    }
    //передача бита
    outData = outData & 0xF9;     //TCK = 0 TMS = 0   1011 1111 & 1111 1001 = 1011 1001 (7 pin - trst dwn, 3 pin - TMS dwn, 2 pin - clk dwn)
    outData = outData | 0x40;     //TRST = 1
    parport->setData(outData);
    outData = outData | 0x02;     //TCK = 1 -> idle mode   1011 1001 OR 0000 0010 = 1011 1011 (7 pin - trst dwn, 3 pin - TMS dwn, 2 pin - clk up)
    parport->setData(outData);
}
//---------------------------------------------------------------------------
void OCDS::assignTms(int tms)
{
    if (tms==0)
    {
        outData = outData & 0xfb; // 1111 1111 & 1111 1011 = 1111 1011 (3 pin - TMS dwn)
    }
    else
    {
        outData = outData | 0x04; // 1111 1011 OR 0000 0100 = 1111 1111 (3 pin - TMS up)
    }
    //передача бита
    outData = outData & 0xFD;     //TCK = 0  1111 1x11 & 1111 1101 = 1111 1x01 (2 pin - clk dwn)
    parport->setData(outData);
    outData = outData | 0x02;       //TCK = 1   1111 1x11 OR 0000 0010 = 1111 1x11 (2 pin - clk up)
    parport->setData(outData);
}
//---------------------------------------------------------------------------
void OCDS::assignTdi(int tdi, int tms)
{
    if (tdi==0)
    {
        outData = outData & 0xfe; // 1111 1111 & 1111 1110 = 1111 1110 (1 pin - TDI dwn)
    }
    else
    {
        outData = outData | 0x01; // 1111 1110 OR 0000 0001 = 1111 1111 (1 pin - TDI up)
    }
    if (tms==0)
    {
        outData = outData & 0xfb; // 1111 1111 & 1111 1011 = 1111 1011 (3 pin - TMS dwn)
    }
    else
    {
        outData = outData | 0x04; // 1111 1011 OR 0000 0100 = 1111 1111 (3 pin - TMS up)
    }
    //передача бита
    outData = outData & 0xFD;     //TCK = 0 // 1111 1x1x & 1111 1101 = 1111 1x0x (2 pin - clk dwn)
    parport->setData(outData);
    outData = outData | 0x02;     //TCK = 1  // 1111 1x0x OR 0000 0010 = 1111 1x1x (2 pin - clk up)
    parport->setData(outData);
}
//---------------------------------------------------------------------------
void OCDS::idle(int n)
{
    for (int i=0; i<n; i++)
    {
        //передача бита
        outData = outData & 0xF9; //TCK = 0 TMS = 0 //1111 1111 & 1111 1001 = 1111 1001 (3 pin - TMS dwn, 2 pin - clk dwn)
        parport->setData(outData);
        outData = outData | 0x02;   //TCK = 1 //1111 1001 OR 0000 0010 = 1111 1011 (3 pin - TMS dwn, 2 pin - clk up)
        parport->setData(outData);
    }
}
//---------------------------------------------------------------------------
void OCDS::shiftIr(int instr)
{
    assignTms(1);
    assignTms(1);
    assignTms(0);
    assignTms(0);
    for (int i=0; i<7; i++)
    {
        assignTdi((instr>>i)%2, 0); //1110 1101 %2 0111 0110 (ostatok 1) -> vivod
    }
    assignTdi((instr>>7)%2, 1);     //1110 1101
    assignTms(1);
    assignTms(0);
}
//---------------------------------------------------------------------------
void OCDS::shiftDr(int data, int bits)
{
    assignTms(1);
    assignTms(0);
    assignTms(0);
    for (int i=0; i<bits-1; i++)
    {
        assignTdi((data>>i)%2, 0);
    }
    assignTdi((data>>(bits-1))%2, 1);
    assignTms(1);
    assignTms(0);
}
//---------------------------------------------------------------------------
void OCDS::cerberusInit(int haltBit)
{
    trst0(5);
    shiftIr(0x10);
    if (haltBit == 0)
    {
        shiftDr(0x0, 16);
    }
    else
    {
        shiftDr(0x3, 16);    // halt after reset
    }

    shiftIr(0xc0);
    shiftDr(0x2, 3);
    shiftIr(0xc1);
}
//---------------------------------------------------------------------------
int OCDS::tdoCheck(void)
{
    unsigned int cx, ax;

    tdoBit=0;
    //передача бита
    ax = outData & 0xFD; //TCK = 0
    parport->setData(ax);
    cx = ax;

    ax = parport->get_status();
    ax = ax & 0x0010;
    tdoBit = ax;

    ax = cx;
    ax = ax | 0x02;        //TCK = 1
    parport->setData(ax);
    outData = ax;
    return(tdoBit);
}
//---------------------------------------------------------------------------
void OCDS::ioConfig(int data)
{
    shiftDr(data*16, 13);  // dddddddd0000
}
//---------------------------------------------------------------------------
void OCDS::ioSetAddress(int data)
{
    int instr = 1;
    assignTms(1);
    assignTms(0);
    assignTms(0);
    for (int i=0; i<4; i++)
    {
        assignTdi((instr>>i)%2, 0);
    }
    assignTdi(data%2, 0);
    int j=0;
    while (tdoCheck())
    {
        j++;
        if(j == magic_counter)
        {
#ifdef EMIT_LOG_ENABLED
            emit error(tr("##### ERROR ##### ioSetAddress: Превышено время ожидания стартового бита при установке адреса"));
            QCoreApplication::processEvents();
#else
            logger << tr("##### ERROR ##### ioSetAddress: Превышено время ожидания стартового бита при установке адреса\n");
            QCoreApplication::processEvents();
#endif
            break;
        }
    }
    if (tdoBit==0)
    {
        for (int i=1; i<24; i++) assignTdi((data>>i)%2, 0);
    }
    assignTdi(1, 1);
    assignTms(1);
    assignTms(0);
}
//---------------------------------------------------------------------------
void OCDS::ioWriteBlock(int data)
{
    int instr = 2;
    assignTms(1);
    assignTms(0);
    assignTms(0);
    for (int i=0; i<4; i++)
    {
        assignTdi((instr>>i)%2, 0);
    }
    assignTdi(data%2, 0);
    int j=0;
    while (tdoCheck())
    {
        j++;
        if(j == magic_counter)
        {
#ifdef EMIT_LOG_ENABLED
            emit error(tr("##### ERROR ##### ioWriteBlock: Превышено время ожидания стартового бита при записи блока"));
            QCoreApplication::processEvents();
#else
            logger << tr("##### ERROR ##### ioWriteBlock: Превышено время ожидания стартового бита при записи блока\n");
            QCoreApplication::processEvents();
#endif
            break;
        }
    }
    for (int i=1; i<16; i++)
    {
        assignTdi((data>>i)%2, 0);
    }
    assignTdi(1, 1);
    assignTms(1);
    assignTms(0);
}
//---------------------------------------------------------------------------
int OCDS::ioReadBlock(void)
{
    int instr = 3;
    assignTms(1);
    assignTms(0);
    assignTms(0);
    for (int i=0; i<5; i++)
    {
        assignTdi((instr>>i)%2, 0);
    }
    int j=0;
    while (tdoCheck())
    {
        j++;
        if(j == magic_counter)
        {
#ifdef EMIT_LOG_ENABLED
            emit error("##### ERROR #####");
            emit error(tr("ioReadBlock: Превышено время ожидания стартового бита при чтении"));
            QCoreApplication::processEvents();
#else
            logger << "##### ERROR #####\n";
            logger << tr("ioReadBlock: Превышено время ожидания стартового бита при чтении\n");
            QCoreApplication::processEvents();
#endif
            break;
        }
    }
    ReadData=-1;
    if (tdoBit==0)
    {
        ReadData=0;
        for (int i=0; i<16; i++)
        {
            if(tdoCheck() != 0)
            {
                ReadData = ReadData + pow(2.0, i);
            }
        }
    }
    assignTdi(1, 1);
    assignTms(1);
    assignTms(0);
    return (ReadData);
}
//---------------------------------------------------------------------------
void OCDS::ioWriteWord(int data)
{
    int instr = 4;
    assignTms(1);
    assignTms(0);
    assignTms(0);
    for (int i=0; i<4; i++)
    {
        assignTdi((instr>>i)%2, 0);
    }
    assignTdi(data%2, 0);
    int j=0;
    while (tdoCheck())
    {
        j++;
        if(j == magic_counter)
        {
#ifdef EMIT_LOG_ENABLED
            emit error(tr("##### ERROR ##### ioWriteWord: Превышено время ожидания стартового бита при записи"));
            QCoreApplication::processEvents();
#else
            logger << tr("##### ERROR ##### ioWriteWord: Превышено время ожидания стартового бита при записи\n");
            QCoreApplication::processEvents();
#endif
            break;
        }
    }
    if (tdoBit==0)
    {
        for (int i=1; i<16; i++)
        {
            assignTdi((data>>i)%2, 0);
        }
    }
    assignTdi(1, 1);
    assignTms(1);
    assignTms(0);
}
//---------------------------------------------------------------------------
int OCDS::ioReadWord(void)
{
    int instr = 5;
    assignTms(1);
    assignTms(0);
    assignTms(0);
    for (int i=0; i<5; i++)
    {
        assignTdi((instr>>i)%2, 0);
    }
    int j=0;
    while (tdoCheck())
    {
        j++;
        if(j == magic_counter)
        {
#ifdef EMIT_LOG_ENABLED
            emit error(tr("##### ERROR ##### ioReadWord: Превышено время ожидания стартового бита при чтении"));
            QCoreApplication::processEvents();
#else
            logger << tr("##### ERROR ##### ioReadWord: Превышено время ожидания стартового бита при чтении\n");
            QCoreApplication::processEvents();
#endif
            break;
        }
    }
    ReadData=-1;
    if (tdoBit==0)
    {
        ReadData=0;
        for (int i=0; i<16; i++)
        {
            if(tdoCheck() != 0)
            {
                ReadData = ReadData + pow(2.0, i);
            }
        }
    }
    assignTdi(1, 1);
    assignTms(1);
    assignTms(0);
    return (ReadData);
}
//---------------------------------------------------------------------------
void OCDS::ioWriteByte(int data)
{
    int instr = 8;
    assignTms(1);
    assignTms(0);
    assignTms(0);
    for (int i=0; i<4; i++)
    {
        assignTdi((instr>>i)%2, 0);
    }
    assignTdi(data%2, 0);
    int j=0;
    while (tdoCheck())
    {
        j++;
        if(j == magic_counter)
        {
#ifdef EMIT_LOG_ENABLED
            emit error(tr("##### ERROR ##### ioWriteByte: Превышено время ожидания стартового бита при записи байта"));
            QCoreApplication::processEvents();
#else
            logger << tr("##### ERROR ##### ioWriteByte: Превышено время ожидания стартового бита при записи байта\n");
            QCoreApplication::processEvents();
#endif
            break;
        }
    }
    for (int i=1; i<16; i++)
    {
        assignTdi((data>>i)%2, 0);
    }
    assignTdi(1, 1);
    assignTms(1);
    assignTms(0);
}
//---------------------------------------------------------------------------
void OCDS::ioSupervisor(void)
{
    int data = 0xb;
    assignTms(1);
    assignTms(0);
    assignTms(0);
    for (int i=0; i<5; i++)
    {
        assignTdi((data>>i)%2, 0);
    }
    int j=0;
    while (tdoCheck())
    {
        j++;
        if(j == magic_counter)
        {
#ifdef EMIT_LOG_ENABLED
            emit error(tr("##### ERROR ##### ioSupervisor: Превышено время ожидания стартового бита при команде IO_SUPERVISOR"));
            QCoreApplication::processEvents();
#else
            logger << tr("##### ERROR ##### ioSupervisor: Превышено время ожидания стартового бита при команде IO_SUPERVISOR\n");
            QCoreApplication::processEvents();
#endif
            break;
        }
    }
    for (int i=0; i<15; i++)
    {
        assignTdi(1,0);
    }
    assignTdi(1,1);
    assignTms(1);
    assignTms(0);
}
//---------------------------------------------------------------------------
void OCDS::wordProgram(int addr, int data)
{
    ioSetAddress(0x080555);
    ioWriteWord(0xaa00);

    ioSetAddress(0x080aaa);
    ioWriteWord(0x0055);

    ioSetAddress(0x080555);
    ioWriteWord(0xa000);

    ioSetAddress(addr);
    ioWriteWord(data);
#ifdef EMIT_LOG_ENABLED
    //    emit debug(QString(tr("wordProgram: addr 0x%1 data 0x%2")).arg(addr, 0, 16).arg(data, 0, 16));
    //    QCoreApplication::processEvents();
#else
    //    logger << QString(tr("wordProgram: addr 0x%1 data 0x%2\n")).arg(addr, 0, 16).arg(data, 0, 16);
    //    QCoreApplication::processEvents();
#endif
}
//---------------------------------------------------------------------------
void OCDS::sectorErase(int addr)
{
    ioSetAddress(0x080555);
    ioWriteWord(0xaa00);

    ioSetAddress(0x080aaa);
    ioWriteWord(0x0055);

    ioSetAddress(0x080555);
    ioWriteWord(0x8000);

    ioSetAddress(0x080555);
    ioWriteWord(0xaa00);

    ioSetAddress(0x080aaa);
    ioWriteWord(0x0055);

    ioSetAddress(addr);
    ioWriteWord(0x0030);
}
//---------------------------------------------------------------------------
void OCDS::blockErase(int addr)
{
    ioSetAddress(0x080555);
    ioWriteWord(0xaa00);

    ioSetAddress(0x080aaa);
    ioWriteWord(0x0055);

    ioSetAddress(0x080555);
    ioWriteWord(0x8000);

    ioSetAddress(0x080555);
    ioWriteWord(0xaa00);

    ioSetAddress(0x080aaa);
    ioWriteWord(0x0055);

    ioSetAddress(addr);
    ioWriteWord(0x0050);
}
//---------------------------------------------------------------------------
void OCDS::allErase(void)
{
    ioSetAddress(0x080555);
    ioWriteWord(0xaa00);

    ioSetAddress(0x080aaa);
    ioWriteWord(0x0055);

    ioSetAddress(0x080555);
    ioWriteWord(0x8000);

    ioSetAddress(0x080555);
    ioWriteWord(0xaa00);

    ioSetAddress(0x080aaa);
    ioWriteWord(0x0055);

    ioSetAddress(0x080555);
    ioWriteWord(0x1000);
}
//------------------------------------------------------------------------
void OCDS::EraseAllFlash(void)
{
    int temp;
    hardReset(1000);
    cerberusInit(1);
    hardReset(1000);
    ioSupervisor();
    ioConfig(1);
    allErase();
    idle(20000);
    ioSetAddress(0);
    temp = ioReadWord();
    if (temp != 0xFFFF)
    {
#ifdef EMIT_LOG_ENABLED
        emit error(tr("##### ERROR ##### EraseAllFlash: Ошибка при стирании флэш-памяти"));
        QCoreApplication::processEvents();
#else
        logger << tr("##### ERROR ##### EraseAllFlash: Ошибка при стирании флэш-памяти\n");
        QCoreApplication::processEvents();
#endif
    }
    else
    {
#ifdef EMIT_LOG_ENABLED
        emit info(tr("Флэш-память стерта"));
        QCoreApplication::processEvents();
#else
        logger << tr("Флэш-память стерта\n");
        QCoreApplication::processEvents();
#endif
    }

#ifndef EMIT_LOG_ENABLED
    logger.flush();
#endif
}
//---------------------------------------------------------------------------
void OCDS::EraseSectorFlash(int addr)
{
    hardReset(1000);
    cerberusInit(1);
    hardReset(1000);
    ioSupervisor();
    ioConfig(1);
    sectorErase(addr);
#ifdef EMIT_LOG_ENABLED
    emit info(tr("Сектор стерт"));
    QCoreApplication::processEvents();
#else
    logger << tr("Сектор стерт\n");
    QCoreApplication::processEvents();
#endif
}
//---------------------------------------------------------------------------
void OCDS::EraseBlockFlash(int addr)
{
    hardReset(1000);
    cerberusInit(1);
    hardReset(1000);
    ioSupervisor();
    ioConfig(1);
    blockErase(addr);
#ifdef EMIT_LOG_ENABLED
    emit info(tr("Блок стерт"));
    QCoreApplication::processEvents();
#else
    logger << tr("Блок стерт\n");
    QCoreApplication::processEvents();
#endif
}
//---------------------------------------------------------------------------
int OCDS::WriteDataFlash(int addr, int data, bool silense)
{
    int temp;
    if (addr%2==1)
    {
        addr++;
    }

    //    QTime t;
    //    t.start();

    hardReset(1000);
    cerberusInit(1);
    hardReset(1000);
    ioSupervisor();
    ioConfig(1);
    wordProgram(addr,data);
    temp = ioReadWord();
    if (temp!= data)
    {
#ifdef EMIT_LOG_ENABLED
        emit error(tr("##### ERROR ##### WriteDataFlash: Ошибка при записи данных"));
        emit error(QString("data = 0x%1 ioReadWord() = 0x%2").arg(data, 0, 16).arg(temp, 0, 16));
        QCoreApplication::processEvents();
#else
        logger << tr("##### ERROR ##### WriteDataFlash: Ошибка при записи данных\n");
        logger << QString("data = 0x%1 ioReadWord() = 0x%2\n").arg(data, 0, 16).arg(temp, 0, 16);
        QCoreApplication::processEvents();
#endif
        return -1;
    }
    else
    {
#ifdef EMIT_LOG_ENABLED
        //        emit info(QString("Time elapsed: %1 ms").arg(t.elapsed()));
        if(!silense)
        {
            emit info(tr("Данные ") + IntToHex(data, 16)+ tr(" по адресу ") + IntToHex(addr, 16) + tr(" записаны"));
            QCoreApplication::processEvents();
        }
#else
        if(!silense)
        {
            logger << tr("Данные ") + IntToHex(data, 16)+ tr(" по адресу ") + IntToHex(addr, 16) + tr(" записаны\n");
            QCoreApplication::processEvents();
        }
#endif
    }
    return 0;
}
//---------------------------------------------------------------------------
void OCDS::CheckAddress(int addr)
{
    //    int m=Memory.FindAddrPos(addr);
    if (memory[addr].active)
    {
#ifdef EMIT_LOG_ENABLED
        emit error(tr("##### ERROR ##### Заданный адрес не существует"));
        QCoreApplication::processEvents();
#else
        logger << tr("##### ERROR ##### Заданный адрес не существует\n");
        QCoreApplication::processEvents();
#endif
    }
}
//---------------------------------------------------------------------------
//универсальный переводчик из различного типа данных в строку типа "10110101"
int OCDS::HBDconvert (QString conv_value)
{
    char* str = new char[conv_value.length()+1];
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    strcpy(str, conv_value.toAscii().data());
#else
    strcpy(str, conv_value.toLocal8Bit().data());
#endif
    int temp=0;
    if(str[conv_value.length()-1] == 'b') //из бинарного вида
    {
        //обрезка последнего знака "b"
        conv_value = conv_value.left(conv_value.length()-1);
        for (int k=0; k<conv_value.length(); k++)
        {
            if (conv_value[conv_value.length()-k] == '1')
            {
                temp = temp + pow(2.0, k);
            }
        }
    }
    if(str[conv_value.length()-1] == 'h') //из hex-вида
    {
        conv_value = conv_value.left(conv_value.length()-1);    //обрезка последнего знака "h"
        conv_value = QString("0x")+conv_value;                  //перевод десятичного числа в десятичное число
        temp = conv_value.toInt();
    }
    if((str[conv_value.length()-1] >= '0') & ((str[conv_value.length()-1] <= '9')))
    {
        temp = conv_value.toInt();   //перевод десятичного числа в строку с набором 10101
    }
    return temp;
}
//---------------------------------------------------------------------------
int OCDS::HexToInt (QString HEX)
{
    bool ok;
    QString hexstr = HEX;
    hexstr = QString("0x")+hexstr;
    int OutNum = hexstr.toInt(&ok, 16);
    if(ok) return OutNum;
    else return -1;
}
//---------------------------------------------------------------------------
QString OCDS::IntToHex(int a, int b)
{
    return QString("%1").arg(a,0,b);
}
//---------------------------------------------------------------------------
void OCDS::SaveMemoryToFlash()
{
    int dataout=0;
    int count_active = 0;
    float delta = 0;
    float current_progress = 0.0f;

    if(!max_write)
    {
#ifdef EMIT_LOG_ENABLED
        emit error(tr("SaveMemoryToFlash: Нет данных"));
        QCoreApplication::processEvents();
#else
        logger << tr("SaveMemoryToFlash: Нет данных\n");
        QCoreApplication::processEvents();
#endif
        return;
    }

    QTime t;
    t.start();

#ifdef EMIT_LOG_ENABLED
    emit info(tr("Записываем данные"));
    QCoreApplication::processEvents();
#else
    logger << tr("Записываем данные\n");
    QCoreApplication::processEvents();
#endif
    hardReset(1000);
    cerberusInit(1);
    hardReset(1000);
    ioSupervisor();
    ioConfig(1);

    for (int cc=0; cc<MAX_SIZE; cc+=2)
    {
        if(memory[cc].active)
        {
            count_active++;
        }
    }
    delta = (float)100 / (float)count_active;

    for (int cc=0; cc<MAX_SIZE; cc+=2)
    {
        if(memory[cc].active)
        {
            emit progress((int)current_progress);
            current_progress += delta;

            QCoreApplication::processEvents();
            if(memory[cc+1].active)
            {
                dataout=memory[cc].data+256*memory[cc+1].data;
            }
            else
            {
                dataout=memory[cc].data+0xFF00;
            }
            wordProgram(cc, dataout);
            int attempt=0;
            do
            {
                ioSetAddress(cc);
                ioReadWord();
                attempt++;
            } while ((ReadData != dataout) & (attempt<3));
            if ((ReadData != dataout) & (attempt==3))
            {
#ifdef EMIT_LOG_ENABLED
                emit error(QString(tr("##### ERROR ##### Ошибка записи по адресу: 0x%1")).arg(IntToHex(cc,16)));
                //                emit error(QString("ReadData = 0x%1").arg(ReadData, 0, 16));
                //                emit error(QString("dataout  = 0x%1").arg(dataout,  0, 16));
                QCoreApplication::processEvents();
#else
                logger << QString(tr("##### ERROR ##### Ошибка записи по адресу: 0x%1\n")).arg(IntToHex(cc,16));
                logger << QString("ReadData = 0x%1\n").arg(ReadData, 0, 16);
                logger << QString("dataout  = 0x%1\n").arg(dataout,  0, 16);
                QCoreApplication::processEvents();
#endif
                return;
            }
        }
    }
    emit progress(100);

#ifdef EMIT_LOG_ENABLED
    emit info(QString("Time elapsed: %1 ms").arg(t.elapsed()));
    emit info(QString(tr("записано %1 слов")).arg(max_write));
    emit info(tr("Данные записаны"));
    QCoreApplication::processEvents();
#else
    logger << QString("Time elapsed: %1 ms\n").arg(t.elapsed());
    logger << QString(tr("записано %1 слов\n")).arg(max_write);
    logger << tr("Данные записаны\n");
    QCoreApplication::processEvents();
#endif
    //    ResetOff();

    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Запись данных"));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(tr("Данные записаны\n"));
    msgBox.exec();

#ifndef EMIT_LOG_ENABLED
    logger.flush();
#endif
}
//---------------------------------------------------------------------------
void OCDS::printMemory()
{
    if(!max_write)
    {
#ifdef EMIT_LOG_ENABLED
        emit error(tr("##### ERROR ##### printMemory: Нет данных"));
        QCoreApplication::processEvents();
#else
        logger << tr("##### ERROR ##### printMemory: Нет данных\n");
        QCoreApplication::processEvents();
#endif
        return;
    }

    QStringListModel *model = new QStringListModel;
    QStringList list;
    for(int i=0; i<MAX_SIZE; i++)
    {
        if(memory[i].active)
        {
            list << QString("Address 0x%1\tData 0x%2")
                    .arg(i, 0, 16)
                    .arg(memory[i].data, 0, 16);
        }
    }
    model->setStringList(list);
    QListView *lv = new QListView;
    lv->setWindowTitle(tr("Данные Memory"));
    lv->setMinimumSize(640, 480);
    lv->setModel(model);
    lv->show();
}
//---------------------------------------------------------------------------
void OCDS::checkOutData()
{
#ifdef EMIT_LOG_ENABLED
    emit debug("-----");
    emit debug((outData & TRST) ? "TRST 1" : "TRST 0");
    emit debug((outData & RST)  ? "RST  1" : "RST  0");
    emit debug((outData & TMS)  ? "TMS  1" : "TMS  0");
    emit debug((outData & TCK)  ? "TCK  1" : "TCK  0");
    emit debug((outData & TDI)  ? "TDI  1" : "TDI  0");
    QCoreApplication::processEvents();
#else
    logger << "-----";
    logger << ((outData & TRST) ? "TRST 1\n" : "TRST 0\n");
    logger << ((outData & RST)  ? "RST  1\n" : "RST  0\n");
    logger << ((outData & TMS)  ? "TMS  1\n" : "TMS  0\n");
    logger << ((outData & TCK)  ? "TCK  1\n" : "TCK  0\n");
    logger << ((outData & TDI)  ? "TDI  1\n" : "TDI  0\n");
    QCoreApplication::processEvents();
#endif
}
//---------------------------------------------------------------------------
void OCDS::ResetOff()
{
    cerberusInit(0);
    hardReset(1);
#ifdef EMIT_LOG_ENABLED
    emit debug(tr("Reset"));
    QCoreApplication::processEvents();
#else
    logger << "Reset\n";
    QCoreApplication::processEvents();
#endif
}
//---------------------------------------------------------------------------
int OCDS::ReadWord(int addr, bool silense)
{
    ioSetAddress(addr);
    if(!silense)
    {
#ifdef EMIT_LOG_ENABLED
        emit info(QString("addr %1 data 0x%2").arg(addr).arg(ioReadWord(), 0, 16));
        QCoreApplication::processEvents();
#else
        logger << QString("addr %1 data 0x%2\n").arg(addr).arg(ioReadWord(), 0, 16);
        QCoreApplication::processEvents();
#endif
    }
    return ioReadWord();
}
//---------------------------------------------------------------------------
void OCDS::ViewData()
{
#if 1
    if(!max_write)
    {
#ifdef EMIT_LOG_ENABLED
        emit error(tr("SaveMemoryToFlash: Нет данных"));
        QCoreApplication::processEvents();
#else
        logger << tr("SaveMemoryToFlash: Нет данных\n");
        QCoreApplication::processEvents();
#endif
        return;
    }
#endif

#if 1
    QTime t;
    t.start();

    int dataout=0;
    for (int cc=0; cc<MAX_SIZE; cc+=2)
    {
        if(memory[cc].active)
        {
            if(memory[cc+1].active)
            {
                dataout=memory[cc].data+256*memory[cc+1].data;
            }
            else
            {
                dataout=memory[cc].data+0xFF00;
            }
            Q_UNUSED(dataout);
            //            wordProgram(addr,dataout);
        }
    }
#ifdef EMIT_LOG_ENABLED
    emit info(QString("Time elapsed: %1 ms").arg(t.elapsed()));
    QCoreApplication::processEvents();
#else
    logger << QString("Time elapsed: %1 ms\n").arg(t.elapsed());
    QCoreApplication::processEvents();
#endif
#endif

#if 0
    QImage image(1024, 256, QImage::Format_Indexed8);
    uchar *bits = image.bits();
    for(int i=0; i<(1024*256); i++) bits[i] = Memory.ReadData(i);
    //    bits[1024/2 * 256 + 1024/2] = 0;
    QGraphicsScene * scene = new QGraphicsScene;
    QGraphicsView  * graphicsView = new QGraphicsView;
    graphicsView->setScene(scene);

    QPixmap pixmap;
    pixmap.convertFromImage(image, Qt::AutoColor);
    scene->addPixmap(pixmap);
    graphicsView->show();
#endif

    //    emit debug("ViewData()");
    //    QCoreApplication::processEvents();
}
//---------------------------------------------------------------------------
