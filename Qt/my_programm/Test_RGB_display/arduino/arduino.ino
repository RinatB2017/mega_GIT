//---------------------------------------------------------------
#include "FastLED.h"
//---------------------------------------------------------------
#define BAUDRATE    115200
#define work_serial Serial

#define SCREEN_WIDTH  32
#define SCREEN_HEIGTH 16
//---------------------------------------------------------------
#define LEDS_PER_STRIP  (SCREEN_WIDTH * SCREEN_HEIGTH)
#define LED_PIN 9
//---------------------------------------------------------------
CRGB line_leds[LEDS_PER_STRIP];
//---------------------------------------------------------------
CLEDController *controllers;
//---------------------------------------------------------------
#pragma pack (push, 1)

struct LED
{
  uint8_t color_R;
  uint8_t color_G;
  uint8_t color_B;
} led;

union PACKET
{
  struct {
    uint8_t brightness;
    LED leds[SCREEN_WIDTH][SCREEN_HEIGTH];
  } body;
  uint8_t buf[sizeof(body)];
};

#pragma pack(pop)
//---------------------------------------------------------------
#define MAX_ASCII_BUF (sizeof(PACKET) * 2 + 2)
uint8_t ascii_buf[MAX_ASCII_BUF];
int index_ascii_buf = 0;

#define MAX_MODBUS_BUF (MAX_ASCII_BUF / 2)
uint8_t modbus_buf[MAX_MODBUS_BUF];
int index_modbus_buf = 0;

int imcomingByte = 0;

int i_brightness = 0;

int index = 0;
//---------------------------------------------------------------
void init_leds(void)
{
  controllers = &FastLED.addLeds<WS2812, LED_PIN, GRB>(line_leds, LEDS_PER_STRIP);
}
//---------------------------------------------------------------
void show_leds(void)
{
  controllers->show(line_leds, LEDS_PER_STRIP, i_brightness);
}
//---------------------------------------------------------------
void clear_leds(void)
{
  int x = 0;
  for (x = 0; x < LEDS_PER_STRIP; x++) line_leds[x] = 0;
}
//---------------------------------------------------------------
bool set_pixel(unsigned int addr, CRGB color)
{
  if (addr > LEDS_PER_STRIP)
  {
    return false;
  }

  int line = addr / SCREEN_WIDTH;
  int offset = addr % SCREEN_WIDTH;
  if ((line % 2) == 1)
  {
    addr = SCREEN_WIDTH * line + (SCREEN_WIDTH - offset) - 1;
  }

  line_leds[addr] = color;
  return true;
}
//---------------------------------------------------------------
void debug(String text)
{
  work_serial.println(text);
}
//---------------------------------------------------------------
uint8_t convert(uint8_t x)
{
  uint8_t res = 0;
  switch (x)
  {
    case '0':  res = 0x0;  break;
    case '1':  res = 0x1;  break;
    case '2':  res = 0x2;  break;
    case '3':  res = 0x3;  break;
    case '4':  res = 0x4;  break;
    case '5':  res = 0x5;  break;
    case '6':  res = 0x6;  break;
    case '7':  res = 0x7;  break;
    case '8':  res = 0x8;  break;
    case '9':  res = 0x9;  break;
    case 'A':  res = 0xA;  break;
    case 'B':  res = 0xB;  break;
    case 'C':  res = 0xC;  break;
    case 'D':  res = 0xD;  break;
    case 'E':  res = 0xE;  break;
    case 'F':  res = 0xF;  break;
    default:
      break;
  }
  return res;
}
//---------------------------------------------------------------
uint8_t convert_ascii_to_value(uint8_t hi, uint8_t lo)
{
  uint8_t b_hi = convert(hi);
  uint8_t b_lo = convert(lo);

  uint8_t r_byte = (b_hi << 4) | b_lo;
  return r_byte;
}
//---------------------------------------------------------------
bool convert_raw_to_modbus(void)
{
  if (index_ascii_buf == 0)
  {
    debug("index_ascii_buf == 0");
    return false;
  }
  if (index_ascii_buf % 2)
  {
    debug("index_ascii_buf % 2");
    debug("index_ascii_buf " + String(index_ascii_buf));
    return false;
  }

  int index_modbus_buf = 0;
  for (int n = 0; n < index_ascii_buf; n += 2)
  {
    modbus_buf[index_modbus_buf] = convert_ascii_to_value(ascii_buf[n], ascii_buf[n + 1]);
    index_modbus_buf++;
    if (index_modbus_buf > MAX_MODBUS_BUF)
    {
      debug("index_modbus_buf > MAX_MODBUS_BUF");
      debug("index_modbus_buf " + String(index_modbus_buf));
      debug("MAX_MODBUS_BUF " + String(MAX_MODBUS_BUF));
      return false;
    }
  }
  if (index_modbus_buf < sizeof(PACKET))
  {
    debug("index_modbus_buf " + String(index_modbus_buf));
    debug("sizeof(PACKET) " + String(sizeof(PACKET)));
    return false;
  }
  return true;
}
//---------------------------------------------------------------
void command(void)
{
  bool ok = convert_raw_to_modbus();
  if (!ok)
  {
    debug("FAIL");
    return;
  }

  struct PACKET *i_packet = (struct PACKET *)&modbus_buf;

  i_brightness = i_packet->body.brightness;

  CRGB led;
  int index = 0;
  for (int y = 0; y < SCREEN_HEIGTH; y++)
  {
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
      led.r = i_packet->body.leds[x][y].color_R;
      led.g = i_packet->body.leds[x][y].color_G;
      led.b = i_packet->body.leds[x][y].color_B;

      set_pixel(index++, led);
    }
  }

  show_leds();
  //debug("OK");
}
//---------------------------------------------------------------
void setup()
{
  work_serial.begin(BAUDRATE);

  init_leds();
  clear_leds();
  show_leds();
}
//---------------------------------------------------------------
void serialEvent()
{
  while (work_serial.available())
  {
    imcomingByte = work_serial.read();
    switch (imcomingByte)
    {
      case ':':
        index_ascii_buf = 0;
        break;

      case '\r':
      case '\n':
        command();
        index_ascii_buf = 0;
        break;

      default:
        if (index_ascii_buf < MAX_ASCII_BUF)
        {
          ascii_buf[index_ascii_buf] = imcomingByte;
          index_ascii_buf++;
        }
        break;
    }
  }
}
//---------------------------------------------------------------
void test(void)
{
  if (index < LEDS_PER_STRIP)
    index++;
  else
    index = 0;

  clear_leds();

  i_brightness = 1;

  CRGB led;
  led.r = 255;
  led.g = 255;
  led.b = 255;

  set_pixel(index, led);

  show_leds();
}
//---------------------------------------------------------------
void calc_elapsed_time(void)
{
  i_brightness = 1;

  CRGB led;
  led.r = 255;
  led.g = 255;
  led.b = 255;

  unsigned long StartTime = millis();
  for (int n = 0; n < LEDS_PER_STRIP; n++)
  {
    clear_leds();
    set_pixel(n, led);
    show_leds();
  }

  unsigned long CurrentTime = millis();
  unsigned long ElapsedTime = CurrentTime - StartTime;

  debug(String("Time elapsed ") + ElapsedTime + " " + ElapsedTime / 512);
  while (1);
}
//---------------------------------------------------------------
void loop(void)
{
  //test();
  //calc_elapsed_time();
}
//---------------------------------------------------------------

