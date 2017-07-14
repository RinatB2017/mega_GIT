
#include "pet.h"
#include "petlib.h"

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
//-------------------------------------------------------------------------------------
/**
 * ������� ������������� ������ ���������� ���
 */
struct PETDEVICE 
{
	/* ���������� ������� ���������� ��� ����� */
	//socket cmd_socket;
	//socket event_socket;

	/* ������ ��� ������ */
	uint8_t tx_frame_buf[PET_FRAME_MAX];
	uint8_t rx_frame_buf[PET_FRAME_MAX];

	//
	// TO DO: ������ ����������� ���� � ������ ����� �� ������ �)
	//

	int nLastError;
};
//-------------------------------------------------------------------------------------
LPPET PETOpen(const char *cszURI, uint16_t nPort)
{
	LPPET lpPET;

	lpPET = (LPPET)malloc(sizeof(*lpPET));
	if (lpPET == NULL) 
	{
		//
		// TO DO: ...
		//
		return NULL;
	}

	//
	// TO DO: ...
	//

	lpPET->nLastError = PET_SUCCESS;

	return lpPET;
}
//-------------------------------------------------------------------------------------
int PETClose(LPPET lpPET)
{
	if (lpPET == NULL) 
	{
		//
		// TO DO: ...
		//
	}

	//
	// TO DO: ...
	//

	free(lpPET);

	return PET_SUCCESS;
}
//-------------------------------------------------------------------------------------
/**
 * �������� �������
 */
int PETTest(LPPET lpPET, void *pData, size_t nDataLength)
{
	int i, iResult;
	pet_frame_t *lpReq; /* ������ */
	pet_frame_t *lpRsp; /* ����� */

	if (lpPET == NULL) 
	{
		//
		// TO DO: ...
		//
	}

	/* ������������ ������� */
	lpReq = (pet_frame_t *)lpPET->tx_frame_buf; /* ���� ��� ����� ������ � �����������, ���� �� ����� ����� ���������� �������������� !!!!! */
	lpReq->addr = 0x00; // ��������
	lpReq->cmd = PET_CMD_TEST;
	lpReq->len = (nDataLength < PET_FRAME_DATA_MAX) ? nDataLength : PET_FRAME_DATA_MAX;
	if (pData != NULL) 
	{
		memcpy(lpReq->data, pData, lpReq->len);
	}
	else 
	{
		for (i = 0; i < lpReq->len; i++) 
		{
			lpReq->data[i] = i;
		}
	}

	/* ���������� ����� ��� ������ */
	lpRsp = (pet_frame_t *)lpPET->rx_frame_buf; /* ���� ��� ����� ������ � �����������, ���� �� ����� ����� ���������� �������������� !!!!! */

	/* ����� ������� ����������� ��� ���� ������ */
	iResult = PETCmd(lpPET, lpReq, lpRsp, 0);
	if (iResult != PET_SUCCESS) 
	{
		return iResult;
	}

	/* �������� ������������ ������ */
	iResult = PET_SUCCESS;
	if (lpRsp->addr != lpReq->addr || lpRsp->cmd != lpReq->cmd || lpRsp->len != lpReq->len) 
	{
		PETSetLastError(lpPET, 9999); /* �� ����� ��� �� ������� �� ��������� ������ ������ ������� */
		return PET_FAULT;
	}
	
	for (i = 0; i < lpReq->len; i++) 
	{
		if (lpRsp->data[i] != lpReq->data[i]) 
		{
			PETSetLastError(lpPET, 9998); /* �� ����� ��� �� ������� �� ��������� ������ ������ ������� */
			return PET_FAULT;
		}
	}

	return PET_SUCCESS;
}
//-------------------------------------------------------------------------------------
//
// TO DO: ��� ������� ���
//
//-------------------------------------------------------------------------------------
/**
 * ����� ���������� ���� ������
 * iFlags - �� �������� ��� ������ ������� ��������� �������
 */
int PETCmd(LPPET lpPET, const pet_frame_t *lpReq, pet_frame_t *lpRsp, int iFlags)
{
	/* ���� ����� �� ���������� :) */
	assert(lpPET != NULL);
	assert(lpReq != NULL);
	assert(lpRsp != NULL);

	//
	// TO DO: ...
	//

#if 0	
	sendto(
		//sock,
		lpReq,
		PET_FRAME_LEN(lpReq)//,
		//0,
		//(struct sockaddr*)&sa,
		//sizeof(struct sockaddr_in)
		);
#endif
	
	//
	// TO DO: ...
	//

#if 0
	recvfrom(
		//sock, 
		(void*)lpRsp,
		PET_FRAME_MAX//,
		//0,
		//(struct sockaddr*)&sa,
		//address_len
		);
#endif

	return PET_SUCCESS;
}
//-------------------------------------------------------------------------------------
void PETSetLastError(LPPET lpPET, int nError)
{
	if (lpPET != NULL) 
	{
		lpPET->nLastError = nError;
	}
	else 
	{
		//
		// TO DO: ...
		//
	}
}
//-------------------------------------------------------------------------------------
int PETGetLastError(LPPET lpPET)
{
	if (lpPET != NULL) 
	{
		return lpPET->nLastError;
	}
	else 
	{
		//
		// TO DO: ...
		//
	}
}
//-------------------------------------------------------------------------------------
