//---------------------------------------------------------------
#include "FastLED.h"
//---------------------------------------------------------------
#define BAUDRATE  57600
#define work_serial    Serial

#define SCREEN_WIDTH  32 
#define SCREEN_HEIGTH 16
//---------------------------------------------------------------
#define LEDS_PER_STRIP  512
#define LED_PIN 9
//---------------------------------------------------------------
CRGB line_leds[LEDS_PER_STRIP];
//---------------------------------------------------------------
CLEDController *controllers;
//---------------------------------------------------------------
#pragma pack (push, 1)

union PACKET
{
    struct {
        uint8_t begin_packet;
        uint8_t brightness;
        uint8_t leds[SCREEN_WIDTH * SCREEN_HEIGTH * 3];
        uint8_t enfd_packet;
    } body;
    uint8_t buf[sizeof(body)];
};

#pragma pack(pop)
//---------------------------------------------------------------
int imcomingByte = 0;
String application_command;

int i_brightness = 0;
int i_color_R = 0;
int i_color_G = 0;
int i_color_B = 0;
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
  if (addr < LEDS_PER_STRIP)
  {
    line_leds[addr] = color;
    return true;
  }

  return false;
}
//---------------------------------------------------------------
void command(void)
{
  String brightness = getValue(application_command, ';', 0);
  String color_R = getValue(application_command, ';', 1);
  String color_G = getValue(application_command, ';', 2);
  String color_B = getValue(application_command, ';', 3);

  i_brightness = brightness.toInt();
  i_color_R = color_R.toInt();
  i_color_G = color_G.toInt();
  i_color_B = color_B.toInt();

  work_serial.println("===");
  //work_serial.println(application_command);
  work_serial.println("B " + String(i_brightness));
  work_serial.println("R " + String(i_color_R));
  work_serial.println("G " + String(i_color_G));
  work_serial.println("B " + String(i_color_B));

  for (int n = 0; n < LEDS_PER_STRIP; n++)
  {
    line_leds[n].r = i_color_R;
    line_leds[n].g = i_color_G;
    line_leds[n].b = i_color_B;
  }
  show_leds();
}
//---------------------------------------------------------------
//
// https://stackoverflow.com/questions/9072320/split-string-into-string-array
//
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
//---------------------------------------------------------------
void setup()
{
  work_serial.begin(BAUDRATE);
  work_serial.println("example: brightness;color_R;color_G;color_B");

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
      case '\r':
      case '\n':
        command();
        application_command = "";
        break;

      default:
        application_command.concat((char) imcomingByte);
        break;
    }
  }
}
//---------------------------------------------------------------
void loop(void)
{

}
//---------------------------------------------------------------

