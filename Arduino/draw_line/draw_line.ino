
#include <FAB_LED.h>

const int num_strip = 5;        // количество полос
const int cnt_led_in_strip = 5; // количество светодиодов в полосе
const int max_cnt_led = (num_strip + 2) * cnt_led_in_strip; // нужен запас, чтобы было откуда выезжать и куда заезжать полосе
int len_line = 5;     // длина линии
const int max_len_line = 5; // макс. длина линии

int index_led = 0;

int color_index = 0;
uint8_t brightness_R = 0;
uint8_t brightness_G = 0;
uint8_t brightness_B = 255;

grb array_leds[max_cnt_led] = {};

// Declare the LED protocol and the port
ws2812b<D,7>  strip_7;
ws2812b<D,6>  strip_6;
ws2812b<D,5>  strip_5;
ws2812b<D,4>  strip_4;
ws2812b<D,3>  strip_3;

// The pixel array to display
grb  pixels[cnt_led_in_strip] = {};

int delay_ms = 100;
//-----------------------------------------------------------------------------
boolean stringComplete = false;  
int incomingByte = 0;
//-----------------------------------------------------------------------------
#define serial_WIFI  Serial
//-----------------------------------------------------------------------------
#define CMD_0x01  0x01
//-----------------------------------------------------------------------------
typedef struct HEADER
{
    uint8_t addr;
    uint8_t cmd;
    uint8_t len;
} HEADER_t;
//-----------------------------------------------------------------------------
#define MAX_LEN_ASCII   100
#define MAX_LEN_MODBUS  50
//-----------------------------------------------------------------------------
int buf_ascii[MAX_LEN_ASCII];
int len_ascii = 0;
//-----------------------------------------------------------------------------
uint8_t buf_modbus[MAX_LEN_MODBUS];
int len_modbus = 0;
//-----------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------
union F_01
{
    struct BODY
    {
        HEADER  header;
        struct data
        {
            uint16_t xxx;
        } data_t;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//-----------------------------------------------------------------------------
void func_0x01(void)
{
  /*
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
  */
}
//-----------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------
void work()
{
    analize();
}
//-----------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------
void clear_all()
{
  // Turn off the LEDs
  strip_3.clear(cnt_led_in_strip);
  strip_4.clear(cnt_led_in_strip);
  strip_5.clear(cnt_led_in_strip);
  strip_6.clear(cnt_led_in_strip);
  strip_7.clear(cnt_led_in_strip);
}
//-----------------------------------------------------------------------------
void setup()
{
  clear_all();
  
  serial_WIFI.begin(9600);
}
//-----------------------------------------------------------------------------
void prepare_line()
{
  for(int i=0; i<max_cnt_led; i++)
  {
    array_leds[i].r = 0;
    array_leds[i].g = 0;
    array_leds[i].b = 0;
  }    

  for(int n=0; n<len_line; n++)
  {
    array_leds[index_led+n].r = brightness_R;
    array_leds[index_led+n].g = brightness_G;
    array_leds[index_led+n].b = brightness_B;
  }
  
  if(index_led<(max_cnt_led - cnt_led_in_strip))
  {
    index_led++;
  }
  else
  {
    index_led = 0;
    switch(color_index)
    {
      case 0:
        brightness_R = 255;
        brightness_G = 0;
        brightness_B = 0;
        color_index = 1;
        break;
        
      case 1:
        brightness_R = 0;
        brightness_G = 255;
        brightness_B = 0;
        color_index = 2;
        break;
        
      case 2:
        brightness_R = 0;
        brightness_G = 0;
        brightness_B = 255;
        color_index = 0;
        break;     
        
     default:
        color_index = 0;
        break;
    }
  }
}
//-----------------------------------------------------------------------------
void draw_line()
{
  int index = 0;
  //---
  index = cnt_led_in_strip;
  for(int i = 0; i < cnt_led_in_strip; i++)
  {
    pixels[i].r = array_leds[index + i].r;
    pixels[i].g = array_leds[index + i].g;
    pixels[i].b = array_leds[index + i].b;
  }
  strip_3.sendPixels(cnt_led_in_strip, pixels);
  //---
  index += cnt_led_in_strip;
  for(int i = 0; i < cnt_led_in_strip; i++)
  {
    pixels[i].r = array_leds[index + i].r;
    pixels[i].g = array_leds[index + i].g;
    pixels[i].b = array_leds[index + i].b;
  }
  strip_4.sendPixels(cnt_led_in_strip, pixels);
  //---
  index += cnt_led_in_strip;
  for(int i = 0; i < cnt_led_in_strip; i++)
  {
    pixels[i].r = array_leds[index + i].r;
    pixels[i].g = array_leds[index + i].g;
    pixels[i].b = array_leds[index + i].b;
  }
  strip_5.sendPixels(cnt_led_in_strip, pixels);
  //---
  index += cnt_led_in_strip;
  for(int i = 0; i < cnt_led_in_strip; i++)
  {
    pixels[i].r = array_leds[index + i].r;
    pixels[i].g = array_leds[index + i].g;
    pixels[i].b = array_leds[index + i].b;
  }
  strip_6.sendPixels(cnt_led_in_strip, pixels);
  //---
  index += cnt_led_in_strip;
  for(int i = 0; i < cnt_led_in_strip; i++)
  {
    pixels[i].r = array_leds[index + i].r;
    pixels[i].g = array_leds[index + i].g;
    pixels[i].b = array_leds[index + i].b;
  }
  strip_7.sendPixels(cnt_led_in_strip, pixels);
  //---
}
//-----------------------------------------------------------------------------
void sleep()
{
  delay(delay_ms);
}
//-----------------------------------------------------------------------------
void new_loop()
{
  //prepare_line();
  //draw_line();
  //sleep();
  
  if(stringComplete)
  {
    work();
  }
}
//-----------------------------------------------------------------------------
void loop()
{
  new_loop();
}
//-----------------------------------------------------------------------------

