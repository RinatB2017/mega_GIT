//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------
int addr0 = 2;
int addr1 = 3;
int addr2 = 4;
int addr3 = 5;
int addr4 = 6;
int addr5 = 7;
int addr6 = 8;
int addr7 = 9;

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
//--------------------------------------------------------------------------------
uint8_t get_address()
{
  union U_BYTE temp;
  temp.value = 0;
  temp.bites.bit0 = digitalRead(addr0);
  temp.bites.bit1 = digitalRead(addr1);
  temp.bites.bit2 = digitalRead(addr2);
  temp.bites.bit3 = digitalRead(addr3);
  temp.bites.bit4 = digitalRead(addr4);

  temp.bites.bit5 = digitalRead(addr5);
  temp.bites.bit6 = digitalRead(addr6);
  temp.bites.bit7 = digitalRead(addr7);
  
  return temp.value;
}
//--------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
}
//--------------------------------------------------------------------------------
void loop() {
  Serial.println(get_address(), BIN);
  //Serial.println(get_address());
  delay(1000);
}
//--------------------------------------------------------------------------------

