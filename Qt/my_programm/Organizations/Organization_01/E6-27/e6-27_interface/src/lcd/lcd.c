//--------------------------------------------------------------------------------------------------------------
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

#include <stm32f10x_gpio.h>
#include <stm32f10x.h>

#include "../font/common.h"
#include "../fonts.h"
#include "lcd.h"
//--------------------------------------------------------------------------------------------------------------
extern void Delay(uint32_t ms);
//--------------------------------------------------------------------------------------------------------------
/* AHB clock period in nanoseconds */
#define T_HCK	14

#define DISP_ORIENTATION	1

static u32 DISPLAY_CODE;   // SPP

#define LCDBUS_RSLOW_ADDR	((vu16 *)0x60000000)
#define LCDBUS_RSHIGH_ADDR	((vu16 *)(0x60000000 | (1<<(19+1))))

#define SWAP_INT(A,B)	do {int __x; __x = (A); (A) = (B); (B) = __x;} while(0)

static u16 LCD_ConBgColor = 0;
static u16 LCD_ConFgColor = 0;

static u16 LCD_BigCharBgColor = 0;
static u16 LCD_BigCharFgColor = 0;

static u16 LCD_BigCharX = 0;
static u16 LCD_BigCharY = 0;

static unsigned int LCD_ConRow;
static unsigned int LCD_ConCol;

char printf_buf[256];

tFont *font;

