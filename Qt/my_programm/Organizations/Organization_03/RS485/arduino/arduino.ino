//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------
#include "Timer.h"
//--------------------------------------------------------------------------------
#pragma pack (push, 1)
//--------------------------------------------------------------------------------
union U_BYTE
{
  uint8_t value;
  struct
  {
    uint8_t bit0: 1;
    uint8_t bit1: 1;
    uint8_t bit2: 1;
    uint8_t bit3: 1;
    uint8_t bit4: 1;
    uint8_t bit5: 1;
    uint8_t bit6: 1;
    uint8_t bit7: 1;
  } bites;
};

enum CMD {
  CMD_TEST    = 0,
  CMD_READ    = 1,
  CMD_WRITE   = 2,
  CMD_RESET   = 3
};

union PELCO_PACKET
{
  struct PELCO_BODY
  {
    uint8_t sync;
    uint8_t address;
    uint8_t cmd1;
    uint8_t cmd2;
    uint8_t data1;
    uint8_t data2;
    uint8_t crc;
  } body;
  uint8_t buf[sizeof(struct PELCO_BODY)];
};

typedef struct HEADER
{
  uint16_t    prefix_16;                  // префикс
  uint8_t     addr_8;                     // адрес модуля
  uint8_t     cmd_8;                      // команда
  uint16_t    len_16;                     // длина данных
} header;
//--------------------------------------------------------------------------------
union QUESTION_TEST
{
  struct QUESTION_TEST_BODY
  {
    struct HEADER   header;
    uint16_t    crc16;                  // контрольная сумма
  } body;
  unsigned char buf[sizeof(struct QUESTION_TEST_BODY)];
};

union ANSWER_TEST
{
  struct ANSWER_TEST_BODY
  {
    struct HEADER   header;
    struct DATA
    {
      uint16_t result;
    } data;
    uint16_t    crc16;                  // контрольная сумма
  } body;
  unsigned char buf[sizeof(struct ANSWER_TEST_BODY)];
};
//--------------------------------------------------------------------------------
union QUESTION_RESET
{
  struct QUESTION_RESET_BODY
  {
    struct HEADER   header;
    uint16_t    crc16;                  // контрольная сумма
  } body;
  unsigned char buf[sizeof(struct QUESTION_RESET_BODY)];
};

union ANSWER_RESET
{
  struct ANSWER_RESET_BODY
  {
    struct HEADER   header;
    struct DATA
    {
      uint32_t    addr_cam_32;                // адрес камеры
      uint16_t    time_interval_16;           // интервал дворника
      uint32_t    time_washout_32;            // время помывки
      uint32_t    time_pause_washout_32;      // время между помывками
      uint32_t    preset_washout_32;          // пресет помывки
    } data;
    uint16_t    crc16;                  // контрольная сумма
  } body;
  unsigned char buf[sizeof(struct ANSWER_RESET_BODY)];
};
//--------------------------------------------------------------------------------
union QUESTION_READ
{
  struct QUESTION_READ_BODY
  {
    struct HEADER      header;
    uint16_t    crc16;                      // контрольная сумма
  } body;
  unsigned char buf[sizeof(struct QUESTION_READ_BODY)];
};

union ANSWER_READ
{
  struct ANSWER_READ_BODY
  {
    struct HEADER      header;
    struct DATA
    {
      uint32_t    addr_cam_32;                // адрес камеры
      uint16_t    time_interval_16;           // интервал дворника
      uint32_t    time_washout_32;            // время помывки
      uint32_t    time_pause_washout_32;      // время между помывками
      uint32_t    preset_washout_32;          // пресет помывки
    } data;
    uint16_t    crc16;                      // контрольная сумма
  } body;
  unsigned char buf[sizeof(struct ANSWER_READ_BODY)];
};
//--------------------------------------------------------------------------------
union QUESTION_WRITE
{
  struct QUESTION_WRITE_BODY
  {
    struct HEADER      header;
    struct DATA
    {
      uint32_t    addr_cam_32;                // адрес камеры
      uint16_t    time_interval_16;           // интервал дворника
      uint32_t    time_washout_32;            // время помывки
      uint32_t    time_pause_washout_32;      // время между помывками
      uint32_t    preset_washout_32;          // пресет помывки
    } data;
    uint16_t    crc16;                      // контрольная сумма
  } body;
  unsigned char buf[sizeof(struct QUESTION_WRITE_BODY)];
};

