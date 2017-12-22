
#include "pet.h"
#include "petlib.h"

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
//-------------------------------------------------------------------------------------
/**
 * Скрытое представление данных устройства ПЭТ
 */
struct PETDEVICE 
{
	/* Внутренние сетевые переменные для связи */
	//socket cmd_socket;
	//socket event_socket;

	/* Буферы для обмена */
	uint8_t tx_frame_buf[PET_FRAME_MAX];
	uint8_t rx_frame_buf[PET_FRAME_MAX];

	//
	// TO DO: Прочие необходимые вещи о котрых никто не узнает Ж)
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
 * Тестовая команда
 */
int PETTest(LPPET lpPET, void *pData, size_t nDataLength)
{
	int i, iResult;
	pet_frame_t *lpReq; /* Запрос */
	pet_frame_t *lpRsp; /* Ответ */

	if (lpPET == NULL) 
	{
		//
		// TO DO: ...
		//
	}

	/* Формирование запроса */
	lpReq = (pet_frame_t *)lpPET->tx_frame_buf; /* Хотя это можно делать и динамически, даже из стека чтобы обеспечить кроспоточность !!!!! */
	lpReq->addr = 0x00; // Например
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

	/* Подготовка места для ответа */
	lpRsp = (pet_frame_t *)lpPET->rx_frame_buf; /* Хотя это можно делать и динамически, даже из стека чтобы обеспечить кроспоточность !!!!! */

	/* Вызов единого обработчика для всех команд */
	iResult = PETCmd(lpPET, lpReq, lpRsp, 0);
	if (iResult != PET_SUCCESS) 
	{
		return iResult;
	}

	/* Проверка правильности ответа */
	iResult = PET_SUCCESS;
	if (lpRsp->addr != lpReq->addr || lpRsp->cmd != lpReq->cmd || lpRsp->len != lpReq->len) 
	{
		PETSetLastError(lpPET, 9999); /* То число что мы выделим из диапазона ошибок именно длельки */
		return PET_FAULT;
	}
	
	for (i = 0; i < lpReq->len; i++) 
	{
		if (lpRsp->data[i] != lpReq->data[i]) 
		{
			PETSetLastError(lpPET, 9998); /* То число что мы выделим из диапазона ошибок именно длельки */
			return PET_FAULT;
		}
	}

	return PET_SUCCESS;
}
//-------------------------------------------------------------------------------------
//
// TO DO: Все команды ПЭТ
//
//-------------------------------------------------------------------------------------
/**
 * Общий обработчик всех команд
 * iFlags - на будующее для особых случаев выполения команды
 */
int PETCmd(LPPET lpPET, const pet_frame_t *lpReq, pet_frame_t *lpRsp, int iFlags)
{
	/* Чтоб самим не накосячить :) */
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
