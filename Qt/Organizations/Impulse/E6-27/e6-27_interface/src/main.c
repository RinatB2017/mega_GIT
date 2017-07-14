//--------------------------------------------------------------------------------------------------------------
#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_fsmc.h>
#include <stm32f10x_flash.h>
#include <stm32f10x_i2c.h>
#include <stm32f10x_dma.h>

#include <stdlib.h>

#include "touchscreen/touchscreen.h"
#include "lcd/lcd.h"
#include "sdcard/sdcard.h"

#include "font/common.h"
#include "i2c/i2c.h"
//--------------------------------------------------------------------------------------------------------------
void print_sd_error(SD_Error err);
extern void LCD_Init(void);
//--------------------------------------------------------------------------------------------------------------
volatile uint32_t counter_1ms;

ErrorStatus HSEStartUpStatus;

RCC_ClocksTypeDef RCC_Clocks;

#if 0
#define LCDRegister 					(*((volatile u16*) 0x60000000))
#define LCDMemory 					(*((volatile u16*) 0x60020000))
#define CTR_WRITE_DATA	0x0022
#endif
//--------------------------------------------------------------------------------------------------------------
bool redraw_TEST = true;

#define MAX_BUTTONS 10
#define MAX_CAPTION_LEN 10
typedef struct {
  bool is_active;
  int ID;
  int x1;
  int y1;
  int x2;
  int y2;
  char caption[MAX_CAPTION_LEN];
  u16 color;
  int row;
  int col;
} BUTTON;

