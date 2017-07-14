#ifndef CNROM_H
#define CNROM_H
#include "mappers/mapper.h"

class CNROM : public Mapper
{
public:
    CNROM();

    void WriteMemory(unsigned short offset, unsigned char val);
    byte ReadMemory(unsigned short offset);

    void Init(unsigned char, unsigned char);

};

#endif // CNROM_H
