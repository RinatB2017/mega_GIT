//---------------------------------------------------------------
#include "TFTLCD.h"
//---------------------------------------------------------------
#ifdef USE_DISPLAY
  #define LCD_CS A3
  #define LCD_CD A2
  #define LCD_WR A1
  #define LCD_RD A0 
  // optional
  #define LCD_RESET A4
  
  // Color definitions
  #define BLACK           0x0000
  #define BLUE            0x001F
  #define RED             0xF800
  #define GREEN           0x07E0
  #define CYAN            0x07FF
  #define MAGENTA         0xF81F
  #define YELLOW          0xFFE0 
  #define WHITE           0xFFFF
  
  TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
#endif
//---------------------------------------------------------------
#define MAX_X  16
#define MAX_Y  8
//---------------------------------------------------------------
#define CMD_0x01  0x01
//---------------------------------------------------------------
#ifdef USE_DISPLAY
  uint16_t  display_buf[MAX_X][MAX_Y];
#endif
//---------------------------------------------------------------
#define MAX_LEN_ASCII   1000
//650
#define MAX_LEN_MODBUS  (MAX_LEN_ASCII / 2)
//---------------------------------------------------------------
uint8_t buf_ascii[MAX_LEN_ASCII];
int len_ascii = 0;
//---------------------------------------------------------------
uint8_t buf_modbus[MAX_LEN_MODBUS];
int len_modbus = 0;
//---------------------------------------------------------------
int incomingByte = 0;
//---------------------------------------------------------------
#pragma pack (push, 1)
//--------------------------------------------------------------------------------
union LED
{
  uint32_t value;
  struct BODY
  {
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t alpha;
  } body;
};

typedef struct
{
    uint8_t  address;
    uint8_t  cmd;
    uint16_t count_data;
}
RGB_HEADER;

