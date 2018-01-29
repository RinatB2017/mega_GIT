//-------------------------------------------------------
//
//-------------------------------------------------------
int pin_485 = 8;
int led_0 = 10;
int led_1 = 11;
int led_2 = 12;
//-------------------------------------------------------
int imcomingByte = 0;
uint8_t buf[100];
int index = 0;
int flag = 0;
//-------------------------------------------------------
void write_RS485()
{
  digitalWrite(pin_485, HIGH);
}
//-------------------------------------------------------
void read_RS485()
{
  digitalWrite(pin_485, LOW);
}
//-------------------------------------------------------
void serialEvent()
{
  while (Serial.available())
  {
    // считываем байт
    imcomingByte = Serial.read();;
    buf[index] = imcomingByte;
    if(index < 100)
    {
      index++;
    }
  }
  flag = 1;
}
//-------------------------------------------------------
void blink_on()
{
  digitalWrite(led_0, HIGH);
  digitalWrite(led_1, HIGH);
  digitalWrite(led_2, HIGH);
}
//-------------------------------------------------------
void blink_off()
{
  digitalWrite(led_0, LOW);
  digitalWrite(led_1, LOW);
  digitalWrite(led_2, LOW);
}
//-------------------------------------------------------
void setup()
{
  Serial.begin(57600);

  pinMode(pin_485,  OUTPUT);

  pinMode(led_0,    OUTPUT);
  pinMode(led_1,    OUTPUT);
  pinMode(led_2,    OUTPUT);

  read_RS485();
}
//-------------------------------------------------------
void loop()
{
  if (flag == 1)
  {
    write_RS485();
    delay(1);
    for(int n=0; n<index; n++)
    {
      Serial.write(buf[n]);
      delay(1);
    }
    index = 0;
    read_RS485();

    flag = 0;
  }
  delay(100);
}
//-------------------------------------------------------
