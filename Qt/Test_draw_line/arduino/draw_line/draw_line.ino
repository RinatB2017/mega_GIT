#include <EEPROM.h>
#include <FAB_LED.h>

const int num_strip = 5;        // количество полос
const int cnt_led_in_strip = 5; // количество светодиодов в полосе
const int max_cnt_led = (num_strip + 2) * cnt_led_in_strip; // нужен запас, чтобы было откуда выезжать и куда заезжать полосе
const int max_len_line = 5; // макс. длина линии

int index_led = 0;

int color_index = 0;

grb array_leds[max_cnt_led] = {};

// Declare the LED protocol and the port
ws2812b<D,7>  strip_7;
ws2812b<D,6>  strip_6;
ws2812b<D,5>  strip_5;
ws2812b<D,4>  strip_4;
ws2812b<D,3>  strip_3;

// The pixel array to display
grb  pixels[cnt_led_in_strip] = {};
//-----------------------------------------------------------------------------
boolean stringComplete = false;  
int incomingByte = 0;
//-----------------------------------------------------------------------------
#define serial_WIFI  Serial
//-----------------------------------------------------------------------------
#define CMD_0x01  0x01
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
typedef struct HEADER
{
    uint8_t addr;
    uint8_t cmd;
    uint8_t len;
} HEADER_t;

union F_01
{
    struct BODY
    {
        HEADER  header;
        struct DATA
        {
          uint16_t len_line;    // длина линии
          uint16_t delay_ms;    // задержка
          uint8_t brightness_R;
          uint8_t brightness_G;
          uint8_t brightness_B;
        } data_t __attribute__((packed));
    } body_t __attribute__((packed));
    unsigned char buf[sizeof(BODY)];
};

union MEMORY
{
  struct M_BODY
  {
    uint16_t len_line;     // длина линии
    uint16_t delay_ms;     // задержка
    uint8_t brightness_R;
    uint8_t brightness_G;
    uint8_t brightness_B;
  } m_body_t __attribute__((packed));
  unsigned char buf[sizeof(M_BODY)];
};

MEMORY mem;
//-----------------------------------------------------------------------------
void logging(String text)
{
  //serial_WIFI.println(text);  
}
//-----------------------------------------------------------------------------
void logging(int num)
{
  //serial_WIFI.println(num);  
}
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
void func_0x01(void)
{
//  logging("sizeof(F_01)");
//  logging(sizeof(F_01));
  if(len_modbus != sizeof(F_01))
  {
    logging("Error size");
    return;
  }
  
  F_01 *packet = (F_01 *)buf_modbus;

  uint16_t t_len_line     = packet->body_t.data_t.len_line;
  uint16_t t_delay_ms     = packet->body_t.data_t.delay_ms;
  uint8_t  t_brightness_R = packet->body_t.data_t.brightness_R;
  uint8_t  t_brightness_G = packet->body_t.data_t.brightness_G;
  uint8_t  t_brightness_B = packet->body_t.data_t.brightness_B;

  if(t_len_line > max_len_line) t_len_line = max_len_line;
  if(t_delay_ms < 10) t_delay_ms = 10;

  mem.m_body_t.len_line = t_len_line;
  mem.m_body_t.delay_ms = t_delay_ms;
  mem.m_body_t.brightness_R = t_brightness_R;
  mem.m_body_t.brightness_G = t_brightness_G;
  mem.m_body_t.brightness_B = t_brightness_B;

  save_EEPROM();
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
      //:0001070300E803FF0000
      //:0001070500C800FF0000
      logging("CMD_0x01");
      func_0x01();
      break;
      
    default:
      break;
  }
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
  
    case '\r':
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
void prepare_line()
{
  for(int i=0; i<max_cnt_led; i++)
  {
    array_leds[i].r = 0;
    array_leds[i].g = 0;
    array_leds[i].b = 0;
  }    

  for(int n=0; n<mem.m_body_t.len_line; n++)
  {
    array_leds[index_led+n].r = mem.m_body_t.brightness_R;
    array_leds[index_led+n].g = mem.m_body_t.brightness_G;
    array_leds[index_led+n].b = mem.m_body_t.brightness_B;
  }
  
  if(index_led<(max_cnt_led - cnt_led_in_strip))
  {
    index_led++;
  }
  else
  {
    index_led = 0;
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
  delay(mem.m_body_t.delay_ms);
}
//-----------------------------------------------------------------------------
void load_EEPROM()
{
  for(int n=0; n<sizeof(MEMORY); n++)
  {
    mem.buf[n] = EEPROM.read(n);
  }

  if(mem.m_body_t.len_line < 1) mem.m_body_t.len_line = 1;
  if(mem.m_body_t.len_line < max_len_line) mem.m_body_t.len_line = max_len_line;

  if(mem.m_body_t.delay_ms < 10) mem.m_body_t.delay_ms = 10;

  logging(mem.m_body_t.len_line);
  logging(mem.m_body_t.delay_ms);
  logging(mem.m_body_t.brightness_R);
  logging(mem.m_body_t.brightness_G);
  logging(mem.m_body_t.brightness_B);
}
//-----------------------------------------------------------------------------
void save_EEPROM()
{
  for(int n=0; n<sizeof(MEMORY); n++)
  {
    EEPROM.write(n, mem.buf[n]);
  }
}
//-----------------------------------------------------------------------------
void setup()
{
  serial_WIFI.begin(115200);

  mem.m_body_t.len_line = 5;
  mem.m_body_t.delay_ms = 100;
  mem.m_body_t.brightness_R = 0;
  mem.m_body_t.brightness_G = 0;
  mem.m_body_t.brightness_B = 0;

  load_EEPROM();
  
  clear_all();
}
//-----------------------------------------------------------------------------
void loop()
{
  if(stringComplete)
  {
    analize();
  }
  prepare_line();
  draw_line();
  sleep();
}
//-----------------------------------------------------------------------------

