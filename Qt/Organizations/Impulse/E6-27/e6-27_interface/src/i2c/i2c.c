//*********************************************************************************
#include <stm32f10x_gpio.h>
#include <stm32f10x_i2c.h>
#include <stm32f10x_rcc.h>

#include "../lcd/lcd.h"
#include "i2c.h"

#define I2C_C1	I2C1	//interface number
//#define I2C_C2	I2C2	//interface number

//*********************************************************************************
//	@brief  I2C Configuration
//*********************************************************************************
void I2C_Configuration(void)
{
  I2C_InitTypeDef  I2C_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  //RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);//

  /* Configure I2C1 pins: PB6->SCL and PB7->SDA */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  I2C_DeInit(I2C_C1);
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_16_9;
  I2C_InitStructure.I2C_OwnAddress1 = 1;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = 100000;  /* 100kHz */

  I2C_Cmd(I2C_C1, ENABLE);
  I2C_Init(I2C_C1, &I2C_InitStructure);
  I2C_AcknowledgeConfig(I2C_C1, ENABLE);
  
#if 0  
  I2C_DeInit(I2C_C2);
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_16_9;
  I2C_InitStructure.I2C_OwnAddress1 = 2;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = 100000;  /* 100kHz */
  
  I2C_Cmd(I2C_C2, ENABLE);
  I2C_Init(I2C_C2, &I2C_InitStructure);
  I2C_AcknowledgeConfig(I2C_C2, ENABLE);
#endif

  // Enable interrupts from I2C2 module
  I2C_ITConfig(I2C1, I2C_IT_BUF, ENABLE);
}
//*********************************************************************************
void I2C_ByteWrite(I2C_TypeDef* I2Cx, uint16_t WriteAddr, uint8_t val)
{
  LCD_Printf("w0 ");
  I2C_GenerateSTART(I2Cx, ENABLE);
  LCD_Printf("w1 ");
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  LCD_Printf("w2 ");
  I2C_SendData(I2Cx, (uint8_t)(WriteAddr & 0x7F));
  LCD_Printf("w3 ");
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  LCD_Printf("w4 ");
  I2C_SendData(I2Cx, val);
  while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  LCD_Printf("w5 ");
  I2C_GenerateSTOP(I2Cx, ENABLE);
  LCD_Printf("w6\n");
}
//*********************************************************************************
uint8_t I2C_ByteRead(I2C_TypeDef* I2Cx, uint16_t ReadAddr)
{
  uint8_t tmp;

  LCD_Printf("r0 ");
  while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
  LCD_Printf("r1 ");
  I2C_GenerateSTART(I2Cx, ENABLE);
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  LCD_Printf("r2 ");
  tmp=I2C_ReceiveData(I2Cx);
  while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_RECEIVED));
  LCD_Printf("r3 ");
  I2C_AcknowledgeConfig(I2Cx, DISABLE);
  I2C_GenerateSTOP(I2Cx, ENABLE);
  LCD_Printf("r4\n");
  
  return tmp;
}
//*****************************************************************************
