


/*
 * Реализатора симулятора работы БРГ
 *
 * 07/09/2016
 */


#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "pet.h"
#include "pet_log.h"
#include "pet_utils.h"
#include "pet_brg_sim.h"





/**
 * Обработчик команды PET_CMD_TEST
 */
int pet_brg_sim_cmd_test(const pet_frame_t *req, pet_frame_t *rsp, pet_brg_sim_t* brg_sim)
{
   pet_assert(req != NULL && rsp != NULL);
   pet_assert(req->cmd == PET_CMD_TEST);
   pet_assert(brg_sim != NULL);

   /* Формирование ответа */
   rsp->addr = req->addr;
   rsp->cmd = req->cmd;
   rsp->len = req->len;
   memcpy(rsp->data, req->data, req->len);

   return PET_SUCCESS;
}


/**
 * Обработчик команды PET_CMD_DISCOVERY
 */
int pet_brg_sim_cmd_discovery(const pet_frame_t *req, pet_frame_t *rsp, pet_brg_sim_t* brg_sim)
{
   pet_discovery_info_t *info;

   pet_assert(req != NULL && rsp != NULL);
   pet_assert(req->cmd == PET_CMD_DISCOVERY);

   /* Формирование ответа */
   rsp->addr = req->addr;
   rsp->cmd = PET_CMD_DISCOVERY;
   rsp->len = sizeof(*info);

   info = (pet_discovery_info_t *)rsp->data;
   info->vid = PET_BRG_SIM_VID;
   info->pid = PET_BRG_SIM_PID;
   memcpy(info->uid, brg_sim->uid, sizeof(info->uid));

   return PET_SUCCESS;
}


/**
 * Обработчик команды PET_CMD_GET_DEVICE_INFO
 */
int pet_brg_sim_cmd_get_device_info(const pet_frame_t *req, pet_frame_t *rsp, pet_brg_sim_t* brg_sim)
{
   pet_device_info_t *info;

   pet_assert(req != NULL && rsp != NULL);
   pet_assert(req->cmd == PET_CMD_GET_DEVICE_INFO);
   pet_assert(brg_sim != NULL);

   /* Формирование ответа */
   rsp->addr = req->addr;
   rsp->cmd = PET_CMD_GET_DEVICE_INFO;
   rsp->len = sizeof(*info);

   info = (pet_device_info_t *)rsp->data;

   info->vid = PET_BRG_SIM_VID;
   info->pid = PET_BRG_SIM_PID;
   memcpy(info->uid, brg_sim->uid, sizeof(info->uid));
   info->hw_version = PET_BRG_SIM_HW_VERSION;
   info->sw_version = PET_BRG_SIM_SW_VERSION;
   info->prot_version = PET_VERSION;
   info->hw_timestamp = PET_BRG_SIM_HW_TIMESTAMP;
   info->sw_timestamp = PET_BRG_SIM_SW_TIMESTAMP;
   info->work_time = -1;

   return PET_SUCCESS;
}


/**
 * Обработчик команды PET_CMD_RESET
 */
int pet_brg_sim_cmd_reset(const pet_frame_t *req, pet_frame_t *rsp, pet_brg_sim_t* brg_sim)
{
   pet_assert(req != NULL && rsp != NULL);
   pet_assert(req->cmd == PET_CMD_RESET);
   pet_assert(brg_sim != NULL);

   /* Формирование ответа */
   rsp->addr = req->addr;
   rsp->cmd = PET_CMD_RESET;
   rsp->len = 0;

   pet_brg_sim_reset(brg_sim);

   return PET_SUCCESS;
}


/**
 * Обработчик команды PET_CMD_SHUTDOWN
 */
int pet_brg_sim_cmd_shutdown(const pet_frame_t *req, pet_frame_t *rsp, pet_brg_sim_t* brg_sim)
{
   pet_assert(req != NULL && rsp != NULL);
   pet_assert(req->cmd == PET_CMD_SHUTDOWN);
   pet_assert(brg_sim != NULL);

   //
   // TO DO: ...
   //

   return PET_NOT_IMPLEMENT;
}


/**
 * Обработчик команды PET_CMD_GET_CONTROL
 */
int pet_brg_sim_get_control(const pet_frame_t *req, pet_frame_t *rsp, pet_brg_sim_t* brg_sim)
{
   pet_assert(req != NULL && rsp != NULL);
   pet_assert(req->cmd == PET_CMD_GET_CONTROL);
   pet_assert(brg_sim != NULL);

   //
   // TO DO: ...
   //

   return PET_NOT_IMPLEMENT;
}


