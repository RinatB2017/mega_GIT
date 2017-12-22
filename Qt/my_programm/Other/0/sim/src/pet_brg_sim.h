/*
 * pet_brg_sim.h
 *
 *  Created on: 08 сент. 2016 г.
 *      Author: В. Клишин
 */

#ifndef PET_BRG_SIM_H_
#define PET_BRG_SIM_H_



#define PET_BRG_SIM_HW_VERSION      PET_MAKE_VERSION(0, 0)
#define PET_BRG_SIM_SW_VERSION      PET_MAKE_VERSION(0, 1)
#define PET_BRG_SIM_HW_TIMESTAMP    -1
#define PET_BRG_SIM_SW_TIMESTAMP    -1


#define PET_BRG_SIM_VID    PET_PID_UNKNOWN
#define PET_BRG_SIM_PID    PET_PID_BRG_SIM

/**
 *
 */
typedef struct pet_brg_sim {
   //uint16_t    vid;
   //uint16_t    pid;
   uint8_t     uid[6];

} pet_brg_sim_t;



pet_brg_sim_t *pet_brg_sim_open(void);
pet_brg_sim_t *pet_brg_sim_close(pet_brg_sim_t *brg_sim);

int pet_brg_sim_cmd_proc(const pet_frame_t *req, pet_frame_t *rsp, void *instance);

void pet_brg_sim_reset(pet_brg_sim_t *brg_sim);


#endif /* PET_BRG_SIM_H_ */
