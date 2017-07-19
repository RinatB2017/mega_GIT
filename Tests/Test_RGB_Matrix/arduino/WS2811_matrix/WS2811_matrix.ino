//---------------------------------------------------------------
#include "FastLED.h"
//---------------------------------------------------------------
#define LED_PIN_1 4
#define LED_PIN_2 5
#define LED_PIN_3 6
#define LED_PIN_4 7
#define LED_PIN_5 8
#define LED_PIN_6 9
#define LED_PIN_7 10
#define LED_PIN_8 11
//---------------------------------------------------------------
#define NUM_LEDS_PER_STRIP 6
#define NUM_STRIPS         8
//---------------------------------------------------------------
uint8_t gBrightness = 128;
//---------------------------------------------------------------
CLEDController *controllers[NUM_STRIPS];
//---------------------------------------------------------------
CRGB matrix[NUM_STRIPS][NUM_LEDS_PER_STRIP];
//---------------------------------------------------------------
char data[NUM_LEDS_PER_STRIP][NUM_STRIPS] = { 0 };
//---------------------------------------------------------------
#define MAX_LEN 128

uint8_t input_buf[MAX_LEN];
uint8_t output_buf[MAX_LEN];
int input_buf_index;
//---------------------------------------------------------------
//---------------------------------------------------------------
typedef struct
{
  uint8_t address;
  uint8_t cmd;
  uint8_t count_data;
} 
RGB_HEADER;
//---------------------------------------------------------------
union CMD_0x01_QUESTION
{
  struct {
    RGB_HEADER header;
    uint8_t data[NUM_LEDS_PER_STRIP][NUM_STRIPS];
    uint16_t crc16;
  } 
  body;
  uint8_t buf[sizeof(body)];
};

