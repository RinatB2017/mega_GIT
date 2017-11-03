//--------------------------------------------------------------------------------
//
//
//
//--------------------------------------------------------------------------------
#define serial_RS485  Serial3
#define serial_RS232  Serial
//--------------------------------------------------------------------------------
int pin_DIR = 8;

int incomingByte = 0;
//--------------------------------------------------------------------------------
void RS485_ON()
{
  digitalWrite(pin_DIR, HIGH);
}
//--------------------------------------------------------------------------------
void RS485_OFF()
{
  digitalWrite(pin_DIR, LOW);
}
//--------------------------------------------------------------------------------
void setup()
{
  serial_RS232.begin(9600);
  serial_RS485.begin(9600);

  pinMode(pin_DIR, OUTPUT);

  RS485_OFF();
}
//--------------------------------------------------------------------------------
void serialEvent()
{
  while(serial_RS232.available())
  {
    incomingByte = serial_RS232.read();
    RS485_ON();
    serial_RS485.write(incomingByte);
    RS485_OFF();
  }
}
//--------------------------------------------------------------------------------
void serialEvent3()
{
  while(serial_RS485.available())
  {
    incomingByte = serial_RS485.read();
    serial_RS232.write(incomingByte);
  }
}
//--------------------------------------------------------------------------------
void loop()
{
  /*
  if(serial_RS485.available())
  {
    incomingByte = serial_RS485.read();
    serial_RS232.write(incomingByte);
  }
  */
}
//--------------------------------------------------------------------------------


