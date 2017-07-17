//---------------------------------------------------------------
/*
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif 
*/
//---------------------------------------------------------------
#include <Wire.h>
#include <PCA9685.h>
//---------------------------------------------------------------
#define serial_WIFI  Serial
//---------------------------------------------------------------
PCA9685 ledDriver_0;
PCA9685 ledDriver_1; 
PCA9685 ledDriver_2;
//---------------------------------------------------------------
byte address_0;
byte address_1;
byte address_2;
//---------------------------------------------------------------
#pragma pack (push, 1)
//---------------------------------------------------------------
#define CMD_0x01  0x01
//---------------------------------------------------------------
#define MAX_SCREEN_X  6
#define MAX_SCREEN_Y  6
//---------------------------------------------------------------
enum TYPE
{
    HOT  = 0xF0,
    COLD = 0x0F
};
//---------------------------------------------------------------
typedef struct HEADER
{
    uint8_t addr;
    uint8_t cmd;
    uint8_t len;
} HEADER_t;
//---------------------------------------------------------------
//---------------------------------------------------------------
union F_01
{
    struct BODY
    {
        HEADER  header;
        struct data
        {
            uint8_t leds[MAX_SCREEN_X][MAX_SCREEN_Y];
        } data_t;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//---------------------------------------------------------------
#pragma pack(pop)
//---------------------------------------------------------------
unsigned char x_buf[MAX_SCREEN_X][MAX_SCREEN_Y] = { \
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
    0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11,
    0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,
    0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23
    };
//---------------------------------------------------------------
int incomingByte = 0;
//---------------------------------------------------------------
boolean stringComplete = false;  
//---------------------------------------------------------------
int ledPin = 13;
//---------------------------------------------------------------
#define MAX_LEN_ASCII   100
#define MAX_LEN_MODBUS  50
//---------------------------------------------------------------
int buf_ascii[MAX_LEN_ASCII];
int len_ascii = 0;
//---------------------------------------------------------------
uint8_t buf_modbus[MAX_LEN_MODBUS];
int len_modbus = 0;
//---------------------------------------------------------------
int x = 0;
int y = 0;
int val = 0;
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
int normalize(int value)
{
  return abs(value - 100);
}
//---------------------------------------------------------------
void set(int x, int y, int val)
{
  unsigned char temp = x_buf[x][y];
  char device = (temp >> 4) & 0x0F;
  char addr = (temp & 0x0F);
  
  if(val < 0)    val = 0;
  if(val > 100)  val = 100;
  
  val = normalize(val);
  
  switch(device)
  {
    case 0: ledDriver_0.setLEDDimmed(addr, val); break;
    case 1: ledDriver_1.setLEDDimmed(addr, val); break;
    case 2: ledDriver_2.setLEDDimmed(addr, val); break;
      
    default:
      break;
  }
}
//---------------------------------------------------------------
void clear_all()
{
  for(int i=0; i<16; i++)  
  {
    ledDriver_0.setLEDDimmed(i, normalize(0));
    ledDriver_1.setLEDDimmed(i, normalize(0));
    ledDriver_2.setLEDDimmed(i, normalize(0));
  }
}
//---------------------------------------------------------------
void func_0x01(void)
{
  if(len_modbus < sizeof(F_01)) return;
  
  F_01 *packet = (F_01 *)buf_modbus;
  uint8_t addr = packet->body.header.addr;
  uint8_t cmd = packet->body.header.cmd;
  uint8_t len = packet->body.header.len;
  
  for(int y=0; y<MAX_SCREEN_Y; y++)
  {
    for(int x=0; x<MAX_SCREEN_X; x++)
    {
      set(x, y, packet->body.data_t.leds[x][y]);
    }
  }
}
//---------------------------------------------------------------
void analize()
{
  if(len_ascii % 2) return;
  if(len_ascii < (sizeof(HEADER) * 2)) return;
  
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
  HEADER *header = (HEADER *)buf_modbus;
  uint8_t cmd = header->cmd;
  switch(cmd)
  {
    case CMD_0x01:
      func_0x01();
      break;
      
    default:
      break;
  }
}
//---------------------------------------------------------------
void work()
{
    analize();
}
//---------------------------------------------------------------
void serialEvent()
{
  while (serial_WIFI.available()) 
  {
    // считываем байт
    incomingByte = serial_WIFI.read();
    switch(incomingByte)
    {
    case ':':
      stringComplete = false;
      len_ascii = 0;
      break;
  
    case '\n':
      stringComplete = true;
      break;
  
    default:
      if(len_ascii < MAX_LEN_ASCII)
      {
        buf_ascii[len_ascii] = incomingByte;
        len_ascii++;
      }
      else
      {
        stringComplete = false;
        len_ascii = 0;
      }      
      break;  
    }
  }
}
//---------------------------------------------------------------
void test()
{
  for(int level=0; level<=100; level++)  
  {
    for(int i=0; i<16; i++)  
    {
      ledDriver_0.setLEDDimmed(i,level);
      ledDriver_1.setLEDDimmed(i,level);
      ledDriver_2.setLEDDimmed(i,level);
    }
    delay(1);     
  }
  for(int level=100; level>=0; level--)
  {
    for(int i=0; i<16; i++)  
    {
      ledDriver_0.setLEDDimmed(i,level);
      ledDriver_1.setLEDDimmed(i,level);
      ledDriver_2.setLEDDimmed(i,level);
    }
    delay(1);     
  }
}
//---------------------------------------------------------------
void setup()
{
  //serial_WIFI.begin(115200);
  //serial_WIFI.begin(57600);
  //serial_WIFI.begin(38400);
  //serial_WIFI.begin(19200);
  serial_WIFI.begin(9600);
  //serial_WIFI.begin(7500);
  
  //pinMode(ledPin, OUTPUT);
  
  address_0 = 0x40;
  address_1 = 0x41;
  address_2 = 0x42;
  
  Wire.begin();

  ledDriver_0.begin(address_0); 
  ledDriver_1.begin(address_1);
  ledDriver_2.begin(address_2);

  ledDriver_0.init();
  ledDriver_1.init();
  ledDriver_2.init();
  
  clear_all();
}
//---------------------------------------------------------------
void loop()
{
  if(stringComplete)
  {
    work();
  }
  //test();
  //Serial.println(F_CPU);
}
//---------------------------------------------------------------

