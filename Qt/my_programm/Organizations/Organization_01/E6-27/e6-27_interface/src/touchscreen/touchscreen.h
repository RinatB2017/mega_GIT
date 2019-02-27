/****************************************************************************
 *
 * Project: ------
 *
 * Copyright by Olimex Ltd. All rights reserved.
 *
 * File: touchscreen.h
 * Description: Driver for the FPC-K320QVB-V1-O1 touchscreen.
 * Developer: Dimitar Dimitrov ( dimitar,olimex.com )
 *
 * Last change: $Date$
 * Revision: $Revision: 29 $
 * Id: $Id$
 *
 ****************************************************************************/

#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#ifdef __cplusplus
extern "C" {
#endif

void ts_init(void);
int ts_calibrate(void (*tscal_ui_cb)(int x, int y));
void ts_poll(int *x, int *y, int *pressed);

#ifdef __cplusplus
}
#endif

#endif	/* TOUCHSCREEN_H */

