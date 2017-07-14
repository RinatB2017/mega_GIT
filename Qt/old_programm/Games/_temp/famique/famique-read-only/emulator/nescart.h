#ifndef NESCART_H
#define NESCART_H

#include "mappers/mapper.h"
#include "famicom.h"
#include <stdio.h>


#define MAPPER_NOT_SUPPORTED 0xF0F0F0F0

class NESCart
{
public:
    NESCart();
    ~NESCart();
    int Load(char *filename);

    int Save(char *filename);




    Mapper *mROMMapper;
    byte           bMapper;
    byte           bNum16kb;
    byte           bNum8kb;
    byte           bTrainer;





      famicom_header mROM_Header;
private:

long           iLen;
FILE           *mFileHandle;










};

#endif // NESCART_H
