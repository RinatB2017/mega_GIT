#ifndef NROM_H
#define NROM_H

#include "mappers/mapper.h"
#include "famicom.h"

class NROM : public Mapper
{
public:
    NROM();
    ~NROM();

    /*overloaded functions from superclass */
    void WriteMemory(unsigned short offset, unsigned char val);
    byte ReadMemory(unsigned short offset);
    void Init(byte numPRG, byte numCHR);

};

#endif // NROM_H
