#include "apu.h"
#include "memory.h"

APU *global_apu_ptr;


int motherfuckingsepecallback(void *, cpu_addr_t addr)
{
  return  global_apu_ptr->mem->ReadMemory(addr);
}

APU::APU()
{


    global_apu_ptr = this;



    blarggAPU = new Nes_Apu();
    blarggbuf = new Blip_Buffer();

    blarggbuf->sample_rate(44100);

    blarggbuf->clock_rate(1789773);

    blarggAPU->output(blarggbuf);



    this->APU_addr = 0x4000;


    snd = new Sound_Queue();
    snd->init(44100);



    cpu_cycles_total = cpu_cycles_remain = 0;
    blarggAPU->dmc_reader(motherfuckingsepecallback);


}

APU::~APU()
{

    delete blarggbuf;
    delete blarggAPU;
    //delete [] out_buf;
    delete snd;


}

int APU::dmc_read(void *, cpu_addr_t addr)//redefine our overloaded member function I had to put in Nes_Apu
{


    return  mem->ReadMemory(addr);

}

void APU::RenderAudio(int length)
{
    blarggAPU->end_frame(length);
    blarggbuf->end_frame(length);
    this->cpu_cycles_total -= length;
    if(blarggbuf->samples_avail() >= BUF_SIZE)
    {
        size_t count = blarggbuf->read_samples(out_buf, BUF_SIZE);
        output_samples(out_buf, count);

    }


}

void APU::output_samples(const blip_sample_t *sample, size_t count)
{

    snd->write( sample, count );
}

