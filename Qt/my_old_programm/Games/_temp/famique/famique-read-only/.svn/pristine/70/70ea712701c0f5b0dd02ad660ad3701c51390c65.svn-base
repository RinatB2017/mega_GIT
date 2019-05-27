#ifndef MAPPER_H
#define MAPPER_H

#include "famicom.h"

class Mapper//mapper superclass
{
public:
    Mapper();

    ~Mapper();

   virtual void WriteMemory(unsigned short offset, unsigned char val) = 0;
   virtual  unsigned char ReadMemory(unsigned short  offset) = 0;
   //virtual void Reset() = 0;
   byte *GetCPUMemMap();
   byte *GetPPUMemMap();

   void LoadCHR(byte *data, long len);
   void LoadPRG(byte *data, long len);

   virtual void Init(byte, byte) = 0;

   byte *bPRGBanks[32];
   byte *bCHRBanks[64];


   byte bNumPRGBanks;
   byte bNumCHRBanks;

   byte *PRG, *CHR;

   byte *CHRPtr_high; //0x0000
   byte *CHRPtr_low; //0x1000



   bool           bVertical;
   bool           bHorizontal;
   bool           bSingle;
   word           wSingleMirroringBase;
   bool           bFourScreen;
   bool           bHasSRAM;


   byte *SRAM;
   int LoadSRAMFile(char *file);
   int SaveSRAMFile();
   char *SRAM_filename;
   void InitFreshSRAM();
   void WriteSRAM(word offset, byte val);
   byte ReadSRAM(word offset);



   byte ReadCHR(word offset);
   void WriteCHR(word offset, byte val);

   void SetupCHR(int num_chrbanks);
   void SetupVRAM8k();

   void SwapPRG16(int _16kbank, int banknumber);
   void SwapCHR8(int banknumber);
   void SwapCHR4(int _4kbank, int banknumber);


private:

   byte page_cross;





};

#endif // MAPPER_H
