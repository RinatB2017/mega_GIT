//***************************************************************************//**
//EEPROM LIB v 1.0
//******************************************************************************/
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_rcc.h"

#include "i2c.h"

//#define I2C_FLASH_PAGESIZE	32	//not used yet
#define EEPROM_HW_ADDRESS	0xA0	/* E0 = E1 = E2 = 0 */
#define I2C_EE			I2C1	//interface number

void Delay_ms(uint32_t ms);

/***************************************************************************//**
 *  @brief  I2C Configuration
 ******************************************************************************/
void I2C_Configuration(void)
{
  I2C_InitTypeDef  I2C_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO , ENABLE);//

  /* Configure I2C1 pins: PB6->SCL and PB7->SDA */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  I2C_DeInit(I2C_EE);
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_16_9;
  I2C_InitStructure.I2C_OwnAddress1 = 1;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = 100000;  /* 100kHz */

  I2C_Cmd(I2C_EE, ENABLE);
  I2C_Init(I2C_EE, &I2C_InitStructure);
  I2C_AcknowledgeConfig(I2C_EE, ENABLE);
}
//*********************************************************************************
void I2C_EE_ByteWrite(uint8_t val, uint16_t WriteAddr)
{
  /* Send START condition */
  I2C_GenerateSTART(I2C_EE, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C_EE, EEPROM_HW_ADDRESS, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  /* Send the EEPROM's internal address to write to : MSB of the address first */
  I2C_SendData(I2C_EE, (uint8_t)((WriteAddr & 0xFF00) >> 8));

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send the EEPROM's internal address to write to : LSB of the address */
  I2C_SendData(I2C_EE, (uint8_t)(WriteAddr & 0x00FF));

  /* Test on EV8 and clear it */
  while(! I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  I2C_SendData(I2C_EE, val);

  /* Test on EV8 and clear it */
  while (!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send STOP condition */
  I2C_GenerateSTOP(I2C_EE, ENABLE);

  //delay between write and read...not less 4ms
  Delay_ms(5);
}
//*********************************************************************************
uint8_t I2C_EE_ByteRead( uint16_t ReadAddr)
{
  uint8_t tmp;

  /* While the bus is busy */
  while(I2C_GetFlagStatus(I2C_EE, I2C_FLAG_BUSY));

  /* Send START condition */
  I2C_GenerateSTART(I2C_EE, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C_EE, EEPROM_HW_ADDRESS, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));


  /* Send the EEPROM's internal address to read from: MSB of the address first */
  I2C_SendData(I2C_EE, (uint8_t)((ReadAddr & 0xFF00) >> 8));

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send the EEPROM's internal address to read from: LSB of the address */
  I2C_SendData(I2C_EE, (uint8_t)(ReadAddr & 0x00FF));

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));


  /* Send STRAT condition a second time */
  I2C_GenerateSTART(I2C_EE, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send EEPROM address for read */
  I2C_Send7bitAddress(I2C_EE, EEPROM_HW_ADDRESS, I2C_Direction_Receiver);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C_EE,I2C_EVENT_MASTER_BYTE_RECEIVED));//I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

  tmp=I2C_ReceiveData(I2C_EE);


  I2C_AcknowledgeConfig(I2C_EE, DISABLE);

  /* Send STOP Condition */
  I2C_GenerateSTOP(I2C_EE, ENABLE);

  return tmp;
}
//*********************************************************************************
void I2C_ByteWrite(uint8_t val, uint16_t WriteAddr)
{
  /* Send START condition */
  I2C_GenerateSTART(I2C_EE, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send the EEPROM's internal address to write to : MSB of the address first */
  I2C_SendData(I2C_EE, (uint8_t)((WriteAddr & 0xFF00) >> 8));

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send STOP condition */
  I2C_GenerateSTOP(I2C_EE, ENABLE);

  //delay between write and read...not less 4ms
  Delay_ms(5);
}
//*********************************************************************************
uint8_t I2C_ByteRead( uint16_t ReadAddr)
{
  uint8_t tmp;

  /* While the bus is busy */
  while(I2C_GetFlagStatus(I2C_EE, I2C_FLAG_BUSY));

  /* Send START condition */
  I2C_GenerateSTART(I2C_EE, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_MODE_SELECT));


  tmp=I2C_ReceiveData(I2C_EE);

  I2C_AcknowledgeConfig(I2C_EE, DISABLE);

  /* Send STOP Condition */
  I2C_GenerateSTOP(I2C_EE, ENABLE);

  return tmp;
}
//*******************************************************************************
void Delay_ms(uint32_t ms)
{
  volatile uint32_t nCount;
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq (&RCC_Clocks);

  nCount=(RCC_Clocks.HCLK_Frequency/10000)*ms;
  for (; nCount!=0; nCount--);
}
//*****************************************************************************