typedef struct
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
}
LED_COLOR;
//--------------------------------------------------------------------------------
union CMD_0x01_QUESTION
{
    struct BODY{
        RGB_HEADER header;
        uint8_t     brightness;
        LED_COLOR   data[MAX_X][MAX_Y];
        uint16_t    crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};

union CMD_0x01_ANSWER
{
    struct BODY{
        RGB_HEADER header;
        uint16_t crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
#pragma pack(pop)
//---------------------------------------------------------------
#ifdef USE_DISPLAY
int w = 0;
int h = 0;
int max_x = 0;
int max_y = 0;
int size_x = 0;
int size_y = 0;
#endif
//---------------------------------------------------------------
void serialEvent();
//---------------------------------------------------------------
void print_log(String text)
{
  //Serial.println(text);
}
//---------------------------------------------------------------
void PrintHex8(uint8_t data) // prints 8-bit data in hex with leading zeroes
{
  char tmp[16];
  sprintf(tmp, "%.2X",data);
  Serial1.print(tmp);
}
//---------------------------------------------------------------
void draw_led(int x, int y, int color)
{
#ifdef USE_DISPLAY
  tft.fillRect(y*size_y, x*size_x, size_y, size_x, color);
#endif

  LED led;
  led.value = color;

  Serial1.print(":");
  PrintHex8(x);
  PrintHex8(y);
  PrintHex8(led.body.R);
  PrintHex8(led.body.G);
  PrintHex8(led.body.B);
  Serial1.println("");
}
//---------------------------------------------------------------
unsigned char convert_ascii_to_value(char hi, char lo)
{
  char b_hi = 0;
  char b_lo = 0;
  
  //---
  switch(hi)
  {
    case '0':  b_hi = 0x0;  break;
    case '1':  b_hi = 0x1;  break;
    case '2':  b_hi = 0x2;  break;
    case '3':  b_hi = 0x3;  break;
    case '4':  b_hi = 0x4;  break;
    case '5':  b_hi = 0x5;  break;
    case '6':  b_hi = 0x6;  break;
    case '7':  b_hi = 0x7;  break;
    case '8':  b_hi = 0x8;  break;
    case '9':  b_hi = 0x9;  break;
    case 'A':  b_hi = 0xA;  break;
    case 'B':  b_hi = 0xB;  break;
    case 'C':  b_hi = 0xC;  break;
    case 'D':  b_hi = 0xD;  break;
    case 'E':  b_hi = 0xE;  break;
    case 'F':  b_hi = 0xF;  break;
    default:
      break;
  }
  //---
    switch(lo)
  {
    case '0':  b_lo = 0x0;  break;
    case '1':  b_lo = 0x1;  break;
    case '2':  b_lo = 0x2;  break;
    case '3':  b_lo = 0x3;  break;
    case '4':  b_lo = 0x4;  break;
    case '5':  b_lo = 0x5;  break;
    case '6':  b_lo = 0x6;  break;
    case '7':  b_lo = 0x7;  break;
    case '8':  b_lo = 0x8;  break;
    case '9':  b_lo = 0x9;  break;
    case 'A':  b_lo = 0xA;  break;
    case 'B':  b_lo = 0xB;  break;
    case 'C':  b_lo = 0xC;  break;
    case 'D':  b_lo = 0xD;  break;
    case 'E':  b_lo = 0xE;  break;
    case 'F':  b_lo = 0xF;  break;
    default:
      break;
  }
  //---
  return ((b_hi << 4) | b_lo);
}
//---------------------------------------------------------------
void func_0x01(void)
{
  print_log("func_0x01");
  if(len_modbus < sizeof(CMD_0x01_QUESTION))
  {
    print_log("bad len");
    return;
  }
  
  CMD_0x01_QUESTION *packet = (CMD_0x01_QUESTION *)buf_modbus;
  uint8_t addr = packet->body.header.address;
  uint8_t cmd = packet->body.header.cmd;
  uint8_t len = packet->body.header.count_data;
  
  for(int y=0; y<MAX_Y; y++)
  {
    for(int x=0; x<MAX_X; x++)
    {
      LED led;
      led.body.R = packet->body.data[x][y].R;
      led.body.G = packet->body.data[x][y].G;
      led.body.B = packet->body.data[x][y].B;
      led.body.alpha = 0;
#ifdef USE_DISPLAY
      display_buf[x][y] = led.value;
#endif      
      draw_led(x,y,led.value);
    }
  }
  
  CMD_0x01_ANSWER answer;
  Serial.print(":");
  for(int n=0; n<sizeof(answer); n++)
  {
    PrintHex8(answer.buf[n]);
  }
  Serial.println("");
}
//---------------------------------------------------------------
void analize()
{
  if(len_ascii % 2) 
  {
    //tft.fillScreen(BLUE);  //TODO
    return;
  }
  if(len_ascii < (sizeof(RGB_HEADER) * 2)) 
  {
    //tft.fillScreen(RED);  //TODO
    return;
  }
  
  //---
  len_modbus = 0;
  for(int n=0; n<len_ascii; n+=2)
  {
      if(len_modbus < MAX_LEN_MODBUS)
      {
        buf_modbus[len_modbus] = convert_ascii_to_value(buf_ascii[n],
                                                        buf_ascii[n+1]);
        len_modbus++;
      }
  }
  //---
  RGB_HEADER *header = (RGB_HEADER *)buf_modbus;
  uint8_t cmd = header->cmd;
  switch(cmd)
  {
    case CMD_0x01:
      //tft.fillScreen(GREEN);  //TODO
      func_0x01();
      break;
      
    default:
      break;
  }
}
//---------------------------------------------------------------
void serialEventRun() 
{
  if (Serial.available())
          serialEvent();
//  if (Serial1.available())
//          serialEvent1();
//  if (Serial2.available())
//          serialEvent2();
//  if (Serial3.available())
//          serialEvent2();
}
//---------------------------------------------------------------
void serialEvent()
{
  while (Serial.available()) 
  {
    // считываем байт
    incomingByte = Serial.read();
    switch(incomingByte)
    {
    case ':':
      len_ascii = 0;
      break;
  
    case '\r':
    case '\n':
      print_log("analize");
      analize();
      break;
  
    default:
      if(len_ascii < MAX_LEN_ASCII)
      {
        buf_ascii[len_ascii] = incomingByte;
        len_ascii++;
      }
      else
      {
        len_ascii = 0;
      }      
      break;  
    }
  }
}
//---------------------------------------------------------------
#ifdef USE_DISPLAY
void test(void)
{
  tft.fillScreen(BLACK);

  char flag = 0;
  for(int y=0; y<max_y; y++)
  {
    for(int x=0; x<max_x; x++)
    {
      flag = !flag;
      if(flag)
      {
        draw_led(x, y, WHITE);
      }
      else
      {
        draw_led(x, y, BLACK);
      }
    }
    flag = !flag;
  }
}
#endif
//---------------------------------------------------------------
#ifdef USE_DISPLAY
void work(void)
{
  for(int y=0; y<max_y; y++)
  {
    for(int x=0; x<max_x; x++)
    {
      draw_led(x, y, display_buf[x][y]);
    }
  }
}  
#endif
//---------------------------------------------------------------
void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);

#ifdef USE_DISPLAY
  w = tft.height();
  h = tft.width();

  max_x = MAX_X;
  max_y = MAX_Y;

  size_x = w / max_x;
  size_y = h / max_y;
  
  for(int y=0; y<MAX_Y; y++)
  {
    for(int x=0; x<MAX_X; x++)
    {
      display_buf[x][y] = 0;
    }
  }

  tft.initDisplay(); 
  tft.fillScreen(BLACK);
#endif
}
//---------------------------------------------------------------
void loop()
{
  //test();
  //work();
}
//---------------------------------------------------------------