BUTTON buttons[MAX_BUTTONS];
int index = 0;
//--------------------------------------------------------------------------------------------------------------
void Delay(uint32_t ms)
{
  uint32_t curTicks = counter_1ms;
  while((counter_1ms - curTicks) < ms);
}
//--------------------------------------------------------------------------------------------------------------
void SysTick_Handler()
{
  counter_1ms++;
}
//--------------------------------------------------------------------------------------------------------------
void blink(void)
{
  GPIO_InitTypeDef PORT;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE);
  PORT.GPIO_Pin = (GPIO_Pin_6);
  PORT.GPIO_Mode = GPIO_Mode_Out_PP;
  PORT.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init( GPIOD , &PORT);

  while(1)
  {
      GPIOD->ODR |= (GPIO_Pin_6);
      Delay(1000);
      GPIOD->ODR &= ~(GPIO_Pin_6);
      Delay(1000);
  }
}
//--------------------------------------------------------------------------------------------------------------
bool check_press_button(int x1, int y1, int x2, int y2, 
                        int press_x, int press_y)
{
  if(press_x<x1) return false;
  if(press_x>x2) return false;
  if(press_y<y1) return false;
  if(press_y>y2) return false;
  return true;
}
//--------------------------------------------------------------------------------------------------------------
void my_memcpy(void *dest, const void *src, int n)
{
  for(int i=0; i<n; i++)
    ((unsigned char*)dest)[i] = ((unsigned char*)src)[i];
}
//--------------------------------------------------------------------------------------------------------------
u16 my_strlen(const char *s)
{
  u16 i = 0;
  while(1)
  {
    if(s[i] == 0)
      break;
    i++;
  } 
  return i;
}
//--------------------------------------------------------------------------------------------------------------
void init_menu_item(int x, int y, int w, int h, u16 color, 
                    char *caption, int col, int row,
                    int id)
{
  if(index > MAX_BUTTONS) return;
  
  buttons[index].is_active = true;
  buttons[index].ID = id;
  buttons[index].x1 = x;
  buttons[index].y1 = y;
  buttons[index].x2 = x+w;
  buttons[index].y2 = y+h;
  buttons[index].color = color;
  buttons[index].col = col;
  buttons[index].row = row;
  
  my_memcpy(buttons[index].caption, caption, my_strlen(caption));
  
  index++;
}
//--------------------------------------------------------------------------------------------------------------
void draw_menu_items(void)
{
  for(int n=0; n<MAX_BUTTONS; n++)
  {
    if(buttons[n].is_active)
    {
      LCD_DrawLine(buttons[n].x1, buttons[n].y1, buttons[n].x2, buttons[n].y1, buttons[n].color);
      LCD_ConSetPos(buttons[n].col, buttons[n].row); 
      LCD_Printf("%s", buttons[n].caption);
      LCD_DrawLine(buttons[n].x1, buttons[n].y2, buttons[n].x2, buttons[n].y2, buttons[n].color);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------
/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();
  
  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);
  
  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  
  if(HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    
    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
    
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);
    
    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);
    
    /* ADCCLK = PCLK2/4 */
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);
    
    
    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    
    /* Enable PLL */
    RCC_PLLCmd(ENABLE);
    
    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }
    
    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    
    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
    RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
  }
}
//--------------------------------------------------------------------------------------------------------------
/* reverse: переворачиваем строку s на месте */
void reverse(char s[])
{
  int i, j;
  char c;

  for (i = 0, j = my_strlen(s)-1; i<j; i++, j--) 
  {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}
//--------------------------------------------------------------------------------------------------------------
void itoa(int n, char s[])
{
  int i, sign;

  if ((sign = n) < 0)  /* записываем знак */
      n = -n;          /* делаем n положительным числом */
  i = 0;
  do {       			/* генерируем цифры в обратном порядке */
      s[i++] = n % 10 + '0';	/* берем следующую цифру */
  } while ((n /= 10) > 0);    /* удаляем */
  if (sign < 0)
      s[i++] = '-';
  s[i] = '\0';
  reverse(s);
}
//--------------------------------------------------------------------------------------------------------------
void test(int x, int y, int pressed)
{
  int w = 0;
  int h = 0;
  int aw = 0;
  int ah = 0;
  int lcd_x = 0;
  int lcd_y = 0;
  tFont font;
  
  if(redraw_TEST)
  {
    redraw_TEST = false;
    LCD_Clear(LCD_COLOR_GRAY);

    //LCDBUS_WriteReg(0x0004, 0xFFFF);
    
    font = Digits_24;
    LCD_SetCurrentBigFont(&font);
    LCD_GetParam(&font, '0', &w, &h);
    LCD_GetParam(&font, '.', &aw, &ah);
    LCD_SetColorBigChar(LCD_COLOR_GRAY, LCD_COLOR_GREEN);

    lcd_x = 20;
    lcd_y = 10;
    LCD_BigPrintXY(lcd_x, lcd_y);
    LCD_BigPrintf("000000");

    lcd_x = 40+(w*6);
    LCD_BigPrintXY(lcd_x, lcd_y);
    LCD_BigPrintf("000000");

    font = lDigits_48;
    LCD_SetCurrentBigFont(&font);
    LCD_GetParam(&font, '0', &w, &h);
    LCD_GetParam(&font, '.', &aw, &ah);
    LCD_SetColorBigChar(LCD_COLOR_GRAY, LCD_COLOR_GREEN);
    
    lcd_x = (LCD_WIDTH - (w*10+aw)) / 2;
    lcd_y = (LCD_HEIGHT - (2*h+10)) / 2;
    LCD_BigPrintXY(lcd_x, lcd_y);
    LCD_BigPrintf("1.012345678");
    
    font = lDigits_48;
    LCD_SetCurrentBigFont(&font);
    LCD_GetParam(&font, '0', &w, &h);
    LCD_GetParam(&font, '.', &aw, &ah);
    
    lcd_y += h;
    lcd_x = (LCD_WIDTH - (w*10+aw)) / 2;
    LCD_BigPrintXY(lcd_x, lcd_y);
    LCD_BigPrintf("1.012345678");

    font = iDigits_48;
    LCD_SetCurrentBigFont(&font);
    LCD_GetParam(&font, '0', &w, &h);
    LCD_GetParam(&font, '.', &aw, &ah);
    
    lcd_y += h;
    lcd_x = (LCD_WIDTH - (w*10+aw)) / 2;
    LCD_BigPrintXY(lcd_x, lcd_y);
    LCD_BigPrintf("1.012345678");
    
    //LCD_DrawGrid(10, LCD_COLOR_BLACK);
  }
}
//--------------------------------------------------------------------------------------------------------------
void i2c(int x, int y, int pressed)
{
  LCD_Clear(LCD_COLOR_GRAY);

  LCD_ConSetPos(2, 0); 
  LCD_Printf("I2C_Configuration\n");
  I2C_Configuration();
  unsigned char send_byte = 0x55;
  LCD_Printf("I2C_ByteWrite %d\n", send_byte);
  I2C_ByteWrite(I2C1, 1, send_byte);
  unsigned char temp = I2C_ByteRead(I2C1, 1);
  LCD_Printf("I2C_ByteRead %d", temp);
}
//--------------------------------------------------------------------------------------------------------------
void main_loop(void)
{
  ts_init();
  //gps_init();

  //int x = 0;
  //int y = 0;
  //int pressed = 0;
  
  LCD_Clear(LCD_COLOR_GRAY);
  
  //lcd_test();
  int code = 0;
  while(1)
  {
    //ts_poll(&x,&y,&pressed);
    switch(code)
    {
      case 0: lcd_test('0'); break;
      case 1: lcd_test('1'); break;
      case 2: lcd_test('2'); break;
      case 3: lcd_test('3'); break;
      case 4: lcd_test('4'); break;
      case 5: lcd_test('5'); break;
      case 6: lcd_test('6'); break;
      case 7: lcd_test('7'); break;
      case 8: lcd_test('8'); break;
      case 9: lcd_test('9'); break;
      default:
	break;
    }
      
    if(code<9) 
      code++;
    else 
      code = 0;
    //Delay(1000);
  }
}
//--------------------------------------------------------------------------------------------------------------
int main(void)
{
    /* Конфигурируем таймер SysTick на срабатывание 1000 раз в секунду */
    // RCC_GetClocksFreq(&RCC_Clocks);
    // SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);    
    int err = SysTick_Config(SystemCoreClock / 1000);
    
#if 0
    GPIO_InitTypeDef PORT;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF , ENABLE);
    PORT.GPIO_Pin = (GPIO_Pin_All);
    PORT.GPIO_Mode = GPIO_Mode_Out_PP;
    PORT.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init( GPIOF , &PORT);
    
    GPIOF->ODR = (GPIO_Pin_All);
    Delay(1000);
    GPIOF->ODR = (GPIO_Pin_0);
    Delay(1000);
    GPIOF->ODR = (GPIO_Pin_15);
    Delay(1000);
    while(1)
    {
      GPIOF->ODR = 0x5555;
      Delay(1000);
      GPIOF->ODR = 0xAAAA;
      Delay(1000);
    }
#endif
  
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
    //GPIOB->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5);

    RCC_Configuration();
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
			RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
			  RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF |
			    RCC_APB2Periph_GPIOG | RCC_APB2Periph_USART1 |
			      RCC_APB2Periph_ADC3, ENABLE);
    

    /* Enable the FSMC Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
    
    /* Initialize the LCD */
    LCD_Init();
    LCD_ConSetColor(LCD_COLOR_BLACK, LCD_COLOR_GREEN);
    if(err)
      LCD_Clear(LCD_COLOR_RED);
    else
      LCD_Clear(LCD_COLOR_BLACK);

    main_loop();
      
    //test_dma();
    while(1);
    //blink();

    return 0;
}
//--------------------------------------------------------------------------------------------------------------
