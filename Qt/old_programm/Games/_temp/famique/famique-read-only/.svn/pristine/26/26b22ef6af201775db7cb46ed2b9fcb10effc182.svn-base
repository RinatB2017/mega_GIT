#include "ppu_base.h"

#include <string.h>
PPU_base::PPU_base()
{

draw = 0;
this->NMI_occured = false;

texmem = new byte [256*256*4]; //256x256, OpenGL textures has to be powers of two.
memset(texmem, 0, 256*256*4);
}
