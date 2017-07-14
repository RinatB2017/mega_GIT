#include "nes.h"
#include <QString>

NES::NES(GLWidget *qtglWidget)
{

    this->m_CPU = NULL;
    this->m_NESLoader = NULL;
    this->m_Init = false;
    this->m_isRunning = false;
    this->m_Controller = NULL;

    this->m_NESInput = new NESInput();
    this->m_NESInput->SetDefault();
    this->m_ConfigFile = new CFGParser;
    m_ROMpath = 0;

    if(m_ConfigFile->Load() != FILE_OPEN_ERROR)
    {
        this->m_NESInput->SetControllerBinding1(m_ConfigFile->GetCFG()->Control1_keybindings, true);
        this->m_ROMpath = m_ConfigFile->GetCFG()->ROM_PATH;
    }
    else m_ROMpath = 0;

    this->m_DrawWidget = qtglWidget;


}

NES::~NES()
{
    this->m_ConfigFile->UpdateConfigData(this->GetInputHandle()->GetController(0), this->GetInputHandle()->GetController(1), false, m_ROMpath);
    this->m_ConfigFile->Save();
    delete this->m_NESInput;
    if(m_ROMpath)
        delete  m_ROMpath;

}

int NES::OpenROM( char *filename)
{


    if(m_Init)
    {
        if(GetCPUHandle()->isRunning())
            Stop();
        m_Init = false;

        delete this->m_CPU;
        m_CPU = 0;

        delete this->m_NESCart;
        m_NESCart = 0;

    }
    m_CPU = 0;

    m_NESCart = 0;



    this->m_NESCart = new NESCart();

    switch(m_NESCart->Load(filename))
    {
    case FILE_OPEN_ERROR:
      return FILE_OPEN_ERROR;
    break;
    case FILE_READ_ERROR:
     return FILE_READ_ERROR;
    break;
    case FILE_WRONG_FILETYPE:

        return FILE_WRONG_FILETYPE;
    break;

    case MAPPER_NOT_SUPPORTED:

     break;



    case 0:

    break;


    default:break;
    }
    if((m_NESCart->mROM_Header.info2 & 0xC) == 0xC )
    {
    }
    if(m_NESCart->mROM_Header.num_8kb_ram_banks)
    {

    }
    else
    {
    }

    if((m_NESCart->mROM_Header.video_mode & 1) == 1)
    {
    }
    else
    {

    }



    if((m_NESCart->mROM_Header.info1 & 0x2) == 0x2 || m_NESCart->bMapper == 1) //assume SRAM for all MMC1 games
    {
        // see if we've got a SRAM-file already...
        QString sram_filename = QString(filename) + ".fsr";
        if(m_NESCart->mROMMapper->LoadSRAMFile(sram_filename.toLatin1().data()))
        {
            //ERRROR loading SRAM file
        }
        else
        {
            //restored SRAM into CPU memory map.
        }


    }


    this->m_CPU = new M6502(this->m_NESCart);




        m_Init=true;



       this->Start();

       return 0;
}


void NES::CloseROM()
{

}

void NES::Stop()
{
    this->m_isRunning = false;
}

void NES::Start()
{
    if(m_Init)
        m_isRunning = true;

}

void NES::Update()
{









}


void NES::run()
{
    while(this->isRunning())
    {
        this->m_CPU->NextOpCode();
        this->m_CPU->ppu_2->RunCycles(m_CPU->GetCycleCount() * 3);

        if(m_CPU->ppu_2->inVBlank())
        {
            if(m_CPU->ppu_2->NMI_in_VBlank() && !m_CPU->ppu_2->NMI_occured)
            {
                m_CPU->memory->control1.controller = this->m_Controller;
                m_CPU->memory->control2.controller = m_Controller;

                m_CPU->NMI();
                m_Draw = true;       
            }

        }

    //    m_Draw = m_CPU->ppu_2->draw;
      //  m_CPU->ppu_2->draw = false;

//        this->sleep(1);

    }
}


bool NES::isRunning()
{
    return m_isRunning;
}