union CMD_0x01_ANSWER
{
  struct {
    RGB_HEADER header;
    uint16_t crc16;
  } 
  body;
  uint8_t buf[sizeof(body)];
};
//---------------------------------------------------------------
#define RGB_CMD_0x01  0x01
//---------------------------------------------------------------
//---------------------------------------------------------------
void draw_data(void)
{
  for(int y=0; y<NUM_STRIPS; y++)
  {
    for(int x=0; x<NUM_LEDS_PER_STRIP; x++)
    {
      switch(data[x][y])
      {
      case 'r':
      case 'R':
        matrix[y][x] = CRGB::Red;
        break;

      case 'g':
      case 'G':
        matrix[y][x] = CRGB::Green;
        break;

      case 'b':
      case 'B':
        matrix[y][x] = CRGB::Blue;
        break;

      default:
        matrix[y][x] = 0;
        break;
      }
    }
  }
}
//---------------------------------------------------------------
int modbus_receive(void)
{
  uint8_t i;
  uint8_t s;

  input_buf_index = 0;
  while(true) 
  {
    if(!Serial2.available()) 
    {
      i = 0;
      while (!Serial2.available()) 
      {
        delay(1);
        if (++i == 10) 
        {
          /* Too late, bye */
          return -1;
        }
      }
    }
    s = Serial2.read();
    if(s >= 0)
    {
      if(input_buf_index < MAX_LEN)
      {
        input_buf[input_buf_index] = s;
        input_buf_index++;
      }
    }
  }
  return input_buf_index;
}
//---------------------------------------------------------------
void clear_input_buf(void)
{
  for(int n=0; n<MAX_LEN; n++)
  {
    input_buf[n] = 0;
  }
  input_buf_index = 0;
}
//---------------------------------------------------------------
void cmd_0x01(void)
{
  CMD_0x01_QUESTION *question = (CMD_0x01_QUESTION *)input_buf;
  for(int y=0; y<NUM_STRIPS; y++)
  {
    for(int x=0; x<NUM_LEDS_PER_STRIP; x++)
    {
      data[x][y] = question->body.data[x][y];
    }
  }
  
  //---
#if 0  
  CMD_0x01_ANSWER answer;

  answer.body.header.address = 0;
  answer.body.header.cmd = RGB_CMD_0x01;
  answer.body.header.count_data = 0;
  answer.body.crc16 = powersupply_crc16((unsigned char *)&answer.buf, sizeof(CMD_0x01_ANSWER) - 2);

  for(unsigned int n=0; n<sizeof(CMD_0x01_ANSWER); n++)
  {
    Serial2.write(answer.buf[n]);
  }
#endif  
}
//---------------------------------------------------------------
uint16_t powersupply_crc16(uint8_t *pcBlock, uint16_t len)
{
  uint16_t crc = 0xFFFF;

  for (int pos=0; pos<len; pos++)
  {
    crc ^= (uint16_t)pcBlock[pos];

    for (int i=8; i!=0; i--)
    {
      if ((crc & 0x0001) != 0)
      {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else
        crc >>= 1;
    }
  }
#ifdef USE_HTONS
  return htons(crc);
#else
  return crc;
#endif
}
//---------------------------------------------------------------
void check_packet(void)
{
  if(input_buf_index < sizeof(RGB_HEADER)) 
  {
    return;
  }

  RGB_HEADER *header = (RGB_HEADER *)input_buf;
  unsigned char cmd = header->cmd;

  switch(cmd)
  {
  case RGB_CMD_0x01:  
    cmd_0x01();  
    break;    

  default:
    break;
  }
}
//---------------------------------------------------------------
void draw(void)
{
  draw_data();
  controllers[0]->show(matrix[0], NUM_LEDS_PER_STRIP, gBrightness);
  controllers[1]->show(matrix[1], NUM_LEDS_PER_STRIP, gBrightness);
  controllers[2]->show(matrix[2], NUM_LEDS_PER_STRIP, gBrightness);
  controllers[3]->show(matrix[3], NUM_LEDS_PER_STRIP, gBrightness);
  controllers[4]->show(matrix[4], NUM_LEDS_PER_STRIP, gBrightness);
  controllers[5]->show(matrix[5], NUM_LEDS_PER_STRIP, gBrightness);
  controllers[6]->show(matrix[6], NUM_LEDS_PER_STRIP, gBrightness);
  controllers[7]->show(matrix[7], NUM_LEDS_PER_STRIP, gBrightness);
}
//---------------------------------------------------------------
void modbus(void)
{
  if(Serial2.available()) 
  {
    clear_input_buf();
    modbus_receive();
    check_packet();
  }
}
//---------------------------------------------------------------
void setup()
{
  Serial2.begin(9600);

  controllers[0] = &FastLED.addLeds<WS2812, LED_PIN_1, GRB>(matrix[0], NUM_LEDS_PER_STRIP);
  controllers[1] = &FastLED.addLeds<WS2812, LED_PIN_2, GRB>(matrix[1], NUM_LEDS_PER_STRIP);
  controllers[2] = &FastLED.addLeds<WS2812, LED_PIN_3, GRB>(matrix[2], NUM_LEDS_PER_STRIP);
  controllers[3] = &FastLED.addLeds<WS2812, LED_PIN_4, GRB>(matrix[3], NUM_LEDS_PER_STRIP);
  controllers[4] = &FastLED.addLeds<WS2812, LED_PIN_5, GRB>(matrix[4], NUM_LEDS_PER_STRIP);
  controllers[5] = &FastLED.addLeds<WS2812, LED_PIN_6, GRB>(matrix[5], NUM_LEDS_PER_STRIP);
  controllers[6] = &FastLED.addLeds<WS2812, LED_PIN_7, GRB>(matrix[6], NUM_LEDS_PER_STRIP);
  controllers[7] = &FastLED.addLeds<WS2812, LED_PIN_8, GRB>(matrix[7], NUM_LEDS_PER_STRIP);
  
  gBrightness = 25;
}
//---------------------------------------------------------------
void loop()
{
  modbus();
  draw();
}
//---------------------------------------------------------------


