//---------------------------------------------------------------
#include "FastLED.h"
#include "BluetoothSerial.h"
//---------------------------------------------------------------
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
//---------------------------------------------------------------
BluetoothSerial SerialBT;
const char* ble_name = "Halo";
//---------------------------------------------------------------
#define BAUDRATE    115200
#define debug_serial Serial
#define bt_serial   SerialBT
//---------------------------------------------------------------
#define LEDS_PER_STRIP  6 //52
#define LED_PIN         16
//---------------------------------------------------------------
CRGB line_leds[LEDS_PER_STRIP];
//---------------------------------------------------------------
CLEDController *controllers;
//---------------------------------------------------------------
#define MAX_ASCII_BUF 50
uint8_t ascii_buf[MAX_ASCII_BUF];
int index_ascii_buf = 0;

#define MAX_MODBUS_BUF (MAX_ASCII_BUF / 2)
uint8_t modbus_buf[MAX_MODBUS_BUF];
int index_modbus_buf = 0;

int imcomingByte = 0;

int i_brightness = 20;
//---------------------------------------------------------------
#pragma pack (push, 1)

struct HEADER {
  uint8_t address;
  uint8_t command;
  uint8_t cnt_data;
  uint8_t data[];
};

struct CMD_1 {
  uint8_t brightness;
  uint8_t color_R;
  uint8_t color_G;
  uint8_t color_B;
};

union U_UINT32 {
  uint32_t value;
  struct {
    uint8_t d;
    uint8_t c;
    uint8_t b;
    uint8_t a;
  } bytes;
};

#pragma pack(pop)
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
uint32_t crc32(const char *buf, size_t len)
{
  static uint32_t table[256];
  static int have_table = 0;
  uint32_t rem;
  uint8_t octet;
  int i, j;
  uint32_t crc = 0;
  const char *p, *q;

  /* This check is not thread safe; there is no mutex. */
  if (have_table == 0) {
    /* Calculate CRC table. */
    for (i = 0; i < 256; i++) {
      rem = static_cast<uint32_t>(i);  /* remainder from polynomial division */
      for (j = 0; j < 8; j++) {
        if (rem & 1) {
          rem >>= 1;
          rem ^= 0xedb88320;
        } else
          rem >>= 1;
      }
      table[i] = rem;
    }
    have_table = 1;
  }

  crc = ~crc;
  q = buf + len;
  for (p = buf; p < q; p++) {
    octet = static_cast<uint8_t>(*p);  /* Cast to unsigned octet. */
    crc = (crc >> 8) ^ table[(crc & 0xff) ^ octet];
  }
  return ~crc;
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
bool check_crc32(void)
{
  uint32_t calc_crc32 = crc32((char *)&modbus_buf, sizeof(HEADER) + sizeof(CMD_1));

  U_UINT32 temp;
  temp.bytes.a = modbus_buf[index_modbus_buf - 4];
  temp.bytes.b = modbus_buf[index_modbus_buf - 3];
  temp.bytes.c = modbus_buf[index_modbus_buf - 2];
  temp.bytes.d = modbus_buf[index_modbus_buf - 1];
  uint32_t packet_crc32 = temp.value;

  if (calc_crc32 != packet_crc32)
  {
    debug_serial.println("calc_crc32");
    debug_serial.println(calc_crc32,   HEX);
    debug_serial.println("packet_crc32");
    debug_serial.println(packet_crc32, HEX);

    return false;
  }
  return true;
}
//---------------------------------------------------------------
void command(void)
{
  debug_serial.println("command");
  if (index_ascii_buf == 0)
  {
    debug_serial.println("index_ascii_buf == 0");
    return;
  }
  if (index_ascii_buf % 2)
  {
    debug_serial.println("index_ascii_buf % 2");
    return;
  }

  index_modbus_buf = 0;
  for (int n = 0; n < index_ascii_buf; n += 2)
  {
    modbus_buf[index_modbus_buf] = convert_ascii_to_value(ascii_buf[n], ascii_buf[n + 1]);
    index_modbus_buf++;
    if (index_modbus_buf > MAX_MODBUS_BUF)
    {
      debug_serial.println("index_modbus_buf > MAX_MODBUS_BUF");
      return;
    }
  }
  if (index_modbus_buf < sizeof(HEADER))
  {
    debug_serial.println("index_modbus_buf < sizeof(HEADER)");
    return;
  }

  struct HEADER *packet = (struct HEADER *)&modbus_buf;
  uint8_t address  = packet->address;
  uint8_t command  = packet->command;
  uint8_t cnt_data = packet->cnt_data;

  uint8_t brightness = 0;
  uint8_t color_R = 0;
  uint8_t color_G = 0;
  uint8_t color_B = 0;

  if (cnt_data == sizeof(CMD_1))
  {
    struct CMD_1 *data_cmd_1 = (struct CMD_1 *)&packet->data;
    brightness = data_cmd_1->brightness;
    color_R = data_cmd_1->color_R;
    color_G = data_cmd_1->color_G;
    color_B = data_cmd_1->color_B;
  }

  if (index_modbus_buf != (sizeof(HEADER) + cnt_data + 4))
  {
    debug_serial.println("bad len");
    debug_serial.println(index_modbus_buf);
    debug_serial.println(sizeof(HEADER) + cnt_data + 4);
    return;
  }

  if (!check_crc32())
  {
    debug_serial.println("bad CRC32");
    return;
  }

  if (address != 0)
  {
    debug_serial.println("bad address");
    return;
  }

  switch (command)
  {
    case 0:
      debug_serial.println("cmd_0");
      i_brightness = brightness;
      for (int n = 0; n < LEDS_PER_STRIP; n++)
      {
        line_leds[n].r = color_R;
        line_leds[n].g = color_G;
        line_leds[n].b = color_B;
      }
      show_leds();
      break;

    case 1:
      debug_serial.println("cmd_1");
      for (int n = 0; n < LEDS_PER_STRIP; n++)
      {
        line_leds[n].r = 255;
        line_leds[n].g = 0;
        line_leds[n].b = 0;
      }
      show_leds();
      break;

    case 2:
      debug_serial.println("cmd_2");
      for (int n = 0; n < LEDS_PER_STRIP; n++)
      {
        line_leds[n].r = 0;
        line_leds[n].g = 255;
        line_leds[n].b = 0;
      }
      show_leds();
      break;

    case 3:
      debug_serial.println("cmd_3");
      for (int n = 0; n < LEDS_PER_STRIP; n++)
      {
        line_leds[n].r = 0;
        line_leds[n].g = 0;
        line_leds[n].b = 255;
      }
      show_leds();
      break;

    case 4:
      debug_serial.println("cmd_4");
      for (int n = 0; n < LEDS_PER_STRIP; n++)
      {
        line_leds[n].r = 255;
        line_leds[n].g = 255;
        line_leds[n].b = 255;
      }
      show_leds();
      break;

    default:
      break;
  }
}
//---------------------------------------------------------------
void setup()
{
  Serial.begin(BAUDRATE);
  SerialBT.begin(ble_name); //Bluetooth device name

  init_leds();
  clear_leds();
  show_leds();
}
//---------------------------------------------------------------
void loop() {
  while (bt_serial.available())
  {
    imcomingByte = bt_serial.read();
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

