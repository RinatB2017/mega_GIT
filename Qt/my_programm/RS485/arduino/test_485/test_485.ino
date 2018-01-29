/*
   http://adatum.ru/podklyuchenie-konvertera-rs-485-v-ttl-k-arduino.html
*/

#define SerialTxControl 8   //RS485 управляющий контакт на arduino pin 8
#define RS485Transmit    HIGH
#define RS485Receive     LOW

char buffer[100];
void setup(void)
{
  Serial.begin(9600);
  pinMode(SerialTxControl, OUTPUT);
  digitalWrite(SerialTxControl, RS485Transmit);
  Serial.println("TEST");
  delay(100);
  digitalWrite(SerialTxControl, RS485Receive);
}

void loop(void)
{
  digitalWrite(SerialTxControl, RS485Receive);  // читаем данные с порта

  int i = 0;
  if (Serial.available()) 
  {
    delay(100);
    while ( Serial.available() && i < 99)
    {
      buffer[i++] = Serial.read();
    }
    buffer[i++] = '\0';
  }
  if (i > 0)
  {
    Serial.println(buffer); // Выводим что приняли с других устройств
  }
}
