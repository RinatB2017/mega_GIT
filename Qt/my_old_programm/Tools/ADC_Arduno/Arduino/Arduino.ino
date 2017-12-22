//---------------------------------------------------------------
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif 
//---------------------------------------------------------------
#define MAX_CHANNEL	8
//---------------------------------------------------------------
union DATA_INT16 {
    uint16_t value;
    struct {
        uint8_t a;
        uint8_t b;
    } bytes;
};

union DATA_INT24 {
    uint32_t value;
    struct {
        uint8_t a;
        uint8_t b;
        uint8_t c;
        uint8_t d;
    } bytes;
};

union DATA_INT32 {
    uint32_t value;
    struct {
        uint8_t a;
        uint8_t b;
        uint8_t c;
        uint8_t d;
    } bytes;
};
//---------------------------------------------------------------
enum CAPACITY {
    CAPACITY_8  = 8,
    CAPACITY_16 = 16,
    CAPACITY_24 = 24,
    CAPACITY_32 = 32
};

union HEADER {
  struct {
      uint16_t signature;
      uint8_t  capacity;      // разрядность (8, 16, 24, 32)
      uint8_t  count_channel;  // кол-во каналов
    } body;
    uint8_t buf[sizeof(body)];
};
//---------------------------------------------------------------
#define MAX_SIZE	256
uint8_t buf[MAX_SIZE];
//---------------------------------------------------------------
void convert_data_to_ascii(unsigned char data)
{
    unsigned char hi = (data >> 4) & 0x0F;
    unsigned char lo = (data & 0x0F);

    switch(hi)
    {
    case 0x00: Serial.print("0"); break;
    case 0x01: Serial.print("1"); break;
    case 0x02: Serial.print("2"); break;
    case 0x03: Serial.print("3"); break;
    case 0x04: Serial.print("4"); break;
    case 0x05: Serial.print("5"); break;
    case 0x06: Serial.print("6"); break;
    case 0x07: Serial.print("7"); break;
    case 0x08: Serial.print("8"); break;
    case 0x09: Serial.print("9"); break;
    case 0x0A: Serial.print("A"); break;
    case 0x0B: Serial.print("B"); break;
    case 0x0C: Serial.print("C"); break;
    case 0x0D: Serial.print("D"); break;
    case 0x0E: Serial.print("E"); break;
    case 0x0F: Serial.print("F"); break;
    default: break;
    }

    switch(lo)
    {
    case 0x00: Serial.print("0"); break;
    case 0x01: Serial.print("1"); break;
    case 0x02: Serial.print("2"); break;
    case 0x03: Serial.print("3"); break;
    case 0x04: Serial.print("4"); break;
    case 0x05: Serial.print("5"); break;
    case 0x06: Serial.print("6"); break;
    case 0x07: Serial.print("7"); break;
    case 0x08: Serial.print("8"); break;
    case 0x09: Serial.print("9"); break;
    case 0x0A: Serial.print("A"); break;
    case 0x0B: Serial.print("B"); break;
    case 0x0C: Serial.print("C"); break;
    case 0x0D: Serial.print("D"); break;
    case 0x0E: Serial.print("E"); break;
    case 0x0F: Serial.print("F"); break;
    default: break;
    }
}
//---------------------------------------------------------------
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
//---------------------------------------------------------------
void setup(void)
{
  Serial.begin(9600);
}
//---------------------------------------------------------------
void loop(void)
{
  Serial.print("+ 0.12345678");
  Serial.print('\r');
  Serial.print('\n');
  delay(1000);
}
//---------------------------------------------------------------



