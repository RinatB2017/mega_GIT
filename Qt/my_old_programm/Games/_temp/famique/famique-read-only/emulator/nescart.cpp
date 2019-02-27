#include <memory.h>
#include <string.h>
#include "nescart.h"

#include "mappers/nrom.h"
#include "mappers/mmc1.h"
#include "mappers/cnrom.h"
#include "mappers/uxrom.h"





NESCart::NESCart()
{
    mROMMapper = 0;





}

NESCart::~NESCart()
{
if(mROMMapper != 0)
    delete mROMMapper;
mROMMapper = 0;

memset(&this->mROM_Header, 0, sizeof(famicom_header));

}

int NESCart::Load(char *filename)
{


    //memset(&head, 0, sizeof(famicom_header));



    byte *pTempPRG;
    byte *pTempCHR;



    const char NES_IDENT[4] = {'N','E','S',0x1a};



   memset(&mROM_Header, 0, sizeof(famicom_header));


   mFileHandle = NULL;
   mFileHandle = fopen(filename, "rb");
   if(!mFileHandle)
       return FILE_OPEN_ERROR;


   rewind(mFileHandle);

   if(fread(&mROM_Header, 1, sizeof(famicom_header), mFileHandle) != sizeof(famicom_header))
        return FILE_READ_ERROR;



  if( strcmp((const char *)mROM_Header.NES_IDENT, NES_IDENT) == 0)
      return FILE_WRONG_FILETYPE;

  //let's find out which mapper we haffi load

  bMapper = (mROM_Header.info1 & 0xf0) >> 4;
  bMapper |= (mROM_Header.info2 & 0xf0);


   switch(bMapper)
   {

       /* we overload the mROMMapper base class with the relevant mapper class */
     case 0:

          this->mROMMapper = new NROM;

          break;

     case 1:
          this->mROMMapper = new mmc1;

          break;

     case 2:
          this->mROMMapper = new UxROM;

          break;

      case 3:
          this->mROMMapper = new CNROM;

          break;

     default:

          this->mROMMapper = new NROM;
          return MAPPER_NOT_SUPPORTED;
           break;

   }


  this->bNum16kb = mROM_Header.num_16kb_rom_banks;
  this->bNum8kb = mROM_Header.num_8kb_vrom_banks;

  pTempPRG = new byte[bNum16kb * MEM_16k];
  pTempCHR = new byte[bNum8kb * MEM_8k];

  fread(pTempPRG, MEM_16k * bNum16kb, 1, mFileHandle);
  fread(pTempCHR, MEM_8k * bNum8kb, 1, mFileHandle);

  mROMMapper->CHR = pTempCHR;
  mROMMapper->PRG = pTempPRG;

  mROMMapper->Init(bNum16kb, bNum8kb); //set up the banks we've just loaded with the appropriate mapper

  if((mROM_Header.info1 & 0x1) != 0x1)
      this->mROMMapper->bHorizontal = true;
  else
      this->mROMMapper->bVertical = true;





  fclose(mFileHandle);

  delete [] pTempPRG;
  delete [] pTempCHR;
  pTempCHR = 0;
  pTempPRG = 0;
  mROMMapper->CHR = 0;
  mROMMapper->PRG = 0;


  return 0;//all went fine
}
