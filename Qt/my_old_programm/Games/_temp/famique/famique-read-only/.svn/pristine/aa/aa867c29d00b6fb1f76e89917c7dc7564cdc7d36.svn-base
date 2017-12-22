#ifndef UXROM_H
#define UXROM_H
#include "mapper.h"

class UxROM : public Mapper
{
public:
    UxROM();

    void WriteMemory(unsigned short offset, unsigned char val);
    byte ReadMemory(unsigned short offset);

    void Init(unsigned char numprgbanks, unsigned char numchrbanks);


};

#endif // UXROM_H