union ANSWER_WRITE
{
  struct ANSWER_WRITE_BODY
  {
    struct HEADER      header;
    struct DATA
    {
      uint32_t    addr_cam_32;                // адрес камеры
      uint16_t    time_interval_16;           // интервал дворника
      uint32_t    time_washout_32;            // время помывки
      uint32_t    time_pause_washout_32;      // время между помывками
      uint32_t    preset_washout_32;          // пресет помывки
    }  data;
    uint16_t    crc16;                      // контрольная сумма
  } body;
  unsigned char buf[sizeof(struct ANSWER_WRITE_BODY)];
};
//--------------------------------------------------------------------------------
#pragma pack(pop)
//--------------------------------------------------------------------------------
char Pelco[20] = { 0xFF, 0, 0, 0, 0, 0 }; //структура
//--------------------------------------------------------------------------------
long cnt_second = 0;
//--------------------------------------------------------------------------------
#define Set_Preset   3
#define Clr_Preset  5
#define Go_Preset   7
#define Run_Pattern 0x23
#define Move    59
#define Save    58
#define Wiper   63
//--------------------------------------------------------------------------------
/*
   если пошел дождь, то поведение такое же, как и при помывке, только не дергаем камерой
*/
enum {
  STATUS_IDLE = 0,
  STATUS_RAIN,          // дождь
  STATUS_WASHOUT,       // моемся
  STATUS_WASHOUT_PAUSE  // пауза
};
int state = STATUS_IDLE;
//--------------------------------------------------------------------------------
int pin_485   = 13; //2;
int led_blink = 3;
int led_pump  = A0;
int led_relay = 5;

int pin_rain = 6;
int pin_level = 7;

int addr0 = A5;
int addr1 = A4;
int addr2 = A3;
int addr3 = A2;
int addr4 = A1;

int delay_485_ms = 10;

