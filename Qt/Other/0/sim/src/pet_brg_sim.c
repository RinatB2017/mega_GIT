/*
 * pet_brg_sim.c
 *
 *  Created on: 08 сент. 2016 г.
 *      Author: В. Клишин
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pet.h"
#include "pet_log.h"
#include "pet_utils.h"

#include "pet_brg_sim.h"






/**
 *
 */
pet_brg_sim_t *pet_brg_sim_open(void)
{
   pet_brg_sim_t *brg_sim;

   brg_sim = (pet_brg_sim_t *)malloc(sizeof(*brg_sim));

   if (brg_sim != NULL) {

      brg_sim->uid[0] = (uint8_t)rand();
      brg_sim->uid[1] = (uint8_t)rand();
      brg_sim->uid[2] = (uint8_t)rand();
      brg_sim->uid[3] = (uint8_t)rand();
      brg_sim->uid[4] = (uint8_t)rand();
      brg_sim->uid[5] = (uint8_t)rand();

   }

   return brg_sim;
}


/**
 *
 */
pet_brg_sim_t *pet_brg_sim_close(pet_brg_sim_t *brg_sim)
{
   pet_assert(brg_sim != NULL);
   free(brg_sim);
   return (pet_brg_sim_t *)NULL;
}

void pet_brg_sim_reset(pet_brg_sim_t *brg_sim)
{
   //
   // TO DO: ...
   //
}



/*
int pet_brg_sim_cmd_proc(const pet_frame_t *req, pet_frame_t *rsp, void *instance);

*/








