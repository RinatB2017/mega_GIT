#include "lcd_lib_2.h"
#include "port_macros.h"

/*макросы для работы с пинами и портом
при желании их здесь можно переопределить*/

#define LCD_ClearPin(x)             _PM_ClearPin(x)
#define LCD_SetPin(x)               _PM_SetPin(x)
#define LCD_DirPort(port, value)    _PM_DirPort(port, value) 
#define LCD_DirPin(pin, value)      _PM_DirPin(pin, value) 
#define LCD_WritePort(port, value)  _PM_WritePort(port, value) 
#define LCD_ReadPort(port, value)   _PM_ReadPort(port, value)

/*________________________________________________________________*/
          
#define LCD_COM_ENTRY_MODE_SET     (1<<2)|(LCD_DEC_INC_DDRAM<<1)|(LCD_SHIFT_RIGHT_LEFT<<0)
#define LCD_COM_DISPLAY_CONTR      (1<<3)|(LCD_DISPLAY_OFF_ON<<2)|(LCD_CURSOR_OFF_ON<<1)|(LCD_CURSOR_BLINK_OFF_ON<<0)
#define LCD_COM_FUNCTION_SET       (1<<5)|(LCD_BUS_4_8_BIT<<4)|(LCD_ONE_TWO_LINE<<3)|(LCD_FONT58_FONT511<<2)
#define LCD_COM_INIT_1              0x30
#define LCD_DELAY_STROB             2
#define LCD_DELAY_WAIT              40
#define LCD_FL_BF                   7

/*_________________________________________________________________*/

void LCD_WriteComInit(uint8_t data)
{
  delay_us(LCD_DELAY_WAIT);
  LCD_ClearPin(LCD_RS);  
  
#if (LCD_BUS_4_8_BIT == 0)
  data &= 0xf0;
#endif
  
  LCD_WritePort(LCD_PORT, data);	
  LCD_SetPin(LCD_EN);
  delay_us(LCD_DELAY_STROB);
  LCD_ClearPin(LCD_EN);
}


/*общая функция*/
INLINE static void LCD_CommonFunc(uint8_t data)
{
#if (LCD_BUS_4_8_BIT == 0) 
  
  uint8_t tmp; 
  tmp = (data & 0xf0);
  LCD_WritePort(LCD_PORT, tmp);
  LCD_SetPin(LCD_EN);
  delay_us(LCD_DELAY_STROB);
  LCD_ClearPin(LCD_EN);

  data = __swap_nibbles(data); 
  tmp = (data & 0xf0);
    
  LCD_WritePort(LCD_PORT, tmp);
  LCD_SetPin(LCD_EN);
  delay_us(LCD_DELAY_STROB);
  LCD_ClearPin(LCD_EN);
  
#else 
  
  LCD_WritePort(LCD_PORT, data);
  LCD_SetPin(LCD_EN);
  delay_us(LCD_DELAY_STROB);
  LCD_ClearPin(LCD_EN);
  
#endif
}

/*функция ожидания готовности lcd*/
INLINE static void LCD_Wait(void)
{
#if (LCD_CHECK_FL_BF == 1)
 #if (LCD_BUS_4_8_BIT == 0)
  
  uint8_t data, tmp;
  data = 0;
  LCD_DirPort(LCD_PORT, 0x00);
  LCD_WritePort(LCD_PORT, 0xff);
  LCD_SetPin(LCD_RW);
  LCD_ClearPin(LCD_RS);
  do{
    LCD_SetPin(LCD_EN);
    delay_us(LCD_DELAY_STROB);
    LCD_ReadPort(LCD_PORT, data);
    LCD_ClearPin(LCD_EN);  
   
    LCD_SetPin(LCD_EN);
    delay_us(LCD_DELAY_STROB);
    LCD_ReadPort(LCD_PORT, tmp);
    LCD_ClearPin(LCD_EN);  
    
  } while((data & (1<<LCD_FL_BF))!= 0);
  LCD_ClearPin(LCD_RW);
  LCD_DirPort(LCD_PORT, 0xff);
  
  #else
  
  uint8_t data;
  LCD_DirPort(LCD_PORT, 0x00);
  LCD_WritePort(LCD_PORT, 0xff);
  LCD_SetPin(LCD_RW);
  LCD_ClearPin(LCD_RS);
  do{
     LCD_SetPin(LCD_EN);
     delay_us(LCD_DELAY_STROB);
     LCD_ReadPort(LCD_PORT, data);
     LCD_ClearPin(LCD_EN);
  } while((data & (1<<LCD_FL_BF))!= 0);
  LCD_ClearPin(LCD_RW);
  LCD_DirPort(LCD_PORT, 0xff);
   
  #endif    
#else
  delay_us(LCD_DELAY_WAIT);
#endif  
}

/*функция записи команды*/
void LCD_WriteCom(uint8_t data)
{
  LCD_Wait();
  LCD_ClearPin(LCD_RS);	
  LCD_CommonFunc(data);
}

/*функция записи данных*/
void LCD_WriteData(char data)
{
  LCD_Wait();
  LCD_SetPin(LCD_RS);	    
  LCD_CommonFunc(data);
}

/*функция инициализации*/
void LCD_Init(void)
{
  LCD_DirPort(LCD_PORT, 0xff);
  LCD_DirPin(LCD_RS, _OUT);
  LCD_DirPin(LCD_RW, _OUT);  
  LCD_DirPin(LCD_EN, _OUT);
  
  delay_ms(40);
  
  LCD_WriteComInit(LCD_COM_INIT_1); 
  delay_ms(10);
  LCD_WriteComInit(LCD_COM_INIT_1);
  delay_ms(2);
  LCD_WriteComInit(LCD_COM_INIT_1);
  
#if (LCD_BUS_4_8_BIT == 0) 
  LCD_WriteComInit(LCD_COM_FUNCTION_SET);
#endif

  LCD_WriteCom(LCD_COM_FUNCTION_SET);
  LCD_WriteCom(LCD_COM_DISPLAY_CONTR);  
  LCD_WriteCom(LCD_CLEAR_DISPLAY);  
  delay_ms(2);
  LCD_WriteCom(LCD_COM_ENTRY_MODE_SET); 
  
}

/*функция вывда строки из ОЗУ*/
void LCD_SendStr(char *str)
{
  uint8_t data;
  while (*str){
    data =  *str++;
    LCD_WriteData(data);
  }
}


#ifdef __GNUC__

/*функция вывода строки из флэш памяти*/
void LCD_SendStrFl(char *str)
{
  char data;			
  while (*str){
    data = pgm_read_byte(str);
    str++;
    LCD_WriteData(data);
  }
}

void LCD_SetUserChar(uint8_t const *sym, uint8_t adr)
{
   uint8_t data;	
   uint8_t i;

   LCD_WriteCom((1<<0x06)|((adr&0x07)<<0x03));
   
   i = 0;
   while (i<8){
      data = pgm_read_byte(sym);
      sym++;
      LCD_WriteData(data);
      i++;
   }    
}

//для IARa и CodeVision
#else

/*функция вывода строки из флэш памяти*/
void LCD_SendStrFl(char __flash *str)
{
  char data;			
  while (*str){
    data = *str++;
    LCD_WriteData(data);
  }
}

void LCD_SetUserChar(uint8_t __flash *sym, uint8_t adr)
{
   uint8_t data;	
   uint8_t i;

   LCD_WriteCom((1<<0x06)|((adr&0x07)<<0x03));
   
   i = 0;
   while (i<8){
      data = *sym++;
      LCD_WriteData(data);
      i++;
   }    
}

#endif
