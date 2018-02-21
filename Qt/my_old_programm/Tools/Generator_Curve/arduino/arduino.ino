//---------------------------------------------------------------
#include <Wire.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;

// Set this value to 9, 8, 7, 6 or 5 to adjust the resolution
#define DAC_RESOLUTION    (8)
//---------------------------------------------------------------
#define MAX_SLIDER  32
//---------------------------------------------------------------
int incomingByte = 0;

int index = 0;
int buf[MAX_SLIDER] = { 0 };
//---------------------------------------------------------------
#define MAX_LEN_ASCII   (MAX_SLIDER * 2)
#define MAX_LEN_MODBUS  MAX_SLIDER
//---------------------------------------------------------------
int buf_ascii[MAX_LEN_ASCII];
int len_ascii = 0;
//---------------------------------------------------------------
char convert(char a)
{
  char temp = 0;
  switch (a)
  {
    case '0':  temp = 0x0;  break;
    case '1':  temp = 0x1;  break;
    case '2':  temp = 0x2;  break;
    case '3':  temp = 0x3;  break;
    case '4':  temp = 0x4;  break;
    case '5':  temp = 0x5;  break;
    case '6':  temp = 0x6;  break;
    case '7':  temp = 0x7;  break;
    case '8':  temp = 0x8;  break;
    case '9':  temp = 0x9;  break;
    case 'A':  temp = 0xA;  break;
    case 'B':  temp = 0xB;  break;
    case 'C':  temp = 0xC;  break;
    case 'D':  temp = 0xD;  break;
    case 'E':  temp = 0xE;  break;
    case 'F':  temp = 0xF;  break;
    default:
      break;
  }
  return temp;
}
//---------------------------------------------------------------
unsigned char convert_ascii_to_char(char a, char b)
{
  char b_a = convert(a);
  char b_b = convert(b);

  return ((b_a << 4) | b_b);
}
//---------------------------------------------------------------
uint16_t convert_ascii_to_uint16(char a, char b, char c, char d)
{
  char b_a = convert(a);
  char b_b = convert(b);
  char b_c = convert(c);
  char b_d = convert(d);

  return ((b_a << 12) | (b_b << 8) | (b_c << 4) | b_d);
}
//---------------------------------------------------------------
void analize(void)
{
  if (len_ascii % 2)
  {
    Serial.print("ERROR: 0 ");
    Serial.println(len_ascii);
    return;
  }
  if (len_ascii != MAX_LEN_ASCII)
  {
    Serial.print("ERROR: 1 ");
    Serial.println(len_ascii);
    return;
  }

  Serial.println("Work");
  //---
  for (int n = 0; n < len_ascii; n += 4)
  {
    buf[n] = convert_ascii_to_uint16(buf_ascii[n],
                                     buf_ascii[n + 1],
                                     buf_ascii[n + 2],
                                     buf_ascii[n + 3]);
  }
}
//---------------------------------------------------------------
void serialEvent()
{
  while (Serial.available())
  {
    // считываем байт
    incomingByte = Serial.read();
    switch (incomingByte)
    {
      case ':':
        len_ascii = 0;
        break;

      case '\r':
      case '\n':
        analize();
        break;

      default:
        if (len_ascii < MAX_LEN_ASCII)
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
void init_buf(void)
{
#if 0
  int delta = 4096 / MAX_SLIDER;
  int value = 0;
  for (int n = 0; n < MAX_SLIDER; n++)
  {
    buf[n] = value;
    value += delta;
  }
#endif
}
//---------------------------------------------------------------
void setup()
{
  Serial.begin(115200);

  // For Adafruit MCP4725A1 the address is 0x62 (default) or 0x63 (ADDR pin tied to VCC)
  // For MCP4725A0 the address is 0x60 or 0x61
  // For MCP4725A2 the address is 0x64 or 0x65
  dac.begin(0x62);

  init_buf();
}
//---------------------------------------------------------------
void loop()
{
#if 0
  for (int n = 0; n < MAX_SLIDER; n++)
  {
    dac.setVoltage(buf[n], false);
  }
#else
  if (index < MAX_SLIDER)
    index++;
  else
    index = 0;
  dac.setVoltage(buf[index], false);
#endif
}
//---------------------------------------------------------------

