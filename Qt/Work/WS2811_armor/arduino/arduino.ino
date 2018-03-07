//---------------------------------------------------------------
// тест по-быстрому
// 0 - 20
// 1 - 30
// 2 - 36
// 3 - 40
// 4 - 36
// 5 - 36
// 6 - 25
// 7 - 20
// 8 - 25
//---------------------------------------------------------------
#include <EEPROM.h>
#include "FastLED.h"
//---------------------------------------------------------------
#define CMD_SET_DELAY_MS    1
#define CMD_SET_BRIGHTNESS  2

#define CMD_01 10
#define CMD_02 20
#define CMD_03 30
#define CMD_04 40
#define CMD_05 50

#define MAX_BRIGHTNESS  150 // 150 выше моргает
#define MAX_DELAY_MS  1000

enum {
  RED = 0,
  GREEN,
  BLUE
};

#define NUM_STRIPS 9

#define SIZE_MATRIX_0 20
#define SIZE_MATRIX_1 30
#define SIZE_MATRIX_2 36
#define SIZE_MATRIX_3 40
#define SIZE_MATRIX_4 36
#define SIZE_MATRIX_5 30
#define SIZE_MATRIX_6 25
#define SIZE_MATRIX_7 20
#define SIZE_MATRIX_8 25
//---------------------------------------------------------------
CRGB matrix0[SIZE_MATRIX_0];
CRGB matrix1[SIZE_MATRIX_1];
CRGB matrix2[SIZE_MATRIX_2];
CRGB matrix3[SIZE_MATRIX_3];
CRGB matrix4[SIZE_MATRIX_4];
CRGB matrix5[SIZE_MATRIX_5];
CRGB matrix6[SIZE_MATRIX_6];
CRGB matrix7[SIZE_MATRIX_7];
CRGB matrix8[SIZE_MATRIX_8];
//---------------------------------------------------------------
const int max_address = SIZE_MATRIX_0 + SIZE_MATRIX_5 + SIZE_MATRIX_8 + SIZE_MATRIX_4 + (SIZE_MATRIX_3 / 2);
//---------------------------------------------------------------
CLEDController *controllers[NUM_STRIPS];
//---------------------------------------------------------------
int current_led = 0;    // текущий светодиод
//---------------------------------------------------------------
union DATA
{
  struct MEMORY
  {
    uint8_t gBrightness;
    unsigned int current_state;
    unsigned int delay_ms;
  } memory_t;
  char buf[sizeof(memory_t)];
} data_t;
//---------------------------------------------------------------
CRGB led_color = CRGB::Red;
int current_color = RED;

int addr0 = 0;
int addr1 = SIZE_MATRIX_0;
int addr2 = addr1 + SIZE_MATRIX_5;
int addr3 = addr2 + SIZE_MATRIX_8;
int addr4 = addr3 + SIZE_MATRIX_4;

bool flag = false;
//---------------------------------------------------------------
#define MAX_ASCII_BUF 50
uint8_t ascii_buf[MAX_ASCII_BUF];
int index_ascii_buf = 0;

#define MAX_MODBUS_BUF (MAX_ASCII_BUF / 2)
uint8_t modbus_buf[MAX_MODBUS_BUF];
int index_modbus_buf = 0;

int imcomingByte = 0;
//---------------------------------------------------------------
#define LED_PIN_1 2
#define LED_PIN_2 3
#define LED_PIN_3 4
#define LED_PIN_4 5
#define LED_PIN_5 6
#define LED_PIN_6 7
#define LED_PIN_7 8
#define LED_PIN_8 9
#define LED_PIN_9 10
//--------------------------------------------------------------------------------
#pragma pack (push, 1)

typedef struct PACKET
{
  uint8_t     cmd_8;  // команда
  uint16_t    data;   // данные
} header;