/**
 * Обработчик команды PET_CMD_SET_CONTROL
 */
int pet_brg_sim_set_control(const pet_frame_t *req, pet_frame_t *rsp, pet_brg_sim_t* brg_sim)
{
   pet_assert(req != NULL && rsp != NULL);
   pet_assert(req->cmd == PET_CMD_SET_CONTROL);
   pet_assert(brg_sim != NULL);

   //
   // TO DO: ...
   //

   return PET_NOT_IMPLEMENT;
}


/**
 * Обработчик команды PET_CMD_GET_STATE
 */
int pet_brg_sim_get_state(const pet_frame_t *req, pet_frame_t *rsp, pet_brg_sim_t* brg_sim)
{
   pet_assert(req != NULL && rsp != NULL);
   pet_assert(req->cmd == PET_CMD_GET_STATE);
   pet_assert(brg_sim != NULL);

   //
   // TO DO: ...
   //

   return PET_NOT_IMPLEMENT;
}


/**
 * Обработчик команды PET_CMD_GET_STATISTICS
 */
int pet_brg_sim_get_statistics(const pet_frame_t *req, pet_frame_t *rsp, pet_brg_sim_t* brg_sim)
{
   pet_assert(req != NULL && rsp != NULL);
   pet_assert(req->cmd == PET_CMD_GET_STATISTICS);
   pet_assert(brg_sim != NULL);

   //
   // TO DO: ...
   //

   return PET_NOT_IMPLEMENT;
}


/**
 * Обработчик команды PET_CMD_GET_CH_CONTROL
 */
int pet_brg_sim_get_ch_control(const pet_frame_t *req, pet_frame_t *rsp, pet_brg_sim_t* brg_sim)
{
   pet_assert(req != NULL && rsp != NULL);
   pet_assert(req->cmd == PET_CMD_GET_CH_CONTROL);
   pet_assert(brg_sim != NULL);

   //
   // TO DO: ...
   //

   return PET_NOT_IMPLEMENT;
}


/**
 * Обработчик команды PET_CMD_SET_CH_CONTROL
 */
int pet_brg_sim_set_ch_control(const pet_frame_t *req, pet_frame_t *rsp, pet_brg_sim_t* brg_sim)
{
   pet_assert(req != NULL && rsp != NULL);
   pet_assert(req->cmd == PET_CMD_SET_CH_CONTROL);
   pet_assert(brg_sim != NULL);

   //
   // TO DO: ...
   //

   return PET_NOT_IMPLEMENT;
}


/**
 * Обработчик команды PET_CMD_GET_CH_STATE
 */
 int pet_brg_sim_get_ch_state(const pet_frame_t *req, pet_frame_t *rsp, pet_brg_sim_t* brg_sim)
{
   pet_assert(req != NULL && rsp != NULL);
   pet_assert(req->cmd == PET_CMD_GET_CH_STATE);
   pet_assert(brg_sim != NULL);

   //
   // TO DO: ...
   //

   return PET_NOT_IMPLEMENT;
}


/**
 * Обработчик команды PET_CMD_GET_CH_STATISTICS
 */
int pet_brg_sim_get_ch_statistics(const pet_frame_t *req, pet_frame_t *rsp, pet_brg_sim_t* brg_sim)
{
   pet_assert(req != NULL && rsp != NULL);
   pet_assert(req->cmd == PET_CMD_GET_CH_STATISTICS);
   pet_assert(brg_sim != NULL);

   //
   // TO DO: ...
   //

   return PET_NOT_IMPLEMENT;
}


/**
 * Обработчик команды PET_CMD_GET_CALIBRATION
 */
int pet_brg_sim_get_calibration(const pet_frame_t *req, pet_frame_t *rsp, pet_brg_sim_t* brg_sim)
{
   pet_assert(req != NULL && rsp != NULL);
   pet_assert(req->cmd == PET_CMD_GET_CALIBRATION);
   pet_assert(brg_sim != NULL);

   //
   // TO DO: ...
   //

   return PET_NOT_IMPLEMENT;
}


/**
 * Обработчик команды PET_CMD_SET_CALIBRATION
 */
