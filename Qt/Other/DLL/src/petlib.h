
#ifndef _PETLIB_H
#define _PETLIB_H
//-------------------------------------------------------------------------------------
#include <stddef.h>
#include "pet.h"
//-------------------------------------------------------------------------------------
/*
����������� ���� ������ ���������� ���������� ���
*/
typedef struct PETDEVICE PET, *LPPET;
//-------------------------------------------------------------------------------------
LPPET PETOpen(const char *cszURI, uint16_t nPort);
int PETClose(LPPET lpPET); 

int PETTest(LPPET lpPET, void *pData, size_t nDataLength);
int PETGetDeviceInfo(LPPET lpPET, pet_device_info_t *lpInfo);
int PETVawe(LPPET lpPET, int nChannel, int nMode, int nCount, pet_wave_point_t *lpPoins);

//
// TO DO: � ��� �� ��������
//
void PETSetLastError(LPPET lpPET, int nError);
//-------------------------------------------------------------------------------------
#endif	/* _PETLIB_H */
