/****************************************************************************
 *
 * Project: ------
 *
 * Copyright by Olimex Ltd. All rights reserved.
 *
 * File: touchscreen-hw.h
 * Description: Driver for the FPC-K320QVB-V1-O1 touchscreen.
 * Developer: Dimitar Dimitrov ( dimitar,olimex.com )
 *
 * Last change: $Date$
 * Revision: $Revision: 29 $
 * Id: $Id$
 *
 ****************************************************************************/

#ifndef TOUCHSCREEN_HW_H
#define TOUCHSCREEN_HW_H

#include <stm32f10x.h>

#ifdef __cplusplus
extern "C" {
#endif

void tshw_init(void);
void tshw_get_raw(u16 *adc_x, u16 *adc_y);
void tshw_prepare_wait(void);
void tshw_poll(u16 *adc_x, u16 *adc_y, int *pressed);

#ifdef __cplusplus
}
#endif

#endif	/* TOUCHSCREEN_HW_H */