int pet_brg_sim_set_calibration(const pet_frame_t *req, pet_frame_t *rsp, pet_brg_sim_t* brg_sim)
{
   pet_assert(req != NULL && rsp != NULL);
   pet_assert(req->cmd == PET_CMD_SET_CALIBRATION);
   pet_assert(brg_sim != NULL);

   //
   // TO DO: ...
   //

   return PET_NOT_IMPLEMENT;
}


/**
 * Обработчик команды PET_CMD_CONFIG_SAVE
 */
int pet_brg_sim_config_save(const pet_frame_t *req, pet_frame_t *rsp, pet_brg_sim_t* brg_sim)
{
   pet_assert(req != NULL && rsp != NULL);
   pet_assert(req->cmd == PET_CMD_CONFIG_SAVE);
   pet_assert(brg_sim != NULL);

   //
   // TO DO: ...
   //

   return PET_NOT_IMPLEMENT;
}


/**
 * Обработчик команды PET_CMD_CONFIG_DEFAULT
 */
int pet_brg_sim_config_default(const pet_frame_t *req, pet_frame_t *rsp, pet_brg_sim_t* brg_sim)
{
   pet_assert(req != NULL && rsp != NULL);
   pet_assert(req->cmd == PET_CMD_CONFIG_DEFAULT);
   pet_assert(brg_sim != NULL);

   //
   // TO DO: ...
   //

   return PET_NOT_IMPLEMENT;
}


/**
 * Обработчик команды PET_CMD_UPDATE
 */
int pet_brg_sim_update(const pet_frame_t *req, pet_frame_t *rsp, pet_brg_sim_t* brg_sim)
{
   pet_assert(req != NULL && rsp != NULL);
   pet_assert(req->cmd == PET_CMD_UPDATE);
   pet_assert(brg_sim != NULL);

   //
   // TO DO: ...
   //

   return PET_NOT_IMPLEMENT;
}


/**
 * Обработчик команды PET_CMD_WAVE
 */
int pet_brg_sim_cmd_wave(const pet_frame_t *req, pet_frame_t *rsp, pet_brg_sim_t* brg_sim)
{
   const int16_t wave_pattern [PET_WAVE_POINT_MAX] = {
         0x0000, 0x0000, 0x0000, 0x0000, 0x131C, 0x1E77, 0x2481, 0x26F5,
         0x2710, 0x25AD, 0x2368, 0x20A8, 0x1DB5, 0x1ABE, 0x17E1, 0x152F,
         0x12B2, 0x106F, 0x0E64, 0x0C92, 0x0AF4, 0x0987, 0x0845, 0x072C,
         0x0636, 0x0560, 0x04A6, 0x0404, 0x0378, 0x02FE, 0x0295, 0x023A,
         0x01EB, 0x01A8, 0x016D, 0x013A, 0x010F, 0x00E9, 0x00C9, 0x00AD,
         0x0095, 0x0080, 0x006E, 0x005F, 0x0052, 0x0047, 0x003D, 0x0034,
         0x002D, 0x0027, 0x0021, 0x001D, 0x0019, 0x0015, 0x0012, 0x0010,
         0x000E, 0x000C, 0x000A, 0x0009, 0x0007, 0x0006, 0x0006, 0x0005
      };

   pet_wave_t *pet_wave_req;
   pet_wave_t *pet_wave_rsp;

   float k_x, k_y;

   int i;

   pet_assert(req != NULL && rsp != NULL);
   pet_assert(req->cmd == PET_CMD_WAVE);
   pet_assert(brg_sim != NULL);

   pet_wave_req = (pet_wave_t *)req->data;
   pet_wave_rsp = (pet_wave_t *)rsp->data;


   if (pet_wave_req->channel < 0 || pet_wave_req->channel > 3) {
      return PET_BAD_CHANNEL;
   }

   if (pet_wave_req->mode & PET_WAVE_MODE_EN) {

      pet_wave_rsp->channel = pet_wave_req->channel;
      pet_wave_rsp->mode = pet_wave_req->mode & ~PET_WAVE_MODE_EN;
      pet_wave_rsp->count = (pet_wave_req->count < PET_WAVE_POINT_MAX) ? pet_wave_req->count : PET_WAVE_POINT_MAX;

      k_x = ((float)rand()) / ((float)RAND_MAX);
      k_y = ((float)rand()) / ((float)RAND_MAX);

      for (i = 0; i < pet_wave_rsp->count; i++) {
         pet_wave_rsp->point[i].xp = (int16_t)(-k_x * wave_pattern[i]);
         pet_wave_rsp->point[i].xn = (int16_t)(-(1.0f - k_x) * wave_pattern[i]);
         pet_wave_rsp->point[i].yp = (int16_t)(-k_y * wave_pattern[i]);
         pet_wave_rsp->point[i].yn = (int16_t)(-(1.0f - k_y) * wave_pattern[i]);

      }
   }
   else {
      pet_wave_rsp->channel = pet_wave_req->channel;
      pet_wave_rsp->mode = pet_wave_req->mode;
      pet_wave_rsp->count = 0;
   }

   /* Формирование ответа */
   rsp->addr = req->addr;
   rsp->cmd = PET_CMD_WAVE;
   rsp->len = sizeof(*pet_wave_rsp) + pet_wave_rsp->count * sizeof(pet_wave_rsp->point[0]);

   return PET_SUCCESS;
}


