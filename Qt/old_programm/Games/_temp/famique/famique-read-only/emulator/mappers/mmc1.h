#ifndef MMC1_H
#define MMC1_H
#include "mapper.h"

class mmc1 : public Mapper
{
public:
    mmc1();
    ~mmc1();

    /*overloaded functions from Mapper */
    void WriteMemory(unsigned short offset, unsigned char val);
    byte ReadMemory(unsigned short offset);

    void Init(unsigned char, unsigned char);

/* MMC1 specific registers / counters */

    byte hidden_register;
    byte hidden_register_count;

    //struct control_reg mmc1_control;

    byte rd0, rd1, rd2, rd3;
 //   byte *CHR4k;
    void SwitchPRG();
    void SwitchCHR();
    void SwitchSRAM();
    void Mirroring();
    void Synchronize();


};

#endif // MMC1_H
