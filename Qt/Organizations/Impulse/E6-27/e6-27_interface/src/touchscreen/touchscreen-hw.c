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

#include <stm32f10x_gpio.h>
#include <stm32f10x_adc.h>

#include "touchscreen-hw.h"

#define TS_PIN_XR	GPIO_Pin_3
#define TS_PIN_YU	GPIO_Pin_1
#define TS_PIN_XL	GPIO_Pin_2
#define TS_PIN_YD	GPIO_Pin_0
#define TS_PORT		GPIOC

#define TS_ADCCH_XR	ADC_Channel_13
#define TS_ADCCH_YU	ADC_Channel_11
#define TS_ADCCH_XL	ADC_Channel_12
#define TS_ADCCH_YD	ADC_Channel_10
#define TS_ADC		ADC3


static void delay(volatile int i)
{
  for(;i;--i);
}


void tshw_init(void)
{
  ADC_InitTypeDef ADC_InitStructure;

  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(TS_ADC, &ADC_InitStructure);

  tshw_prepare_wait();
  delay(100);
}

static void GPIO_SetAnalog(u16 pin)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(TS_PORT, &GPIO_InitStructure);
}


static void GPIO_SetOutput(u16 pin, int val)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(TS_PORT, &GPIO_InitStructure);

  if(val)
  {
	  GPIO_SetBits(TS_PORT, pin);
  } else 
  {
	  GPIO_ResetBits(TS_PORT, pin);
  }
}

static void GPIO_SetInputPulledUp(u16 pin)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(TS_PORT, &GPIO_InitStructure);
}

static int GPIO_GetInputState(u16 pin)
{
  return GPIO_ReadInputDataBit(TS_PORT, pin);
}


static u16 ADC_Measure(u16 ch)
{
  u16 val;

  /* ADC regular channel14 configuration */ 
  ADC_RegularChannelConfig(TS_ADC, ch, 1, ADC_SampleTime_55Cycles5);

  /* Enable ADC */
  ADC_Cmd(TS_ADC, ENABLE);
  delay(100);

#if 1
  /* Enable ADC reset calibaration register */   
  ADC_ResetCalibration(TS_ADC);
  /* Check the end of ADC reset calibration register */
  while(ADC_GetResetCalibrationStatus(TS_ADC));

  /* Start ADC calibaration */
  ADC_StartCalibration(TS_ADC);
  /* Check the end of ADC calibration */
  while(ADC_GetCalibrationStatus(TS_ADC));
#endif     
  /* Start ADC Software Conversion */ 
  ADC_Cmd(TS_ADC, ENABLE);

  while(!ADC_GetFlagStatus(TS_ADC,ADC_FLAG_EOC));

  val = ADC_GetConversionValue(TS_ADC);

  ADC_Cmd(TS_ADC, DISABLE);

  return val;
}

void tshw_get_raw(u16 *adc_x, u16 *adc_y)
{
	u32 val1, val2;
	
	/*
	 * YD=0 YU=1, measure XL and XR
	 * YD=1 YU=0, measure XL and XR
	 * The average of the previous four samples is the X value.
	 *
	 * XL=0 XR=1, measure YD and YU
	 * XL=1 XR=0, measure YD and YU
	 * The average of the previous four samples is the Y value.
	 *
	 * A total of 8 ADC measurements are needed!
	 */
	
	GPIO_SetAnalog(TS_PIN_XL | TS_PIN_XR);
	GPIO_SetOutput(TS_PIN_YD, 0);
	GPIO_SetOutput(TS_PIN_YU, 1);
	delay(100);
	val1 = (ADC_Measure(TS_ADCCH_XL) + ADC_Measure(TS_ADCCH_XR))/2;
	
	GPIO_SetOutput(TS_PIN_YD, 1);
	GPIO_SetOutput(TS_PIN_YU, 0);
	delay(100);
	val2 = (ADC_Measure(TS_ADCCH_XL) + ADC_Measure(TS_ADCCH_XR))/2;
	
	*adc_y = (val1+((1<<12)-val2))/4;
	
	GPIO_SetAnalog(TS_PIN_YD | TS_PIN_YU);
	GPIO_SetOutput(TS_PIN_XL, 0);
	GPIO_SetOutput(TS_PIN_XR, 1);
	delay(100);
	val1 = (ADC_Measure(TS_ADCCH_YD) + ADC_Measure(TS_ADCCH_YU))/2;

	GPIO_SetOutput(TS_PIN_XL, 1);
	GPIO_SetOutput(TS_PIN_XR, 0);
	delay(100);
	val2 = (ADC_Measure(TS_ADCCH_YD) + ADC_Measure(TS_ADCCH_YU))/2;
	
	*adc_x = (val1+((1<<12)-val2))/4;
}

void tshw_prepare_wait(void)
{
	/* 
	 * Drive Y low and enable a pull-up on X. When touchscreen is
	 * pressed X will be pulled low by Y and an interrupt will be
	 * generated
	 */
	GPIO_SetOutput(TS_PIN_YD, 0);
	GPIO_SetOutput(TS_PIN_YU, 0);
	GPIO_SetInputPulledUp(TS_PIN_XL | TS_PIN_XR);
}

void tshw_poll(u16 *adc_x, u16 *adc_y, int *pressed)
{
	if(GPIO_GetInputState(TS_PIN_XL) == 0) 
	{
		*pressed = 1;
		tshw_get_raw(adc_x,adc_y);
		tshw_prepare_wait();
		delay(100);
	}
	else 
	{
		*pressed = 0;
	}
}
