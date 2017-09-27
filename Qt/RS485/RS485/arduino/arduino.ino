//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------
#include "Timer.h"
//--------------------------------------------------------------------------------
#define serial_RS232  Serial
#define serial_RS485  Serial1
//--------------------------------------------------------------------------------
#define MAX_BUF  200

Timer t;

uint8_t buf[MAX_BUF];
int index_buf = 0;

int pin_BLINK = A8;   // светодиод
int pin_PUMP = A9;    // насос
int pin_RELAY = A10;  // реле

int pin_RAIN = A0;    // датчик дождя
int pin_LEVEL = A1;   // датчик уровня

// адрес
int pin_A0 = A2;
int pin_A1 = A3;
int pin_A2 = A4;
int pin_A3 = A5;
int pin_A4 = A6;
//--------------------------------------------------------------------------------
union U_BYTE
{
    uint8_t value;
    struct
    {
        uint8_t bit0:1;
        uint8_t bit1:1;
        uint8_t bit2:1;
        uint8_t bit3:1;
        uint8_t bit4:1;
        uint8_t bit5:1;
        uint8_t bit6:1;
        uint8_t bit7:1;
    } bites;
};
//--------------------------------------------------------------------------------
typedef struct HEADER
{
    uint16_t    prefix_16;                  // префикс
    uint8_t     addr_8;                     // адрес модуля
    uint8_t     cmd_8;                      // команда
    uint16_t    len_16;                     // длина данных
};

union NEW_PACKET
{
    struct BODY
    {
        HEADER      header;

        uint32_t    addr_cam_32;                // адрес камеры
        uint16_t    time_interval_16;           // интервал дворника
        uint32_t    time_washout_32;            // время помывки
        uint32_t    time_pause_washout_32;      // время между помывками
        uint32_t    preset_washout_32;          // пресет помывки
        uint32_t    time_preset_washout_32;     // времен помывки

        uint16_t    crc16;                      // контрольная сумма
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
uint16_t crc16(uint8_t *pcBlock, uint8_t len)
{
    uint16_t a, crc = 0xFFFF;
    while(len--)
    {
        a = *pcBlock++ << 8;
        crc ^= a;
        for(int i=0; i<8; i++)
            crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
    }
    return crc;
}
//--------------------------------------------------------------------------------
void clear_buf()
{
  for(int i=0; i<MAX_BUF; i++)
  {
    buf[i] = 0;
  }
  index_buf = 0;
}
//--------------------------------------------------------------------------------
char flag = 0;
void doSomething()
{
  flag =!flag;
  if(flag)
    blink_ON(); 
  else
    blink_OFF(); 
}
//--------------------------------------------------------------------------------
int incomingByte = 0;
void serialEvent()
{
  int cnt = 0;
  int length_to_read = 1;

  while (length_to_read != 0) 
  {
    if (!serial_RS485.available()) 
    {
      int i = 0;
      while (!serial_RS485.available()) 
      {
        delay(1);
        if (++i == 10) 
        {
          length_to_read  = 0;
          if(cnt > 0)
          {
            logging(String("Reading " + String(cnt) + " bytes"));
            
            command();
            
            String temp;
            for(int n=0; n<index_buf; n++)
            {
              temp += String(buf[n], HEX);
            }
            logging(temp);

            clear_buf();
          }
          return;
        }
      }
    }

    incomingByte = serial_RS485.read();
    if(index_buf < MAX_BUF)
    {
      buf[index_buf] = incomingByte;
      index_buf++;
    }
    
    cnt++;
  }
}
//--------------------------------------------------------------------------------
void setup_RS232()
{
  serial_RS232.begin(9600);
}
//--------------------------------------------------------------------------------
void setup_RS485()
{
  serial_RS485.begin(9600);
}
//--------------------------------------------------------------------------------
void init_timer()
{
  t.every(500, doSomething);
}
//--------------------------------------------------------------------------------
void blink_ON()
{
  digitalWrite(pin_BLINK, HIGH);
}
//--------------------------------------------------------------------------------
void blink_OFF()
{
  digitalWrite(pin_BLINK, LOW);
}
//--------------------------------------------------------------------------------
void pump_ON()
{
  digitalWrite(pin_PUMP, HIGH);
}
//--------------------------------------------------------------------------------
void pump_OFF()
{
  digitalWrite(pin_PUMP, LOW);
}
//--------------------------------------------------------------------------------
void relay_ON()
{
  digitalWrite(pin_RELAY, HIGH);
}
//--------------------------------------------------------------------------------
void relay_OFF()
{
  digitalWrite(pin_RELAY, LOW);
}
//--------------------------------------------------------------------------------
bool check_RAIN()
{
  return (digitalRead(pin_RAIN) == HIGH);
}
//--------------------------------------------------------------------------------
bool check_LEVEL()
{
  return (digitalRead(pin_LEVEL) == HIGH);
}
//--------------------------------------------------------------------------------
uint8_t get_address()
{
  U_BYTE temp;
  temp.value = 0;
  temp.bites.bit0 = (digitalRead(pin_A0) == HIGH);
  temp.bites.bit1 = (digitalRead(pin_A1) == HIGH);
  temp.bites.bit2 = (digitalRead(pin_A2) == HIGH);
  temp.bites.bit3 = (digitalRead(pin_A3) == HIGH);
  temp.bites.bit4 = (digitalRead(pin_A4) == HIGH);
  
  return temp.value;
}
//--------------------------------------------------------------------------------
void logging(String text)
{
  serial_RS232.println(text);
}
//--------------------------------------------------------------------------------
void work()
{
  if(index_buf == 0)  return;
  if(index_buf < sizeof(HEADER)) return;
  
  NEW_PACKET *packet = (NEW_PACKET *)&buf;
  uint16_t crc = crc16((uint8_t *)&buf, sizeof(NEW_PACKET) - 2);
  if(crc != packet->body.crc16)  return;
}
//--------------------------------------------------------------------------------
void command()
{
}
//--------------------------------------------------------------------------------
void setup()
{
  clear_buf();
  
  pinMode(pin_BLINK, OUTPUT);
  pinMode(pin_PUMP,  OUTPUT);
  pinMode(pin_RELAY, OUTPUT);

  pinMode(pin_RAIN,  INPUT);
  pinMode(pin_LEVEL, INPUT);

  pinMode(pin_A0,  INPUT);
  pinMode(pin_A1,  INPUT);
  pinMode(pin_A2,  INPUT);
  pinMode(pin_A3,  INPUT);
  pinMode(pin_A4,  INPUT);

  setup_RS232();
  setup_RS485();
  init_timer();
  
  logging(String("address 0x" + String(get_address(), HEX)));
}
//--------------------------------------------------------------------------------
void loop()
{
  t.update();
  work();
}
//--------------------------------------------------------------------------------