bool flag_blink = false;
//--------------------------------------------------------------------------------
void debug(String text)
{
  //Serial.println(text);
}
//--------------------------------------------------------------------------------
void blink_ON()
{
  digitalWrite(led_blink, HIGH);
}
//--------------------------------------------------------------------------------
void blink_OFF()
{
  digitalWrite(led_blink, LOW);
}
//--------------------------------------------------------------------------------
void pump_ON()
{
  digitalWrite(led_pump, HIGH);
}
//--------------------------------------------------------------------------------
void pump_OFF()
{
  digitalWrite(led_pump, LOW);
}
//--------------------------------------------------------------------------------
void relay_ON()
{
  digitalWrite(led_relay, HIGH);
}
//--------------------------------------------------------------------------------
void relay_OFF()
{
  digitalWrite(led_relay, LOW);
}
//--------------------------------------------------------------------------------
bool check_RAIN()
{
  return (digitalRead(pin_rain) == HIGH);
}
//--------------------------------------------------------------------------------
bool check_LEVEL()
{
  return (digitalRead(pin_level) == HIGH);
}
//--------------------------------------------------------------------------------
uint8_t get_address()
{
  //return 4;
  
  union U_BYTE temp;
  temp.value = 0;
  temp.bites.bit0 = (digitalRead(addr0) == LOW);
  temp.bites.bit1 = (digitalRead(addr1) == LOW);
  temp.bites.bit2 = (digitalRead(addr2) == LOW);
  temp.bites.bit3 = (digitalRead(addr3) == LOW);
  temp.bites.bit4 = (digitalRead(addr4) == LOW);
  return temp.value;
}
//--------------------------------------------------------------------------------
uint16_t crc16(uint8_t *pcBlock, uint8_t len)
{
  uint16_t a, crc = 0xFFFF;
  while (len--)
  {
    a = *pcBlock++ << 8;
    crc ^= a;
    for (int i = 0; i < 8; i++)
      crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
  }
  return crc;
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
void convert_data_to_ascii(uint8_t data, uint8_t *hi_str, uint8_t *lo_str)
{
  uint8_t hi = (data >> 4) & 0x0F;
  uint8_t lo = (data & 0x0F);

  switch (hi)
  {
    case 0x00: *hi_str = '0'; break;
    case 0x01: *hi_str = '1'; break;
    case 0x02: *hi_str = '2'; break;
    case 0x03: *hi_str = '3'; break;
    case 0x04: *hi_str = '4'; break;
    case 0x05: *hi_str = '5'; break;
    case 0x06: *hi_str = '6'; break;
    case 0x07: *hi_str = '7'; break;
    case 0x08: *hi_str = '8'; break;
    case 0x09: *hi_str = '9'; break;
    case 0x0A: *hi_str = 'A'; break;
    case 0x0B: *hi_str = 'B'; break;
    case 0x0C: *hi_str = 'C'; break;
    case 0x0D: *hi_str = 'D'; break;
    case 0x0E: *hi_str = 'E'; break;
    case 0x0F: *hi_str = 'F'; break;
    default: break;
  }

  switch (lo)
  {
    case 0x00: *lo_str = '0'; break;
    case 0x01: *lo_str = '1'; break;
    case 0x02: *lo_str = '2'; break;
    case 0x03: *lo_str = '3'; break;
    case 0x04: *lo_str = '4'; break;
    case 0x05: *lo_str = '5'; break;
    case 0x06: *lo_str = '6'; break;
    case 0x07: *lo_str = '7'; break;
    case 0x08: *lo_str = '8'; break;
    case 0x09: *lo_str = '9'; break;
    case 0x0A: *lo_str = 'A'; break;
    case 0x0B: *lo_str = 'B'; break;
    case 0x0C: *lo_str = 'C'; break;
    case 0x0D: *lo_str = 'D'; break;
    case 0x0E: *lo_str = 'E'; break;
    case 0x0F: *lo_str = 'F'; break;
    default: break;
  }
}
//--------------------------------------------------------------------------------
int imcomingByte = 0;
//--------------------------------------------------------------------------------
#define MAX_ASCII_BUF   200
#define MAX_MODBUS_BUF  100
//--------------------------------------------------------------------------------
uint8_t ascii_buf[MAX_ASCII_BUF];
uint8_t modbus_buf[MAX_MODBUS_BUF];
//--------------------------------------------------------------------------------
int index_ascii_buf = 0;
int index_modbus_buf = 0;
//--------------------------------------------------------------------------------
uint32_t    addr_cam_32 = 0;                // адрес камеры
uint16_t    time_interval_16 = 0;           // интервал дворника
uint32_t    time_washout_32 = 0;            // время помывки
uint32_t    time_pause_washout_32 = 0;      // время между помывками
uint32_t    preset_washout_32 = 0;          // пресет помывки
//--------------------------------------------------------------------------------
void write_RS485()
{
  digitalWrite(pin_485, HIGH);
}
//--------------------------------------------------------------------------------
void read_RS485()
{
  digitalWrite(pin_485, LOW);
}
//--------------------------------------------------------------------------------
void send_byte(uint8_t data)
{
  Serial.write(data);
}
//--------------------------------------------------------------------------------
void send_data(void)
{
  uint8_t hi = 0;
  uint8_t lo = 0;

  String temp;

  temp += ":";
  for (int n = 0; n < index_modbus_buf; n++)
  {
    convert_data_to_ascii(modbus_buf[n], &hi, &lo);

    temp += (char)hi;
    temp += (char)lo;
  }
  //temp += "\n";

  write_RS485();

  delay(delay_485_ms);
  Serial.println(temp);
  delay(delay_485_ms);

  read_RS485();
}
//--------------------------------------------------------------------------------
void f_read(void)
{
  debug("f_read");
  union QUESTION_READ *packet = (union QUESTION_READ *)&modbus_buf;

  uint16_t crc = crc16((uint8_t *)&modbus_buf, sizeof(union QUESTION_READ) - 2);
  if (crc != packet->body.crc16)
  {
    debug("crc != packet->body.crc16");
    return;
  }

  uint8_t addr = get_address();
  if (addr != packet->body.header.addr_8)
  {
    debug("addr incorrect");
    return;
  }

  union ANSWER_READ answer;

  answer.body.header.prefix_16 = packet->body.header.prefix_16;
  answer.body.header.addr_8 = packet->body.header.addr_8;
  answer.body.header.cmd_8 = packet->body.header.cmd_8;
  answer.body.header.len_16 = sizeof(answer.body.data);

  answer.body.data.addr_cam_32 = addr_cam_32;                        // адрес камеры
  answer.body.data.time_interval_16 = time_interval_16;              // интервал дворника
  answer.body.data.time_washout_32 = time_washout_32;                // время помывки
  answer.body.data.time_pause_washout_32 = time_pause_washout_32;    // время между помывками
  answer.body.data.preset_washout_32 = preset_washout_32;            // пресет помывки

  answer.body.crc16 = crc16((uint8_t *)&answer.buf, sizeof(union ANSWER_READ) - 2);

  for (int n = 0; n < sizeof(answer); n++)
  {
    modbus_buf[n] = answer.buf[n];
  }
  index_modbus_buf = sizeof(answer);
  send_data();
}
//--------------------------------------------------------------------------------
void f_write(void)
{
  debug("f_write");
  union QUESTION_WRITE *packet = (union QUESTION_WRITE *)&modbus_buf;

  uint16_t crc = crc16((uint8_t *)&modbus_buf, sizeof(union QUESTION_WRITE) - 2);
  if (crc != packet->body.crc16)
  {
    debug("crc != packet->body.crc16");
    return;
  }

  uint8_t addr = get_address();
  if (addr != packet->body.header.addr_8)
  {
    debug("addr incorrect");
    return;
  }

  addr_cam_32 = packet->body.data.addr_cam_32;                       // адрес камеры
  time_interval_16 = packet->body.data.time_interval_16;             // интервал дворника
  time_washout_32 = packet->body.data.time_washout_32;               // время помывки
  time_pause_washout_32 = packet->body.data.time_pause_washout_32;   // время между помывками
  preset_washout_32 = packet->body.data.preset_washout_32;           // пресет помывки

  union ANSWER_WRITE answer;

  answer.body.header.prefix_16 = packet->body.header.prefix_16;
  answer.body.header.addr_8 = packet->body.header.addr_8;
  answer.body.header.cmd_8 = packet->body.header.cmd_8;
  answer.body.header.len_16 = sizeof(answer.body.data);

  answer.body.data.addr_cam_32 = addr_cam_32;                        // адрес камеры
  answer.body.data.time_interval_16 = time_interval_16;              // интервал дворника
  answer.body.data.time_washout_32 = time_washout_32;                // время помывки
  answer.body.data.time_pause_washout_32 = time_pause_washout_32;    // время между помывками
  answer.body.data.preset_washout_32 = preset_washout_32;            // пресет помывки

  answer.body.crc16 = crc16((uint8_t *)&answer.buf, sizeof(union ANSWER_WRITE) - 2);

  for (int n = 0; n < sizeof(answer); n++)
  {
    modbus_buf[n] = answer.buf[n];
  }
  index_modbus_buf = sizeof(answer);
  send_data();
}
//--------------------------------------------------------------------------------
void f_reset(void)
{
  debug("f_reset");
  union QUESTION_RESET *packet = (union QUESTION_RESET *)&modbus_buf;

  uint16_t crc = crc16((uint8_t *)&modbus_buf, sizeof(union QUESTION_RESET) - 2);
  if (crc != packet->body.crc16)
  {
    debug("crc != packet->body.crc16");
    return;
  }

  uint8_t addr = get_address();
  if (addr != packet->body.header.addr_8)
  {
    debug("addr incorrect");
    return;
  }

  union ANSWER_RESET answer;

  answer.body.header.prefix_16 = packet->body.header.prefix_16;
  answer.body.header.addr_8 = packet->body.header.addr_8;
  answer.body.header.cmd_8 = packet->body.header.cmd_8;
  answer.body.header.len_16 = sizeof(answer.body.data);
  answer.body.data.addr_cam_32 = addr_cam_32;                      // адрес камеры
  answer.body.data.time_interval_16 = time_interval_16;            // интервал дворника
  answer.body.data.time_washout_32 = time_washout_32;              // время помывки
  answer.body.data.time_pause_washout_32 = time_pause_washout_32;  // время между помывками
  answer.body.data.preset_washout_32 = preset_washout_32;          // пресет помывки

  answer.body.crc16 = crc16((uint8_t *)&answer.buf, sizeof(union ANSWER_RESET) - 2);

  for (int n = 0; n < sizeof(answer); n++)
  {
    modbus_buf[n] = answer.buf[n];
  }
  index_modbus_buf = sizeof(answer);
  send_data();
}
//--------------------------------------------------------------------------------
void f_test(void)
{
  debug("f_test");
  union QUESTION_TEST *packet = (union QUESTION_TEST *)&modbus_buf;

  uint16_t crc = crc16((uint8_t *)&modbus_buf, sizeof(union QUESTION_TEST) - 2);
  if (crc != packet->body.crc16)
  {
    debug("crc != packet->body.crc16");
    return;
  }

  uint8_t addr = get_address();
  if (addr != packet->body.header.addr_8)
  {
    debug("addr incorrect");
    return;
  }

  union ANSWER_TEST answer;

  answer.body.header.prefix_16 = packet->body.header.prefix_16;
  answer.body.header.addr_8 = packet->body.header.addr_8;
  answer.body.header.cmd_8 = packet->body.header.cmd_8;
  answer.body.header.len_16 = sizeof(answer.body.data);
  answer.body.crc16 = crc16((uint8_t *)&answer.buf, sizeof(union ANSWER_TEST) - 2);

  for (int n = 0; n < sizeof(answer); n++)
  {
    modbus_buf[n] = answer.buf[n];
  }
  index_modbus_buf = sizeof(answer);
  send_data();
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

  debug("analize");
  struct HEADER *packet = (struct HEADER *)&modbus_buf;
  uint8_t cmd = packet->cmd_8;
  switch (cmd)
  {
    case CMD_READ:
      f_read();
      break;
    case CMD_WRITE:
      f_write();
      break;
    case CMD_RESET:
      f_reset();
      break;
    case CMD_TEST:
      f_test();
      break;
    default:
      //logging(String("unknown cmd ") + String(cmd, HEX));
      break;
  }
}
//--------------------------------------------------------------------------------
void serialEvent()
{
  while (Serial.available())
  {
    // считываем байт
    imcomingByte = Serial.read();;
    switch (imcomingByte)
    {
      case ':':
        index_ascii_buf = 0;
        break;

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
//void blink_on()
//{
//  blink_ON();
//  pump_ON();
//  relay_ON();
//}
//--------------------------------------------------------------------------------
//void blink_off()
//{
//  blink_OFF();
//  pump_OFF();
//  relay_OFF();
//}
//--------------------------------------------------------------------------------
Timer t;
//--------------------------------------------------------------------------------
void camera_save_position (void)
{
  Pelco[1] = addr_cam_32;
  Pelco[2] = 0;
  Pelco[3] = Go_Preset;
  Pelco[4] = 0;
  Pelco[5] = Save;

  Pelco [6] = Pelco [1] ^ Pelco [2] ^ Pelco [3] ^ Pelco [4] ^ Pelco [5] ; // вычисление контрольной суммы

  write_RS485();
  delay(delay_485_ms);
  send_byte(0xFF);
  send_byte(Pelco[1]);
  send_byte(Pelco[2]);
  send_byte(Pelco[3]);
  send_byte(Pelco[4]);
  send_byte(Pelco[5]);
  send_byte(Pelco[6]);
  delay(delay_485_ms);
  read_RS485();
}
//--------------------------------------------------------------------------------
void camera_move_position (void)
{
  // 59 пресет помывки
  // 59 сохранить положение камеры до помывки

  Pelco[1] = addr_cam_32;
  Pelco[2] = 0;
  Pelco[3] = Go_Preset;
  Pelco[4] = 0;
  Pelco[5] = Move;

  Pelco [6] = Pelco [1] ^ Pelco [2] ^ Pelco [3] ^ Pelco [4] ^ Pelco [5] ; // вычисление контрольной суммы

  write_RS485();
  delay(delay_485_ms);
  send_byte(0xFF);
  send_byte(Pelco[1]);
  send_byte(Pelco[2]);
  send_byte(Pelco[3]);
  send_byte(Pelco[4]);
  send_byte(Pelco[5]);
  send_byte(Pelco[6]);
  delay(delay_485_ms);
  read_RS485();
}
//--------------------------------------------------------------------------------
void camera_return (void)
{
  // 59 пресет помывки
  // 59 сохранить положение камеры до помывки

  Pelco[1] = addr_cam_32;
  Pelco[2] = 0;
  Pelco[3] = Go_Preset;
  Pelco[4] = 0;
  Pelco[5] = Save;

  Pelco [6] = Pelco [1] ^ Pelco [2] ^ Pelco [3] ^ Pelco [4] ^ Pelco [5] ; // вычисление контрольной суммы

  write_RS485();
  delay(delay_485_ms);
  send_byte(0xFF);
  send_byte(Pelco[1]);
  send_byte(Pelco[2]);
  send_byte(Pelco[3]);
  send_byte(Pelco[4]);
  send_byte(Pelco[5]);
  send_byte(Pelco[6]);
  delay(delay_485_ms);
  read_RS485();
}
//--------------------------------------------------------------------------------
void camera_wiper (void)
{
  Pelco[1] = addr_cam_32;
  Pelco[2] = 0;
  Pelco[3] = Go_Preset;
  Pelco[4] = 0;
  Pelco[5] = Wiper;

  Pelco [6] = Pelco [1] ^ Pelco [2] ^ Pelco [3] ^ Pelco [4] ^ Pelco [5] ; // вычисление контрольной суммы

  write_RS485();
  delay(delay_485_ms);
  send_byte(0xFF);
  send_byte(Pelco[1]);
  send_byte(Pelco[2]);
  send_byte(Pelco[3]);
  send_byte(Pelco[4]);
  send_byte(Pelco[5]);
  send_byte(Pelco[6]);
  delay(delay_485_ms);
  read_RS485();
}
//--------------------------------------------------------------------------------
void camera_Run_Tur_1 (void)
{
  Pelco[1] = addr_cam_32;
  Pelco[2] = 0;
  Pelco[3] = Run_Pattern;
  Pelco[4] = 0;
  Pelco[5] = 0;

  Pelco [6] = Pelco [1] ^ Pelco [2] ^ Pelco [3] ^ Pelco [4] ^ Pelco [5] ; // вычисление контрольной суммы

  write_RS485();
  delay(delay_485_ms);
  send_byte(0xFF);
  send_byte(Pelco[1]);
  send_byte(Pelco[2]);
  send_byte(Pelco[3]);
  send_byte(Pelco[4]);
  send_byte(Pelco[5]);
  send_byte(Pelco[6]);
  delay(delay_485_ms);
  read_RS485();
}
//--------------------------------------------------------------------------------
void takeReading()
{
  bool flag_lvl  = check_LEVEL();
  bool flag_rain = check_RAIN();

  flag_blink = !flag_blink;
  if (flag_blink)
  {
    blink_ON();
  }
  else
  {
    blink_OFF();
  }
  //---
  //relay_ON(); //TODO

  // мало воды
  if (flag_lvl)
  {
    pump_ON();
  }
  else
  {
    pump_OFF();
  }

  if (flag_rain)
  {
    state = STATUS_RAIN;
  }
  else
  {
    state = STATUS_IDLE;
  }

  switch (state)
  {
    case STATUS_IDLE:
      cnt_second = 0;
      camera_move_position();
      state = STATUS_WASHOUT;
      break;

    case STATUS_RAIN:			// дождь
      if (cnt_second < time_interval_16)
      {
        cnt_second++;
      }
      else
      {
        camera_wiper();
        cnt_second = 0;
      }
      break;

    case STATUS_WASHOUT:		// моемся
      if (!(cnt_second % time_interval_16))
      {
        camera_wiper();
      }
      if (cnt_second >= time_washout_32)
      {
        cnt_second = 0;
        camera_return();
        state = STATUS_WASHOUT_PAUSE;
      }
      cnt_second++;
      break;

    case STATUS_WASHOUT_PAUSE:	// пауза
      if (cnt_second >= time_pause_washout_32)
      {
        cnt_second = 0;
        camera_move_position();
        state = STATUS_WASHOUT;
      }
      cnt_second++;
      break;

    default:
      state = STATUS_IDLE;
      break;
  }
}
//--------------------------------------------------------------------------------
void setup()
{
  Serial.begin(57600);

  pinMode(pin_485,    OUTPUT);

  pinMode(led_blink,  OUTPUT);
  pinMode(led_pump,   OUTPUT);
  pinMode(led_relay,  OUTPUT);

  pinMode(pin_rain,   INPUT);
  pinMode(pin_level,  INPUT);

  pinMode(addr0,  INPUT);
  pinMode(addr1,  INPUT);
  pinMode(addr2,  INPUT);
  pinMode(addr3,  INPUT);
  pinMode(addr4,  INPUT);

  //подтяжка
  digitalWrite(addr0,  HIGH);
  digitalWrite(addr1,  HIGH);
  digitalWrite(addr2,  HIGH);
  digitalWrite(addr3,  HIGH);
  digitalWrite(addr4,  HIGH);

  digitalWrite(pin_rain,  HIGH);
  digitalWrite(pin_level, HIGH);
  //---

  t.every(1000, takeReading); // 1 sec

  read_RS485();
}
//--------------------------------------------------------------------------------
void loop()
{
  t.update();
}
//--------------------------------------------------------------------------------