/**
 * Обработчик команды PET_CMD_EVENT
 */
int pet_brg_sim_event(const pet_frame_t *req, pet_frame_t *rsp, pet_brg_sim_t* brg_sim)
{
   pet_assert(req != NULL && rsp != NULL);
   pet_assert(req->cmd == PET_CMD_EVENT);
   pet_assert(brg_sim != NULL);

   //
   // TO DO: ...
   //

   return PET_NOT_IMPLEMENT;
}


///////////////////////////////////////////////////////////////////////////////


/**
 * Таблица команд
 */
const struct pet_brg_sim_cmd_tbl_node {
   uint8_t cmd;
   int (*cmd_func)(const pet_frame_t *req, pet_frame_t *rsp, pet_brg_sim_t* brg_sim);
} pet_brg_sim_cmd_tbl[] = {

   {PET_CMD_TEST,                pet_brg_sim_cmd_test},
   {PET_CMD_DISCOVERY,           pet_brg_sim_cmd_discovery},
   {PET_CMD_GET_DEVICE_INFO,     pet_brg_sim_cmd_get_device_info},
   {PET_CMD_RESET,               pet_brg_sim_cmd_reset},
   {PET_CMD_SHUTDOWN,            pet_brg_sim_cmd_shutdown},
   {PET_CMD_GET_CONTROL,         pet_brg_sim_get_control},
   {PET_CMD_SET_CONTROL,         pet_brg_sim_set_control},
   {PET_CMD_GET_STATE,           pet_brg_sim_get_state},
   {PET_CMD_GET_STATISTICS,      pet_brg_sim_get_statistics},
   {PET_CMD_GET_CH_CONTROL,      pet_brg_sim_get_ch_control },
   {PET_CMD_SET_CH_CONTROL,      pet_brg_sim_set_ch_control},
   {PET_CMD_GET_CH_STATE,        pet_brg_sim_get_ch_state},
   {PET_CMD_GET_CH_STATISTICS,   pet_brg_sim_get_ch_statistics},
   {PET_CMD_GET_CALIBRATION,     pet_brg_sim_get_calibration},
   {PET_CMD_SET_CALIBRATION,     pet_brg_sim_set_calibration },
   {PET_CMD_CONFIG_SAVE,         pet_brg_sim_config_save},
   {PET_CMD_CONFIG_DEFAULT,      pet_brg_sim_config_default},
   {PET_CMD_UPDATE,              pet_brg_sim_update },
   {PET_CMD_WAVE,                pet_brg_sim_cmd_wave},
   {PET_CMD_EVENT,               pet_brg_sim_event}
};

#define PET_BRG_SIM_CMD_NUM   (sizeof(pet_brg_sim_cmd_tbl) / sizeof(pet_brg_sim_cmd_tbl[0]))


/**
 * Выполняет обработку запрошенной команды
 */
int pet_brg_sim_cmd_proc(const pet_frame_t *req, pet_frame_t *rsp, void *instance)
{
   int i;

   pet_assert(req != NULL);

   /* Обработка */
   for (i = 0; i < PET_BRG_SIM_CMD_NUM; i++) {
      if (pet_brg_sim_cmd_tbl[i].cmd == req->cmd) {
         pet_assert(pet_brg_sim_cmd_tbl[i].cmd_func != NULL);
         return (*(pet_brg_sim_cmd_tbl[i].cmd_func))(req, rsp, (pet_brg_sim_t *)instance);
      }
   }

   return PET_BAD_CMD;
}