#pragma pack(pop)
//--------------------------------------------------------------------------------
void serialEvent()
{
  while (Serial.available())
  {
    // считываем байт
    imcomingByte = Serial.read();
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
//--------------------------------------------------------------------------------
void command(void)
{
  debug("command");
  if (index_ascii_buf == 0)
  {
    debug("index_ascii_buf == 0");
    return;
  }
  if (index_ascii_buf % 2)
  {
    debug("index_ascii_buf % 2");
    return;
  }

  int index_modbus_buf = 0;
  for (int n = 0; n < index_ascii_buf; n += 2)
  {
    modbus_buf[index_modbus_buf] = convert_ascii_to_value(ascii_buf[n], ascii_buf[n + 1]);
    index_modbus_buf++;
    if (index_modbus_buf > MAX_MODBUS_BUF)
    {
      debug("index_modbus_buf > MAX_MODBUS_BUF");
      return;
    }
  }
  if (index_modbus_buf < 3)
  {
    debug("index_modbus_buf < 3");
    return;
  }

  struct PACKET *packet = (struct PACKET *)&modbus_buf;
  uint8_t cmd = packet->cmd_8;
  switch (cmd)
  {
    case CMD_SET_DELAY_MS:
      data_t.memory_t.delay_ms = packet->data;
      if (data_t.memory_t.delay_ms > MAX_DELAY_MS)
      {
        data_t.memory_t.delay_ms = MAX_DELAY_MS;
      }
      send_answer(cmd);
      break;

    case CMD_SET_BRIGHTNESS:
      data_t.memory_t.gBrightness = packet->data;
      if (data_t.memory_t.gBrightness > MAX_BRIGHTNESS)
      {
        data_t.memory_t.gBrightness = MAX_BRIGHTNESS;
      }
      send_answer(cmd);
      break;

    case CMD_01:
      data_t.memory_t.current_state = CMD_01;
      save_eeprom();
      send_answer(cmd);
      break;

    case CMD_02:
      data_t.memory_t.current_state = CMD_02;
      save_eeprom();
      send_answer(cmd);
      break;

    case CMD_03:
      data_t.memory_t.current_state = CMD_03;
      save_eeprom();
      send_answer(cmd);
      break;

    case CMD_04:
      data_t.memory_t.current_state = CMD_04;
      save_eeprom();
      send_answer(cmd);
      break;

    case CMD_05:
      data_t.memory_t.current_state = CMD_05;
      save_eeprom();
      send_answer(cmd);
      break;
  }
}
//--------------------------------------------------------------------------------
void send_answer(uint8_t cmd)
{
  String answer;
  answer += ":";
  answer += convert_data_to_ascii(cmd);
  answer += "FF";
  answer += "FF";

  Serial.println(answer);
}
//--------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------
uint8_t convert_ascii_to_value(uint8_t hi, uint8_t lo)
{
  uint8_t b_hi = convert(hi);
  uint8_t b_lo = convert(lo);

  uint8_t r_byte = (b_hi << 4) | b_lo;
  return r_byte;
}
//--------------------------------------------------------------------------------
String convert_data_to_ascii(uint8_t data)
{
    uint8_t hi = (data >> 4) & 0x0F;
    uint8_t lo = (data & 0x0F);

    String hi_str;
    switch(hi)
    {
    case 0x00: hi_str="0"; break;
    case 0x01: hi_str="1"; break;
    case 0x02: hi_str="2"; break;
    case 0x03: hi_str="3"; break;
    case 0x04: hi_str="4"; break;
    case 0x05: hi_str="5"; break;
    case 0x06: hi_str="6"; break;
    case 0x07: hi_str="7"; break;
    case 0x08: hi_str="8"; break;
    case 0x09: hi_str="9"; break;
    case 0x0A: hi_str="A"; break;
    case 0x0B: hi_str="B"; break;
    case 0x0C: hi_str="C"; break;
    case 0x0D: hi_str="D"; break;
    case 0x0E: hi_str="E"; break;
    case 0x0F: hi_str="F"; break;
    default: break;
    }

    String lo_str;
    switch(lo)
    {
    case 0x00: lo_str="0"; break;
    case 0x01: lo_str="1"; break;
    case 0x02: lo_str="2"; break;
    case 0x03: lo_str="3"; break;
    case 0x04: lo_str="4"; break;
    case 0x05: lo_str="5"; break;
    case 0x06: lo_str="6"; break;
    case 0x07: lo_str="7"; break;
    case 0x08: lo_str="8"; break;
    case 0x09: lo_str="9"; break;
    case 0x0A: lo_str="A"; break;
    case 0x0B: lo_str="B"; break;
    case 0x0C: lo_str="C"; break;
    case 0x0D: lo_str="D"; break;
    case 0x0E: lo_str="E"; break;
    case 0x0F: lo_str="F"; break;
    default: break;
    }

    return (hi_str+lo_str);
}
//---------------------------------------------------------------
void f_01(void)
{
  clear_leds();
  set_left_pixel(current_led,  led_color);
  set_right_pixel(current_led, led_color);

  //set_horizont(led_color);
  if (current_led % 20 == 0)
  {
    flag = !flag;
  }
  if (flag)
    set_horizont(CRGB::Red);
  else
    set_horizont(CRGB::Blue);

  show_leds();

  if (current_led < max_address)
  {
    current_led++;
  }
  else
  {
    current_led = 0;
    switch_color();
  }

  delay(data_t.memory_t.delay_ms);
}
//---------------------------------------------------------------
void f_02(void)
{
  clear_leds();
  set_left_pixel(current_led,  led_color);
  set_right_pixel(current_led, led_color);

  //set_horizont(led_color);
  if (current_led % 20 == 0)
  {
    flag = !flag;
  }
  if (flag)
    set_horizont(CRGB::Red);
  else
    set_horizont(CRGB::Blue);

  if (current_led == addr1)
  {
    set_left_pixel(current_led, CRGB::Black);
    set_right_pixel(current_led, CRGB::Black);
    delay(data_t.memory_t.delay_ms * 8);
  }
  if (current_led == addr2)
  {
    set_left_pixel(current_led, CRGB::Black);
    set_right_pixel(current_led, CRGB::Black);
    delay(data_t.memory_t.delay_ms * 20);
  }
  if (current_led == addr3)
  {
    set_left_pixel(current_led, CRGB::Black);
    set_right_pixel(current_led, CRGB::Black);
    delay(data_t.memory_t.delay_ms * 36);
  }

  show_leds();

  if (current_led < max_address)
  {
    current_led++;
  }
  else
  {
    current_led = 0;
    switch_color();
  }

  delay(data_t.memory_t.delay_ms);
}
//---------------------------------------------------------------
void f_03(void)
{
  for (int n = 0; n < max_address; n += 2)
  {
    set_left_pixel(n,     CRGB::Red);
    set_left_pixel(n + 1,   CRGB::Blue);
    set_right_pixel(n,    CRGB::Red);
    set_right_pixel(n + 1,  CRGB::Blue);
    set_horizont(CRGB::Red);
  }
  show_leds();
  delay(data_t.memory_t.delay_ms);

  for (int n = 0; n < max_address; n += 2)
  {
    set_left_pixel(n,     CRGB::Blue);
    set_left_pixel(n + 1,   CRGB::Red);
    set_right_pixel(n,    CRGB::Blue);
    set_right_pixel(n + 1,  CRGB::Red);
    set_horizont(CRGB::Blue);
  }
  show_leds();
  delay(data_t.memory_t.delay_ms);
}
//---------------------------------------------------------------
void f_04(void)
{

}
//---------------------------------------------------------------
void f_05(void)
{

}
//---------------------------------------------------------------
void debug(String text)
{
  //Serial.println(text);
}
//---------------------------------------------------------------
void init_leds(void)
{
  debug("init_leds");
  controllers[0] = &FastLED.addLeds<WS2812, LED_PIN_1, GRB>(matrix0, SIZE_MATRIX_0);
  controllers[1] = &FastLED.addLeds<WS2812, LED_PIN_2, GRB>(matrix1, SIZE_MATRIX_1);
  controllers[2] = &FastLED.addLeds<WS2812, LED_PIN_3, GRB>(matrix2, SIZE_MATRIX_2);
  controllers[3] = &FastLED.addLeds<WS2812, LED_PIN_4, GRB>(matrix3, SIZE_MATRIX_3);
  controllers[4] = &FastLED.addLeds<WS2812, LED_PIN_5, GRB>(matrix4, SIZE_MATRIX_4);
  controllers[5] = &FastLED.addLeds<WS2812, LED_PIN_6, GRB>(matrix5, SIZE_MATRIX_5);
  controllers[6] = &FastLED.addLeds<WS2812, LED_PIN_7, GRB>(matrix6, SIZE_MATRIX_6);
  controllers[7] = &FastLED.addLeds<WS2812, LED_PIN_8, GRB>(matrix7, SIZE_MATRIX_7);
  controllers[8] = &FastLED.addLeds<WS2812, LED_PIN_9, GRB>(matrix8, SIZE_MATRIX_8);
}
//---------------------------------------------------------------
void show_leds(void)
{
  controllers[0]->show(matrix0, SIZE_MATRIX_0, data_t.memory_t.gBrightness);
  controllers[1]->show(matrix1, SIZE_MATRIX_1, data_t.memory_t.gBrightness);
  controllers[2]->show(matrix2, SIZE_MATRIX_2, data_t.memory_t.gBrightness);
  controllers[3]->show(matrix3, SIZE_MATRIX_3, data_t.memory_t.gBrightness);
  controllers[4]->show(matrix4, SIZE_MATRIX_4, data_t.memory_t.gBrightness);
  controllers[5]->show(matrix5, SIZE_MATRIX_5, data_t.memory_t.gBrightness);
  controllers[6]->show(matrix6, SIZE_MATRIX_6, data_t.memory_t.gBrightness);
  controllers[7]->show(matrix7, SIZE_MATRIX_7, data_t.memory_t.gBrightness);
  controllers[8]->show(matrix8, SIZE_MATRIX_8, data_t.memory_t.gBrightness);
}
//---------------------------------------------------------------
void clear_leds(void)
{
  int x = 0;
  for (x = 0; x < SIZE_MATRIX_0; x++) matrix0[x] = 0;
  for (x = 0; x < SIZE_MATRIX_1; x++) matrix1[x] = 0;
  for (x = 0; x < SIZE_MATRIX_2; x++) matrix2[x] = 0;
  for (x = 0; x < SIZE_MATRIX_3; x++) matrix3[x] = 0;
  for (x = 0; x < SIZE_MATRIX_4; x++) matrix4[x] = 0;
  for (x = 0; x < SIZE_MATRIX_5; x++) matrix5[x] = 0;
  for (x = 0; x < SIZE_MATRIX_6; x++) matrix6[x] = 0;
  for (x = 0; x < SIZE_MATRIX_7; x++) matrix7[x] = 0;
  for (x = 0; x < SIZE_MATRIX_8; x++) matrix8[x] = 0;
}
//---------------------------------------------------------------
bool set_left_pixel(unsigned int addr, CRGB color)
{
  if (addr < addr1)
  {
    matrix0[addr] = color;
    return true;
  }
  if (addr < addr2)
  {
    matrix5[addr - addr1] = color;
    return true;
  }
  if (addr < addr3)
  {
    matrix8[addr - addr2] = color;
    return true;
  }
  if (addr < addr4)
  {
    //matrix4[addr - addr3] = color;

    //FIXME перевернут по ошибке
    matrix4[abs(addr4 - addr - 1)] = color;

    return true;
  }
  if (addr < max_address)
  {
    delay(data_t.memory_t.delay_ms);
    //??? matrix3[addr - addr4] = color;
    //matrix3[abs(max_address - addr - addr4)] = color;
    return true;
  }

  return false;
}
//---------------------------------------------------------------
bool set_right_pixel(unsigned int addr, CRGB color)
{
  if (addr < addr1)
  {
    matrix7[addr] = color;
    return true;
  }
  if (addr < addr2)
  {
    matrix1[addr - addr1] = color;
    return true;
  }
  if (addr < addr3)
  {
    matrix6[addr - addr2] = color;
    return true;
  }
  if (addr < addr4)
  {
    matrix2[addr - addr3] = color;
    return true;
  }
  if (addr < max_address)
  {
    delay(data_t.memory_t.delay_ms);
    //matrix3[addr - addr4] = color;
    //??? matrix3[max_address + (addr - addr4)] = color;
    return true;
  }

  return false;
}
//---------------------------------------------------------------
void set_horizont(CRGB color)
{
  for (int x = 0; x < SIZE_MATRIX_3; x++) matrix3[x] = color;
}
//---------------------------------------------------------------
void switch_color(void)
{
  switch (current_color)
  {
    case RED:
      debug("Red");
      led_color = CRGB::Green;
      current_color = GREEN;
      break;

    case GREEN:
      debug("Green");
      led_color = CRGB::Blue;
      current_color = BLUE;
      break;

    case BLUE:
      debug("Blue");
      led_color = CRGB::Red;
      current_color = RED;
      break;

    default:
      debug("default");
      led_color = CRGB::Red;
      current_color = RED;
      break;
  }
}
//---------------------------------------------------------------
/*
  union DATA
  {
    struct MEMORY
    {
      uint8_t gBrightness;
      unsigned int current_state;
      unsigned int delay_ms;
    } memory_t;
    char buf[sizeof(memory_t)];
  } data_t;
*/
//---------------------------------------------------------------
void load_eeprom(void)
{
  for (unsigned int n = 0; n < sizeof(data_t.memory_t); n++)
  {
    data_t.buf[n] = EEPROM.read(n);
  }

  bool needed_write_eeprom = false;
  if (data_t.memory_t.gBrightness > MAX_BRIGHTNESS)
  {
    data_t.memory_t.gBrightness = MAX_BRIGHTNESS;
    needed_write_eeprom = true;
  }
  if (data_t.memory_t.delay_ms > MAX_DELAY_MS)
  {
    data_t.memory_t.delay_ms = MAX_DELAY_MS;
    needed_write_eeprom = true;
  }

  if ( needed_write_eeprom)
  {
    data_t.memory_t.current_state = CMD_01;
    save_eeprom();
  }
}
//---------------------------------------------------------------
void save_eeprom(void)
{
  for (unsigned int n = 0; n < sizeof(data_t.memory_t); n++)
  {
    EEPROM.write(n, data_t.buf[n]);
  }
}
//---------------------------------------------------------------
void setup()
{
  Serial.begin(57600);
  debug("addr0 = " + String(addr0));
  debug("addr1 = " + String(addr1));
  debug("addr2 = " + String(addr2));
  debug("addr3 = " + String(addr3));
  debug("addr4 = " + String(addr4));
  init_leds();

  load_eeprom();
  data_t.memory_t.delay_ms = 10;
}
//---------------------------------------------------------------
void loop(void)
{
  switch (data_t.memory_t.current_state)
  {
    case CMD_01:
      f_01();
      break;
    case CMD_02:
      f_02();
      break;
    case CMD_03:
      f_03();
      break;
    case CMD_04:
      f_04();
      break;
    case CMD_05:
      f_05();
      break;

    default:
      data_t.memory_t.current_state = CMD_01;
      break;
  }
}
//---------------------------------------------------------------

