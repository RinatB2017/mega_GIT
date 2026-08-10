//---------------------------------------------------------------------------
#ifndef OCDS_H
#define OCDS_H
//---------------------------------------------------------------------------
#include <QObject>
//---------------------------------------------------------------------------
//#include "memory.h"
//---------------------------------------------------------------------------
#define TRST    0x40
#define RST     0x20
#define TMS     0x4
#define TCK     0x2
#define TDI     0x1
//---------------------------------------------------------------------------
class Parport;
class QTextStream;
//---------------------------------------------------------------------------
class OCDS : public QObject
{
    Q_OBJECT

public:
    explicit OCDS(QObject* parent = 0);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

    void progress(int);

public slots:
    void ReadHEX (QString filename);
    void ReadH86 (QString filename);
    int WriteDataFlash(int addr, int data, bool silense = false);
    void CheckAddress(int addr);
    void EraseAllFlash(void);
    void EraseSectorFlash(int addr);
    void EraseBlockFlash(int addr);
    void SaveMemoryToFlash(void);
    void ResetOff(void);
    void printMemory(void);
    int ReadWord(int addr, bool silense = false);
    void ViewData(void);

private slots:
    void log(const QString &data);

private:
    QWidget *parent;
    Parport *parport;
    int ReadData;
    int tdoBit;
    unsigned char outData; //WORD
    int max_write;
    int magic_counter;

    void connect_log(void);

    int  HBDconvert (QString conv_value);

    void hardReset(int);
    void trst0(int);
    void assignTms(int);
    void assignTdi(int,int);
    void idle(int);
    void shiftIr(int);
    void shiftDr(int,int);
    void cerberusInit(int);
    int  tdoCheck(void);

    void ioConfig(int);
    void ioSetAddress(int);
    void ioWriteBlock(int);
    int  ioReadBlock(void);
    void ioWriteWord(int);
    int  ioReadWord(void);
    void ioWriteByte(int);
    void ioSupervisor(void);
        
    void wordProgram(int,int);
    void sectorErase(int);
    void blockErase(int);
    void allErase(void);

    int  HexToInt (QString HEX);
    QString IntToHex(int a, int b);
    void checkOutData(void);
};
//---------------------------------------------------------------------------
#endif