//--------------------------------------------------------------------------------------------------------------
/* Initialize the FSMC bus used for driving the LCD */
static void FSMC_LCDBUS_Init(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  pw,pr;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOG | RCC_APB2Periph_GPIOE |
                         RCC_APB2Periph_GPIOF, ENABLE);
  
  /*-- GPIO Configuration ------------------------------------------------------*/
  /* SRAM Data lines configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |
    GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
    GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
      GPIO_Pin_15;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  //TODO - get rid of address lines!!!
  /* SRAM Address lines configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
    GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 |
      GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
    GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  /* NOE and NWE configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  /* NE1 configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  
  /* NBL0, NBL1 configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  /*-- FSMC Configuration ------------------------------------------------------*/
  pr.FSMC_AddressSetupTime = (5/T_HCK+1);
  pr.FSMC_AddressHoldTime = (5/T_HCK+1);
  pr.FSMC_DataSetupTime = (100/T_HCK+1);
  pr.FSMC_BusTurnAroundDuration = 0;
  pr.FSMC_CLKDivision = 0;
  pr.FSMC_DataLatency = 0;
  pr.FSMC_AccessMode = FSMC_AccessMode_A;
  
  pw.FSMC_AddressSetupTime = (5/T_HCK+1);
  pw.FSMC_AddressHoldTime = (5/T_HCK+1);
  pw.FSMC_DataSetupTime = ((20+15)/T_HCK+1);
  pw.FSMC_BusTurnAroundDuration = 0;
  pw.FSMC_CLKDivision = 0;
  pw.FSMC_DataLatency = 0;
  pw.FSMC_AccessMode = FSMC_AccessMode_A;
  
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM2;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Enable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &pr;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &pw;
  
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
  
  /* Enable FSMC Bank1_SRAM Bank */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM2, ENABLE);
}
//--------------------------------------------------------------------------------------------------------------
static void lcdled_set(int val)
{
  if(!val) 
  {
    GPIO_SetBits(GPIOD, GPIO_Pin_13);
  } 
  else 
  {
    GPIO_ResetBits(GPIOD, GPIO_Pin_13);
  }
}
//--------------------------------------------------------------------------------------------------------------
static void lcdreset_set(int val)
{
  if(val) 
  {
    GPIO_SetBits(GPIOE, GPIO_Pin_2);
  } 
  else 
  {
    GPIO_ResetBits(GPIOE, GPIO_Pin_2);
  }
}
//--------------------------------------------------------------------------------------------------------------
static void lcdreset_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  lcdreset_set(1);
}
//--------------------------------------------------------------------------------------------------------------
static void lcdled_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  lcdled_set(1);
}
//--------------------------------------------------------------------------------------------------------------
void LCDBUS_WriteReg(u16 regn, u16 val)
{
  *LCDBUS_RSLOW_ADDR = regn;
  *LCDBUS_RSHIGH_ADDR = val;
}
//--------------------------------------------------------------------------------------------------------------
static u16 LCDBUS_ReadReg(u16 regn)
{
  u32 val;
  
  *LCDBUS_RSLOW_ADDR = regn;
  val = *LCDBUS_RSHIGH_ADDR;
  
  return val;
}
//--------------------------------------------------------------------------------------------------------------
void LCD_FillMem(unsigned int addr, const u16 data, unsigned int nitems)
{
  LCDBUS_WriteReg(0x20, addr & 0xff);		/* low addr */
  LCDBUS_WriteReg(0x21, (addr>>8) & 0x1ff);	/* high addr */
  *LCDBUS_RSLOW_ADDR = 0x22;
  while(nitems--) 
  {
    *LCDBUS_RSHIGH_ADDR = data;
  }
}
//--------------------------------------------------------------------------------------------------------------
void LCD_DrawPicture(int x, int y, int w, int h, const uint16_t *data)
{
  LCDBUS_WriteReg(0x0020, y);		//starting Horizontal GRAM Address
  LCDBUS_WriteReg(0x0021, x);		//starting Vertical GRAM Address
  
  LCDBUS_WriteReg(0x0050, y);		//Horizontal GRAM Start Position
  LCDBUS_WriteReg(0x0051, y+h);		//Horizontal GRAM end Position
  LCDBUS_WriteReg(0x0052, x);		//Vertical GRAM Start Position
  LCDBUS_WriteReg(0x0053, x+w);		//Vertical GRAM end Position

  unsigned int addr = y*LCD_HEIGHT+x;
  //unsigned int addr = y*0x100+x;
  unsigned int nitems = w*h;
  LCDBUS_WriteReg(0x20, addr & 0xff);		/* low addr */
  LCDBUS_WriteReg(0x21, (addr>>8) & 0x1ff);	/* high addr */
  *LCDBUS_RSLOW_ADDR = 0x22;
  int index = 0;
  while(nitems--) 
  {
    *LCDBUS_RSHIGH_ADDR = data[index];
    index++;
  }
  
  LCDBUS_WriteReg(0x0020, 0x0000);		//starting Horizontal GRAM Address
  LCDBUS_WriteReg(0x0021, 0x0000);		//starting Vertical GRAM Address
  
  LCDBUS_WriteReg(0x0050, 0x0000);		//Horizontal GRAM Start Position
  LCDBUS_WriteReg(0x0051, LCD_HEIGHT-1);	//Horizontal GRAM end Position     //239
  LCDBUS_WriteReg(0x0052, 0x0000);		//Vertical GRAM Start Position
  LCDBUS_WriteReg(0x0053, LCD_WIDTH-1);		//Vertical GRAM end Position       //319
}
//--------------------------------------------------------------------------------------------------------------
void LCD_Clear(u16 bgcolor)
{
  LCD_FillMem(0, bgcolor, LCD_HEIGHT*LCD_WIDTH);
}
//--------------------------------------------------------------------------------------------------------------
inline void LCD_WriteRAM(u16 color)
{
  *LCDBUS_RSHIGH_ADDR = color;
}
//--------------------------------------------------------------------------------------------------------------
inline void LCD_SetCursor(unsigned int x, unsigned int y)
{
  unsigned int addr;
  
  addr = y*0x100 + x;
  
  LCDBUS_WriteReg(0x20, addr & 0xff);		/* low addr */
  LCDBUS_WriteReg(0x21, (addr>>8) & 0x1ff);	/* high addr */
  *LCDBUS_RSLOW_ADDR = 0x22;			/* data reg in IR */
}
//--------------------------------------------------------------------------------------------------------------
// x=[0-LCD_WIDTH] , y=[0-LCD_HEIGHT]
inline void LCD_SetPixel(unsigned int x, unsigned int y, u16 color)
{
  LCD_SetCursor(x,y);
  LCD_WriteRAM(color);
}
//--------------------------------------------------------------------------------------------------------------
void LCD_DrawLine_norm(int x0, int y0, int x1, int y1, u16 color, bool swap)
{
  const int mul = (1<<12);
  int a, b, y;
  
  a = ((y1 - y0) * mul) / (x1 - x0);
  b = ((y0 * x1 - y1 * x0) * mul) / (x1 - x0);

  while(x0 <= x1) 
  {
    y = a * x0 + b;
    y = (y + mul/2) / mul;
    if(!swap) 
    {
      LCD_SetPixel(x0, y, color);
    } 
    else 
    {
      LCD_SetPixel(y, x0, color);
    }
    x0++;
  }
}
//--------------------------------------------------------------------------------------------------------------
unsigned int my_abs(int x)
{
  if(x>0) return x;
  return -x;
}
//--------------------------------------------------------------------------------------------------------------
void LCD_ConSetColor(u16 bgcolor, u16 fgcolor)
{
  LCD_ConBgColor = bgcolor;
  LCD_ConFgColor = fgcolor;
}
//--------------------------------------------------------------------------------------------------------------
void LCD_ConSetPos(unsigned int row, unsigned int col)
{
  LCD_ConRow = row;
  LCD_ConCol = col;
}
//--------------------------------------------------------------------------------------------------------------
void LCD_DrawRect(int x, int y, int w, int h, u16 color)
{
  for(int i=0; i<h; i++, y++) 
  {
    u32 addr = y*0x100 + x;
    LCD_FillMem(addr, color, w);
  }
}
//--------------------------------------------------------------------------------------------------------------
void LCD_DrawLine(int x0, int y0, int x1, int y1, u16 color)
{
  if(x0 == x1) 
  {
    LCD_SetPixel(x0, y0, color);
    while(y0 != y1) 
    {
      if(y0<y1)
        y0++;
      else
        y0--;
      LCD_SetPixel(x0, y0, color);
    }
    return;
  }
  if(y0 == y1) 
  {
    LCD_SetPixel(x0, y0, color);
    while(x0 != x1) 
    {
      if(x0<x1)
        x0++;
      else
        x0--;
      LCD_SetPixel(x0, y0, color);
    }
    return;
  }
  if(my_abs(x1-x0) > my_abs(y1-y0)) 
  {
    if(x0 > x1) 
    {
      SWAP_INT(x0, x1);
      SWAP_INT(y0, y1);
    }
    LCD_DrawLine_norm(x0,y0,x1,y1, color, false);
  } 
  else 
  {
    if(y0 > y1) 
    {
      SWAP_INT(x0, x1);
      SWAP_INT(y0, y1);
    }
    LCD_DrawLine_norm(y0,x0,y1,x1, color, true);
  }
}
//--------------------------------------------------------------------------------------------------------------
void LCD_DrawChar(unsigned int row, unsigned int col, char c, u16 bgcolor, u16 fgcolor)
{
  unsigned int x,y;
  int i,j;
  uc16 *font_char;
  
  x = 16 * col;
  if(x > (LCD_WIDTH-16))
    x = LCD_WIDTH-16;
  y = 24 * row;
  if(y > (LCD_HEIGHT-24))
    y = LCD_HEIGHT-24;
  
  c -= 32;
  if(c<=0) c = 0;
  if(c>94) c = 94;
  
  font_char = &ASCII_Table[c*24];
  LCD_SetCursor(x,y);
  for(i=0; i<24; i++) 
  {
    for(j=0; j<16; ++j) 
    {
      if(*font_char & (1u<<j))
        LCD_WriteRAM(fgcolor);
      else
        LCD_WriteRAM(bgcolor);
    }
    font_char++;
    LCD_SetCursor(x,y+i);
  }
}
//--------------------------------------------------------------------------------------------------------------
void LCD_Printf(const char *fmt, ...)
{
  char *p;
  
  va_list lst;
  va_start(lst, fmt);
  vsprintf(printf_buf, fmt, lst);
  va_end(lst);

  p = printf_buf;
  while(*p) 
  {
    if(*p == '\n') 
    {
      while(LCD_ConCol < 15) 
      {
        LCD_DrawChar(LCD_ConRow, LCD_ConCol++, ' ', LCD_ConBgColor, LCD_ConFgColor);
      }
      
      LCD_ConRow++;
      LCD_ConCol = 0;
    } 
    else 
    {
      if(LCD_ConCol >= 15) 
      {
        LCD_ConRow++;
        LCD_ConCol = 0;
      }
      LCD_DrawChar(LCD_ConRow, LCD_ConCol++, *p, LCD_ConBgColor, LCD_ConFgColor);
    }
    p++;
  }
}
//--------------------------------------------------------------------------------------------------------------
void LCD_SetTestPattern(void)
{
  const unsigned int chunk = (LCD_HEIGHT*LCD_WIDTH)/10;
  unsigned int i;
  
  LCD_SetCursor(0,0);
  for(i=0; i<chunk; ++i)
    LCD_WriteRAM(LCD_COLOR_WHITE);
  for(i=0; i<chunk; ++i)
    LCD_WriteRAM(LCD_COLOR_BLACK);
  for(i=0; i<chunk; ++i)
    LCD_WriteRAM(LCD_COLOR_GRAY);
  
  for(i=0; i<chunk; ++i)
    LCD_WriteRAM(LCD_COLOR_BLUE);
  for(i=0; i<chunk; ++i)
    LCD_WriteRAM(LCD_COLOR_GREEN);
  for(i=0; i<chunk; ++i)
    LCD_WriteRAM(LCD_COLOR_RED);
  
  for(i=0; i<chunk; ++i)
    LCD_WriteRAM(LCD_COLOR_YELLOW);
  for(i=0; i<chunk; ++i)
    LCD_WriteRAM(LCD_COLOR_MAGENTA);
  for(i=0; i<chunk; ++i)
    LCD_WriteRAM(LCD_COLOR_CYAN);
  
  for(i=0; i<chunk; ++i)
    LCD_WriteRAM(LCD_COLOR_WHITE);
}
//--------------------------------------------------------------------------------------------------------------
void LCD_Init(void)
{ 
  // SPP +
  FSMC_LCDBUS_Init();
  lcdreset_init();
  lcdled_init();
  
  lcdled_set(1);
  
  lcdreset_set(1);
  Delay(100);
  lcdreset_set(0);
  Delay(100);
  lcdreset_set(1);
  Delay(100);
  Delay(50); /* delay 50 ms */
  
  
  DISPLAY_CODE = LCDBUS_ReadReg(0);
  LCDBUS_WriteReg(0x0000, 0x0001); //start Int. osc
  Delay(50);
  LCDBUS_WriteReg(0x0001, 0x0100); //Set SS bit (shift direction of outputs is from S720 to S1)
  LCDBUS_WriteReg(0x0002, 0x0700); //select  the line inversion
#if (DISP_ORIENTATION == 1)
  LCDBUS_WriteReg(0x0003, 0x1030); //Entry mode(Horizontal : increment,Vertical : increment, AM=0) //horizontal
#else
  LCDBUS_WriteReg(0x0003, 0x1038); //Entry mode(Horizontal : increment,Vertical : increment, AM=1) //vertical
#endif
  LCDBUS_WriteReg(0x0004, 0x0000); //Resize control(No resizing) 0000
  LCDBUS_WriteReg(0x0008, 0x0202); //front and back porch 2 lines
  LCDBUS_WriteReg(0x0009, 0x0000); //select normal scan
  LCDBUS_WriteReg(0x000A, 0x0000); //display control 4
  LCDBUS_WriteReg(0x000C, 0x0000); //system interface(2 transfer /pixel), internal sys clock,  	
  LCDBUS_WriteReg(0x000D, 0x0000); //Frame marker position
  LCDBUS_WriteReg(0x000F, 0x0000); //selects clk, enable and sync signal polarity,
  LCDBUS_WriteReg(0x0010, 0x0000); //	
  LCDBUS_WriteReg(0x0011, 0x0000); //power control 2 reference voltages = 1:1,
  LCDBUS_WriteReg(0x0012, 0x0000); //power control 3 VRH
  LCDBUS_WriteReg(0x0013, 0x0000); //power control 4 VCOM amplitude
  Delay(50);
  LCDBUS_WriteReg(0x0010, 0x17B0); //power control 1 BT,AP
  LCDBUS_WriteReg(0x0011, 0x0137); //power control 2 DC,VC
  Delay(50);
  LCDBUS_WriteReg(0x0012, 0x0139); //power control 3 VRH
  Delay(50);
  LCDBUS_WriteReg(0x0013, 0x1d00); //power control 4 vcom amplitude
  LCDBUS_WriteReg(0x0029, 0x0011); //power control 7 VCOMH
  Delay(50);
  LCDBUS_WriteReg(0x0030, 0x0007);
  LCDBUS_WriteReg(0x0031, 0x0403);
  LCDBUS_WriteReg(0x0032, 0x0404);
  LCDBUS_WriteReg(0x0035, 0x0002);
  LCDBUS_WriteReg(0x0036, 0x0707);
  LCDBUS_WriteReg(0x0037, 0x0606);
  LCDBUS_WriteReg(0x0038, 0x0106);
  LCDBUS_WriteReg(0x0039, 0x0007);
  LCDBUS_WriteReg(0x003c, 0x0700);
  LCDBUS_WriteReg(0x003d, 0x0707);
  LCDBUS_WriteReg(0x0020, 0x0000); //starting Horizontal GRAM Address
  LCDBUS_WriteReg(0x0021, 0x0000); //starting Vertical GRAM Address
  
  LCDBUS_WriteReg(0x0050, 0x0000); //Horizontal GRAM Start Position
  LCDBUS_WriteReg(0x0051, 0x00EF); //Horizontal GRAM end Position     //239
  LCDBUS_WriteReg(0x0052, 0x0000); //Vertical GRAM Start Position
  LCDBUS_WriteReg(0x0053, 0x013F); //Vertical GRAM end Position       //319
  
  // SPP +
  
  switch (DISPLAY_CODE)
  {
  case 0x9320:
    LCDBUS_WriteReg(0x0060, 0x2700); //starts scanning from G1, and 320 drive lines
    break;
  case 0x9325:
    LCDBUS_WriteReg(0x0060, 0xA700); //starts scanning from G1, and 320 drive lines
    break;
  }
  
  // SPP -
  
  LCDBUS_WriteReg(0x0061, 0x0001); //fixed base display
  LCDBUS_WriteReg(0x006a, 0x0000); //no scroll
  LCDBUS_WriteReg(0x0090, 0x0010); //set Clocks/Line =16, Internal Operation Clock Frequency=fosc/1,
  LCDBUS_WriteReg(0x0092, 0x0000); //set gate output non-overlap period=0
  LCDBUS_WriteReg(0x0093, 0x0003); //set Source Output Position=3
  LCDBUS_WriteReg(0x0095, 0x0110); //RGB interface(Clocks per line period=16 clocks)
  LCDBUS_WriteReg(0x0097, 0x0110); //set Gate Non-overlap Period 0 locksc
  LCDBUS_WriteReg(0x0098, 0x0110); //
  LCDBUS_WriteReg(0x0007, 0x0173); //display On  
}
//--------------------------------------------------------------------------------------------------------------
void LCD_SetColorBigChar(u16 bgcolor, u16 fgcolor)
{
  LCD_BigCharBgColor = bgcolor;
  LCD_BigCharFgColor = fgcolor;
}
//--------------------------------------------------------------------------------------------------------------
void LCD_GetParam(tFont *font, int code, int *w, int *h)
{
  for(int s=0; s<font->length; s++)
  {
    tChar c = font->chars[s];
    if(c.code == code)
    {
      const tImage *image = c.image;
      *w = image->width;
      *h = image->height;
      return;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------
void LCD_DrawBigChar(int pos_x, int pos_y, long int code, int *w, int *h)
{
  for(int s=0; s<font->length; s++)
  {
    tChar c = font->chars[s];
    if(c.code == code)
    {
      const tImage *image = c.image;
      const uint16_t *data  = image->data;
      uint16_t width  = image->width;
      uint16_t height = image->height;
      *w = width;
      *h = height;
#if 0
      LCD_DrawPicture(pos_x, pos_y, width, height, data);
#else      
      int x, y;
      int index = 0;
      for(y=height; y>0; y--)
      {
	for(x=0; x<width; x++)
	{
	  u16 plot = data[index];
	  if(plot == LCD_COLOR_WHITE) 
	  {
	    plot = LCD_BigCharBgColor;
	  }
	  else
	  {
	    if(plot == LCD_COLOR_BLACK) 
	    {
	      plot = LCD_BigCharFgColor;
	    }
	  }
	  LCD_SetPixel(pos_y+y, pos_x+x, plot);
	  index++;
	}
      }
#endif      
      return;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------
void LCD_BigPrintXY(int pos_x, int pos_y)
{
  LCD_BigCharX = pos_x;
  LCD_BigCharY = pos_y;
}
//--------------------------------------------------------------------------------------------------------------
void LCD_SetCurrentBigFont(tFont *new_font)
{
  font = new_font;
}
//--------------------------------------------------------------------------------------------------------------
void LCD_BigPrintf(const char *fmt, ...)
{
  char *p;
  
  va_list lst;
  va_start(lst, fmt);
  vsprintf(printf_buf, fmt, lst);
  va_end(lst);

  int x = LCD_BigCharX;
  int y = LCD_BigCharY;
  int w = 0;
  int h = 0;
  p = printf_buf;
  while(*p) 
  {
    if(*p == '\n') 
    {
      x=LCD_BigCharX;
      y+=h;
    } 
    else 
    {
      LCD_DrawBigChar(x, y, *p, &w, &h);
      x+=w;
    }
    p++;
  }
}
//--------------------------------------------------------------------------------------------------------------
void LCD_DrawGrid(u16 step, u16 color)
{
  for(int y=0; y<319; y+=step)
  {
    LCD_DrawLine(0, y, 239, y, color);
  }
  for(int x=0; x<239; x+=step)
  {
    LCD_DrawLine(x, 0, x, 319, color);
  }
}
//--------------------------------------------------------------------------------------------------------------
#define CTR_VERT_START                  0x0052
#define CTR_VERT_END                    0x0053
#define CTR_HORZ_START                  0x0050
#define CTR_HORZ_END                    0x0051
#define CTR_HORZ_ADDRESS                0x0020
#define CTR_VERT_ADDRESS                0x0021
//--------------------------------------------------------------------------------------------------------------
void LCDSetBounds(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
  LCDBUS_WriteReg(CTR_VERT_START, left);
  LCDBUS_WriteReg(CTR_VERT_END,   right);
  LCDBUS_WriteReg(CTR_HORZ_START, top);
  LCDBUS_WriteReg(CTR_HORZ_END,   bottom);

  LCDBUS_WriteReg(CTR_HORZ_ADDRESS, top);
  LCDBUS_WriteReg(CTR_VERT_ADDRESS, left);
}
//--------------------------------------------------------------------------------------------------------------
void draw_rectangle_color(int w, int h, u16 color)
{
  *LCDBUS_RSLOW_ADDR = 0x22;
  int nitems = w*h;
  while(nitems--) 
  {
    *LCDBUS_RSHIGH_ADDR = color;
  }
}
//--------------------------------------------------------------------------------------------------------------
void draw_chessboard(void)
{
  bool flag = false;
  int w=20;
  int h=20;
  for(int y=0; y<240; y+=h)
  {
    for(int x=0; x<320; x+=w)
    {
      LCDSetBounds(x, y, x+w, y+h);
      draw_rectangle_color(w, h, flag ? LCD_COLOR_WHITE : LCD_COLOR_BLACK);
      flag = !flag;
    }
    flag = !flag;
  }
}
//--------------------------------------------------------------------------------------------------------------
void draw_picture(int w, int h, const uint16_t data[])
{
  *LCDBUS_RSLOW_ADDR = 0x22;
  int nitems = w*h;
  int index = 0;
  uint16_t buf[nitems];

  for(int n=0; n<nitems; n++)
  {
    if(data[n] == 0xffff) 
      buf[n] = LCD_COLOR_GRAY;
    else
      buf[n] = LCD_COLOR_GREEN; //data[n];
  }

  while(nitems--) 
  {
    *LCDBUS_RSHIGH_ADDR = buf[index];
    index++;
  }
}
//--------------------------------------------------------------------------------------------------------------
void lcd_test(int code)
{
  int x = 50;
  int y = 50;
  //int w = 24;
  //int h = 58;

#if 0 
  tFont f;
  f = Big_Font;
  LCD_SetCurrentBigFont(&f);
  
  for(int s=0; s<font->length; s++)
  {
    tChar c = font->chars[s];
    if(c.code == code)
    {
      tChar c = font->chars[s];
      const tImage *image = c.image;
      const uint16_t *data  = image->data;
      uint16_t width  = image->width;
      uint16_t height = image->height;
  
      LCDSetBounds(x, y, x+width, y+height-1);
      draw_picture(width, height, data);
      return;
    }
  }
#endif  
  
#if 1  
  tFont f;
  f = Big_Font;
  LCD_SetCurrentBigFont(&f);
  
  for(int s=0; s<font->length; s++)
  {
    tChar c = font->chars[s];
    if(c.code == code)
    {
      tChar c = font->chars[s];
      const tImage *image = c.image;
      const uint16_t *data  = image->data;
      uint16_t width  = image->width;
      uint16_t height = image->height;
  
      for(x=0; x<13*width; x+=width)
      {
	for(y=0; y<5*height; y+=height)
	{
	  LCDSetBounds(x, y, x+width, y+height-1);
	  draw_picture(width, height, data);
	}
      }
    }
  }
#endif

#if 0
  for(x=0; x<320; x+=h)
  {
    for(y=0; y<240; y+=w)
    {
      LCDSetBounds(x, y, x+h, y+w-1);
      draw_picture(w, h, image_data_lDigits_48_0x36);
    }
  }
#endif  
}
//--------------------------------------------------------------------------------------------------------------
