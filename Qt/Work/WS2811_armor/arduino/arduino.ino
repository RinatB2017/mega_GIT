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
#include "FastLED.h"
//---------------------------------------------------------------
#define NUM_STRIPS 9
//---------------------------------------------------------------
enum {
  RED = 0,
  GREEN,
  BLUE
};
//---------------------------------------------------------------
int leds_per_strip[NUM_STRIPS] = { 20, 30, 36, 40, 36, 30, 25, 20, 25 };
//---------------------------------------------------------------
CRGB matrix0[20];
CRGB matrix1[30];
CRGB matrix2[36];
CRGB matrix3[40];
CRGB matrix4[36];
CRGB matrix5[30];
CRGB matrix6[25];
CRGB matrix7[20];
CRGB matrix8[25];
//---------------------------------------------------------------
const int max_address = leds_per_strip[0] + leds_per_strip[5] + leds_per_strip[8] + leds_per_strip[4] + (leds_per_strip[3] / 2);
//---------------------------------------------------------------
CLEDController *controllers[NUM_STRIPS];
uint8_t gBrightness = 12;  // 150 выше моргает
//---------------------------------------------------------------
int current_led = 0;    // текущий светодиод
unsigned int delay_ms = 10;
//---------------------------------------------------------------
CRGB led_color = CRGB::Red;
int current_color = RED;

int addr0 = 0;
int addr1 = leds_per_strip[0];
int addr2 = addr1 + leds_per_strip[5];
int addr3 = addr2 + leds_per_strip[8];
int addr4 = addr3 + leds_per_strip[4];

bool flag = false;
//---------------------------------------------------------------
#define MAX_LEN 200
uint8_t serial_buf[MAX_LEN];
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
//---------------------------------------------------------------
void debug(String text)
{
  Serial.println(text);
}
//---------------------------------------------------------------
void init_leds(void)
{
  debug("init_leds");
  controllers[0] = &FastLED.addLeds<WS2812, LED_PIN_1, GRB>(matrix0, leds_per_strip[0]);
  controllers[1] = &FastLED.addLeds<WS2812, LED_PIN_2, GRB>(matrix1, leds_per_strip[1]);
  controllers[2] = &FastLED.addLeds<WS2812, LED_PIN_3, GRB>(matrix2, leds_per_strip[2]);
  controllers[3] = &FastLED.addLeds<WS2812, LED_PIN_4, GRB>(matrix3, leds_per_strip[3]);
  controllers[4] = &FastLED.addLeds<WS2812, LED_PIN_5, GRB>(matrix4, leds_per_strip[4]);
  controllers[5] = &FastLED.addLeds<WS2812, LED_PIN_6, GRB>(matrix5, leds_per_strip[5]);
  controllers[6] = &FastLED.addLeds<WS2812, LED_PIN_7, GRB>(matrix6, leds_per_strip[6]);
  controllers[7] = &FastLED.addLeds<WS2812, LED_PIN_8, GRB>(matrix7, leds_per_strip[7]);
  controllers[8] = &FastLED.addLeds<WS2812, LED_PIN_9, GRB>(matrix8, leds_per_strip[8]);
}
//---------------------------------------------------------------
void show_leds(void)
{
  controllers[0]->show(matrix0, leds_per_strip[0], gBrightness);
  controllers[1]->show(matrix1, leds_per_strip[1], gBrightness);
  controllers[2]->show(matrix2, leds_per_strip[2], gBrightness);
  controllers[3]->show(matrix3, leds_per_strip[3], gBrightness);
  controllers[4]->show(matrix4, leds_per_strip[4], gBrightness);
  controllers[5]->show(matrix5, leds_per_strip[5], gBrightness);
  controllers[6]->show(matrix6, leds_per_strip[6], gBrightness);
  controllers[7]->show(matrix7, leds_per_strip[7], gBrightness);
  controllers[8]->show(matrix8, leds_per_strip[8], gBrightness);
}
//---------------------------------------------------------------
void clear_leds(void)
{
  int x = 0;
  for (x = 0; x < leds_per_strip[0]; x++) matrix0[x] = 0;
  for (x = 0; x < leds_per_strip[1]; x++) matrix1[x] = 0;
  for (x = 0; x < leds_per_strip[2]; x++) matrix2[x] = 0;
  // горизонталь for (x = 0; x < leds_per_strip[3]; x++) matrix3[x] = 0;
  for (x = 0; x < leds_per_strip[4]; x++) matrix4[x] = 0;
  for (x = 0; x < leds_per_strip[5]; x++) matrix5[x] = 0;
  for (x = 0; x < leds_per_strip[6]; x++) matrix6[x] = 0;
  for (x = 0; x < leds_per_strip[7]; x++) matrix7[x] = 0;
  for (x = 0; x < leds_per_strip[8]; x++) matrix8[x] = 0;
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
    delay(delay_ms);
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
    delay(delay_ms);
    //matrix3[addr - addr4] = color;
    //??? matrix3[max_address + (addr - addr4)] = color;
    return true;
  }

  return false;
}
//---------------------------------------------------------------
void set_horizont(CRGB color)
{
  for (int x = 0; x < leds_per_strip[3]; x++) matrix3[x] = color;
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
void setup()
{
  int addr0 = 0;
  int addr1 = leds_per_strip[0];
  int addr2 = addr1 + leds_per_strip[5];
  int addr3 = addr2 + leds_per_strip[8];
  int addr4 = addr3 + leds_per_strip[4];

  Serial.begin(57600);
  debug("addr0 = " + String(addr0));
  debug("addr1 = " + String(addr1));
  debug("addr2 = " + String(addr2));
  debug("addr3 = " + String(addr3));
  debug("addr4 = " + String(addr4));
  init_leds();
}
//---------------------------------------------------------------
void loop(void)
{
  clear_leds();
  set_left_pixel(current_led,  led_color);
  set_right_pixel(current_led, led_color);

  //set_horizont(led_color);
  if (current_led % 20 == 0)
  {
    flag = !flag;
  }
  if(flag)
    set_horizont(CRGB::Red);
  else
    set_horizont(CRGB::Blue);

#if 0
  if (current_led == addr1)
  {
    set_left_pixel(current_led, CRGB::Black);
    set_right_pixel(current_led, CRGB::Black);
    delay(delay_ms * 8);
  }
  if (current_led == addr2)
  {
    set_left_pixel(current_led, CRGB::Black);
    set_right_pixel(current_led, CRGB::Black);
    delay(delay_ms * 20);
  }
  if (current_led == addr3)
  {
    set_left_pixel(current_led, CRGB::Black);
    set_right_pixel(current_led, CRGB::Black);
    delay(delay_ms * 36);
  }
#endif
  
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

  delay(delay_ms);
}
//---------------------------------------------------------------

